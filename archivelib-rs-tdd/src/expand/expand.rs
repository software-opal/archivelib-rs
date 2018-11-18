#![allow(
  dead_code,
  mutable_transmutes,
  non_camel_case_types,
  non_snake_case,
  non_upper_case_globals,
  unused_mut
)]

use crate::consts::{END_OF_FILE_FLAG, MIN_RUN_LENGTH135_IS_3, MAX_RUN_LENGTH140};
use crate::expand::{RExpandData, Result};
use crate::support::{BitwiseRead, BitwiseWrite};

const U8_MAX: u16 = u8::max_value() as u16;

impl<R: BitwiseRead, W: BitwiseWrite> RExpandData<R, W> {
  pub fn expand(self: &mut Self) -> Result<()> {
    let mut buffer_pos: i16 = 0;

    let l_uncompressed_buffer278 = &self.uncompressed_buffer;
    let max_size279: usize = self.max_uncompressed_data_size as usize;
    let max_run_start = max_size279 - MAX_RUN_LENGTH140 - 1;

    // Seed bits182 with the first 2 bits
    self.read_bits(2 * 8);
    while self.error_counter243 < 5 {
      let byte_or_run_length203 = self.get_next_item();
      if byte_or_run_length203 <= U8_MAX {
        // byte_or_run_length203 is the decompressed byte
        l_uncompressed_buffer278[buffer_pos] = byte_or_run_length203 as u8;
        buffer_pos += 1;
        if buffer_pos >= max_size279 {
          buffer_pos = 0;
          self
            .output_store
            .write_all(l_uncompressed_buffer278[..max_size279])?;
        }
      } else {
        // Copy the run of `run_length276` bytes from earlier in the output.
        // byte_or_run_length203 >= 0x100 indicates a flag
        // run_length276 = byte_or_run_length203 - 0x100 + 3; which is the length
        // of the run. Flag value of byte_or_run_length203 ==
        let run_length276 = MIN_RUN_LENGTH135_IS_3 + byte_or_run_length203 - (U8_MAX + 1);
        if run_length276 == END_OF_FILE_FLAG {
          // byte_or_run_length203 == 0x1FE. End of file.
          break;
        } else {
          let run_start226 = (buffer_pos - self.calculate_run_offset() - 1) % max_size279;
          if run_start226 < max_run_start && buffer_pos < max_run_start {
            loop {
              run_length276 -= 1;
              if !(run_length276 >= 0) {
                break;
              }
              l_uncompressed_buffer278[buffer_pos] = l_uncompressed_buffer278[run_start226];
              buffer_pos += 1;
              run_start226 += 1;
            }
          } else {
            loop {
              run_length276 -= 1;
              if !(run_length276 >= 0) {
                break;
              }
              l_uncompressed_buffer278[buffer_pos] = l_uncompressed_buffer278[run_start226];
              buffer_pos += 1;
              if buffer_pos >= max_size279 {
                buffer_pos = 0;
                self
                  .output_store
                  .write_all(l_uncompressed_buffer278[..max_size279])?;
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
        .write_all(l_uncompressed_buffer278[..buffer_pos]);
    }
    return Ok(());
  }
}
