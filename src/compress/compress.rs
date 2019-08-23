use std::convert::{TryFrom, TryInto};
use std::io::Read;

use crate::compress::{RCompressData, Result};
use crate::consts::{
  CONST_N153_SUB_1_IS_4095, CONST_N154_IS_4, END_OF_FILE_FLAG, MAX_RUN_LENGTH140,
  MIN_RUN_LENGTH135_IS_3,
};
use crate::support::BitwiseWrite;

const UCHAR_MAX: usize = 255;

fn fn445(arg278: &[u8], buff_pos: usize, arg446: i16) -> i16 {
  ((arg446 << CONST_N154_IS_4) ^ i16::from(arg278[buff_pos + 2]))
    & cast!(CONST_N153_SUB_1_IS_4095 as i16)
}
fn fn447(arg163: &mut [i16], arg164: &mut [i16], buff_pos: usize, arg201: i16) {
  let local204 = arg163[cast!(arg201 as usize)];
  if local204 != -1 {
    arg164[cast!(local204 as usize)] = cast!(buff_pos as i16);
  }
  arg164[cast!(buff_pos as usize)] = arg201;
  arg163[cast!(buff_pos as usize)] = local204;
  arg163[cast!(arg201 as usize)] = cast!(buff_pos as i16);
}
fn fn448(arg163: &mut [i16], arg164: &mut [i16], s: usize) {
  let local204 = arg164[s];
  if local204 != -1 {
    arg164[s] = -1;
    arg163[cast!(local204 as usize)] = -1;
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

    self.dat169 = 0;
    self.dat168 = 0;
    let mut var201 = i16::try_from(
      ((u16::from(self.uncompressed_buffer[buffer_pos]) << CONST_N154_IS_4)
        ^ u16::from(self.uncompressed_buffer[buffer_pos + 1]))
        & CONST_N153_SUB_1_IS_4095,
    )
    .unwrap();
    var201 = fn445(&self.uncompressed_buffer, buffer_pos, var201) + (cast!(max_size279 as i16));

    while var209 > MAX_RUN_LENGTH140 + 4 {
      self.fn199(cast!(buffer_pos as i16), var201);
      if (self.dat168) < 3 {
        let val = u16::from(self.uncompressed_buffer[buffer_pos]);
        self.fn202(val, 0)?;
        fn447(
          &mut self.dat_arr163,
          &mut self.dat_arr164,
          buffer_pos,
          var201,
        );
        buffer_pos += 1;
        var201 = fn445(&self.uncompressed_buffer, buffer_pos, var201) + (cast!(max_size279 as i16));
        var209 -= 1;
      } else {
        var209 -= cast!((self.dat168) as usize);
        let a1 = (self.dat168 + cast!((UCHAR_MAX + 1 - MIN_RUN_LENGTH135_IS_3) as i16))
          .try_into()
          .unwrap();
        let a2 = self.dat169.try_into().unwrap();
        self.fn202(a1, a2)?;
        loop {
          self.dat168 -= 1;
          if self.dat168 < 0 {
            break;
          }
          fn447(
            &mut self.dat_arr163,
            &mut self.dat_arr164,
            buffer_pos,
            var201,
          );
          buffer_pos += 1;
          var201 =
            fn445(&self.uncompressed_buffer, buffer_pos, var201) + (cast!(max_size279 as i16))
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
      self.fn199(cast!(buffer_pos as i16), var201);
      if self.dat168 > cast!(var209 as i16) {
        self.dat168 = cast!(var209 as i16)
      }
      if (self.dat168) < 3 {
        self.dat168 = 1;
        let val = u16::from(self.uncompressed_buffer[buffer_pos]);
        self.fn202(val, 0)?;
      } else {
        let a1 =
          u16::try_from(self.dat168 + cast!((UCHAR_MAX + 1 - MIN_RUN_LENGTH135_IS_3) as i16))
            .unwrap();
        let a2 = cast!((self.dat169) as u16);
        self.fn202(a1, a2)?;
      }
      loop {
        self.dat168 -= 1;
        if self.dat168 < 0 {
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
        var201 = fn445(&self.uncompressed_buffer, buffer_pos, var201) + cast!(max_size279 as i16)
      }
      loop {
        if self.dat168 < 0 {
          break;
        }
        self.dat168 -= 1;
        fn447(
          &mut self.dat_arr163,
          &mut self.dat_arr164,
          buffer_pos,
          var201,
        );
        buffer_pos = (buffer_pos + 1) & size_bitmask280;
        var201 = fn445(&self.uncompressed_buffer, buffer_pos, var201) + cast!(max_size279 as i16);
        var209 -= 1;
      }
    }
    self.fn202(
      (END_OF_FILE_FLAG + (UCHAR_MAX + 1 - MIN_RUN_LENGTH135_IS_3))
        .try_into()
        .unwrap(),
      0,
    )?;
    self.finalise_compresson197()
  }
}
