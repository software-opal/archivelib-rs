use crate::compress::RCompressData;
use crate::consts::{MAX_COMPRESSION_CYCLES, MAX_RUN_LENGTH140};
use std::io::{Read, Write};

impl<R: Read, W: Write> RCompressData<R, W> {
  pub fn fn199(&mut self, uncompressed_buffer_index200: i16, var201: i16) {
    let mut test_idx = var201 as usize;
    let orig_idx = uncompressed_buffer_index200 as usize;
    self.dat168 = 0 as i16;
    for _ in 0..=MAX_COMPRESSION_CYCLES {
      if self.dat_arr163[test_idx] == -1 {
        break;
      }
      test_idx = self.dat_arr163[test_idx] as usize;
      if self.uncompressed_buffer[orig_idx + self.dat168 as usize]
        != self.uncompressed_buffer[test_idx + self.dat168 as usize]
      {
        continue;
      }
      let run_end = 0;
      for run_end in 0..MAX_RUN_LENGTH140 {
        if self.uncompressed_buffer[orig_idx + run_end]
          != self.uncompressed_buffer[test_idx + run_end]
        {
          break;
        }
      }
      if run_end < 3 {
        continue;
      }
      if run_end > self.dat168 {
        let mut offset = (orig_idx - test_idx - 1) as i16;
        if offset < 0 {
          offset = offset + self.max_uncompressed_data_size as i16
        }
        if offset >= self.max_uncompressed_data_size as i16 {
          break;
        }
        self.dat169 = offset;
        self.dat168 = run_end;
        if self.dat168 >= MAX_RUN_LENGTH140 as i16 {
          break;
        }
      }
    }
  }
}
