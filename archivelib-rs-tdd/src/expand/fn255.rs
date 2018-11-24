pub fn fn255(&mut self) {
  let mut run_start226: i16 = 0;
  let mut byte_or_run_length203: i16 = 0;
  let mut bits_to_load219: i16 = 0;
  let mut _283: u16 = 0;
  bits_to_load219 = get_bits(data, 9i32 as u8) as i16;
  if bits_to_load219 as libc::c_int == 0i32 {
    byte_or_run_length203 = get_bits(data, 9i32 as u8) as i16;
    run_start226 = 0i32 as i16;
    while (run_start226 as libc::c_int) < 127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32
    {
      *(self).dat_arr180.offset(run_start226 as isize) = 0i32 as u8;
      run_start226 += 1
    }
    run_start226 = 0i32 as i16;
    while (run_start226 as libc::c_int) < 4096i32 {
      *(self).dat_arr240.offset(run_start226 as isize) = byte_or_run_length203 as u16;
      run_start226 += 1
    }
  } else {
    run_start226 = 0i32 as i16;
    while (run_start226 as libc::c_int) < bits_to_load219 as libc::c_int {
      byte_or_run_length203 = *(self)
        .dat_arr241
        .offset(((self).bits182 as libc::c_int >> 8i32) as isize)
        as i16;
      if byte_or_run_length203 as libc::c_int >= 16i32 + 3i32 {
        _283 = (1u32 << 7i32) as u16;
        loop {
          if 0 != (self).bits182 as libc::c_int & _283 as libc::c_int {
            byte_or_run_length203 =
              *(self).dat_arr190.offset(byte_or_run_length203 as isize) as i16
          } else {
            byte_or_run_length203 =
              *(self).dat_arr189.offset(byte_or_run_length203 as isize) as i16
          }
          _283 = (_283 as libc::c_int >> 1i32) as u16;
          if !(byte_or_run_length203 as libc::c_int >= 16i32 + 3i32) {
            break;
          }
        }
      }
      read_bits(
        data,
        *(self).dat_arr181.offset(byte_or_run_length203 as isize) as i32,
      );
      if byte_or_run_length203 as libc::c_int <= 2i32 {
        if byte_or_run_length203 as libc::c_int == 0i32 {
          byte_or_run_length203 = 1i32 as i16
        } else if byte_or_run_length203 as libc::c_int == 1i32 {
          byte_or_run_length203 = (get_bits(data, 4i32 as u8) as libc::c_int + 3i32) as i16
        } else {
          byte_or_run_length203 =
            (get_bits(data, 9i32 as u8) as libc::c_int + 20i32) as i16
        }
        loop {
          byte_or_run_length203 -= 1;
          if !(byte_or_run_length203 as libc::c_int >= 0i32) {
            break;
          }
          let fresh0 = run_start226;
          run_start226 = run_start226 + 1;
          *(self).dat_arr180.offset(fresh0 as isize) = 0i32 as u8
        }
      } else {
        let fresh1 = run_start226;
        run_start226 = run_start226 + 1;
        *(self).dat_arr180.offset(fresh1 as isize) =
          (byte_or_run_length203 as libc::c_int - 2i32) as u8
      }
    }
    while (run_start226 as libc::c_int) < 127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32
    {
      let fresh2 = run_start226;
      run_start226 = run_start226 + 1;
      *(self).dat_arr180.offset(fresh2 as isize) = 0i32 as u8
    }
    fn258(
      data,
      127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32,
      (self).dat_arr180,
      12i32,
      (self).dat_arr240,
      4096i32 as u16,
    );
  };
}
