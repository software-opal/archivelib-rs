import sys
import pathlib
import re
import hashlib
import subprocess
import tempfile

offset = [20, 24, 28]

ROOT = pathlib.Path(__file__).parent
FUZZ_DIR = ROOT / "fuzz"
FUZZ_CRASHES_DIR = FUZZ_DIR / "artifacts"
AFL_OUTPUT_DIR = FUZZ_DIR / "afl_out"
KNOWN_INPUTS = FUZZ_DIR / "known_inputs"
MINIFIED_OUTPUTS = FUZZ_DIR / "minified_data"
MINIFIED_TEST_OUTPUTS = ROOT / "src/test/minified_data"
NON_ALPHA = re.compile("(^[0-9]|[^a-zA-Z0-9_])")


cargo_afl = ["cargo", "afl"]
afl_opts = ["-t1000", "-m250MB"]
target = ["target/debug/cli", "-d"]


def bytes_to_test_hex(data):
    aparts = []
    bparts = []
    out = ""
    for b in data:
        aparts.append(f"{b:02X}")
        if len(aparts) >= 8:
            bparts.append(" ".join(aparts))
            aparts = []
        if len(bparts) >= 4:
            out += f'      {"  ".join(bparts)}\n'
            bparts = []
    if aparts:
        bparts.append(" ".join(aparts))
    if bparts:
        out += f'      {"  ".join(bparts)}\n'
    return out


def write_test(data, desc):
    sha1 = hashlib.sha1(data).hexdigest()
    (MINIFIED_OUTPUTS / sha1).write_bytes(data)
    (MINIFIED_TEST_OUTPUTS / f"test_{sha1.lower()}.rs").write_text(
        "match_sys_test_data! {\n"
        + f"  // {desc}\n"
        + f'  data => hex!("\n'
        + bytes_to_test_hex(data)
        + '  "),\n'
        + "}\n"
    )
    mod_file = MINIFIED_TEST_OUTPUTS / f"mod.rs"
    try:
        lines = set(mod_file.read_text().splitlines(keepends=False))
    except IOError:
        lines = set()
    lines.add(f"mod test_{sha1.lower()};")
    mod_file.write_text("\n".join(sorted(lines)) + "\n")


def run_build():
    subprocess.run(
        ["cargo", "afl", "build", "--features", "fuzz-afl"],
        cwd=str(ROOT / "cli"),
        check=True,
    )


def run_fuzz(out_dir):
    try:
        res = subprocess.run(
            [
                *cargo_afl,
                "fuzz",
                *afl_opts,
                "-i",
                str(KNOWN_INPUTS),
                "-o",
                str(out_dir),
                "--",
                *target,
            ],
            cwd=str(ROOT),
            check=False,
            timeout=24 * 60 * 60,
        )
        if res.returncode not in [0, 130]:
            res.check_returncode()
    except KeyboardInterrupt:
        pass
    except subprocess.TimeoutExpired:
        pass


def graph_fuzz(fuzz_out, graph_out):
    subprocess.run(
        [*cargo_afl, "plot", str(fuzz_out), str(graph_out)], cwd=str(ROOT), check=True
    )


def run_test_case_minifier(test_case, test_type):
    with tempfile.NamedTemporaryFile() as out:
        res = subprocess.run(
            [
                *cargo_afl,
                "tmin",
                *afl_opts,
                "-i",
                str(test_case),
                "-o",
                out.name,
                "--",
                *target,
            ],
            cwd=str(ROOT),
            check=True,
        )
        min_data = pathlib.Path(out.name).read_bytes()
        write_test(min_data, f"Minified from {test_type}: {test_case.name}")
    # cargo afl tmin -t 1000 -i out/crashes/id\:000001\,sig\:06\,src\:000001\,op\:flip1\,pos\:1 -o min_id_000001 --


def main():
    MINIFIED_OUTPUTS.mkdir(exist_ok=True, parents=True)
    MINIFIED_TEST_OUTPUTS.mkdir(exist_ok=True, parents=True)
    # run_build()
    # run_fuzz(AFL_OUTPUT_DIR)
    # graph_fuzz(AFL_OUTPUT_DIR, FUZZ_DIR / "graph/")
    bad_files = [
        (type, file)
        for type in ["crashes", "hangs"]
        for file in (AFL_OUTPUT_DIR / type).iterdir()
        if file.name != "README.txt"
    ] + [
        (f'fuzz_crash_{dir.name}', file)
        for dir in filter(pathlib.Path.is_dir, FUZZ_CRASHES_DIR.iterdir())
        for file in dir.iterdir()
    ]

    # with tempfile.NamedTemporaryFile() as f:
    #     subprocess.call([*target, bad_files[0][1], f.name], cwd=ROOT)
    # for (type, crash) in bad_files:
    #     run_test_case_minifier(crash, type)
    for (type, crash) in bad_files:
        write_test(crash.read_bytes(), f"Minified from {type}: {crash.name}")

if __name__ == "__main__":
    main()
