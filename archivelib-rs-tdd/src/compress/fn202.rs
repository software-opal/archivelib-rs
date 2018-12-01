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
  fn fn207(data: *mut RCompressData);
}
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
pub unsafe extern "C" fn fn202(
  mut data: *mut RCompressData,
  mut byte_or_run_length203: u16,
  mut _204: u16,
) {
  (*data).bitwise_counter185 = ((*data).bitwise_counter185 as libc::c_int >> 1i32) as u16;
  if (*data).bitwise_counter185 as libc::c_int == 0i32 {
    (*data).bitwise_counter185 = (1u32 << 8i32 - 1i32) as u16;
    if (*data).array165_counter as libc::c_int >= (*data).dat183_IS_CONST_8162 as libc::c_int {
      fn207(data);
      if 0 != (*data).uncompressible {
        return;
      } else {
        (*data).array165_counter = 0i32 as u16
      }
    }
    let fresh0 = (*data).array165_counter;
    (*data).array165_counter = (*data).array165_counter.wrapping_add(1);
    (*data).array165_tmp_counter186 = fresh0;
    *(*data)
      .dat_arr165
      .offset((*data).array165_tmp_counter186 as isize) = 0i32 as u8
  }
  let fresh1 = (*data).array165_counter;
  (*data).array165_counter = (*data).array165_counter.wrapping_add(1);
  *(*data).dat_arr165.offset(fresh1 as isize) = byte_or_run_length203 as u8;
  let ref mut fresh2 = *(*data).dat_arr191.offset(byte_or_run_length203 as isize);
  *fresh2 = (*fresh2).wrapping_add(1);
  if byte_or_run_length203 as libc::c_uint >= 1u32 << 8i32 {
    let ref mut fresh3 = *(*data)
      .dat_arr165
      .offset((*data).array165_tmp_counter186 as isize);
    *fresh3 =
      (*fresh3 as libc::c_int | (*data).bitwise_counter185 as u8 as libc::c_int) as u8;
    let fresh4 = (*data).array165_counter;
    (*data).array165_counter = (*data).array165_counter.wrapping_add(1);
    *(*data).dat_arr165.offset(fresh4 as isize) = _204 as u8;
    let fresh5 = (*data).array165_counter;
    (*data).array165_counter = (*data).array165_counter.wrapping_add(1);
    *(*data).dat_arr165.offset(fresh5 as isize) = (_204 as libc::c_int >> 8i32) as u8;
    byte_or_run_length203 = 0i32 as u16;
    while 0 != _204 {
      byte_or_run_length203 = byte_or_run_length203.wrapping_add(1);
      _204 = (_204 as libc::c_int >> 1i32) as u16
    }
    let ref mut fresh6 = *(*data).dat_arr193.offset(byte_or_run_length203 as isize);
    *fresh6 = (*fresh6).wrapping_add(1)
  };
}
