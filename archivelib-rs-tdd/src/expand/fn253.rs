pub fn fn253(&mut self, mut _254: i16, mut _220: i16, mut _221: i16) {
  let mut run_start226: i16 = 0;
  let mut byte_or_run_length203: i16 = 0;
  let mut bits_to_load219: i16 = 0;
  let mut _283: u16 = 0;
  bits_to_load219 = get_bits(data, _220 as u8) as i16;
  if bits_to_load219 as libc::c_int == 0i32 {
    byte_or_run_length203 = get_bits(data, _220 as u8) as i16;
    run_start226 = 0i32 as i16;
    while (run_start226 as libc::c_int) < _254 as libc::c_int {
      *(self).dat_arr181.offset(run_start226 as isize) = 0i32 as u8;
      run_start226 += 1
    }
    run_start226 = 0i32 as i16;
    while (run_start226 as libc::c_int) < 256i32 {
      *(self).dat_arr241.offset(run_start226 as isize) = byte_or_run_length203 as u16;
      run_start226 += 1
    }
  } else {
    run_start226 = 0i32 as i16;
    while (run_start226 as libc::c_int) < bits_to_load219 as libc::c_int {
      byte_or_run_length203 = ((self).bits182 as libc::c_int >> 13i32) as i16;
      if byte_or_run_length203 as libc::c_int == 7i32 {
        let mut bytes_read: size_t = 3i32 as size_t;
        _283 = (1u32 << 12i32) as u16;
        while 0 != _283 as libc::c_int & (self).bits182 as libc::c_int {
          _283 = (_283 as libc::c_int >> 1i32) as u16;
          byte_or_run_length203 += 1;
          bytes_read = bytes_read.wrapping_add(1)
        }
        // +1 for the final bit that was zero
        read_bits(
          data,
          bytes_read.wrapping_add(1i32 as libc::c_ulong) as i32,
        );
      } else {
        read_bits(data, 3i32);
      }
      let fresh0 = run_start226;
      run_start226 = run_start226 + 1;
      *(self).dat_arr181.offset(fresh0 as isize) = byte_or_run_length203 as u8;
      if !(run_start226 as libc::c_int == _221 as libc::c_int) {
        continue;
      }
      byte_or_run_length203 = get_bits(data, 2i32 as u8) as i16;
      while byte_or_run_length203 as libc::c_int > 0i32 {
        let fresh1 = run_start226;
        run_start226 = run_start226 + 1;
        *(self).dat_arr181.offset(fresh1 as isize) = 0i32 as u8;
        byte_or_run_length203 -= 1
      }
    }
    while (run_start226 as libc::c_int) < _254 as libc::c_int {
      *(self).dat_arr181.offset(run_start226 as isize) = 0i32 as u8;
      run_start226 += 1
    }
    fn258(
      data,
      _254 as i32,
      (self).dat_arr181,
      8i32,
      (self).dat_arr241,
      256i32 as u16,
    );
  };
}
