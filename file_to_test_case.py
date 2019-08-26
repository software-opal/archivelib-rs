import hashlib
import pathlib
import re
import subprocess
import sys
import tempfile

ROOT = pathlib.Path(__file__).parent
FUZZ_DIR = ROOT / "fuzz"
FUZZ_CRASHES_DIR = FUZZ_DIR / "artifacts"
AFL_OUTPUT_DIR = FUZZ_DIR / "afl_out"
KNOWN_INPUTS = FUZZ_DIR / "known_inputs"
MINIFIED_TEST_OUTPUTS = ROOT / "tests/minified_data"
NON_ALPHA = re.compile("(^[0-9]|[^a-zA-Z0-9_])")


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


def write_test(data):
    sha1 = hashlib.sha1(data).hexdigest()
    (KNOWN_INPUTS / sha1).write_bytes(data)
    MINIFIED_TEST_OUTPUTS.mkdir(exist_ok=True)
    (MINIFIED_TEST_OUTPUTS / f"test_{sha1.lower()}.rs").write_text(
        "test_match_sys_decompress! {\n"
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


def main():
    for file in map(pathlib.Path, sys.argv[1:]):
        write_test(file.read_bytes())


if __name__ == "__main__":
    main()
