use std::io::Read;

use crate::compress::RCompressData;
use crate::consts::{CONST_N141_IS_511, CONST_N145_IS_19};
use crate::support::BitwiseWrite;

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn fn216(&mut self, var217: &mut [u16]) {
    for v in var217.iter_mut().take(CONST_N145_IS_19) {
      *v = 0;
    }
    let mut bits_to_load219: usize = CONST_N141_IS_511;
    while bits_to_load219 > 0 && self.byte_run_length_huff_bit_length[cast!(bits_to_load219 as usize) - 1] == 0 {
      bits_to_load219 -= 1
    }
    let mut run_start226: usize = 0;
    while run_start226 < bits_to_load219 {
      let var289: usize = self.byte_run_length_huff_bit_length[run_start226] as usize;
      run_start226 += 1;
      if var289 == 0 {
        let mut var277 = 1;
        while (run_start226) < bits_to_load219 && self.byte_run_length_huff_bit_length[run_start226] == 0 {
          run_start226 += 1;
          var277 += 1
        }
        if var277 <= 2 {
          var217[0] += cast!(var277 as u16);
        } else if var277 <= 18 {
          var217[1] += 1;
        } else if var277 == 19 {
          var217[0] += 1;
          var217[1] += 1;
        } else {
          var217[2] += 1
        }
      } else {
        var217[var289 + 2] += 1
      }
    }
  }
}
