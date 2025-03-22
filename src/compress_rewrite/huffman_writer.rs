use crate::{
  build_from_frequency,
  huffman::{builder::frequency::RootNode, sorts::SortAlgorithm},
};

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Output {
  Encoded(usize),
  Bits(u16, usize),
}

pub fn write_byte_frequency_tree(
  root: &RootNode,
  encoding: &[(u16, usize)],
  sort_algorithm: &impl SortAlgorithm,
) -> Vec<(u16, usize)> {
  if let RootNode::Leaf(value, _) = root {
    vec![(0, 5), (0, 5), (0, 9), (cast!((*value) as u16), 9)]
  } else {
    let (encoding_output, byte_encoding_freq) = build_byte_run_encoding_output(&encoding);
    let (byte_encoding_root, byte_encoding_encoding) =
      build_from_frequency(&byte_encoding_freq, sort_algorithm).unwrap();

    let mut output = write_bit_length_tree(
      &byte_encoding_root,
      &byte_encoding_encoding,
      BitLengthTreeType::ByteEncodingLength,
    );
    output.extend(encoding_output.into_iter().map(|output| match output {
      Output::Bits(bits, bit_len) => (bits, bit_len),
      Output::Encoded(value) => byte_encoding_encoding[value],
    }));

    output
  }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum BitLengthTreeType {
  ByteEncodingLength,
  OffsetBitLength,
}

pub fn write_bit_length_tree(
  root: &RootNode,
  encoding: &[(u16, usize)],
  tree_type: BitLengthTreeType,
) -> Vec<(u16, usize)> {
  if let RootNode::Leaf(encoding_value, _) = root {
    vec![(0, 5), (cast!((*encoding_value) as u16), 5)]
  } else {
    let last_entry_idx = encoding
      .iter()
      .enumerate()
      .rev()
      .find(|(_, (_, bit_len))| *bit_len != 0)
      .map(|(idx, _)| idx)
      .unwrap_or(0);

    let mut output = vec![(cast!((last_entry_idx + 1) as u16), 5)];

    let mut idx: usize = 0;
    while idx <= last_entry_idx {
      let (_, bit_length) = encoding[idx];
      idx += 1;
      if bit_length <= 6 {
        output.push((cast!(bit_length as u16), 3));
      } else {
        output.push((0xFFFE, bit_length - 3));
      }

      if idx == 3 && tree_type == BitLengthTreeType::ByteEncodingLength {
        // The original code has this as `run_start_check` == `idx`; however given the function only has
        //  2 calling sites, and the only values that are passed are `3` and `-1`, I've chosen to simply
        //  check for `== 3` twice.

        while idx < 6 && encoding[idx].1 == 0 {
          idx += 1
        }
        // Write out `0b00` if the smallest encoded value uses a 1 bit encoding; `0b01` for a 2 bit
        //  encoding etc. up to `0b11` for a 3 or higher bit encoding.
        output.push((cast!((idx - 3) as u16), 2));
      }
    }

    output
  }
}

fn gap_encoding(gap: usize) -> Vec<Output> {
  match gap {
    0 => unreachable!(),
    1..=2 => vec![Output::Encoded(0); gap],
    3..=18 => vec![Output::Encoded(1), Output::Bits((gap - 3) as u16, 4)],
    19 => vec![Output::Encoded(0), Output::Encoded(1), Output::Bits(15, 4)],
    20.. => vec![Output::Encoded(2), Output::Bits((gap - 20) as u16, 9)],
  }
}
fn bit_length_encoding(bit_len: usize) -> Vec<Output> {
  match bit_len {
    0 => unreachable!(),
    bit_len => vec![Output::Encoded(bit_len + 2)],
  }
}

fn build_byte_run_encoding_output(encoding: &[(u16, usize)]) -> (Vec<Output>, [u16; 19]) {
  let largest_value = encoding
    .iter()
    .enumerate()
    .rev()
    .find(|(_, (_, bit_len))| *bit_len != 0)
    .map(|(idx, _)| idx)
    .unwrap_or(0);

  let mut data = vec![Output::Bits(cast!((largest_value + 1) as u16), 9)];
  let mut freq = [0; 19];

  let mut idx = 0;
  while idx <= largest_value {
    let bit_length: usize = encoding[idx].1;
    idx += 1;

    let entry = if bit_length == 0 {
      let mut distance_to_next_value = 1;
      while idx <= largest_value && encoding[idx].1 == 0 {
        idx += 1;
        distance_to_next_value += 1
      }
      gap_encoding(distance_to_next_value)
    } else {
      bit_length_encoding(bit_length)
    };

    for e in entry.iter() {
      if let Output::Encoded(v) = e {
        freq[*v] += 1;
      }
    }
    data.extend_from_slice(&entry);
  }

  (data, freq)
}

#[cfg(test)]
mod test {
  use crate::ARCHIVE_LIB_SORT_ALGORITHM;

  use super::*;

  macro_rules! build_encoding {
    (
      length = $len: literal;
      $($idx: literal => $val: literal),+
    ) => {{
      let mut values = [(0,0); $len];
      $(
        {
          values[$idx] = (
            $val as u16,
            stringify!($val).len() - 2
          )
        }
      )+
      values
    }};
  }

  #[test]
  fn test_bit_length_tree_with_byte_encoding_length() {
    let output = write_bit_length_tree(
      &RootNode::Node(3),
      &build_encoding!(
        length = 19;
        2 => 0b0,
        3 => 0b1
      ),
      BitLengthTreeType::ByteEncodingLength,
    );
    assert_eq!(
      output,
      [
        (0b00100, 5),
        (0b000, 3),
        (0b000, 3),
        (0b001, 3),
        (0b00, 2),
        (0b001, 3)
      ]
    );
  }

  #[test]
  fn test_bit_length_tree_with_offset_length() {
    let output = write_bit_length_tree(
      &RootNode::Node(3),
      &build_encoding!(
        length = 19;
        2 => 0b0,
        3 => 0b1
      ),
      BitLengthTreeType::OffsetBitLength,
    );
    assert_eq!(
      output,
      [(0b00100, 5), (0b000, 3), (0b000, 3), (0b001, 3), (0b001, 3)]
    );
  }

  #[test]
  fn test_byte_encoding_tree() {
    let output = write_byte_frequency_tree(
      &RootNode::Node(3),
      &build_encoding!(
        length = 511;
        97 => 0b0,
        510 => 0b1
      ),
      &ARCHIVE_LIB_SORT_ALGORITHM,
    );
    assert_eq!(
      output,
      [
        (0b00100, 5),
        (0b000, 3),
        (0b000, 3),
        (0b001, 3),
        (0b00, 2),
        (0b001, 3),
        (0b111111111, 9),
        (0b0, 1),
        (0b001001101, 9),
        (0b1, 1),
        (0b0, 1),
        (0b110001000, 9),
        (0b1, 1),
      ]
    );
  }

  #[test]
  fn test_build_byte_run_encoding_output() {
    let (output, frequency) = build_byte_run_encoding_output(&build_encoding!(
      length = 511;
      97 => 0b0,
      510 => 0b1
    ));

    assert_eq!(
      frequency,
      [0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    );
    assert_eq!(
      output,
      [
        Output::Bits(511, 9),
        Output::Encoded(2),
        Output::Bits(77, 9),
        Output::Encoded(3),
        Output::Encoded(2),
        Output::Bits(392, 9),
        Output::Encoded(3)
      ]
    );
  }
  #[test]
  fn test_build_byte_run_encoding_output_with_dense_tree() {
    let (output, frequency) = build_byte_run_encoding_output(&build_encoding!(
      length = 511;
      96 => 0b01,
      97 => 0b10,
      99 => 0b00,
      102 => 0b1100,
      120 => 0b1101,
      140 => 0b1110,
      510 => 0b1111
    ));

    assert_eq!(
      frequency,
      [4, 2, 2, 0, 3, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    );
    assert_eq!(
      output,
      vec![
        Output::Bits(511, 9),
        Output::Encoded(2),
        Output::Bits(76, 9),
        Output::Encoded(4),
        Output::Encoded(4),
        Output::Encoded(0),
        Output::Encoded(4),
        Output::Encoded(0),
        Output::Encoded(0),
        Output::Encoded(6),
        Output::Encoded(1),
        Output::Bits(14, 4),
        Output::Encoded(6),
        Output::Encoded(0),
        Output::Encoded(1),
        Output::Bits(15, 4),
        Output::Encoded(6),
        Output::Encoded(2),
        Output::Bits(349, 9),
        Output::Encoded(6),
      ]
    );
  }
}
