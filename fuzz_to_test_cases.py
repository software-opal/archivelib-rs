import hashlib
import os
import pathlib
import re
import subprocess
import sys
import tempfile

ROOT = pathlib.Path(__file__).resolve().parent
FUZZ_DIR = ROOT / "fuzz"
FUZZ_CRASHES_DIR = FUZZ_DIR / "artifacts"
AFL_OUTPUT_DIR = FUZZ_DIR / "afl_out"
KNOWN_INPUTS = FUZZ_DIR / "known_inputs"
MINIFIED_OUTPUTS = FUZZ_DIR / "minified_data"
MINIFIED_TEST_OUTPUTS = ROOT / "tests/minified_data"
NON_ALPHA = re.compile("(^[0-9]|[^a-zA-Z0-9_])")


cargo_afl = ["cargo", "afl"]
afl_opts = ["-t1000", "-m250MB"]
target = ["target/debug/alfuzz_afl", "-d"]


def le_bytes_to_int(bytes) -> int:
    o = 0
    for b in reversed(bytes):
        o = b | (o << 8)
    return o


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
    (KNOWN_INPUTS / sha1).write_bytes(data)
    (MINIFIED_TEST_OUTPUTS / f"test_{sha1.lower()}.rs").write_text(
        "test_match_sys_decompress! {\n"
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


def preseed(out_dir):
    out_dir.mkdir(exist_ok=True)
    inputs = {
        "empty": b"",
        "test": b"test",
        "0-to-256": bytes(range(0, 256)),
        "hus-like": bytes(([0x80] * 45) + [0x88, 0x90]),
    }
    for lvl in range(0, 5):
        for name, input in inputs.items():
            output = subprocess.run(
                ["target/debug/alzip", f"-{lvl}"],
                cwd=ROOT,
                check=True,
                input=input,
                stdout=subprocess.PIPE,
            )
            output = output.stdout
            (out_dir / f"{name}-level-{lvl}").write_bytes(output)
    for p in (ROOT / "test_data").iterdir():
        name = NON_ALPHA.sub("-", f"{p.name}")
        print(name)
        data = p.read_bytes()
        attrs_off = le_bytes_to_int(data[20:24])
        x_coords_off = le_bytes_to_int(data[24:28])
        y_coords_off = le_bytes_to_int(data[28:32])
        (out_dir / f"{name}-stitc-attrs").write_bytes(data[attrs_off:x_coords_off])
        (out_dir / f"{name}-x-coords").write_bytes(data[x_coords_off:y_coords_off])
        (out_dir / f"{name}-y-coords").write_bytes(data[y_coords_off:])
    # raise ValueError()


def run_build():
    env = dict(os.environ)
    env["RUSTFLAGS"] = " ".join(
        ["-Clink-arg=-fuse-ld=gold", "-Clink-arg=-funroll-loops", "-Copt-level=3"]
    )
    subprocess.run(
        [
            "cargo",
            "afl",
            "build",
            "--features",
            "fuzz-afl",
            "--release",
            "--bin",
            "alfuzz_afl",
        ],
        cwd=str(ROOT / "cli"),
        check=True,
        env=env,
    )
    subprocess.run(["cargo", "build", "--bin=alzip"], cwd=ROOT, check=True)


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
            # timeout=24 * 60 * 60,
            timeout=5 * 60,
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
    write_test(test_case.read_bytes(), f"From {test_type}: {test_case.name}")
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
    run_build()
    preseed(KNOWN_INPUTS)
    # run_fuzz(AFL_OUTPUT_DIR)
    graph_fuzz(AFL_OUTPUT_DIR, FUZZ_DIR / "graph/")
    bad_files = [
        (type, file)
        for type in ["crashes", "hangs"]
        for file in (AFL_OUTPUT_DIR / type).iterdir()
        if file.name != "README.txt"
    ] + [
        # (f"fuzz_crash_{dir.name}", file)
        # for dir in filter(pathlib.Path.is_dir, FUZZ_CRASHES_DIR.iterdir())
        # for file in dir.iterdir()
    ]

    # with tempfile.NamedTemporaryFile() as f:
    #     subprocess.call([*target, bad_files[0][1], f.name], cwd=ROOT)
    for (type, crash) in bad_files:
        run_test_case_minifier(crash, type)


if __name__ == "__main__":
    main()
