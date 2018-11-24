pub fn fn258(
  &mut self,
  mut arg_arr260_len: i32,
  mut arg_arr260: *mut u8,
  mut bit_size261: i32,
  mut output_table262: *mut u16,
  mut max_internal263: u16,
) {
  // AL_ASSERT(max_internal263 == (1 << bit_size261), "");
  let mut _277: [u16; 17] = [0; 17];
  let mut lookup_table287: [u16; 17] = [0; 17];
  let mut lookup_table288: [u16; 18] = [0; 18];
  let mut _204: *mut u16 = 0 as *mut u16;
  let mut i: u32 = 0;
  let mut _289: u32 = 0;
  let mut item209: u32 = 0;
  let mut j: u32 = 0;
  let mut rem_bit_size291: u32 = 0;
  let mut _292: u32 = 0;
  let mut tmp293: u32 = 0;
  let mut _283: u32 = 0;
  memset(
    _277.as_mut_ptr() as *mut libc::c_void,
    0i32,
    (17i32 as libc::c_ulong).wrapping_mul(::std::mem::size_of::<u16>() as libc::c_ulong),
  );
  memset(
    lookup_table287.as_mut_ptr() as *mut libc::c_void,
    0i32,
    (17i32 as libc::c_ulong).wrapping_mul(::std::mem::size_of::<u16>() as libc::c_ulong),
  );
  memset(
    lookup_table288.as_mut_ptr() as *mut libc::c_void,
    0i32,
    (17i32 as libc::c_ulong).wrapping_mul(::std::mem::size_of::<u16>() as libc::c_ulong),
  );
  i = 0i32 as u32;
  while i < arg_arr260_len as libc::c_uint {
    _277[*arg_arr260.offset(i as isize) as usize] =
      _277[*arg_arr260.offset(i as isize) as usize].wrapping_add(1);
    i = i.wrapping_add(1)
  }
  i = 1i32 as u32;
  while i < 17i32 as libc::c_uint {
    // This wraps around to 0.
    lookup_table288[i.wrapping_add(1i32 as libc::c_uint) as usize] = (lookup_table288[i as usize]
      as libc::c_int
      + ((_277[i as usize] as libc::c_int) << (16i32 as libc::c_uint).wrapping_sub(i)))
      as u16;
    i = i.wrapping_add(1)
  }
  if lookup_table288[17usize] as libc::c_int != 0i32 {
    (self).error = -1189i32;
    // mStatus.SetError(AL_INTERNAL_ERROR, INTERNAL_ERROR_1_MSG);
    (self).error_counter243 = 10i32 as i16;
    return;
  } else {
    rem_bit_size291 = (16i32 - bit_size261) as u32;
    i = 1i32 as u32;
    while i <= bit_size261 as libc::c_uint {
      lookup_table288[i as usize] =
        (lookup_table288[i as usize] as libc::c_int >> rem_bit_size291) as u16;
      lookup_table287[i as usize] =
        (1u32 << (bit_size261 as libc::c_uint).wrapping_sub(i)) as u16;
      i = i.wrapping_add(1)
    }
    while i <= 16i32 as libc::c_uint {
      lookup_table287[i as usize] = (1u32 << (16i32 as libc::c_uint).wrapping_sub(i)) as u16;
      i = i.wrapping_add(1)
    }
    i = (lookup_table288[(bit_size261 + 1i32) as usize] as libc::c_int >> rem_bit_size291)
      as u32;
    if i != (1u32 << 16i32) as u16 as libc::c_uint {
      _289 = 1u32 << bit_size261;
      while i != _289 {
        let fresh0 = i;
        i = i.wrapping_add(1);
        *output_table262.offset(fresh0 as isize) = 0i32 as u16
      }
    }
    _292 = arg_arr260_len as u32;
    _283 = 1u32 << 15i32 - bit_size261;
    j = 0i32 as u32;
    while j < arg_arr260_len as libc::c_uint {
      item209 = *arg_arr260.offset(j as isize) as u32;
      if !(item209 == 0i32 as libc::c_uint) {
        tmp293 = (lookup_table288[item209 as usize] as libc::c_int
          + lookup_table287[item209 as usize] as libc::c_int) as u32;
        if item209 <= bit_size261 as libc::c_uint {
          if tmp293 > max_internal263 as libc::c_uint {
            (self).error = -1189i32;
            (self).error_counter243 = 10i32 as i16;
            return;
          } else {
            i = lookup_table288[item209 as usize] as u32;
            while i < tmp293 {
              *output_table262.offset(i as isize) = j as u16;
              i = i.wrapping_add(1)
            }
          }
        } else {
          _289 = lookup_table288[item209 as usize] as u32;
          _204 = &mut *output_table262.offset((_289 >> rem_bit_size291) as isize) as *mut u16;
          i = item209.wrapping_sub(bit_size261 as libc::c_uint);
          while i != 0i32 as libc::c_uint {
            if *_204 as libc::c_int == 0i32 {
              let ref mut fresh1 = *(self).dat_arr189.offset(_292 as isize);
              *fresh1 = 0i32 as u16;
              *(self).dat_arr190.offset(_292 as isize) = *fresh1;
              let fresh2 = _292;
              _292 = _292.wrapping_add(1);
              *_204 = fresh2 as u16
            }
            if 0 != _289 & _283 {
              _204 = &mut *(self).dat_arr190.offset(*_204 as isize) as *mut u16
            } else {
              _204 = &mut *(self).dat_arr189.offset(*_204 as isize) as *mut u16
            }
            _289 <<= 1i32;
            i = i.wrapping_sub(1)
          }
          *_204 = j as u16
        }
        lookup_table288[item209 as usize] = tmp293 as u16
      }
      j = j.wrapping_add(1)
    }
    return;
  };
}
