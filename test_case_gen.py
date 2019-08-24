import ast
import multiprocessing
import os
import pathlib
import pickle
import re
import shlex
import statistics
import subprocess
import sys
import typing as typ

HEX_MACRO_RE = re.compile(r'hex!\([\s\n]*"([\s\n0-9A-Fa-f]+)"[\s\n]*\)', re.MULTILINE)
U8_ARRAY_RE = re.compile(
    r"\[((?:(?:[1-9][0-9]*|0x[0-9A-F]{1,2}|0)(?:_?u8)?,?[\s\n]*)+)\]",
    re.MULTILINE | re.IGNORECASE,
)
U8_REPEAT_RE = re.compile(
    r"\[([1-9][0-9]*|0x[0-9A-F]{1,2}|0)(?:_?u8); ([1-9][0-9]*|0x[0-9A-F]{1,2}|0)\]",
    re.MULTILINE,
)
ROOT = pathlib.Path(__file__).resolve().parent

CRASH_STATUS_CODES = {
    101: "Rust Panic",
    134: "C Abort",
    -6: "C Abort",
    999: "Python Timeout",
}

KCOV_COMMON_OPTS = []


def kcov_report(dir):
    return []
    # return ["kcov", *KCOV_OPTS, "--report-only", dir]


def kcov_collect(dir):
    return []
    # return ["kcov", *KCOV_OPTS, "--collect-only", dir]


PICKLED_INPUTS = ROOT / "data.pickle"
UNZIP_EXEC_NEW = ROOT / "target/debug/unalzip"
UNZIP_EXEC_SYS = ROOT / "target/debug/unalszip"
ZIP_EXEC_NEW = ROOT / "target/debug/alzip"
ZIP_EXEC_SYS = ROOT / "target/debug/alszip"

TEST_OUTPUT_ROOT_DIR = ROOT / "gen_test_cases"
TEST_OUTPUT_DIRS = {
    name: TEST_OUTPUT_ROOT_DIR / name
    for name in [
        "match",
        "match_err",
        "match_crash",
        "crash",
        "difference_out",
        "difference_err",
        "unknown",
        "wip",
    ]
}

ERROR_MAPPING = {
    rb'Error: "BinaryTreeError(Type1)"': "BTE1",
    rb'Error: "Binary tree error: Type1"': "BTE1",
    rb'Error: "Internal error: -101\u{0}"': "BTE1",
    rb'Error: "BinaryTreeError(Type2)"': "BTE2",
    rb'Error: "Binary tree error: Type2"': "BTE2",
    rb'Error: "Internal error: -102\u{0}"': "BTE2",
    rb'Error: "IOError: failed to write whole buffer"': "OOM",
    rb'Error: "Attempt to allocate a huge buffer of 65536 bytes for ALMemory\u{0}"': "OOM",
    rb'Error: "InvariantFailure"': "INV",
    rb'Error: "Invariant Failure"': "INV",
}


def sp_run(cmd, **opts):
    cmd = [str(i) for i in cmd]
    # print(f"Running: {' '.join(map(shlex.quote, cmd))}")
    return subprocess.run(cmd, **opts)


def get_all_inputs() -> typ.Set[bytes]:
    inputs = frozenset()
    try:
        with PICKLED_INPUTS.open("rb") as f:
            inputs = inputs.union(pickle.load(f))
    except:
        inputs = inputs | get_fuzz_inputs()
        for folder in ["tests", "src", "old_tests", "archivelib-sys-refactored/src"]:
            inputs = inputs | get_test_case_inputs(ROOT / folder)
    inputs = {i for i in inputs if i is not None}
    with PICKLED_INPUTS.open("wb") as f:
        pickle.dump(inputs, f, protocol=pickle.HIGHEST_PROTOCOL)

    return inputs


def get_fuzz_inputs() -> typ.Set[bytes]:
    folders = []
    folders += [f for f in (ROOT / "fuzz/corpus").iterdir() if f.is_dir()]
    folders += [
        f / subdir
        for f in (ROOT / "fuzz/").iterdir()
        for subdir in ["crashes", "hangs", "queue"]
        if f.is_dir() and f.name.startswith("afl_out")
    ]
    folders.append(ROOT / "fuzz/known_inputs")
    folders.append(ROOT / "fuzz/known_inputs")
    return {
        f.read_bytes() for folder in folders for f in folder.iterdir() if f.is_file()
    }


def get_test_case_inputs(folder=ROOT / "tests") -> typ.Set[bytes]:
    inputs = set()
    for file in folder.iterdir():
        if file.is_dir():
            inputs = inputs | get_test_case_inputs(file)
        elif file.is_file():
            for match in HEX_MACRO_RE.finditer(file.read_text()):
                inputs.add(bytes.fromhex(match.group(1).replace("\n", "")))
            for match in U8_ARRAY_RE.finditer(file.read_text()):
                l = ast.literal_eval(match.group(0).replace("_u8", "").replace("_", ""))
                if all(i <= 255 for i in l):
                    inputs.add(bytes(l))
            for match in U8_REPEAT_RE.finditer(file.read_text()):
                v = int(match.group(1))
                if v <= 255:
                    inputs.add(bytes([v]) * int(match.group(2)))
    return inputs


def run_exec(input, kcov_base, exec, level="4"):
    try:
        result = sp_run(
            [*kcov_collect(kcov_base), exec, f"-{level}"],
            input=input,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=30,
        )
        rc = result.returncode
        out = result.stdout
        err = result.stderr.strip()
    except subprocess.TimeoutExpired as e:
        rc = 999
        out = e.stdout
        err = e.stderr.strip()
    if not err:
        known_err = True
    elif err in ERROR_MAPPING:
        known_err = ERROR_MAPPING[err]
    else:
        known_err = None
    if rc == 0:
        assert known_err is True
    return (rc, out, err, known_err)


def test_case_name(input):
    import hashlib
    sha = hashlib.sha1(input).hexdigest()
    size = len(input) if len(input) < 10000 else 9999
    return f'{size:04}~{sha}'

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
            out += f'{"  ".join(bparts)}\n'
            bparts = []
    if aparts:
        bparts.append(" ".join(aparts))
    if bparts:
        out += f'{"  ".join(bparts)}\n'
    return out


def output_test_case(
    input,
    *,
    sys_out=None,
    new_out=None,
    sys_err=None,
    new_err=None,
    sys_cov=None,
    new_cov=None,
    fail_type,
):
    name = test_case_name(input)
    out = TEST_OUTPUT_DIRS[fail_type] / name
    sp_run(["rm", "-rf", out], check=True)
    out.mkdir(parents=True)
    (out / "input.dat").write_bytes(input)
    (out / "input.txt").write_text(bytes_to_test_hex(input))
    for (o, name) in [(sys_out, "sys_"), (new_out, "new_")]:
        if o is not None:
            (out / f"{name}output.dat").write_bytes(o)
            (out / f"{name}output.txt").write_text(bytes_to_test_hex(o))
    for (o, name) in [(sys_err, "sys_"), (new_err, "new_")]:
        if o is not None:
            (out / f"{name}err.txt").write_bytes(o)

    for (orig, exec, target) in [
        (sys_cov, UNZIP_EXEC_SYS, "sys"),
        (new_cov, UNZIP_EXEC_NEW, "new"),
    ]:
        if orig is not None:
            kcov_out = out / target
            kcov_call = kcov_report(orig)
            if kcov_call:
                sp_run([*kcov_call, exec], check=True)
                sp_run(["cp", "-r", orig, kcov_out], check=True)
    # print(f"{sha1}: {len(input)} byte(s); {fail_type}")


def run(input: bytes):
    name =test_case_name(input)
    to_rm ={k: o / name for k, o in TEST_OUTPUT_DIRS.items()}
    if any(            to_rm[k].is_dir()            for k in ["match", "match_err", "match_crash"]    ):
        # print(f"{sha1}: {len(input)} byte(s); Existing match")
        return
    base = TEST_OUTPUT_DIRS["wip"] / name
    sp_run(["rm", "-rf", *to_rm.values()], check=True)
    sys_base = base / "sys"
    new_base = base / "new"
    sys_base.mkdir(parents=True)
    new_base.mkdir(parents=True)
    (base / "input.dat").write_bytes(input)

    (sys_rc, sys_out, sys_err, sys_known_err) = run_exec(
        input, sys_base, UNZIP_EXEC_SYS
    )
    (new_rc, new_out, new_err, new_known_err) = run_exec(
        input, new_base, UNZIP_EXEC_NEW
    )
    if not sys_known_err:
        print("Unknown error: ", sys_err)
    if not new_known_err:
        print("Unknown error: ", new_err)

    if sys_rc == 0:
        if new_rc != 0:
            output_test_case(
                input,
                sys_out=sys_out,
                sys_err=sys_err,
                new_out=new_out,
                new_err=new_err, sys_cov=sys_base, fail_type="difference_err"
            )
        elif sys_out != new_out:
            output_test_case(
                input,
                sys_out=sys_out,
                sys_err=sys_err,
                new_out=new_out,
                new_err=new_err,
                sys_cov=sys_base,
                fail_type="difference_out",
            )
        else:
            output_test_case(
                input,
                sys_out=sys_out,
                sys_err=sys_err,
                new_out=new_out,
                new_err=new_err,
                sys_cov=sys_base,
                new_cov=new_base,
                fail_type="match",
            )
    elif sys_known_err:
        if new_known_err == sys_known_err:
            output_test_case(
                input,
                sys_out=sys_out,
                sys_err=sys_err,
                new_out=new_out,
                new_err=new_err,
                sys_cov=sys_base,
                new_cov=new_base,
                fail_type="match_err",
            )
        else:
            output_test_case(
                input,
                sys_out=sys_out,
                sys_err=sys_err,
                new_out=new_out,
                new_err=new_err,
                sys_cov=sys_base,
                fail_type="difference_err",
            )
    elif new_known_err == "INV" and sys_rc in CRASH_STATUS_CODES:
        output_test_case(
            input,
            sys_out=sys_out,
            sys_err=sys_err,
            new_out=new_out,
            new_err=new_err,
            new_cov=new_base,
            fail_type="match_crash",
        )
    elif {sys_rc, new_rc} & set(CRASH_STATUS_CODES):
        # Rust crash, Segfault/Abort, Timeout.
        output_test_case(
            input,
            sys_out=sys_out,
            sys_err=sys_err,
            new_out=new_out,
            new_err=new_err,
            fail_type="crash",
        )
    else:
        output_test_case(
            input,
            sys_out=sys_out,
            sys_err=sys_err,
            new_out=new_out,
            new_err=new_err,
            sys_cov=sys_base,
            new_cov=new_base,
            fail_type="unknown",
        )
        print("Unknown data: ", len(input), name)
        print(
            " sys:",
            repr(
                (
                    sys_rc,
                    CRASH_STATUS_CODES.get(sys_rc, "Non-crash"),
                    len(sys_out),
                    sys_err,
                    sys_known_err,
                )
            ),
        )
        print(
            " new:",
            repr(
                (
                    new_rc,
                    CRASH_STATUS_CODES.get(new_rc, "Non-crash"),
                    len(new_out),
                    new_err,
                    new_known_err,
                )
            ),
        )
    sp_run(["rm", "-rf", base])


def main():
    sp_run(["cargo", "+nightly", "build"], check=True, cwd=ROOT)
    sp_run(
        ["cargo", "+nightly", "build"],
        check=True,
        cwd=(ROOT / "archivelib-sys-refactored"),
    )
    data = sorted(get_all_inputs(), key=len)
    lens = list(map(len, data))
    print(f"Total test cases: {len(data)}")
    print(f"Average length: {statistics.mean(lens):0.1f}")
    print(f"Median length:  {statistics.median(lens):0.1f}")

    with multiprocessing.Pool(5) as p:
        for _ in p.imap_unordered(run, data):
            pass


if __name__ == "__main__":
    main()
