use crate::expand::base::ExpandError::InternalError;
use crate::expand::{RExpandData, Result};
use crate::support::{BitwiseReadAheadRead, BitwiseWrite};

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

impl<R: BitwiseReadAheadRead, W: BitwiseWrite> RExpandData<R, W> {
  pub fn fn258(
    &mut self,
    mode: Fn258Mode,
    arg_arr260_len: u32,
    bit_size261: u32,
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

    let mut _277: [u16; 17] = [0; 17];
    let mut lookup_table287: [u16; 17] = [0; 17];
    let mut lookup_table288: [u16; 18] = [0; 18];

    let mut i: u32 = 0;
    let mut item209: u32 = 0;
    let mut j: u32 = 0;
    let mut rem_bit_size291: u32 = 0;
    let mut _292: u32 = 0;
    let mut tmp293: u32 = 0;
    let mut _283: u32 = 0;

    i = 0 as u32;
    while i < arg_arr260_len {
      _277[arg_arr260[i as usize] as usize] = _277[arg_arr260[i as usize] as usize].wrapping_add(1);
      i = i.wrapping_add(1)
    }
    i = 1 as u32;
    while i < 17 {
      // This wraps around to 0.
      lookup_table288[i.wrapping_add(1) as usize] =
        (lookup_table288[i as usize] + ((_277[i as usize]) << (16 - i))) as u16;
      i = i.wrapping_add(1)
    }
    if lookup_table288[17usize] != 0 {
      return Err(InternalError(1));
    } else {
      rem_bit_size291 = (16 - bit_size261) as u32;
      i = 1 as u32;
      while i <= bit_size261 {
        lookup_table288[i as usize] = (lookup_table288[i as usize] >> rem_bit_size291) as u16;
        lookup_table287[i as usize] = (1 << (bit_size261).wrapping_sub(i)) as u16;
        i = i.wrapping_add(1)
      }
      while i <= 16 {
        lookup_table287[i as usize] = (1 << (16 - i)) as u16;
        i = i.wrapping_add(1)
      }
      i = (lookup_table288[(bit_size261 + 1) as usize] >> rem_bit_size291) as u32;
      if i != (1 << 16) {
        let _289 = 1 << bit_size261;
        while i != _289 {
          let fresh0 = i;
          i = i.wrapping_add(1);
          output_table262[fresh0 as usize] = 0 as u16
        }
      }
      _292 = arg_arr260_len as u32;
      _283 = 1 << 15 - bit_size261;
      j = 0 as u32;
      while j < arg_arr260_len {
        item209 = arg_arr260[j as usize] as u32;
        if !(item209 == 0) {
          tmp293 = (lookup_table288[item209 as usize] + lookup_table287[item209 as usize]) as u32;
          if item209 <= bit_size261 {
            if tmp293 > max_internal263 as u32 {
              return Err(InternalError(2));
            } else {
              i = lookup_table288[item209 as usize] as u32;
              while i < tmp293 {
                output_table262[i as usize] = j as u16;
                i = i.wrapping_add(1)
              }
            }
          } else {
            let mut _289 = lookup_table288[item209 as usize] as u32;
            let mut current_table = Fn258DataTable::OutputTable((_289 >> rem_bit_size291) as usize);
                        i = item209.wrapping_sub(bit_size261);
            while i != 0 {
              if data_table!(current_table, output_table262, self) == 0 {
                (self).dat_arr189[_292 as usize] = 0;
                (self).dat_arr190[_292 as usize] = 0;
                data_table!((current_table, output_table262, self) = _292 as u16);
                _292 = _292.wrapping_add(1);
              }
              if 0 != _289 & _283 {
                current_table= Fn258DataTable::Array190(data_table!(current_table, output_table262, self) as usize)
              } else {
                current_table= Fn258DataTable::Array189(data_table!(current_table, output_table262, self) as usize)
              }
              _289 <<= 1;
              i = i.wrapping_sub(1)
            }
            data_table!((current_table, output_table262, self) = j as u16)
          }
          lookup_table288[item209 as usize] = tmp293 as u16
        }
        j = j.wrapping_add(1)
      }
    }
    Ok(())
  }
}
