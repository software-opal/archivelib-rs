use crate::compress::{RCompressData, Result};
use crate::consts::{
  CONST_N153_IS_4096, CONST_N154_IS_4, END_OF_FILE_FLAG, MAX_RUN_LENGTH140, MIN_RUN_LENGTH135_IS_3,
};
use crate::support::BitwiseWrite;
use std::io::Read;

const UCHAR_MAX: usize = 255;

fn fn445(arg278: &[u8], buff_pos: usize, arg446: i16) -> i16 {
  return ((arg446 << CONST_N154_IS_4) ^ (arg278[buff_pos + 2] as i16))
    & (CONST_N153_IS_4096 as i16 - 1);
}
fn fn447(arg163: &mut [i16], arg164: &mut [i16], buff_pos: usize, arg201: i16) {
  let local204 = arg163[arg201 as usize];
  if local204 != -1 {
    arg164[local204 as usize] = buff_pos as i16;
  }
  arg164[buff_pos as usize] = arg201;
  arg163[buff_pos as usize] = local204;
  arg163[arg201 as usize] = buff_pos as i16;
}
fn fn448(arg163: &mut [i16], arg164: &mut [i16], s: usize) {
  let local204 = arg164[s];
  if local204 != -1 {
    arg164[s] = -1;
    arg163[local204 as usize] = -1;
  }
}

fn read_all(reader: &mut impl Read, target: &mut [u8]) -> Result<usize> {
  let mut idx = 0;
  loop {
    match reader.read(&mut target[idx..]) {
      Ok(0) => return Ok(idx),
      Ok(n) => idx += n,
      Err(ref e) if e.kind() == std::io::ErrorKind::Interrupted => {}
      Err(e) => return Err(e.into()),
    }
  }
}

fn read_one(reader: &mut impl Read) -> Result<Option<u8>> {
  let mut dat = [0];
  match read_all(reader, &mut dat)? {
    0 => Ok(None),
    1 => Ok(Some(dat[0])),
    _ => unreachable!(),
  }
}

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn compress(&mut self) -> Result<()> {
    let mut buffer_pos: usize = 0;
    let size_bitmask280 = self.max_uncompressed_data_size_bitmask;
    let max_size279 = self.max_uncompressed_data_size;
    let mut var209 = read_all(
      &mut self.input_store,
      &mut self.uncompressed_buffer[..max_size279],
    )?;
    let mut s = (var209 & size_bitmask280) as usize;

    self.dat169 = 0 as i16;
    self.dat168 = 0 as i16;
    let mut var201 = ((((self.uncompressed_buffer[buffer_pos] as u16) << CONST_N154_IS_4)
      ^ (self.uncompressed_buffer[buffer_pos + 1] as u16))
      & (CONST_N153_IS_4096 as u16 - 1)) as i16;
    var201 = fn445(&self.uncompressed_buffer, buffer_pos, var201) + (max_size279 as i16);

    while var209 > MAX_RUN_LENGTH140 + 4 {
      self.fn199(buffer_pos as i16, var201);
      if (self.dat168) < 3 {
        let val = self.uncompressed_buffer[buffer_pos] as u16;
        self.fn202(val, 0)?;
        fn447(
          &mut self.dat_arr163,
          &mut self.dat_arr164,
          buffer_pos,
          var201,
        );
        buffer_pos += 1;
        var201 = fn445(&mut self.uncompressed_buffer, buffer_pos, var201) + (max_size279 as i16);
        var209 -= 1;
      } else {
        var209 = var209 - (self.dat168 as usize);
        let a1 = (self.dat168 + ((UCHAR_MAX + 1 - MIN_RUN_LENGTH135_IS_3) as i16)) as u16;
        let a2 = self.dat169 as u16;
        self.fn202(a1, a2)?;
        loop {
          self.dat168 -= 1;
          if !(self.dat168 >= 0) {
            break;
          }
          fn447(
            &mut self.dat_arr163,
            &mut self.dat_arr164,
            buffer_pos,
            var201,
          );
          buffer_pos += 1;
          var201 = fn445(&mut self.uncompressed_buffer, buffer_pos, var201) + (max_size279 as i16)
        }
      }
    }
    while var209 < 256 {
      let byte_or_run_length203 = match read_one(&mut self.input_store)? {
        None => break,
        Some(n) => n,
      };
      self.uncompressed_buffer[s] = byte_or_run_length203;
      if (s) < 256 - 1 {
        self.uncompressed_buffer[(s + max_size279)] = self.uncompressed_buffer[(s)]
      }
      fn448(&mut self.dat_arr163, &mut self.dat_arr164, s);
      s = (s + 1) & size_bitmask280;
      var209 += 1
    }
    while var209 > 0 {
      self.fn199(buffer_pos as i16, var201);
      if self.dat168 > var209 as i16 {
        self.dat168 = var209 as i16
      }
      if (self.dat168) < 3 {
        self.dat168 = 1 as i16;
        let val = self.uncompressed_buffer[buffer_pos] as u16;
        self.fn202(val, 0 as u16)?;
      } else {
        let a1 = (self.dat168 + ((UCHAR_MAX + 1 - MIN_RUN_LENGTH135_IS_3) as i16)) as u16;
        let a2 = self.dat169 as u16;
        self.fn202(a1, a2)?;
      }
      loop {
        self.dat168 -= 1;
        if !(self.dat168 >= 0) {
          break;
        }
        let byte_or_run_length203 = match read_one(&mut self.input_store)? {
          None => break,
          Some(n) => n,
        };
        self.uncompressed_buffer[s] = byte_or_run_length203;
        if (s) < 256 - 1 {
          self.uncompressed_buffer[s + max_size279] = self.uncompressed_buffer[s]
        }
        fn448(&mut self.dat_arr163, &mut self.dat_arr164, s);
        s = (s + 1) & size_bitmask280;
        fn447(
          &mut self.dat_arr163,
          &mut self.dat_arr164,
          buffer_pos,
          var201,
        );
        buffer_pos = (buffer_pos + 1) & size_bitmask280;
        var201 = fn445(&self.uncompressed_buffer, buffer_pos, var201) + (max_size279 as i16)
      }
      loop {
        let fresh0 = self.dat168;
        self.dat168 = self.dat168 - 1;
        if !(fresh0 >= 0) {
          break;
        }
        fn447(
          &mut self.dat_arr163,
          &mut self.dat_arr164,
          buffer_pos,
          var201,
        );
        buffer_pos = (buffer_pos + 1) & size_bitmask280;
        var201 = fn445(&self.uncompressed_buffer, buffer_pos, var201) + (max_size279 as i16);
        var209 -= 1;
      }
    }
    self.fn202(
      (END_OF_FILE_FLAG + (UCHAR_MAX + 1 - MIN_RUN_LENGTH135_IS_3)) as u16,
      0,
    )?;
    self.finalise_compresson197()
  }
}
