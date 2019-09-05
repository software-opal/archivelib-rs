import pytest, subprocess
import toml
import pathlib

import os


@pytest.fixture(scope="session")
def project_root():
    return pathlib.Path(__file__).resolve().parent.parent


@pytest.fixture
def cargo_toml(project_root):
    return toml.load(project_root / "Cargo.toml")


def run_make(system_folder):
    build_folder = system_folder / "build"
    if not os.environ.get("ALTEST_NO_BUILD"):
        build_folder.mkdir(exist_ok=True)
        subprocess.run(["cmake", system_folder], cwd=build_folder, check=True)
        subprocess.run(["make"], cwd=build_folder, check=True)
    return build_folder


@pytest.fixture(scope="session")
def orig_sys_build(project_root):
    return run_make(project_root / "archivelib-sys-orig")


@pytest.fixture
def orig_sys_zip(orig_sys_build):
    return orig_sys_build / "alzip"


@pytest.fixture
def orig_sys_unzip(orig_sys_build):
    return orig_sys_build / "alunzip"


@pytest.fixture(scope="session")
def refactored_sys_build(project_root):
    return run_make(project_root / "archivelib-sys-refactored")


@pytest.fixture
def refactored_sys_zip(refactored_sys_build):
    return refactored_sys_build / "alzip"


@pytest.fixture
def refactored_sys_unzip(refactored_sys_build):
    return refactored_sys_build / "alunzip"


@pytest.fixture(scope="session")
def rust_cli_build_folder(project_root):
    opts = []
    if os.environ.get("ALTEST_RELEASE_BUILD"):
        folder = "release"
        opts += ["--release"]
    else:
        folder = "debug"
    if not os.environ.get("ALTEST_NO_BUILD"):
        subprocess.run(
            ["cargo", "build", "--bins", "--locked", *opts],
            cwd=project_root ,
            check=True,
        )
    return project_root / "target" / folder


@pytest.fixture
def rust_unzip(rust_cli_build_folder):
    return rust_cli_build_folder / "unalzip"


@pytest.fixture
def rust_zip(rust_cli_build_folder):
    return rust_cli_build_folder / "alzip"


@pytest.fixture
def al_runner(
    orig_sys_zip,
    orig_sys_unzip,
    refactored_sys_zip,
    refactored_sys_unzip,
    rust_unzip,
    rust_zip,
):
    return Executor(
        {"orig": orig_sys_zip, "refactored": refactored_sys_zip, "rust": rust_zip},
        {
            "orig": orig_sys_unzip,
            "refactored": refactored_sys_unzip,
            "rust": rust_unzip,
        },
    )


class Executor:
    def __init__(self, zips: dict, unzips: dict):
        self.zips = dict(zips)
        self.unzips = dict(unzips)
        assert set(self.zips) == set(self.unzips)

    def smoketest(self):
        (out, err) = self.test_zip(b"")
        assert err is None
        (out, err) = self.test_unzip(out)
        assert err is None
        assert out == b""

    @property
    def impls(self):
        return tuple(sorted(self.zips))

    def exec(self, args, *, input=None, timeout=60, **kwargs):
        with subprocess.Popen(
            args, stdout=subprocess.PIPE, stderr=subprocess.PIPE, stdin=subprocess.PIPE, **kwargs
        ) as proc:
            try:
                outs, errs = proc.communicate(input
            =input,
        timeout=timeout)
            except TimeoutExpired:
                proc.kill()
                outs, errs = proc.communicate()
        return (proc.returncode, outs, errs)

    def run_zip(self, name, input: bytes, *, level=4):
        assert 0 <= level <= 4
        (rc, out, err) = self.exec([self.zips[name], f"-{level}"], input=input)
        # TODO: Convert the error into a error 'type'
        return (rc == 0, out, err)

    def run_all_zip(self, input: bytes, *, level=4):
        return {name: self.run_zip(name, input, level=level) for name in self.impls}

    def test_zip(self, input: bytes, *, level=4):
        results = self.run_all_zip(input, level=level)
        assert len(results) > 0
        has_err = any(not success for (success, out, err) in results.values())
        has_out = any(success for (success, out, err) in results.values())
        print(results)
        if has_err:
            for name, (success, out, err) in results.items():
                assert not success, f"{name} -> {out} | {err}"
            return (None, {name: err for (success, out, err) in results.items()})
        elif has_out:
            for name, (success, out, err) in results.items():
                assert success, f"{name} -> {out} | {err}"
            outs = {name: out for name, (success, out, err) in results.items()}
            out_groups = {
                out: sorted(name for name, i_out in outs.items() if i_out == out)
                for out in set(results.values())
            }
            assert out_groups.values() == [self.impls]
            return (next(out_groups.keys()), None)
