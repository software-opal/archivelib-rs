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
  fn calculate_pointer_depths(
    left_array_ptr: *mut u16,
    right_array_ptr: *mut u16,
    depth_store_ptr: *mut u16,
    depth: u16,
    series_start: i16,
    curr_idx: u16,
  );
}
pub type ALStorage = ();
pub type usize = libc::c_ulong;
pub type __i8 = libc::c_schar;
pub type __u8 = libc::c_uchar;
pub type __i16 = libc::c_short;
pub type __u16 = libc::c_ushort;
pub type __i32 = libc::c_int;
pub type __u32 = libc::c_uint;
pub type i8 = __i8;
pub type i16 = __i16;
pub type i32 = __i32;
pub type u8 = __u8;
pub type u16 = __u16;
pub type u32 = __u32;
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
pub unsafe extern "C" fn fn228(mut data: *mut RCompressData, mut _229: i32) {
  let mut run_start226: i32 = 0;
  let mut _289: i32 = 0;
  let mut _458: u32 = 0;
  run_start226 = 0i32;
  while run_start226 <= 16i32 {
    *(*data).dat_arr167.offset(run_start226 as isize) = 0i32 as u16;
    run_start226 += 1
  }
  calculate_pointer_depths(
    (*data).dat_arr189,
    (*data).dat_arr190,
    (*data).dat_arr167,
    0i32 as u16,
    (*data).dat174,
    _229 as u16,
  );
  _458 = 0i32 as u32;
  run_start226 = 16i32;
  while run_start226 > 0i32 {
    _458 = (_458 as libc::c_uint).wrapping_add(
      ((*(*data).dat_arr167.offset(run_start226 as isize) as libc::c_int) << 16i32 - run_start226)
        as libc::c_uint,
    ) as u32 as u32;
    run_start226 -= 1
  }
  while _458 != 1u32 << 16i32 {
    let ref mut fresh0 = *(*data).dat_arr167.offset(16isize);
    *fresh0 = (*fresh0).wrapping_sub(1);
    run_start226 = 15i32;
    while run_start226 > 0i32 {
      if *(*data).dat_arr167.offset(run_start226 as isize) as libc::c_int != 0i32 {
        let ref mut fresh1 = *(*data).dat_arr167.offset(run_start226 as isize);
        *fresh1 = (*fresh1).wrapping_sub(1);
        *(*data).dat_arr167.offset((run_start226 + 1i32) as isize) =
          (*(*data).dat_arr167.offset((run_start226 + 1i32) as isize) as libc::c_int + 2i32)
            as u16;
        break;
      } else {
        run_start226 -= 1
      }
    }
    _458 = _458.wrapping_sub(1)
  }
  run_start226 = 16i32;
  while run_start226 > 0i32 {
    _289 = *(*data).dat_arr167.offset(run_start226 as isize) as i32;
    loop {
      _289 -= 1;
      if !(_289 >= 0i32) {
        break;
      }
      let fresh2 = (*data).dat_arr_cursor188;
      (*data).dat_arr_cursor188 = (*data).dat_arr_cursor188.offset(1);
      *(*data).dat_arr_cursor178.offset(*fresh2 as isize) = run_start226 as u8
    }
    run_start226 -= 1
  }
}
