use crate::compress::{RCompressData, Result};
use crate::support::BitwiseWrite;
use std::io::Read;

const USHRT_MAX: u16 = u16::max_value();

impl<R: Read, W: BitwiseWrite> RCompressData<R, W> {
  pub fn fn218(
    &mut self,
    mut bits_to_write: i16,
    bit_length: i16,
    run_start_check: i16,
  ) -> Result<()> {
    pure_fn218(
      &self.dat_arr181,
      &mut self.output_store,
      bits_to_write as usize,
      bit_length as usize,
      run_start_check as usize,
    )
  }
}

fn pure_fn218<W>(
  arr181: &[u8],
  out: &mut W,
  mut bits_to_write: usize,
  bit_length: usize,
  run_start_check: usize,
) -> Result<()>
where
  W: BitwiseWrite + Sized,
{
  while bits_to_write > 0 && arr181[bits_to_write - 1] == 0 {
    bits_to_write -= 1
  }
  out.write_bits(bits_to_write, bit_length)?;
  let mut run_start226: usize = 0;
  while run_start226 < bits_to_write {
    let var289 = arr181[run_start226];
    run_start226 = run_start226 + 1;
    if var289 <= 6 {
      out.write_bits(var289, 3)?;
    } else {
      out.write_bits(USHRT_MAX << 1, var289 - 3)?;
    }
    if run_start226 == run_start_check {
      while run_start226 < 6 && arr181[run_start226] == 0 {
        run_start226 += 1
      }
      out.write_bits(run_start226 - 3, 2)?;
    }
  }
  Ok(())
}
