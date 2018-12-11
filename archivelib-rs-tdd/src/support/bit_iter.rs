pub trait BitwiseIterable {
  fn into_bits(self) -> Box<[bool]>;
}

struct BitwiseIter {
  value: u128,
}

impl Iterator for BitwiseIter {
  type Item = bool;
  fn next(&mut self) -> Option<Self::Item> {
    let lsb = self.value & 1;
    self.value = self.value >> 1;
    Some(lsb == 1)
  }
}

macro_rules! implBitwise {
  ($type:path) => {
    impl BitwiseIterable for $type {
      fn into_bits(self) -> Box<[bool]> {
        BitwiseIter { value: self.into() }
          .take(<($type)>::max_value().count_ones() as usize)
          .collect::<Vec<_>>()
          .into_boxed_slice()
      }
    }
  };
}
implBitwise!(u8);
implBitwise!(u16);
implBitwise!(u32);
implBitwise!(u64);
implBitwise!(u128);

#[cfg(test)]
mod tests {
  use super::*;

  #[test]
  fn test_into_bits() {
    assert_eq!(
      0x1u8.into_bits()[..],
      &[true, false, false, false, false, false, false, false]
    );
    assert_eq!(0x0u128.into_bits()[..], &[false; 128]);
    assert_eq!(
      0xffu8.into_bits()[..],
      &[true, true, true, true, true, true, true, true]
    );
    assert_eq!(
      0xf0u8.into_bits()[..],
      &[false, false, false, false, true, true, true, true]
    );
  }
}
