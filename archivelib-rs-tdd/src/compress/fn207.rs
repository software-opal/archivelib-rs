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
  fn write_stored_bits_to_buffer(data: *mut RCompressData, bits203: i16);
  #[no_mangle]
  fn fn224(data: *mut RCompressData, arg204: u16);
  #[no_mangle]
  fn write_bits_to_buffer(data: *mut RCompressData, arg209: i32, bits203: u16);
  #[no_mangle]
  fn fn218(data: *mut RCompressData, length219: i16, arg220: i16, arg221: i16);
  #[no_mangle]
  fn fn211(
    data: *mut RCompressData,
    arg212: i32,
    arg213: *mut u16,
    arg214: *mut u8,
    arg215: *mut u16,
  ) -> i32;
  #[no_mangle]
  fn fn222(data: *mut RCompressData);
  #[no_mangle]
  fn fn216(data: *mut RCompressData, arg217: *mut u16);
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
pub unsafe extern "C" fn fn207(mut data: *mut RCompressData) {
  let mut run_start226: u32 = 0;
  let mut _289: u32 = 0;
  let mut _229: u32 = 0;
  let mut _454: u32 = 0;
  let mut _455: u32 = 0;
  let mut _456: u32 = 0i32 as u32;
  let mut _217: [u16; 37] = [0; 37];
  _229 = fn211(
    data,
    127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32,
    (*data).dat_arr191,
    (*data).dat_arr180,
    (*data).dat_arr192,
  ) as u32;
  _455 = *(*data).dat_arr191.offset(_229 as isize) as u32;
  write_bits_to_buffer(data, 16i32, _455 as u16);
  if _229 >= (127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32) as libc::c_uint {
    fn216(data, _217.as_mut_ptr());
    _229 = fn211(
      data,
      16i32 + 3i32,
      _217.as_mut_ptr(),
      (*data).dat_arr181,
      (*data).dat_arr194,
    ) as u32;
    if _229 >= (16i32 + 3i32) as libc::c_uint {
      fn218(
        data,
        (16i32 + 3i32) as i16,
        5i32 as i16,
        3i32 as i16,
      );
    } else {
      write_bits_to_buffer(data, 5i32, 0i32 as u16);
      write_bits_to_buffer(data, 5i32, _229 as u16);
    }
    fn222(data);
  } else {
    write_bits_to_buffer(data, 5i32, 0i32 as u16);
    write_bits_to_buffer(data, 5i32, 0i32 as u16);
    write_bits_to_buffer(data, 9i32, 0i32 as u16);
    write_bits_to_buffer(data, 9i32, _229 as u16);
  }
  _229 = fn211(
    data,
    14i32 + 1i32,
    (*data).dat_arr193,
    (*data).dat_arr181,
    (*data).dat_arr194,
  ) as u32;
  if _229 >= (14i32 + 1i32) as libc::c_uint {
    fn218(
      data,
      (14i32 + 1i32) as i16,
      5i32 as i16,
      -1i32 as i16,
    );
  } else {
    write_bits_to_buffer(data, 5i32, 0i32 as u16);
    write_bits_to_buffer(data, 5i32, _229 as u16);
  }
  _454 = 0i32 as u32;
  run_start226 = 0i32 as u32;
  while run_start226 < _455 {
    if run_start226.wrapping_rem(8i32 as libc::c_uint) == 0i32 as libc::c_uint {
      let fresh0 = _454;
      _454 = _454.wrapping_add(1);
      _456 = *(*data).dat_arr165.offset(fresh0 as isize) as u32
    } else {
      _456 <<= 1i32
    }
    if 0 != _456 & 1u32 << 8i32 - 1i32 {
      let fresh1 = _454;
      _454 = _454.wrapping_add(1);
      write_stored_bits_to_buffer(
        data,
        (*(*data).dat_arr165.offset(fresh1 as isize) as libc::c_uint).wrapping_add(1u32 << 8i32)
          as i16,
      );
      let fresh2 = _454;
      _454 = _454.wrapping_add(1);
      _289 = *(*data).dat_arr165.offset(fresh2 as isize) as u32;
      let fresh3 = _454;
      _454 = _454.wrapping_add(1);
      _289 = (_289 as libc::c_uint).wrapping_add(
        ((*(*data).dat_arr165.offset(fresh3 as isize) as libc::c_int) << 8i32) as libc::c_uint,
      ) as u32 as u32;
      fn224(data, _289 as i16 as u16);
    } else {
      let fresh4 = _454;
      _454 = _454.wrapping_add(1);
      write_stored_bits_to_buffer(data, *(*data).dat_arr165.offset(fresh4 as isize) as i16);
    }
    if 0 != (*data).uncompressible {
      return;
    } else {
      run_start226 = run_start226.wrapping_add(1)
    }
  }
  run_start226 = 0i32 as u32;
  while run_start226 < (127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32) as libc::c_uint {
    *(*data).dat_arr191.offset(run_start226 as isize) = 0i32 as u16;
    run_start226 = run_start226.wrapping_add(1)
  }
  run_start226 = 0i32 as u32;
  while run_start226 < (14i32 + 1i32) as libc::c_uint {
    *(*data).dat_arr193.offset(run_start226 as isize) = 0i32 as u16;
    run_start226 = run_start226.wrapping_add(1)
  }
}
