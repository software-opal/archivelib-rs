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
  fn write_bits_to_buffer(data: *mut RCompressData, arg209: i32, bits203: u16);
}
pub type ALStorage = ();
pub type usize = libc::c_ulong;
pub type __i8 = libc::c_schar;
pub type __u8 = libc::c_uchar;
pub type __i16 = libc::c_short;
pub type __u16 = libc::c_ushort;
pub type __i32 = libc::c_int;
pub type i8 = __i8;
pub type i16 = __i16;
pub type i32 = __i32;
pub type u8 = __u8;
pub type u16 = __u16;
pub type bool = i8;
#[derive(Copy, Clone)]
#[repr(C)]
pub struct RCompressData {
  pub input_store: *mut libc::c_void,
  pub output_store: *mut libc::c_void,
  pub dat_arr163: *mut i16,
  pub dat_arr164: *mut i16,
  pub dat_arr165: *mut u8,
  pub uncompressed_buffer: *mut u8,
  pub dat_arr167: *mut u16,
  pub dat_arr177: *mut i16,
  pub buffer: *mut u8,
  pub dat_arr180: *mut u8,
  pub dat_arr181: *mut u8,
  pub dat_arr189: *mut u16,
  pub dat_arr190: *mut u16,
  pub dat_arr191: *mut u16,
  pub dat_arr192: *mut u16,
  pub dat_arr193: *mut u16,
  pub dat_arr194: *mut u16,
  pub dat_arr163_len: usize,
  pub dat_arr164_len: usize,
  pub dat_arr165_len: usize,
  pub uncompressed_buffer_len: usize,
  pub dat_arr167_len: usize,
  pub dat_arr177_len: usize,
  pub buffer_len: usize,
  pub dat_arr180_len: usize,
  pub dat_arr181_len: usize,
  pub dat_arr189_len: usize,
  pub dat_arr190_len: usize,
  pub dat_arr191_len: usize,
  pub dat_arr192_len: usize,
  pub dat_arr193_len: usize,
  pub dat_arr194_len: usize,
  pub dat_arr_cursor178: *mut u8,
  pub dat_arr_cursor187: *mut u16,
  pub dat_arr_cursor188: *mut u16,
  pub chars_written: usize,
  pub input_length: usize,
  pub uncompressible: bool,
  pub fail_uncompressible: bool,
  pub dat168: i16,
  pub dat169: i16,
  pub buffer_position: i16,
  pub bits_buffer_used172: u16,
  pub dat173: i16,
  pub dat174: i16,
  pub max_uncompressed_data_size: i16,
  pub max_uncompressed_data_size_bitmask: i16,
  pub bits_buffer182: u16,
  pub dat183_IS_CONST_8162: u16,
  pub array165_counter: u16,
  pub bitwise_counter185: u16,
  pub array165_tmp_counter186: u16,
}
#[no_mangle]
pub unsafe extern "C" fn fn222(mut data: *mut RCompressData) {
  let mut run_start226: i16 = 0;
  let mut _289: i16 = 0;
  let mut bits_to_load219: i16 = 0;
  let mut _277: i16 = 0;
  bits_to_load219 = (127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32) as i16;
  while bits_to_load219 as libc::c_int > 0i32
    && *(*data)
      .dat_arr180
      .offset((bits_to_load219 as libc::c_int - 1i32) as isize) as libc::c_int
      == 0i32
  {
    bits_to_load219 -= 1
  }
  write_bits_to_buffer(data, 9i32, bits_to_load219 as u16);
  run_start226 = 0i32 as i16;
  while (run_start226 as libc::c_int) < bits_to_load219 as libc::c_int {
    let fresh0 = run_start226;
    run_start226 = run_start226 + 1;
    _289 = *(*data).dat_arr180.offset(fresh0 as isize) as i16;
    if _289 as libc::c_int == 0i32 {
      _277 = 1i32 as i16;
      while (run_start226 as libc::c_int) < bits_to_load219 as libc::c_int
        && *(*data).dat_arr180.offset(run_start226 as isize) as libc::c_int == 0i32
      {
        run_start226 += 1;
        _277 += 1
      }
      if _277 as libc::c_int <= 2i32 {
        _289 = 0i32 as i16;
        while (_289 as libc::c_int) < _277 as libc::c_int {
          write_bits_to_buffer(
            data,
            *(*data).dat_arr181.offset(0isize) as i32,
            *(*data).dat_arr194.offset(0isize),
          );
          _289 += 1
        }
      } else if _277 as libc::c_int <= 18i32 {
        write_bits_to_buffer(
          data,
          *(*data).dat_arr181.offset(1isize) as i32,
          *(*data).dat_arr194.offset(1isize),
        );
        write_bits_to_buffer(data, 4i32, (_277 as libc::c_int - 3i32) as u16);
      } else if _277 as libc::c_int == 19i32 {
        write_bits_to_buffer(
          data,
          *(*data).dat_arr181.offset(0isize) as i32,
          *(*data).dat_arr194.offset(0isize),
        );
        write_bits_to_buffer(
          data,
          *(*data).dat_arr181.offset(1isize) as i32,
          *(*data).dat_arr194.offset(1isize),
        );
        write_bits_to_buffer(data, 4i32, 15i32 as u16);
      } else {
        write_bits_to_buffer(
          data,
          *(*data).dat_arr181.offset(2isize) as i32,
          *(*data).dat_arr194.offset(2isize),
        );
        write_bits_to_buffer(data, 9i32, (_277 as libc::c_int - 20i32) as u16);
      }
    } else {
      write_bits_to_buffer(
        data,
        *(*data)
          .dat_arr181
          .offset((_289 as libc::c_int + 2i32) as isize) as i32,
        *(*data)
          .dat_arr194
          .offset((_289 as libc::c_int + 2i32) as isize),
      );
    }
  }
}
