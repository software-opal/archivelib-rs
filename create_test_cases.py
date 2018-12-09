import yaml
import sys
import textwrap
import pathlib


def test_case_for_fn230(call):
    return None
    run_length = run_length
    lookup_table_pre = call["data"]["lookup_table288_pre"]["content"]
    lookup_table_post = call["data"]["lookup_table288_post"]["content"]
    dat_arr167 = call["data"]["dat_arr167"]["content"]
    item209 = call["data"]["item209"]["content"]
    result = call["data"]["_231"]["content"][:run_length]
    return (
        "test_fn230",
        f"""
// Lookup table: {lookup_table_pre}
let result = pure_fn230(
  {run_length},
  &{dat_arr167},
  &{item209},
);
// Lookup table: {lookup_table_post}
assert_eq!(
  result,
  vec!{result},
);""",
    )


def test_case_for_fn199(call):
    max_data_size = call["data"]["max_uncompressed_data_size"]
    start_index = call["data"]["uncompressed_buffer_index200"]
    test_index = call["data"]["_201"]
    dat_arr163 = [
        +(val if val < 0x8000000000000000 else val - 0x10000000000000000)
        for val in call["data"]["dat_arr163"]["content"]
    ]
    uncompressed_buffer = call["data"]["uncompressed_buffer"]["content"]
    if "dat168" in call["data"]:
        dat168 = call["data"]["dat168"]
        dat169 = "Some(%d)" % call["data"]["dat169"]
    else:
        dat168 = "0"
        dat169 = "None"

    return (
        "test_fn199_for_embroidermodder_hus_stitch_attrs",
        f"""
let result = pure_fn199(
  &{dat_arr163},
  &{uncompressed_buffer},
  {max_data_size},  // max_data_size
  {start_index},  // start_index
  {test_index},  // test_index
);
assert_eq!(result, ({dat168}, {dat169}));""",
    )


def test_case_for_fn228(call):
    if "dat_arr_cursor178_after" in call["data"]:
        return test_case_for_fn228_full_call(call)
    else:
        return test_case_for_fn228_depth_generation(call)


def test_case_for_fn228_full_call(call):
    var229 = call["data"]["_229"]
    dat174 = call["data"]["dat174"]
    dat_arr189 = call["data"]["dat_arr189"]["content"]
    dat_arr190 = call["data"]["dat_arr190"]["content"]
    dat_arr167 = call["data"]["dat_arr167"]["content"]
    dat_arr_cursor178 = call["data"]["dat_arr_cursor178"]["content"]
    dat_arr_cursor188 = call["data"]["dat_arr_cursor188"]["content"]
    dat_arr_cursor178_post = call["data"]["dat_arr_cursor178_after"]["content"]
    return (
        "test_fn228_full_call",
        f"""
let input = [0u8; 0];
let mut output = [0u8; 0];
let mut cd = RCompressData::new(&input[..], &mut output[..], 0, 10, true).unwrap();
cd.dat_arr189 = vec!{dat_arr189};
cd.dat_arr190 = vec!{dat_arr190};
cd.dat174 = {dat174};
let mut dat_arr_cursor178 = vec!{dat_arr_cursor178};
let mut dat_arr_cursor188 = vec!{dat_arr_cursor188};
let result = pure_fn228(
  {var229},
  &mut CompressU8ArrayAlias::Custom(0, &mut dat_arr_cursor178),
  &CompressU16ArrayAlias::Custom(0, &mut dat_arr_cursor188),
);
assert_eq!(dat_arr_cursor178, vec!{dat_arr_cursor178_post});""",
    )


def test_case_for_fn228_depth_generation(call):
    initial_index = call["data"]["_229"]
    series_start = call["data"]["dat174"]
    dat_arr189 = call["data"]["dat_arr189"]["content"]
    dat_arr190 = call["data"]["dat_arr190"]["content"]
    dat_arr167 = call["data"]["dat_arr167"]["content"]
    return (
        "test_fn228",
        f"""
let result = pure_fn228(
  &{dat_arr189},
  &{dat_arr190},
  {series_start},  // series_start
  {initial_index},  // initial_index
);
assert_eq!(result, {dat_arr167});""",
    )


def test_case_for_fn225(call):
    var187 = call["data"]["_187"]["content"]
    var177 = call["data"]["_177"]["content"]
    var177_after = call["data"]["_177_after"]["content"]
    run_start226 = call['data']['run_start226']
    _227 = call['data']['_227']
    return (
        "test_fn225",
        f"""
let mut var177 = vec!{var177};
let var187 = vec!{var187};
pure_fn225(
  {run_start226},
  &var187,
  &mut var177,
  {_227},
);
assert_eq!(var177, vec!{var177_after});""",
    )


def main():
    test_cases = set()
    for file in map(pathlib.Path, sys.argv[1:]):
        for call in yaml.safe_load_all(file.open()):
            res = None
            print(call["func"])
            if call["func"] == "fn230":
                res = test_case_for_fn230(call)
            elif call["func"] == "fn199":
                res = test_case_for_fn199(call)
            elif call["func"] == "fn228":
                res = test_case_for_fn228(call)
            elif call["func"] == "fn225":
                res = test_case_for_fn225(call)
            if res:
                test_cases.add(res)
            if len(test_cases) > 10:
                break
        if len(test_cases) > 10:
            break

    test_group_names = set(name for name, _ in test_cases)
    test_groups = {
        name: set(test for n, test in test_cases if n == name)
        for name in test_group_names
    }
    for name, test_cases in test_groups.items():
        with pathlib.Path(f"{name}.rs").open("w") as f:
            lines = ["#[cfg(test)]", "mod tests {", "  use super::*;"]
            for i, test in enumerate(test_cases):
                lines += ["", "  #[test]", f"  fn {name}_{i}() {{"]
                lines += textwrap.indent(test.strip(), " " * 4).splitlines()
                lines += ["  }"]
            lines += ["}"]
            f.write("\n".join(l.rstrip() for l in lines))


if __name__ == "__main__":
    main()
