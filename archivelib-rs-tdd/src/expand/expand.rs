#![allow(
  dead_code,
  mutable_transmutes,
  non_camel_case_types,
  non_snake_case,
  non_upper_case_globals,
  unused_mut
)]

use crate::consts::{END_OF_FILE_FLAG, MAX_RUN_LENGTH140, MIN_RUN_LENGTH135_IS_3};
use crate::expand::{RExpandData, Result};
use crate::support::{BitwiseRead, BitwiseWrite};

const U8_MAX: usize = 0xff; // u8::max_value().into();

impl<R: BitwiseRead, W: BitwiseWrite> RExpandData<R, W> {
  pub fn expand(&mut self) -> Result<()> {
    let mut buffer_pos: usize = 0;

    let max_size279 = self.max_uncompressed_data_size;
    let max_run_start = max_size279 - MAX_RUN_LENGTH140 - 1;

    while self.error_counter243 < 5 {
      let byte_or_run_length203 = self.get_next_item()?;
      if byte_or_run_length203 <= (U8_MAX as u16) {
        // byte_or_run_length203 is the decompressed byte
        self.uncompressed_buffer[buffer_pos] = byte_or_run_length203 as u8;
        buffer_pos += 1;
        if buffer_pos >= max_size279 {
          buffer_pos = 0;
          self
            .output_store
            .write_all(&self.uncompressed_buffer[..max_size279])?;
        }
      } else {
        // Copy the run of `run_length276` bytes from earlier in the output.
        // byte_or_run_length203 >= 0x100 indicates a flag
        // run_length276 = byte_or_run_length203 - 0x100 + 3; which is the length
        // of the run. Flag value of byte_or_run_length203 ==
        let mut run_length276 =
          MIN_RUN_LENGTH135_IS_3 + (byte_or_run_length203 as usize) - (U8_MAX + 1);
        if run_length276 == END_OF_FILE_FLAG {
          // byte_or_run_length203 == 0x1FE. End of file.
          break;
        } else {
          let mut run_start226 =
            (buffer_pos - self.calculate_run_offset()? as usize - 1) % max_size279;
          if run_start226 < max_run_start && buffer_pos < max_run_start {
            loop {
              if !(run_length276 > 0) {
                break;
              }
              run_length276 -= 1;
              self.uncompressed_buffer[buffer_pos] = self.uncompressed_buffer[run_start226];
              buffer_pos += 1;
              run_start226 += 1;
            }
          } else {
            loop {
              if !(run_length276 > 0) {
                break;
              }
              run_length276 -= 1;
              self.uncompressed_buffer[buffer_pos] = self.uncompressed_buffer[run_start226];
              buffer_pos += 1;
              if buffer_pos >= max_size279 {
                buffer_pos = 0;
                self
                  .output_store
                  .write_all(&self.uncompressed_buffer[..max_size279])?;
              }
              run_start226 = (run_start226 + 1) % max_size279;
            }
          }
        }
      }
    }
    if buffer_pos > 0 {
      self
        .output_store
        .write_all(&self.uncompressed_buffer[..buffer_pos])?;
    }
    return Ok(());
  }
}
