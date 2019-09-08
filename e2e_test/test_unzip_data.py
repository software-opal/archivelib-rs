import hashlib
import json
import pathlib

import pytest

E2E_TEST_DIR = pathlib.Path(__file__).resolve().parent


def load_test_data(name, levels=[0, 4]):
    with (E2E_TEST_DIR / f"{name}.json").open("r") as f:
        data = json.load(f)
    for input in map(bytes, data):
        hash = hashlib.sha1(input).hexdigest()
        for level in levels:
            yield pytest.param(level, input, id=f"level={level}; {hash}")


@pytest.mark.parametrize("level, input", load_test_data("short"))
def test_short(al_runner, level, input):
    out, err = al_runner.test_unzip(input, level=level)


@pytest.mark.parametrize("level, input", load_test_data("medium"))
def test_medium(al_runner, level, input):
    out, err = al_runner.test_unzip(input, level=level)


@pytest.mark.parametrize("level, input", load_test_data("long"))
def test_long(al_runner, level, input):
    out, err = al_runner.test_unzip(input, level=level)


@pytest.mark.parametrize("level, input", load_test_data("extra_long"))
def test_extra_long(al_runner, level, input):
    out, err = al_runner.test_unzip(input, level=level)
