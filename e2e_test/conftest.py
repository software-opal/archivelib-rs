import os
import pathlib
import subprocess

import pytest
import toml

from ._support import Executor


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
    return orig_sys_build / "unalzip"


@pytest.fixture(scope="session")
def refactored_sys_build(project_root):
    return run_make(project_root / "archivelib-sys-refactored")


@pytest.fixture
def refactored_sys_zip(refactored_sys_build):
    return refactored_sys_build / "alzip"


@pytest.fixture
def refactored_sys_unzip(refactored_sys_build):
    return refactored_sys_build / "unalzip"


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
            cwd=project_root,
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
