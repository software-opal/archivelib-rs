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
  fn memset(_: *mut libc::c_void, _: libc::c_int, _: libc::c_ulong) -> *mut libc::c_void;
}
pub type ALStorage = ();
pub type ptrdiff_t = libc::c_long;
pub type size_t = libc::c_ulong;
pub type __uint8_t = libc::c_uchar;
pub type __int16_t = libc::c_short;
pub type __uint16_t = libc::c_ushort;
pub type __int32_t = libc::c_int;
pub type __uint32_t = libc::c_uint;
pub type int16_t = __int16_t;
pub type int32_t = __int32_t;
pub type uint8_t = __uint8_t;
pub type uint16_t = __uint16_t;
pub type uint32_t = __uint32_t;
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
pub unsafe extern "C" fn fn258(
  mut data: *mut RExpandData,
  mut arg_arr260_len: int32_t,
  mut arg_arr260: *mut uint8_t,
  mut bit_size261: int32_t,
  mut output_table262: *mut uint16_t,
  mut max_internal263: uint16_t,
) {
  // AL_ASSERT(max_internal263 == (1 << bit_size261), "");
  let mut _277: [uint16_t; 17] = [0; 17];
  let mut lookup_table287: [uint16_t; 17] = [0; 17];
  let mut lookup_table288: [uint16_t; 18] = [0; 18];
  let mut _204: *mut uint16_t = 0 as *mut uint16_t;
  let mut i: uint32_t = 0;
  let mut _289: uint32_t = 0;
  let mut item209: uint32_t = 0;
  let mut j: uint32_t = 0;
  let mut rem_bit_size291: uint32_t = 0;
  let mut _292: uint32_t = 0;
  let mut tmp293: uint32_t = 0;
  let mut _283: uint32_t = 0;
  memset(
    _277.as_mut_ptr() as *mut libc::c_void,
    0i32,
    (17i32 as libc::c_ulong).wrapping_mul(::std::mem::size_of::<uint16_t>() as libc::c_ulong),
  );
  memset(
    lookup_table287.as_mut_ptr() as *mut libc::c_void,
    0i32,
    (17i32 as libc::c_ulong).wrapping_mul(::std::mem::size_of::<uint16_t>() as libc::c_ulong),
  );
  memset(
    lookup_table288.as_mut_ptr() as *mut libc::c_void,
    0i32,
    (17i32 as libc::c_ulong).wrapping_mul(::std::mem::size_of::<uint16_t>() as libc::c_ulong),
  );
  i = 0i32 as uint32_t;
  while i < arg_arr260_len as libc::c_uint {
    _277[*arg_arr260.offset(i as isize) as usize] =
      _277[*arg_arr260.offset(i as isize) as usize].wrapping_add(1);
    i = i.wrapping_add(1)
  }
  i = 1i32 as uint32_t;
  while i < 17i32 as libc::c_uint {
    // This wraps around to 0.
    lookup_table288[i.wrapping_add(1i32 as libc::c_uint) as usize] = (lookup_table288[i as usize]
      as libc::c_int
      + ((_277[i as usize] as libc::c_int) << (16i32 as libc::c_uint).wrapping_sub(i)))
      as uint16_t;
    i = i.wrapping_add(1)
  }
  if lookup_table288[17usize] as libc::c_int != 0i32 {
    (*data).error = -1189i32;
    // mStatus.SetError(AL_INTERNAL_ERROR, INTERNAL_ERROR_1_MSG);
    (*data).error_counter243 = 10i32 as int16_t;
    return;
  } else {
    rem_bit_size291 = (16i32 - bit_size261) as uint32_t;
    i = 1i32 as uint32_t;
    while i <= bit_size261 as libc::c_uint {
      lookup_table288[i as usize] =
        (lookup_table288[i as usize] as libc::c_int >> rem_bit_size291) as uint16_t;
      lookup_table287[i as usize] =
        (1u32 << (bit_size261 as libc::c_uint).wrapping_sub(i)) as uint16_t;
      i = i.wrapping_add(1)
    }
    while i <= 16i32 as libc::c_uint {
      lookup_table287[i as usize] = (1u32 << (16i32 as libc::c_uint).wrapping_sub(i)) as uint16_t;
      i = i.wrapping_add(1)
    }
    i = (lookup_table288[(bit_size261 + 1i32) as usize] as libc::c_int >> rem_bit_size291)
      as uint32_t;
    if i != (1u32 << 16i32) as uint16_t as libc::c_uint {
      _289 = 1u32 << bit_size261;
      while i != _289 {
        let fresh0 = i;
        i = i.wrapping_add(1);
        *output_table262.offset(fresh0 as isize) = 0i32 as uint16_t
      }
    }
    _292 = arg_arr260_len as uint32_t;
    _283 = 1u32 << 15i32 - bit_size261;
    j = 0i32 as uint32_t;
    while j < arg_arr260_len as libc::c_uint {
      item209 = *arg_arr260.offset(j as isize) as uint32_t;
      if !(item209 == 0i32 as libc::c_uint) {
        tmp293 = (lookup_table288[item209 as usize] as libc::c_int
          + lookup_table287[item209 as usize] as libc::c_int) as uint32_t;
        if item209 <= bit_size261 as libc::c_uint {
          if tmp293 > max_internal263 as libc::c_uint {
            (*data).error = -1189i32;
            (*data).error_counter243 = 10i32 as int16_t;
            return;
          } else {
            i = lookup_table288[item209 as usize] as uint32_t;
            while i < tmp293 {
              *output_table262.offset(i as isize) = j as uint16_t;
              i = i.wrapping_add(1)
            }
          }
        } else {
          _289 = lookup_table288[item209 as usize] as uint32_t;
          _204 = &mut *output_table262.offset((_289 >> rem_bit_size291) as isize) as *mut uint16_t;
          i = item209.wrapping_sub(bit_size261 as libc::c_uint);
          while i != 0i32 as libc::c_uint {
            if *_204 as libc::c_int == 0i32 {
              let ref mut fresh1 = *(*data).dat_arr189.offset(_292 as isize);
              *fresh1 = 0i32 as uint16_t;
              *(*data).dat_arr190.offset(_292 as isize) = *fresh1;
              let fresh2 = _292;
              _292 = _292.wrapping_add(1);
              *_204 = fresh2 as uint16_t
            }
            if 0 != _289 & _283 {
              _204 = &mut *(*data).dat_arr190.offset(*_204 as isize) as *mut uint16_t
            } else {
              _204 = &mut *(*data).dat_arr189.offset(*_204 as isize) as *mut uint16_t
            }
            _289 <<= 1i32;
            i = i.wrapping_sub(1)
          }
          *_204 = j as uint16_t
        }
        lookup_table288[item209 as usize] = tmp293 as uint16_t
      }
      j = j.wrapping_add(1)
    }
    return;
  };
}
