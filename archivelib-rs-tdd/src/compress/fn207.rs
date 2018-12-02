use crate::compress::{RCompressData, Result};
use crate::consts::{MAX_COMPRESSION_CYCLES, MAX_RUN_LENGTH140};
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn207(&mut self) {
    let mut run_start226: u32 = 0;
    let mut _289: u32 = 0;
    let mut _229: u32 = 0;
    let mut _454: u32 = 0;
    let mut _455: u32 = 0;
    let mut _456: u32 = 0 as u32;
    let mut _217: [u16; 37] = [0; 37];
    _229 = fn211(
      data,
      127 * 2 + 1 + 1 + 256 - 3 + 1 + 1,
      self.dat_arr191,
      self.dat_arr180,
      self.dat_arr192,
    ) as u32;
    _455 = *self.dat_arr191.offset(_229 as isize) as u32;
    write_bits_to_buffer(data, 16, _455 as u16);
    if _229 >= (127 * 2 + 1 + 1 + 256 - 3 + 1 + 1) {
      fn216(data, _217.as_mut_ptr());
      _229 = fn211(
        data,
        16 + 3,
        _217.as_mut_ptr(),
        self.dat_arr181,
        self.dat_arr194,
      ) as u32;
      if _229 >= (16 + 3) {
        fn218(data, (16 + 3) as i16, 5 as i16, 3 as i16);
      } else {
        write_bits_to_buffer(data, 5, 0 as u16);
        write_bits_to_buffer(data, 5, _229 as u16);
      }
      fn222(data);
    } else {
      write_bits_to_buffer(data, 5, 0 as u16);
      write_bits_to_buffer(data, 5, 0 as u16);
      write_bits_to_buffer(data, 9, 0 as u16);
      write_bits_to_buffer(data, 9, _229 as u16);
    }
    _229 = fn211(
      data,
      14 + 1,
      self.dat_arr193,
      self.dat_arr181,
      self.dat_arr194,
    ) as u32;
    if _229 >= (14 + 1) {
      fn218(data, (14 + 1) as i16, 5 as i16, -1 as i16);
    } else {
      write_bits_to_buffer(data, 5, 0 as u16);
      write_bits_to_buffer(data, 5, _229 as u16);
    }
    _454 = 0 as u32;
    run_start226 = 0 as u32;
    while run_start226 < _455 {
      if run_start226.wrapping_rem(8) == 0 {
        let fresh0 = _454;
        _454 = _454.wrapping_add(1);
        _456 = *self.dat_arr165.offset(fresh0 as isize) as u32
      } else {
        _456 <<= 1
      }
      if 0 != _456 & 1 << 8 - 1 {
        let fresh1 = _454;
        _454 = _454.wrapping_add(1);
        write_stored_bits_to_buffer(
          data,
          (*self.dat_arr165.offset(fresh1 as isize)).wrapping_add(1 << 8) as i16,
        );
        let fresh2 = _454;
        _454 = _454.wrapping_add(1);
        _289 = *self.dat_arr165.offset(fresh2 as isize) as u32;
        let fresh3 = _454;
        _454 = _454.wrapping_add(1);
        _289 = (_289).wrapping_add(((*self.dat_arr165.offset(fresh3 as isize)) << 8)) as u32 as u32;
        fn224(data, _289 as i16 as u16);
      } else {
        let fresh4 = _454;
        _454 = _454.wrapping_add(1);
        write_stored_bits_to_buffer(data, *self.dat_arr165.offset(fresh4 as isize) as i16);
      }
      if 0 != self.uncompressible {
        return;
      } else {
        run_start226 = run_start226.wrapping_add(1)
      }
    }
    run_start226 = 0 as u32;
    while run_start226 < (127 * 2 + 1 + 1 + 256 - 3 + 1 + 1) {
      *self.dat_arr191.offset(run_start226 as isize) = 0 as u16;
      run_start226 = run_start226.wrapping_add(1)
    }
    run_start226 = 0 as u32;
    while run_start226 < (14 + 1) {
      *self.dat_arr193.offset(run_start226 as isize) = 0 as u16;
      run_start226 = run_start226.wrapping_add(1)
    }
  }
}
