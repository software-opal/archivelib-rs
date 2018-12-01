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
pub unsafe extern "C" fn fn230(
  mut data: *mut RCompressData,
  mut bits_to_load219: i32,
  mut item209: *mut u8,
  mut _231: *mut u16,
) {
  // Sibling method is fn258
  // Called with:
  // (CONST_N141_IS_511, dat_arr180, dat_arr192)
  // (CONST_N145_IS_19, dat_arr181, dat_arr194)
  // (CONST_N142_IS_15, dat_arr181, dat_arr194)
  let mut run_start226: i32 = 0;
  let mut lookup_table288: [u16; 18] = [0; 18];
  lookup_table288[1usize] = 0i32 as u16;
  run_start226 = 1i32;
  while run_start226 <= 16i32 {
    lookup_table288[(run_start226 + 1i32) as usize] = ((lookup_table288[run_start226 as usize]
      as libc::c_int
      + *(*data).dat_arr167.offset(run_start226 as isize) as libc::c_int)
      << 1i32) as u16;
    run_start226 += 1
  }
  run_start226 = 0i32;
  while run_start226 < bits_to_load219 {
    let fresh0 = lookup_table288[*item209.offset(run_start226 as isize) as usize];
    lookup_table288[*item209.offset(run_start226 as isize) as usize] =
      lookup_table288[*item209.offset(run_start226 as isize) as usize].wrapping_add(1);
    *_231.offset(run_start226 as isize) = fresh0;
    run_start226 += 1
  }
}
