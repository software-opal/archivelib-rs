use super::data::{DataError, LookupTable};
use crate::support::Reader;

// CONST_N147_IS_5 & _220
const LENGTH_BITS: usize = 5;

impl LookupTable {
  fn fn253(
    &mut self,
    input: &mut dyn Reader<std::io::Read>,
    fill_at: Option<usize>,
  ) -> Result<(), DataError> {
    let bits_to_load = input.read_bits(LENGTH_BITS)? as usize;
    if bits_to_load == 0 {
      unimplemented!();
    } else {
      let index = 0;
      while index < bits_to_load {
        let byte = read_253_bitpacked(&mut input);
      }
      unimplemented!();
    }
  }
}
