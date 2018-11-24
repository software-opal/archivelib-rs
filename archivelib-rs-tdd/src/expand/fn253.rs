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
  fn get_bits(data: *mut RExpandData, bits_to_load219: uint8_t) -> uint16_t;
  #[no_mangle]
  fn fn258(
    data: *mut RExpandData,
    _259: int32_t,
    _260: *mut uint8_t,
    _261: int32_t,
    _262: *mut uint16_t,
    _263: uint16_t,
  );
  #[no_mangle]
  fn read_bits(data: *mut RExpandData, bits_to_load219: int32_t);
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
pub unsafe extern "C" fn fn253(
  mut data: *mut RExpandData,
  mut _254: int16_t,
  mut _220: int16_t,
  mut _221: int16_t,
) {
  let mut run_start226: int16_t = 0;
  let mut byte_or_run_length203: int16_t = 0;
  let mut bits_to_load219: int16_t = 0;
  let mut _283: uint16_t = 0;
  bits_to_load219 = get_bits(data, _220 as uint8_t) as int16_t;
  if bits_to_load219 as libc::c_int == 0i32 {
    byte_or_run_length203 = get_bits(data, _220 as uint8_t) as int16_t;
    run_start226 = 0i32 as int16_t;
    while (run_start226 as libc::c_int) < _254 as libc::c_int {
      *(*data).dat_arr181.offset(run_start226 as isize) = 0i32 as uint8_t;
      run_start226 += 1
    }
    run_start226 = 0i32 as int16_t;
    while (run_start226 as libc::c_int) < 256i32 {
      *(*data).dat_arr241.offset(run_start226 as isize) = byte_or_run_length203 as uint16_t;
      run_start226 += 1
    }
  } else {
    run_start226 = 0i32 as int16_t;
    while (run_start226 as libc::c_int) < bits_to_load219 as libc::c_int {
      byte_or_run_length203 = ((*data).bits182 as libc::c_int >> 13i32) as int16_t;
      if byte_or_run_length203 as libc::c_int == 7i32 {
        let mut bytes_read: size_t = 3i32 as size_t;
        _283 = (1u32 << 12i32) as uint16_t;
        while 0 != _283 as libc::c_int & (*data).bits182 as libc::c_int {
          _283 = (_283 as libc::c_int >> 1i32) as uint16_t;
          byte_or_run_length203 += 1;
          bytes_read = bytes_read.wrapping_add(1)
        }
        // +1 for the final bit that was zero
        read_bits(
          data,
          bytes_read.wrapping_add(1i32 as libc::c_ulong) as int32_t,
        );
      } else {
        read_bits(data, 3i32);
      }
      let fresh0 = run_start226;
      run_start226 = run_start226 + 1;
      *(*data).dat_arr181.offset(fresh0 as isize) = byte_or_run_length203 as uint8_t;
      if !(run_start226 as libc::c_int == _221 as libc::c_int) {
        continue;
      }
      byte_or_run_length203 = get_bits(data, 2i32 as uint8_t) as int16_t;
      while byte_or_run_length203 as libc::c_int > 0i32 {
        let fresh1 = run_start226;
        run_start226 = run_start226 + 1;
        *(*data).dat_arr181.offset(fresh1 as isize) = 0i32 as uint8_t;
        byte_or_run_length203 -= 1
      }
    }
    while (run_start226 as libc::c_int) < _254 as libc::c_int {
      *(*data).dat_arr181.offset(run_start226 as isize) = 0i32 as uint8_t;
      run_start226 += 1
    }
    fn258(
      data,
      _254 as int32_t,
      (*data).dat_arr181,
      8i32,
      (*data).dat_arr241,
      256i32 as uint16_t,
    );
  };
}
