import enum
import functools
import pathlib
import subprocess

import attr


class Executor:
    def __init__(self, zips: dict, unzips: dict):
        self.zips = {name: pathlib.Path(file) for name, file in zips.items()}
        self.unzips = {name: pathlib.Path(file) for name, file in unzips.items()}
        assert set(self.zips) == set(self.unzips)
        assert self.zips
        assert self.unzips
        for file in set(self.zips.values()) | set(self.unzips.values()):
            assert file.exists(), file
            assert file.is_file(), file

    def smoketest(self):
        (out, err) = self.test_zip(b"")
        assert err is None
        (out, err) = self.test_unzip(out)
        assert err is None
        assert out == b""

    @property
    def impls(self):
        return frozenset(self.zips)

    def exec(self, args, *, input=None, timeout=60, **kwargs):
        with subprocess.Popen(
            args,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            stdin=subprocess.PIPE,
            **kwargs,
        ) as proc:
            try:
                outs, errs = proc.communicate(input=input, timeout=timeout)
            except subprocess.TimeoutExpired:
                proc.kill()
                outs, errs = proc.communicate()
        return (proc.returncode, outs, errs)

    def _run(self, zip: bool, name: str, input: bytes, *, level=4):
        repo = self.zips if zip else self.unzips
        assert 0 <= level <= 4
        (rc, out, err) = self.exec([repo[name], f"-{level}"], input=input)

        error = Error.classify(err)
        assert error is False or error.type is not None, error
        # TODO: Convert the error into a error 'type'
        return (rc == 0, out, error)

    def _run_all(self, zip: bool, input: bytes, *, level=4):
        return {name: self._run(zip, name, input, level=level) for name in self.impls}

    def _test(self, zip: bool, input: bytes, level=4):
        results = self._run_all(zip, input, level=level)
        assert len(results) > 0
        has_err = any(not success for (success, out, err) in results.values())
        has_out = any(success for (success, out, err) in results.values())
        print(results)
        if has_err:
            for name, (success, out, err) in results.items():
                assert not success, f"{name} -> {out} | {err}"
            return (None, {name: err for name, (success, out, err) in results.items()})
        elif has_out:
            for name, (success, out, err) in results.items():
                assert success, f"{name} -> {out} | {err}"
            outs = {name: out for name, (success, out, err) in results.items()}
            out_groups = {
                out: frozenset(name for name, i_out in outs.items() if i_out == out)
                for out in set(outs.values())
            }
            first_out = next(iter(out_groups.keys()))
            assert out_groups == {first_out: self.impls}
            return (first_out, None)

    run_zip = functools.partialmethod(_run, True)
    run_all_zip = functools.partialmethod(_run_all, True)
    test_zip = functools.partialmethod(_test, True)

    run_unzip = functools.partialmethod(_run, False)
    run_all_unzip = functools.partialmethod(_run_all, False)
    test_unzip = functools.partialmethod(_test, False)


@enum.unique
class ErrorType(enum.Enum):

    INVARIANT = enum.auto()


@attr.s()
class Error:

    type = attr.ib()
    raw = attr.ib(cmp=False)

    def __bool__(self):
        return self.type is not True

    @classmethod
    def classify(cls, error):
        if not error:
            return False
        errors = [cls.classify_invariant(error)]
        for (etype, checks) in errors:
            if any(checks.values()):
                return cls(etype, error)
        import pprint

        assert False, f"All checks failed:\n{pprint.pformat(errors)}\n{error!r}"

    @classmethod
    def classify_invariant(cls, error):
        lines =[*map(bytes.strip, error.splitlines())]
        print(lines)
        return (
            ErrorType.INVARIANT,
            {
                "Rust Invariant Failure": b'Error: "Invariant Failure"' in lines,
                "Stack smashing": any(
                    b"*** stack smashing detected ***" in line for line in lines
                ),
                "ASAN": any(b"ERROR: AddressSanitizer:" in line for line in lines),
                "munmap_chunk": any(b'munmap_chunk(): invalid pointer'in line for line in lines),
            },
        )
