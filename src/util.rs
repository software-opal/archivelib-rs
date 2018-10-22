
use std::fs::File;
use std::io;
use std::io::Write;

fn as_readable(data: &[u8]) -> String {
  data
    .iter()
    .map(|b| format!("{:02X}", b))
    .collect::<Vec<_>>()
    .chunks(32)
    .map(|s| {
      s.chunks(8)
        .map(|s| s.join(" "))
        .collect::<Vec<_>>()
        .join("  ")
    })
    .collect::<Vec<_>>()
    .join("\n")
}

fn write_block(name: String, compressed_data: &[u8]) -> io::Result<()> {
  let mut data = vec![];
  data.extend(compressed_data);
  File::create(format!("{}.compressed.txt", name))?.write_all(as_readable(&data).as_bytes())?;
  File::create(format!("{}.decompressed.txt", name))?
    .write_all(as_readable(&decompress(&mut data).unwrap()).as_bytes())?;
  Ok(())
}

fn debug_hus(name: &str, data: &[u8]) {
  let block1_start = to_u32(&data[0x14..0x18]) as usize;
  let block2_start = to_u32(&data[0x18..0x1C]) as usize;
  let block3_start = to_u32(&data[0x1C..0x20]) as usize;
  let end = data.len();

  write_block(format!("{}{}", name, 1), &data[block1_start..block2_start]).unwrap();
  write_block(format!("{}{}", name, 2), &data[block2_start..block3_start]).unwrap();
  write_block(format!("{}{}", name, 3), &data[block3_start..end]).unwrap();
}

#[test]
fn something_else() {
  debug_hus(
    "small_heart",
    include_bytes!("../test_data/small_heart.hus"),
  );
  debug_hus(
    "embroidermodder",
    include_bytes!("../test_data/embroidermodder.hus"),
  );
  debug_hus("star", include_bytes!("../test_data/star.hus"));
}
