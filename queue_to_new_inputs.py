import hashlib
import multiprocessing
import os
import pathlib
import subprocess
import sys
import functools
import tempfile

# Workflow taken from this blog post on how to fuzz effectively
# https://foxglovesecurity.com/2016/03/15/fuzzing-workflows-a-fuzz-job-from-start-to-finish/


ROOT = pathlib.Path(__file__).parent


def list_files_in_folder(*folders):
    folders = [folder for folder in folders if folder.is_dir()]
    return {f for folder in folders for f in folder.iterdir() if f.is_file()}


def load_files_in_folder(*folders):
    return {f.read_bytes() for f in list_files_in_folder(*folders)}


def minimise_inputs(executable, out_folder, items):
    with tempfile.TemporaryDirectory(dir=ROOT) as _td:
        td = pathlib.Path(_td)
        assert td.is_dir()
        for i, item in enumerate(sorted(items, key=lambda d: (len(d), d))):
            if 0 < len(item) <= 4096:
                (td / f"input_{i}").write_bytes(item)
        subprocess.run(
            ["afl-cmin", "-i", td, "-o", out_folder, "--", executable], check=True
        )


def minimise_input(executable, out_folder, file):
    out = out_folder / file.name
    subprocess.run(["afl-tmin", "-i", file, "-o", out, "--", executable], check=True)


def main():
    executable = pathlib.Path(sys.argv[1])
    assert executable.is_file()
    folders = [f for f in map(pathlib.Path, sys.argv[2:]) if f.is_dir()]
    new_corpus = load_files_in_folder(*folders)
    count = 0
    cmin_out = ROOT / f"_cmin_{count}_corpus"
    tmin_out = ROOT / f"_tmin_{count}_corpus"
    subprocess.run(["rm", "-rf", cmin_out, tmin_out], check=True)
    tmin_out.mkdir(exist_ok=True)
    minimise_inputs(executable, cmin_out, new_corpus)
    with multiprocessing.Pool(32) as p:
        p.map(
            functools.partial(minimise_input, executable, tmin_out),
            list_files_in_folder(cmin_out),
        )


if __name__ == "__main__":
    main()
