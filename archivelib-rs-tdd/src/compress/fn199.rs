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
pub type ALStorage = ();
pub type usize = libc::c_ulong;
pub type __i8 = libc::c_schar;
pub type __u8 = libc::c_uchar;
pub type __i16 = libc::c_short;
pub type __u16 = libc::c_ushort;
pub type i8 = __i8;
pub type i16 = __i16;
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
pub unsafe extern "C" fn fn199(
  mut data: *mut RCompressData,
  mut uncompressed_buffer_index200: i16,
  mut _201: i16,
) {
  let mut _451: *mut u8 = 0 as *mut u8;
  let mut l_uncompressed_buffer278: *mut u8 = 0 as *mut u8;
  let mut run_start226: i16 = 0;
  let mut _452: i16 = 0;
  let mut _204: i16 = 0;
  let mut _453: i16 = 0;
  _452 = 128i32 as i16;
  (*data).dat168 = 0i32 as i16;
  _451 = &mut *(*data)
    .uncompressed_buffer
    .offset(uncompressed_buffer_index200 as isize) as *mut u8;
  _204 = _201;
  loop {
    _204 = *(*data).dat_arr163.offset(_204 as isize);
    if !(_204 as libc::c_int != -1i32) {
      break;
    }
    _452 -= 1;
    if (_452 as libc::c_int) < 0i32 {
      break;
    }
    l_uncompressed_buffer278 =
      &mut *(*data).uncompressed_buffer.offset(_204 as isize) as *mut u8;
    if *_451.offset((*data).dat168 as isize) as libc::c_int
      != *l_uncompressed_buffer278.offset((*data).dat168 as isize) as libc::c_int
    {
      continue;
    }
    if *_451.offset(0isize) as libc::c_int
      != *l_uncompressed_buffer278.offset(0isize) as libc::c_int
    {
      continue;
    }
    if *_451.offset(1isize) as libc::c_int
      != *l_uncompressed_buffer278.offset(1isize) as libc::c_int
    {
      continue;
    }
    if *_451.offset(2isize) as libc::c_int
      != *l_uncompressed_buffer278.offset(2isize) as libc::c_int
    {
      continue;
    }
    run_start226 = 3i32 as i16;
    while (run_start226 as libc::c_int) < 256i32 {
      if *_451.offset(run_start226 as isize) as libc::c_int
        != *l_uncompressed_buffer278.offset(run_start226 as isize) as libc::c_int
      {
        break;
      }
      run_start226 += 1
    }
    if !(run_start226 as libc::c_int > (*data).dat168 as libc::c_int) {
      continue;
    }
    _453 = (uncompressed_buffer_index200 as libc::c_int - _204 as libc::c_int - 1i32) as i16;
    if (_453 as libc::c_int) < 0i32 {
      _453 = (_453 as libc::c_int + (*data).max_uncompressed_data_size as libc::c_int) as i16
    }
    if _453 as libc::c_int >= (*data).max_uncompressed_data_size as libc::c_int {
      break;
    }
    (*data).dat169 = _453;
    (*data).dat168 = run_start226;
    if (*data).dat168 as libc::c_int >= 256i32 {
      break;
    }
  }
}
