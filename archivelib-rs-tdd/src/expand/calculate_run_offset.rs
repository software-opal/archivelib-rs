use crate::consts::{CONST_N141_IS_511, CONST_N142_IS_15, CONST_N145_IS_19};
use crate::expand::reader::BitwiseReadAheadRead;
use crate::expand::{RExpandData, Result};
use crate::support::{BitwiseRead, BitwiseWrite};

impl<R: BitwiseReadAheadRead, W: BitwiseWrite> RExpandData<R, W> {
  pub fn calculate_run_offset(&mut self) -> Result<uint16_t> {
    let mut run_length276: u16 = self.dat_arr241[self.input_store.read_bits(8) as usize];
    if run_length276 as libc::c_int >= CONST_N142_IS_15 {
      loop {
        if self.input_store.read_bit()? {
          run_length276 = self.dat_arr190[run_length276 as usize]
        } else {
          run_length276 = self.dat_arr189[run_length276 as usize]
        }
        if !(run_length276 as libc::c_int >= CONST_N142_IS_15) {
          break;
        }
      }
    }
    read_bits(
      data,
      *self.dat_arr181.offset(run_length276 as isize) as int32_t,
    );
    if run_length276 as libc::c_int != 0i32 {
      run_length276 = run_length276.wrapping_sub(1);
      run_length276 = (1u32 << run_length276 as libc::c_int)
        .wrapping_add(get_bits(data, run_length276 as uint8_t) as libc::c_uint)
        as int16_t as uint16_t
    }
    return run_length276;
  }
}
