use crate::consts::{END_OF_FILE_FLAG, MAX_RUN_LENGTH140, MIN_RUN_LENGTH135_IS_3};
use crate::expand::{RExpandData, Result};
use crate::support::{BitwiseReadAheadRead, BitwiseWrite};

const UCHAR_MAX: usize = 255;

impl<R: BitwiseReadAheadRead, W: BitwiseWrite> RExpandData<R, W> {
  pub fn expand(&mut self) -> Result<()> {
    let max_size279 = self.max_uncompressed_data_size;
    let size_bitmask280: usize = self.max_uncompressed_data_size_bitmask;
    let mut buffer_pos: usize = 0;
    // Seed bits182 with the first 2 bits
    self.read_bits(2 * 8)?;
    while (self.error_counter243) < 5 {
      let byte_or_run_length203 = self.get_next_item()? as usize;
      if byte_or_run_length203 <= UCHAR_MAX {
        // byte_or_run_length203 is the decompressed byte
        self.uncompressed_buffer[buffer_pos as usize] = byte_or_run_length203 as u8;
        buffer_pos += 1;
        if buffer_pos as usize >= max_size279 {
          self
            .output_store
            .write_all(&self.uncompressed_buffer[..buffer_pos])?;
          buffer_pos = 0;
        }
      } else {
        // Copy the run of `run_length276` bytes from earlier in the output.
        // byte_or_run_length203 >= 0x100 indicates a flag
        // run_length276 = byte_or_run_length203 - 0x100 + 3; which is the length
        // of the run. Flag value of byte_or_run_length203 ==
        let mut run_length276 = byte_or_run_length203 - (UCHAR_MAX + 1) + MIN_RUN_LENGTH135_IS_3;
        if run_length276 == END_OF_FILE_FLAG {
          // byte_or_run_length203 == 0x1FE. End of file.
          break;
        } else {
          let mut run_start226 =
            buffer_pos - (self.calculate_run_offset()? as usize) - 1 & size_bitmask280;
          if run_start226 < max_size279 - MAX_RUN_LENGTH140 - 1
            && buffer_pos < max_size279 - MAX_RUN_LENGTH140 - 1
          {
            loop {
              run_length276 -= 1;
              if !(run_length276 >= 0) {
                break;
              }
              let fresh1 = buffer_pos;
              buffer_pos = buffer_pos + 1;
              let fresh0 = run_start226;
              run_start226 = run_start226 + 1;
              self.uncompressed_buffer[fresh1 as usize] = self.uncompressed_buffer[fresh0 as usize];
            }
          } else {
            loop {
              run_length276 -= 1;
              if !(run_length276 >= 0) {
                break;
              }
              self.uncompressed_buffer[buffer_pos as usize] =
                self.uncompressed_buffer[run_start226 as usize];
              buffer_pos += 1;
              if buffer_pos as usize >= max_size279 {
                self
                  .output_store
                  .write_all(&self.uncompressed_buffer[..buffer_pos])?;
                buffer_pos = 0;
              }
              run_start226 = (run_start226 + 1) & (size_bitmask280)
            }
          }
        }
      }
    }
    if buffer_pos != 0 {
      self
        .output_store
        .write_all(&self.uncompressed_buffer[..buffer_pos])?;
    }
    return Ok(());
  }
}
