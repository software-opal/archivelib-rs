use super::table::LookupTables;
use crate::support::{
  CorrectLookAheadBitwiseRead, CorrectLookAheadBitwiseReader, ExpectedCallLookAheadBitwiseReader,
  LookAheadBitwiseRead,
};

#[test]
fn incorrect_table_generation_from_corner_cases_single_byte_wrong() {
  let data: Vec<u8> = vec![
    0x11, 0x30, 0xfa, 0x0d, 0x66, 0xdc, 0x34, 0x1a, 0x48, 0xbf, 0xff, 0x49, 0x52, 0x54, 0x9d, 0xf4,
    0x5e, 0x1f, 0x61, 0x6a, 0x47, 0xe2, 0xe6, 0x47, 0x41, 0x6d, 0x11, 0x45, 0xc1, 0x54, 0x5c, 0x3b,
    0x76, 0x5c, 0x40, 0x6c, 0xfc, 0x15, 0x45, 0xc0, 0xb5, 0x8a, 0x2d, 0x8b, 0xba, 0x2d, 0x8b, 0x98,
    0x2f, 0x05, 0xef, 0x59, 0x51, 0xaf, 0x1d, 0x88, 0x3d, 0xfe, 0xfa,
  ];
  let consume_calls = {
    // We need to prime the reader so it's in the right place
    let mut c = vec![6, 16];
    // Then these are the calls that the system library makes
    c.extend(vec![
      5, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 9, 6, 3, 3, 4, 3, 3, 4, 3, 3, 5, 6, 2, 4,
      5, 2, 4, 4, 2, 4, 4, 2, 2, 4, 6, 2, 4, 4, 4, 3, 2, 4, 2, 4, 2, 4, 2, 2, 4, 2, 2, 2, 4, 4, 2,
      4, 2, 2, 2, 4, 4, 2, 4, 4, 4, 3, 4, 3, 2, 2, 4, 2, 4, 2, 4, 4, 2, 4, 2, 2, 2, 4, 4, 2, 4, 4,
      2, 3, 2, 2, 2, 4, 4, 2, 4, 4, 4, 2, 4, 4, 2, 4, 4, 4, 2, 4, 5, 2, 4, 5, 5, 2, 3, 4, 3, 2, 4,
      6, 9, 2, 5, 5,
    ]);
    c
  };
  let mut reader = CorrectLookAheadBitwiseReader::new(
    ExpectedCallLookAheadBitwiseReader::new_correct(data, &consume_calls),
  );
  reader.consume_bits(6).unwrap();
  assert_eq!(reader.consume::<u16>(16).unwrap(), 0x4c3e);

  let mut lookups = LookupTables::new();
  lookups.bit_lookup.iter_mut().for_each(|i| *i = 255);
  lookups.bit_lookup_len.iter_mut().for_each(|i| *i = 255);
  lookups.run_offset_lookup.iter_mut().for_each(|i| *i = 255);
  lookups
    .run_offset_lookup_len
    .iter_mut()
    .for_each(|i| *i = 255);
  lookups.tree.left.iter_mut().for_each(|i| *i = 255);
  lookups.tree.right.iter_mut().for_each(|i| *i = 255);
  lookups.generate(&mut reader).unwrap();

  assert_bytes_eq!(
    {
      let mut v = vec![19; 1024];
      v.extend(vec![237; 1024]);
      v.extend(vec![9; 512]);
      v.extend(vec![246; 512]);
      v.extend(vec![247; 512]);
      v.extend(vec![10; 256]);
      v.extend(vec![0; 64]);
      v.extend(vec![3; 32]);
      v.extend(vec![6; 32]);
      v.extend(vec![250; 32]);
      v.extend(vec![253; 32]);
      v.extend(vec![57; 8]);
      v.extend(vec![38; 4]);
      v.extend(vec![76; 4]);
      v.extend(vec![135; 4]);
      v.extend(vec![142; 4]);
      v.extend(vec![199; 4]);
      v.extend(vec![218; 4]);
      v.extend(vec![
        66, 66, 67, 67, 95, 95, 114, 114, 161, 161, 180, 180, 227, 227, 228, 228, 44, 70, 132, 133,
        186, 208, 209, 511, 512, 513, 514, 515, 516, 517, 518, 519,
      ]);
      v
    },
    lookups.bit_lookup
  );
  assert_bytes_eq!(
    [
      6, 0, 0, 7, 0, 0, 7, 0, 0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 12, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0, 0, 0,
      0, 0, 0, 0, 0, 11, 11, 0, 13, 12, 0, 0, 0, 0, 13, 10, 13, 0, 0, 0, 0, 0, 0, 0, 13, 13, 0, 0,
      0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 13, 13, 0, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 0, 10, 0, 13, 0, 0, 0, 13, 10, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 13, 13, 0, 0, 0, 0, 0, 0,
      0, 0, 11, 0, 0, 0, 0, 0, 12, 13, 0, 13, 13, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0,
      0, 12, 12, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 11, 11, 0, 0, 0, 0, 0, 0, 0,
      0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 13, 0, 7, 0, 13, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 13
    ],
    lookups.bit_lookup_len
  );
  assert_bytes_eq!(vec![30; 256], lookups.run_offset_lookup);
  assert_bytes_eq!(
    {
      let mut v = vec![0; 15];
      v.extend(vec![2, 0, 0, 0]);
      v
    },
    lookups.run_offset_lookup_len
  );
  assert_bytes_eq!(
    {
      let mut v = vec![0; 511];
      v.extend(vec![45, 75, 85, 104, 137, 170, 187, 190, 252]);
      v.extend(vec![0; 501]);
      v
    },
    lookups.tree.left
  );
  assert_bytes_eq!(
    {
      let mut v = vec![0; 511];
      v.extend(vec![69, 77, 86, 105, 141, 171, 189, 248, 510]);
      v.extend(vec![0; 501]);
      v
    },
    lookups.tree.right
  );
}
