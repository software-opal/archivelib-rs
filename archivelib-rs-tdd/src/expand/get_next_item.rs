#![allow(
  dead_code,
  mutable_transmutes,
  non_camel_case_types,
  non_snake_case,
  non_upper_case_globals,
  unused_mut
)]
#![feature(libc)]
extern crate libc;
extern "C" {
  #[no_mangle]
  fn read_bits(data: *mut RExpandData, bits_to_load219: int32_t);
  #[no_mangle]
  fn fn253(data: *mut RExpandData, _254: int16_t, _220: int16_t, _221: int16_t);
  #[no_mangle]
  fn fn255(data: *mut RExpandData);
  #[no_mangle]
  fn get_bits(data: *mut RExpandData, bits_to_load219: uint8_t) -> uint16_t;
}
pub type ALStorage = ();
pub type ptrdiff_t = libc::c_long;
pub type size_t = libc::c_ulong;
pub type __uint8_t = libc::c_uchar;
pub type __int16_t = libc::c_short;
pub type __uint16_t = libc::c_ushort;
pub type __int32_t = libc::c_int;
pub type int16_t = __int16_t;
pub type int32_t = __int32_t;
pub type uint8_t = __uint8_t;
pub type uint16_t = __uint16_t;
pub type ssize_t = ptrdiff_t;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct RExpandData {
  pub input_store: *mut libc::c_void,
  pub output_store: *mut libc::c_void,
  pub error: int32_t,
  pub uncompressed_buffer: *mut uint8_t,
  pub dat_arr180: *mut uint8_t,
  pub dat_arr181: *mut uint8_t,
  pub dat_arr189: *mut uint16_t,
  pub dat_arr190: *mut uint16_t,
  pub dat_arr240: *mut uint16_t,
  pub dat_arr241: *mut uint16_t,
  pub compressed_data_buffer242: *mut uint8_t,
  pub uncompressed_buffer_len: size_t,
  pub dat_arr180_len: size_t,
  pub dat_arr181_len: size_t,
  pub dat_arr189_len: size_t,
  pub dat_arr190_len: size_t,
  pub dat_arr240_len: size_t,
  pub dat_arr241_len: size_t,
  pub compressed_data_buffer242_len: size_t,
  pub compressed_data_index: size_t,
  pub bits_in_buffer172: int16_t,
  pub max_uncompressed_data_size: int16_t,
  pub max_uncompressed_data_size_bitmask: int16_t,
  pub bits182: uint16_t,
  pub error_counter243: int16_t,
  pub items_until_next_header: uint16_t,
  pub tmp_bit_buffer245: uint8_t,
  pub loaded_compressed_data_length246: int16_t,
  pub compressed_data_length248: ssize_t,
}
#[no_mangle]
pub unsafe extern "C" fn get_next_item(mut data: *mut RExpandData) -> uint16_t {
  let mut run_length276: uint16_t = 0;
  let mut _283: uint16_t = 0;
  if (*data).items_until_next_header as libc::c_int == 0i32 {
    // This is the first 2 bytes in the file, and it represents the number of
    // calls that this header can handle. It's not exactly the number of bytes
    // because we read a variable number of bits per call.
    (*data).items_until_next_header = get_bits(data, 16i32 as uint8_t);
    fn253(
      data,
      (16i32 + 3i32) as int16_t,
      5i32 as int16_t,
      3i32 as int16_t,
    );
    fn255(data);
    fn253(
      data,
      (14i32 + 1i32) as int16_t,
      5i32 as int16_t,
      -1i32 as int16_t,
    );
    if 0 != (*data).error {
      return 0i32 as uint16_t;
    }
  }
  (*data).items_until_next_header = (*data).items_until_next_header.wrapping_sub(1);
  run_length276 = *(*data)
    .dat_arr240
    .offset(((*data).bits182 as libc::c_int >> 4i32) as isize);
  // run_length276 <= 0xFF are the uncompressed bits.
  // 0x100 <= run_length276 <= 0x1FE are runs (run_length276 - 0x100 + 3) bits
  // long
  if run_length276 as libc::c_int >= 127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32 {
    // No test cases exercise this condition.
    _283 = (1u32 << 3i32) as uint16_t;
    loop {
      if 0 != (*data).bits182 as libc::c_int & _283 as libc::c_int {
        run_length276 = *(*data).dat_arr190.offset(run_length276 as isize)
      } else {
        run_length276 = *(*data).dat_arr189.offset(run_length276 as isize)
      }
      _283 = (_283 as libc::c_int >> 1i32) as uint16_t;
      if !(run_length276 as libc::c_int
        >= 127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32)
      {
        break;
      }
    }
  }
  read_bits(
    data,
    *(*data).dat_arr180.offset(run_length276 as isize) as int32_t,
  );
  return run_length276;
}
