pub trait ToBits {
  fn to_bits(&self) -> Box<[bool]>;
}
pub trait FromBits {
  fn size() -> usize;
  fn from_bits<I>(_: I) -> Self
  where
    I: IntoIterator<Item = bool>;
}

macro_rules! implBitwise {
  ($type:path) => {
    impl ToBits for $type {
      fn to_bits(&self) -> Box<[bool]> {
        let size = Self::size();
        let mut v = vec![false; size];
        for i in 0..size {
          v[size - 1 - i] = (self & (1 << i)) != 0;
        }
        v.into_boxed_slice()
      }
    }
    impl FromBits for $type {
      fn size() -> usize {
        <($type)>::max_value().count_ones() as usize
      }
      fn from_bits<I>(bits: I) -> Self
      where
        I: IntoIterator<Item = bool>,
      {
        let mut data: $type = 0;
        for bit in bits.into_iter() {
          data = (data << 1) | (if bit { 1 } else { 0 })
        }
        data
      }
    }
  };
}
implBitwise!(u8);
implBitwise!(u16);
implBitwise!(u32);
implBitwise!(u64);
implBitwise!(u128);
implBitwise!(usize);

impl ToBits for bool {
  fn to_bits(&self) -> Box<[bool]> {
    vec![*self].into_boxed_slice()
  }
}
impl FromBits for bool {
  fn size() -> usize {
    1
  }
  fn from_bits<I>(bits: I) -> Self
  where
    I: IntoIterator<Item = bool>,
  {
    bits.into_iter().next().unwrap_or(false)
  }
}

impl<I: ToBits> ToBits for Vec<I> {
  fn to_bits(&self) -> Box<[bool]> {
    self
      .iter()
      .flat_map(|v| v.to_bits().into_vec())
      .collect::<Vec<_>>()
      .into_boxed_slice()
  }
}

impl<I: ToBits> ToBits for &[I] {
  fn to_bits(&self) -> Box<[bool]> {
    self
      .iter()
      .flat_map(|v| v.to_bits().into_vec())
      .collect::<Vec<_>>()
      .into_boxed_slice()
  }
}

#[cfg(test)]
mod tests {
  use super::*;

  #[test]
  fn test_to_bits() {
    assert_eq!(
      &0x1u8.to_bits()[..],
      &[false, false, false, false, false, false, false, true]
    );
    assert_eq!(&0x0u128.to_bits()[..], &[false; 128][..]);
    assert_eq!(
      &0xffu8.to_bits()[..],
      &[true, true, true, true, true, true, true, true]
    );
    assert_eq!(
      &0xf0u8.to_bits()[..],
      &[true, true, true, true, false, false, false, false]
    );
  }
}
