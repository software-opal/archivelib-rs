use crate::expand::base::DecompressError::InternalError;
use crate::expand::{RExpandData, Result};
use crate::support::BitRead;
use std::io::Write;

pub enum Fn258Mode {
  Fn253,
  Fn255,
}
pub enum Fn258DataTable {
  Array189(usize),
  Array190(usize),
  OutputTable(usize),
}

macro_rules! data_table {
  ($current_table: ident, $output_table262:ident, $self:ident) => {
    match $current_table {
      Fn258DataTable::OutputTable(idx) => $output_table262[idx],
      Fn258DataTable::Array190(idx) => $self.dat_arr190[idx],
      Fn258DataTable::Array189(idx) => $self.dat_arr189[idx],
    }
  };
  (($current_table: ident, $output_table262:ident, $self:ident) = $val: expr) => {
    match $current_table {
      Fn258DataTable::OutputTable(idx) => $output_table262[idx] = $val,
      Fn258DataTable::Array190(idx) => $self.dat_arr190[idx] = $val,
      Fn258DataTable::Array189(idx) => $self.dat_arr189[idx] = $val,
    }
  };
}

impl<R: BitRead, W: Write> RExpandData<R, W> {
  pub fn fn258(
    &mut self,
    mode: Fn258Mode,
    arg_arr260_len: usize,
    bit_size261: usize,
    max_internal263: u16,
  ) -> Result<()> {
    let arg_arr260 = match mode {
      Fn258Mode::Fn253 => &self.dat_arr181,
      Fn258Mode::Fn255 => &self.dat_arr180,
    };
    let output_table262 = match mode {
      Fn258Mode::Fn253 => &mut self.dat_arr241,
      Fn258Mode::Fn255 => &mut self.dat_arr240,
    };

    let mut var277: [u16; 17] = [0; 17];
    let mut lookup_table287: [u16; 17] = [0; 17];
    let mut lookup_table288: [u16; 18] = [0; 18];

    let rem_bit_size291: usize;
    let mut var292: u32;
    let var283: u32;
    let mut i: usize;
    let mut ij: usize;
    for i in 0..(cast!(arg_arr260_len as usize)) {
      var277[arg_arr260[i] as usize] = var277[arg_arr260[i] as usize].wrapping_add(1);
    }
    for i in 1..17 {
      // This wraps around to 0.
      lookup_table288[i + 1] = (lookup_table288[i].wrapping_add((var277[i]) << (16 - i))) as u16;
    }
    if lookup_table288[17] != 0 {
      return Err(InternalError(1));
    }
    rem_bit_size291 = 16 - bit_size261;
    i = 1;
    while i <= bit_size261 {
      lookup_table288[i] = (lookup_table288[i] >> rem_bit_size291) as u16;
      lookup_table287[i] = (1 << (bit_size261).wrapping_sub(i)) as u16;
      i = i.wrapping_add(1)
    }
    while i <= 16 {
      lookup_table287[i] = (1 << (16 - i)) as u16;
      i = i.wrapping_add(1)
    }
    i = (lookup_table288[bit_size261 + 1] >> rem_bit_size291) as usize;
    if i != (1 << 16) {
      let var289 = 1 << bit_size261;
      while i != var289 {
        let fresh0 = i;
        i = i.wrapping_add(1);
        output_table262[cast!(fresh0 as usize)] = 0_u16
      }
    } else {
      unreachable!("This, in theory, is not a reachable case!");
    }
    var292 = cast!(arg_arr260_len as u32);
    var283 = 1 << (15 - bit_size261);
    ij = 0;
    while ij < arg_arr260_len {
      let item209 = arg_arr260[ij] as usize;
      if item209 != 0 {
        let tmp293: usize =
          (lookup_table288[item209] as usize) + (lookup_table287[item209] as usize);
        if item209 <= bit_size261 {
          if tmp293 > cast!(max_internal263 as usize) {
            return Err(InternalError(2));
          } else {
            i = lookup_table288[cast!(item209 as usize)] as usize;
            while i < tmp293 {
              output_table262[i] = cast!(ij as u16);
              i = i.wrapping_add(1)
            }
          }
        } else {
          let mut var289 = u32::from(lookup_table288[cast!(item209 as usize)]);
          let mut current_table = Fn258DataTable::OutputTable((var289 >> rem_bit_size291) as usize);
          i = item209.wrapping_sub(bit_size261);
          while i != 0 {
            if data_table!(current_table, output_table262, self) == 0 {
              self.dat_arr189[cast!(var292 as usize)] = 0;
              self.dat_arr190[cast!(var292 as usize)] = 0;
              data_table!((current_table, output_table262, self) = cast!(var292 as u16));
              var292 = var292.wrapping_add(1);
            }
            if 0 != var289 & var283 {
              current_table =
                Fn258DataTable::Array190(data_table!(current_table, output_table262, self) as usize)
            } else {
              current_table =
                Fn258DataTable::Array189(data_table!(current_table, output_table262, self) as usize)
            }
            var289 <<= 1;
            i = i.wrapping_sub(1)
          }
          data_table!((current_table, output_table262, self) = cast!(ij as u16))
        }
        lookup_table288[cast!(item209 as usize)] = cast_trunc!(tmp293 as u16)
      }
      ij = ij.wrapping_add(1)
    }
    Ok(())
  }
}
