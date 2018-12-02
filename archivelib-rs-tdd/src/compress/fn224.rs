use crate::compress::{RCompressData, Result};
use crate::consts::{MAX_COMPRESSION_CYCLES, MAX_RUN_LENGTH140};
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn224(&mut self, mut _204: u16) {
    let mut byte_or_run_length203: u16 = 0;
    let mut _457: u16 = 0;
    byte_or_run_length203 = 0 as u16;
    _457 = _204;
    while 0 != _457 {
      byte_or_run_length203 = byte_or_run_length203.wrapping_add(1);
      _457 = (_457 >> 1) as u16
    }
    write_bits_to_buffer(
      data,
      *self.dat_arr181.offset(byte_or_run_length203 as isize) as i32,
      *self.dat_arr194.offset(byte_or_run_length203 as isize),
    );
    if byte_or_run_length203 > 1 {
      write_bits_to_buffer(data, byte_or_run_length203 - 1, _204);
    };
  }
}
