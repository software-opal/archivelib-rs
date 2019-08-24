import hashlib
import pathlib
import re
import sys

offset = [20, 24, 28]

ROOT = pathlib.Path(__file__).parent
NON_ALPHA = re.compile("(^[0-9]|[^a-zA-Z0-9_])")


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


def get_hus_vip_parts(data):
    attrs_off = le_bytes_to_int(data[20:24])
    x_coords_off = le_bytes_to_int(data[24:28])
    y_coords_off = le_bytes_to_int(data[28:32])
    return {
        "stitch_attrs": data[attrs_off:x_coords_off],
        "x_coords": data[x_coords_off:y_coords_off],
        "y_coords": data[y_coords_off:],
    }


def main():
    out_folder = ROOT / "src/test/match_sys/staging"
    out_folder.mkdir(exist_ok=True)
    fuzz_inputs = ROOT / "fuzz/known_inputs"
    out_folder.mkdir(exist_ok=True)
    for p in sys.argv[1:]:
        p = pathlib.Path(p)
        name = NON_ALPHA.sub("_", p.name)
        data = get_hus_vip_parts(p.read_bytes())

        with (out_folder / f"{name}.rs").open("w") as f:
            f.write("test_match_sys_decompress! {\n")
            for (name, compressed_data) in sorted(data.items()):
                f.write(
                    f"  // SHA1 of data: {hashlib.sha1(compressed_data).hexdigest()}\n"
                )
                f.write(f'  {name} => hex!("\n')
                f.write(bytes_to_test_hex(compressed_data))
                f.write('  "),\n')
            f.write("}\n")
        for compressed_data in sorted(data.values()):
            (fuzz_inputs / hashlib.sha1(compressed_data).hexdigest()).write_bytes(
                compressed_data
            )


if __name__ == "__main__":
    main()
