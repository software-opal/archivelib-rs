extern crate archivelib_sys;

pub fn compress(data: &mut [u8]) -> Result<Box<[u8]>, String> {
  let length = data.len();
  let ptr = data.as_mut_ptr();
  let re = unsafe { archivelib_sys::compress(ptr, length) }.to_err();
  match re {
    Ok(res) => Ok(res.into_boxed_slice()),
    Err(Some(err)) => Err(String::from_utf8_lossy(err.as_bytes()).to_string()),
    Err(None) => Err("Unknown".to_string()),
  }
}

pub fn decompress(data: &mut [u8]) -> Result<Box<[u8]>, String> {
  let length = data.len();
  let ptr = data.as_mut_ptr();
  let re = unsafe { archivelib_sys::decompress(ptr, length) }.to_err();
  match re {
    Ok(res) => Ok(res.into_boxed_slice()),
    Err(Some(err)) => Err(String::from_utf8_lossy(err.as_bytes()).to_string()),
    Err(None) => Err("Unknown".to_string()),
  }
}

#[cfg(test)]
mod test2;

#[cfg(test)]
mod tests {
  use super::*;

  use std::iter::repeat;

  const SMALL_HEART_DATA: &[u8] = include_bytes!("../test_data/small_heart.hus");

  fn to_u32(d: &[u8]) -> u32 {
    return d[0] as u32 | ((d[1] as u32 | ((d[2] as u32 | ((d[3] as u32) << 8)) << 8)) << 8);
  }

  #[test]
  fn something() {
    let block1_start = to_u32(&SMALL_HEART_DATA[0x14..0x18]) as usize;
    let block2_start = to_u32(&SMALL_HEART_DATA[0x18..0x1C]) as usize;

    let mut data = vec![];
    data.extend(SMALL_HEART_DATA[block1_start..block2_start].iter());
    let out_data = decompress(&mut data).unwrap();
    let mut block1_expected: Vec<u8> = repeat(0x80).take(0x4D).collect();
    block1_expected.extend(repeat(0x81).take(3));
    block1_expected.extend(repeat(0x80).take(0x374 - 0x50));
    block1_expected.extend([0x81, 0x81, 0x81, 0x81, 0x90].iter());
    assert_eq!(out_data[..], block1_expected[..])
  }

}
