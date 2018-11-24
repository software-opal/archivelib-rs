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
  fn ALStorage_WriteBuffer(
    storage: *mut libc::c_void,
    buffer: *mut uint8_t,
    length: size_t,
  ) -> size_t;
  #[no_mangle]
  fn get_next_item(data: *mut RExpandData) -> uint16_t;
  #[no_mangle]
  fn calculate_run_offset(data: *mut RExpandData) -> uint16_t;
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
pub unsafe extern "C" fn Expand(mut data: *mut RExpandData) -> int32_t {
  let mut run_start226: int16_t = 0;
  let mut run_length276: int16_t = 0;
  let mut byte_or_run_length203: int16_t = 0;
  let mut buffer_pos: int16_t = 0;
  let mut l_uncompressed_buffer278: *mut uint8_t = 0 as *mut uint8_t;
  let mut max_size279: size_t = 0;
  let mut size_bitmask280: int16_t = 0;
  (*data).error_counter243 = 0i32 as int16_t;
  (*data).items_until_next_header = 0i32 as uint16_t;
  (*data).bits182 = 0i32 as uint16_t;
  (*data).tmp_bit_buffer245 = 0i32 as uint8_t;
  (*data).bits_in_buffer172 = 0i32 as int16_t;
  (*data).loaded_compressed_data_length246 = 0i32 as int16_t;
  l_uncompressed_buffer278 = (*data).uncompressed_buffer;
  max_size279 = (*data).max_uncompressed_data_size as size_t;
  size_bitmask280 = (*data).max_uncompressed_data_size_bitmask;
  buffer_pos = 0i32 as int16_t;
  // Seed bits182 with the first 2 bits
  read_bits(data, 2i32 * 8i32);
  while ((*data).error_counter243 as libc::c_int) < 5i32 {
    byte_or_run_length203 = get_next_item(data) as int16_t;
    if byte_or_run_length203 as libc::c_int <= 127i32 * 2i32 + 1i32 {
      // byte_or_run_length203 is the decompressed byte
      *l_uncompressed_buffer278.offset(buffer_pos as isize) = byte_or_run_length203 as uint8_t;
      buffer_pos += 1;
      if !(buffer_pos as size_t >= max_size279) {
        continue;
      }
      buffer_pos = 0i32 as int16_t;
      if !(ALStorage_WriteBuffer((*data).output_store, l_uncompressed_buffer278, max_size279)
        != max_size279)
      {
        continue;
      }
      return 0i32;
    } else {
      // Copy the run of `run_length276` bytes from earlier in the output.
      // byte_or_run_length203 >= 0x100 indicates a flag
      // run_length276 = byte_or_run_length203 - 0x100 + 3; which is the length
      // of the run. Flag value of byte_or_run_length203 ==
      run_length276 =
        (byte_or_run_length203 as libc::c_int - (127i32 * 2i32 + 1i32 + 1i32) + 3i32) as int16_t;
      if run_length276 as libc::c_int == 256i32 + 1i32 {
        // byte_or_run_length203 == 0x1FE. End of file.
        break;
      } else {
        run_start226 =
          (buffer_pos as libc::c_int - calculate_run_offset(data) as libc::c_int - 1i32
            & size_bitmask280 as libc::c_int) as int16_t;
        if (run_start226 as size_t) < max_size279
          .wrapping_sub(256i32 as libc::c_ulong)
          .wrapping_sub(1i32 as libc::c_ulong)
          && (buffer_pos as size_t) < max_size279
            .wrapping_sub(256i32 as libc::c_ulong)
            .wrapping_sub(1i32 as libc::c_ulong)
        {
          loop {
            run_length276 -= 1;
            if !(run_length276 as libc::c_int >= 0i32) {
              break;
            }
            let fresh1 = buffer_pos;
            buffer_pos = buffer_pos + 1;
            let fresh0 = run_start226;
            run_start226 = run_start226 + 1;
            *l_uncompressed_buffer278.offset(fresh1 as isize) =
              *l_uncompressed_buffer278.offset(fresh0 as isize)
          }
        } else {
          loop {
            run_length276 -= 1;
            if !(run_length276 as libc::c_int >= 0i32) {
              break;
            }
            *l_uncompressed_buffer278.offset(buffer_pos as isize) =
              *l_uncompressed_buffer278.offset(run_start226 as isize);
            buffer_pos += 1;
            if buffer_pos as size_t >= max_size279 {
              buffer_pos = 0i32 as int16_t;
              if ALStorage_WriteBuffer((*data).output_store, l_uncompressed_buffer278, max_size279)
                != max_size279
              {
                return 0i32;
              }
            }
            run_start226 =
              (run_start226 as libc::c_int + 1i32 & size_bitmask280 as libc::c_int) as int16_t
          }
        }
      }
    }
  }
  if buffer_pos as libc::c_int != 0i32 {
    ALStorage_WriteBuffer(
      (*data).output_store,
      l_uncompressed_buffer278,
      buffer_pos as size_t,
    );
  }
  return (0 == 0i32) as libc::c_int;
}
