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
  fn read_bits(data: *mut RExpandData, bits_to_load219: int32_t);
}
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
pub type ALStorage = ();
pub type ssize_t = ptrdiff_t;

#[no_mangle]
pub unsafe extern "C" fn calculate_run_offset(mut data: *mut RExpandData) -> uint16_t {
  let mut run_length276: uint16_t = 0;
  let mut _283: uint16_t = 0;
  run_length276 = *(*data)
    .dat_arr241
    .offset(((*data).bits182 as libc::c_int >> 8i32) as isize);
  if run_length276 as libc::c_int >= 14i32 + 1i32 {
    _283 = (1u32 << 7i32) as uint16_t;
    loop {
      if 0 != (*data).bits182 as libc::c_int & _283 as libc::c_int {
        run_length276 = *(*data).dat_arr190.offset(run_length276 as isize)
      } else {
        run_length276 = *(*data).dat_arr189.offset(run_length276 as isize)
      }
      _283 = (_283 as libc::c_int >> 1i32) as uint16_t;
      if !(run_length276 as libc::c_int >= 14i32 + 1i32) {
        break;
      }
    }
  }
  read_bits(
    data,
    *(*data).dat_arr181.offset(run_length276 as isize) as int32_t,
  );
  if run_length276 as libc::c_int != 0i32 {
    run_length276 = run_length276.wrapping_sub(1);
    run_length276 = (1u32 << run_length276 as libc::c_int)
      .wrapping_add(get_bits(data, run_length276 as uint8_t) as libc::c_uint)
      as int16_t as uint16_t
  }
  return run_length276;
}
