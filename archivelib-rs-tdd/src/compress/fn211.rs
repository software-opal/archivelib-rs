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
  fn fn230(
    data: *mut RCompressData,
    length219: i32,
    arg209: *mut u8,
    arg231: *mut u16,
  );
  #[no_mangle]
  fn fn228(data: *mut RCompressData, arg229: i32);
  #[no_mangle]
  fn fn225(
    data: *mut RCompressData,
    i: i32,
    arg187: *mut u16,
    arg177: *mut i16,
    arg227: i16,
  );
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
pub unsafe extern "C" fn fn211(
  mut data: *mut RCompressData,
  mut _212: i32,
  mut _213: *mut u16,
  mut _214: *mut u8,
  mut _215: *mut u16,
) -> i32 {
  let mut run_start226: i32 = 0;
  let mut run_length276: i32 = 0;
  let mut _289: i32 = 0;
  let mut _292: i32 = 0;
  let mut _227: i16 = 0;
  (*data).dat174 = _212 as i16;
  (*data).dat_arr_cursor187 = _213;
  (*data).dat_arr_cursor178 = _214;
  _292 = (*data).dat174 as i32;
  _227 = 0i32 as i16;
  *(*data).dat_arr177.offset(1isize) = 0i32 as i16;
  run_start226 = 0i32;
  while run_start226 < (*data).dat174 as libc::c_int {
    *(*data).dat_arr_cursor178.offset(run_start226 as isize) = 0i32 as u8;
    if 0 != *(*data).dat_arr_cursor187.offset(run_start226 as isize) {
      _227 += 1;
      *(*data).dat_arr177.offset(_227 as isize) = run_start226 as i16
    }
    run_start226 += 1
  }
  if (_227 as libc::c_int) < 2i32 {
    *_215.offset(*(*data).dat_arr177.offset(1isize) as isize) = 0i32 as u16;
    return *(*data).dat_arr177.offset(1isize) as i32;
  } else {
    run_start226 = _227 as libc::c_int / 2i32;
    while run_start226 >= 1i32 {
      fn225(
        data,
        run_start226,
        (*data).dat_arr_cursor187,
        (*data).dat_arr177,
        _227,
      );
      run_start226 -= 1
    }
    (*data).dat_arr_cursor188 = _215;
    loop {
      run_start226 = *(*data).dat_arr177.offset(1isize) as i32;
      if run_start226 < (*data).dat174 as libc::c_int {
        let fresh0 = (*data).dat_arr_cursor188;
        (*data).dat_arr_cursor188 = (*data).dat_arr_cursor188.offset(1);
        *fresh0 = run_start226 as u16
      }
      let fresh1 = _227;
      _227 = _227 - 1;
      *(*data).dat_arr177.offset(1isize) = *(*data).dat_arr177.offset(fresh1 as isize);
      fn225(
        data,
        1i32,
        (*data).dat_arr_cursor187,
        (*data).dat_arr177,
        _227,
      );
      run_length276 = *(*data).dat_arr177.offset(1isize) as i32;
      if run_length276 < (*data).dat174 as libc::c_int {
        let fresh2 = (*data).dat_arr_cursor188;
        (*data).dat_arr_cursor188 = (*data).dat_arr_cursor188.offset(1);
        *fresh2 = run_length276 as u16
      }
      let fresh3 = _292;
      _292 = _292 + 1;
      _289 = fresh3;
      *(*data).dat_arr_cursor187.offset(_289 as isize) =
        (*(*data).dat_arr_cursor187.offset(run_start226 as isize) as libc::c_int
          + *(*data).dat_arr_cursor187.offset(run_length276 as isize) as libc::c_int)
          as u16;
      *(*data).dat_arr177.offset(1isize) = _289 as i16;
      fn225(
        data,
        1i32,
        (*data).dat_arr_cursor187,
        (*data).dat_arr177,
        _227,
      );
      *(*data).dat_arr189.offset(_289 as isize) = run_start226 as u16;
      *(*data).dat_arr190.offset(_289 as isize) = run_length276 as u16;
      if !(_227 as libc::c_int > 1i32) {
        break;
      }
    }
    (*data).dat_arr_cursor188 = _215;
    fn228(data, _289);
    fn230(data, _212, _214, _215);
    return _289;
  };
}
