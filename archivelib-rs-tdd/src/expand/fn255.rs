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
pub unsafe extern "C" fn fn255(mut data: *mut RExpandData) {
  let mut run_start226: int16_t = 0;
  let mut byte_or_run_length203: int16_t = 0;
  let mut bits_to_load219: int16_t = 0;
  let mut _283: uint16_t = 0;
  bits_to_load219 = get_bits(data, 9i32 as uint8_t) as int16_t;
  if bits_to_load219 as libc::c_int == 0i32 {
    byte_or_run_length203 = get_bits(data, 9i32 as uint8_t) as int16_t;
    run_start226 = 0i32 as int16_t;
    while (run_start226 as libc::c_int) < 127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32
    {
      *(*data).dat_arr180.offset(run_start226 as isize) = 0i32 as uint8_t;
      run_start226 += 1
    }
    run_start226 = 0i32 as int16_t;
    while (run_start226 as libc::c_int) < 4096i32 {
      *(*data).dat_arr240.offset(run_start226 as isize) = byte_or_run_length203 as uint16_t;
      run_start226 += 1
    }
  } else {
    run_start226 = 0i32 as int16_t;
    while (run_start226 as libc::c_int) < bits_to_load219 as libc::c_int {
      byte_or_run_length203 = *(*data)
        .dat_arr241
        .offset(((*data).bits182 as libc::c_int >> 8i32) as isize)
        as int16_t;
      if byte_or_run_length203 as libc::c_int >= 16i32 + 3i32 {
        _283 = (1u32 << 7i32) as uint16_t;
        loop {
          if 0 != (*data).bits182 as libc::c_int & _283 as libc::c_int {
            byte_or_run_length203 =
              *(*data).dat_arr190.offset(byte_or_run_length203 as isize) as int16_t
          } else {
            byte_or_run_length203 =
              *(*data).dat_arr189.offset(byte_or_run_length203 as isize) as int16_t
          }
          _283 = (_283 as libc::c_int >> 1i32) as uint16_t;
          if !(byte_or_run_length203 as libc::c_int >= 16i32 + 3i32) {
            break;
          }
        }
      }
      read_bits(
        data,
        *(*data).dat_arr181.offset(byte_or_run_length203 as isize) as int32_t,
      );
      if byte_or_run_length203 as libc::c_int <= 2i32 {
        if byte_or_run_length203 as libc::c_int == 0i32 {
          byte_or_run_length203 = 1i32 as int16_t
        } else if byte_or_run_length203 as libc::c_int == 1i32 {
          byte_or_run_length203 = (get_bits(data, 4i32 as uint8_t) as libc::c_int + 3i32) as int16_t
        } else {
          byte_or_run_length203 =
            (get_bits(data, 9i32 as uint8_t) as libc::c_int + 20i32) as int16_t
        }
        loop {
          byte_or_run_length203 -= 1;
          if !(byte_or_run_length203 as libc::c_int >= 0i32) {
            break;
          }
          let fresh0 = run_start226;
          run_start226 = run_start226 + 1;
          *(*data).dat_arr180.offset(fresh0 as isize) = 0i32 as uint8_t
        }
      } else {
        let fresh1 = run_start226;
        run_start226 = run_start226 + 1;
        *(*data).dat_arr180.offset(fresh1 as isize) =
          (byte_or_run_length203 as libc::c_int - 2i32) as uint8_t
      }
    }
    while (run_start226 as libc::c_int) < 127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32
    {
      let fresh2 = run_start226;
      run_start226 = run_start226 + 1;
      *(*data).dat_arr180.offset(fresh2 as isize) = 0i32 as uint8_t
    }
    fn258(
      data,
      127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32,
      (*data).dat_arr180,
      12i32,
      (*data).dat_arr240,
      4096i32 as uint16_t,
    );
  };
}
