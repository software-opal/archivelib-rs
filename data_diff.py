import pathlib
import sys

import attr
import yaml


@attr.s(repr=False)
class NiceList:

    l = attr.ib()

    def __str__(self):
        return repr(self)

    def __repr__(self):
        a = format_squashed(to_squashed(self.l))
        b = repr(self.l)
        # if len(a) < len(b):
        return a
        # else:
        # return b


EXTRACT_SCALARS = [
    "compressed_data_index",
    "bits_in_buffer172",
    "max_uncompressed_data_size",
    "max_uncompressed_data_size_bitmask",
    "bits182",
    "error_counter243",
    "items_until_next_header",
    "tmp_bit_buffer245",
    "loaded_compressed_data_length246",
    "compressed_data_length248",
]
EXTRACT_ARRAYS = [
    "uncompressed_buffer",
    "dat_arr180",
    "dat_arr181",
    "dat_arr189",
    "dat_arr190",
    "dat_arr240",
    "dat_arr241",
    "compressed_data_buffer242",
]
ExtractTracePoint = attr.make_class(
    "ExtractTracePoint",
    ["file", "line", "pointer", "func", "now", "inpos", "outpos"]
    + EXTRACT_SCALARS
    + EXTRACT_ARRAYS,
)


def convert_trace(trace):

    base = dict(
        file=trace["file"],
        line=trace["line"],
        func=trace["func"],
        now=trace["now"],
        pointer=trace["data"].pop("ptr"),
        inpos=trace["data"].pop("input_store")["position"],
        outpos=trace["data"].pop("output_store")["position"],
    )
    if set(trace["data"]) & set(EXTRACT_SCALARS) == set(EXTRACT_SCALARS):
        tp = ExtractTracePoint(
            **base,
            **scalars_from_data(trace["data"], EXTRACT_SCALARS),
            **arrays_from_data(trace["data"], EXTRACT_ARRAYS),
        )
        assert not trace["data"], repr(trace["data"])
        return tp
    else:
        assert False


def scalars_from_data(data, names):
    return {name: data.pop(name) for name in names}


def arrays_from_data(data, names):
    return {name: NiceList(data.pop(name)["content"]) for name in names}


def format_squashed(squash, skip_none=False):
    out = "blocked_set!["
    for start, end, item in squash:
        if item is None:
            continue
        if start == end - 1:
            out += f"{start} => {item}, "
        else:
            out += f"{start}; {end} => {item}, "
    return out + "]"


def to_squashed(arr):
    if not arr:
        return
    start = 0
    first = arr[0]
    for idx, item in enumerate(arr):
        if item != first:
            yield (start, idx, first)
            start, first = idx, item
    yield (start, len(arr), first)


def main():
    runs = map(
        lambda runs: map(convert_trace, runs),
        (yaml.load_all(pathlib.Path(path).read_text()) for path in sys.argv[1:]),
    )
    for run in runs:
        run = iter(run)
        start = next(run)
        last = start
        for trace in run:
            if trace == last:
                continue
            lines = []
            for s in EXTRACT_SCALARS:
                if getattr(last, s) != getattr(trace, s):
                    lines.append(f"  {s} -#{getattr(last, s)} +#{getattr(trace, s)}")
            for s in EXTRACT_ARRAYS:
                if getattr(last, s) != getattr(trace, s):
                    old = getattr(last, s).l
                    new = getattr(trace, s).l
                    zipped = [
                        (o, n) if n != o else (None, None) for (o, n) in zip(old, new)
                    ]
                    old = format_squashed(
                        to_squashed([o for o, _ in zipped]), skip_none=True
                    )
                    new = format_squashed(
                        to_squashed([n for _, n in zipped]), skip_none=True
                    )
                    lines.append(f"  {s}")
                    lines.append(f"    - {old}")
                    lines.append(f"    + {new}")
            if lines:
                print(f"---{last.file}:{last.line} -- {last.func}({last.now})")
                print(f"+++{trace.file}:{trace.line} -- {trace.func}({trace.now})")
                print("\n".join(lines))
                last = trace
            if trace.func == "get_next_item":
                print(trace)


if __name__ == "__main__":
    main()
