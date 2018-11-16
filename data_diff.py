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


def convert_trace(trace):

    base = dict(
        file=trace.pop("file"),
        line=trace.pop("line"),
        func=trace.pop("func"),
        now=trace.pop("now"),
        pointer=trace["data"].pop("ptr"),
        inpos=trace["data"].pop("input_store")["position"],
        outpos=trace["data"].pop("output_store")["position"],
        # extra={},
    )
    if set(trace["data"]) & set(EXTRACT_SCALARS) == set(EXTRACT_SCALARS):
        tp = ExtractTracePoint(
            **base,
            **scalars_from_data(trace["data"], EXTRACT_SCALARS),
            **arrays_from_data(trace["data"], EXTRACT_ARRAYS),
        )
    elif set(trace["data"]) & set(COMPRESS_SCALARS) == set(COMPRESS_SCALARS):
        tp = CompressTracePoint(
            **base,
            **scalars_from_data(trace["data"], COMPRESS_SCALARS),
            **arrays_from_data(trace["data"], COMPRESS_ARRAYS),
        )
        trace["data"].pop("dat_arr_cursor178")
        trace["data"].pop("dat_arr_cursor187")
        trace["data"].pop("dat_arr_cursor188")
    else:
        print(trace)
        assert False
    assert not trace["data"], repr(trace["data"])
    return tp


def scalars_from_data(data, names):
    return {name: data.pop(name) for name in names}


def arrays_from_data(data, names):
    return {name: NiceList(data.pop(name)["content"]) for name in names}


def create_differ(scalars, arrays):
    def differ(last, trace):
        lines = []
        for s in scalars:
            if getattr(last, s) != getattr(trace, s):
                lines.append(f"  {s} -#{getattr(last, s)} +#{getattr(trace, s)}")
        for s in arrays:
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
        return lines

    return differ


def create_pretty_printer(scalars, arrays):
    def pretty_printer(last):
        lines = []
        for s in scalars:
            lines.append(f"  {s} = {getattr(last, s)}")
        if lines:
            lines.append("  //")
        for s in arrays:
            lines.append(f"  {s} = {getattr(last, s)}")
        return lines

    return pretty_printer


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


def make(name, scalars, arrays):
    cls = attr.make_class(
        name,
        ["file", "line", "pointer", "func", "now", "inpos", "outpos"]
        + scalars
        + arrays,
    )
    cls.diff = create_differ(scalars, arrays)
    cls.print = create_pretty_printer(scalars, arrays)
    return cls


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
COMPRESS_SCALARS = [
    "chars_written",
    "input_length",
    "uncompressible",
    "fail_uncompressible",
    "dat168",
    "dat169",
    "buffer_position",
    "bits_buffer_used172",
    "dat173",
    "dat174",
    "max_uncompressed_data_size",
    "max_uncompressed_data_size_bitmask",
    "bits_buffer182",
    "dat183_IS_CONST_8162",
    "array165_counter",
    "bitwise_counter185",
    "array165_tmp_counter186",
]
COMPRESS_ARRAYS = [
    "dat_arr163",
    "dat_arr164",
    "dat_arr165",
    "uncompressed_buffer",
    "dat_arr167",
    "dat_arr177",
    "buffer",
    "dat_arr180",
    "dat_arr181",
    "dat_arr189",
    "dat_arr190",
    "dat_arr191",
    "dat_arr192",
    "dat_arr193",
    "dat_arr194",
]
ExtractTracePoint = make("ExtractTracePoint", EXTRACT_SCALARS, EXTRACT_ARRAYS)
CompressTracePoint = make("CompressTracePoint", COMPRESS_SCALARS, COMPRESS_ARRAYS)


def main():
    runs = map(
        lambda runs: map(convert_trace, runs),
        (yaml.load_all(pathlib.Path(path).read_text()) for path in sys.argv[1:]),
    )
    for run in runs:
        run = iter(run)
        start = next(run)
        print(f"\n\n-----------{start.pointer}-----------\n")
        last = start
        for trace in run:
            if trace == last:
                continue
            lines = last.diff(trace)
            if lines:
                print(f"---{last.file}:{last.line} -- {last.func}({last.now})")
                print(f"+++{trace.file}:{trace.line} -- {trace.func}({trace.now})")
                print("\n".join(lines))
                last = trace
            # if trace.func in ["Expand"]:
        print("\n".join(start.print()))


if __name__ == "__main__":
    main()
