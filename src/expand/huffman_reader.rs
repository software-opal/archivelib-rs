use crate::consts::EOF_FLAG;
use crate::huffman::builder::depths::huffman_depths_to_tree;
use crate::huffman::tree::Node;
use crate::support::BitwiseRead;

use super::{DecompressError, Result};

pub fn read_byte_frequency_tree<R: BitwiseRead>(reader: &mut R) -> Result<Node> {
  let encoding_bit_len_tree = read_bit_length_tree(reader, BitLengthTreeType::ByteEncodingLength)?;
  let max_entries = cast!((EOF_FLAG + 1) as u16);
  let entries = reader.read_bits(9)?.min(max_entries);

  if entries == 0 {
    let only_node_value = reader.read_bits(9)?;
    if only_node_value >= max_entries {
      Err(DecompressError::InvalidBinaryTree)
    } else {
      Ok(Node::Leaf(only_node_value as usize, 0))
    }
  } else {
    let mut node_depths = vec![0; max_entries as usize];
    let mut idx = 0;
    while idx < entries {
      // Read in based on the tree
      // If 0..=2, then gap
      // Otherwise bit length

      let value = read_encoding(reader, &encoding_bit_len_tree)?;
      idx += match value {
        // Handles 1 or 2 gap(via repeats), and 19
        0 => 1,
        // Handles gaps 3..=18, and 19
        1 => 3 + reader.read_bits(4)?,
        // Handles gaps 20..
        2 => 20 + reader.read_bits(9)?,
        // Handles node depths between 1..=16
        v @ 3..=18 => {
          node_depths[idx as usize] = (v - 2) as u16;
          1
        }
        _ => unreachable!(),
      }
    }
    Ok(huffman_depths_to_tree(&node_depths)?)
  }
}

pub fn read_encoding<R: BitwiseRead>(
  reader: &mut R,
  encoding_bit_len_tree: &Node,
) -> Result<usize> {
  let mut bit_iter = reader.iter_bits();
  let v = encoding_bit_len_tree.parse_value(&mut bit_iter);
  bit_iter.error()?;
  match v {
    Some(v) => Ok(v),
    None => panic!("Binary tree was invalid"),
  }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum BitLengthTreeType {
  ByteEncodingLength,
  OffsetBitLength,
}

pub fn read_bit_length_tree<R: BitwiseRead>(
  reader: &mut R,
  tree_type: BitLengthTreeType,
) -> Result<Node> {
  let max_entries = match tree_type {
    BitLengthTreeType::ByteEncodingLength => 19,
    BitLengthTreeType::OffsetBitLength => 15,
  };
  let entries = reader.read_bits(5)?.min(max_entries).into();

  if entries == 0 {
    let only_node_value = reader.read_bits(5)?;
    if only_node_value >= max_entries {
      Err(DecompressError::InvalidBinaryTree)
    } else {
      Ok(Node::Leaf(only_node_value as usize, 0))
    }
  } else {
    let mut bit_lengths = vec![0; max_entries as usize];

    let mut idx = 0;
    while idx < entries {
      let mut bit_length = reader.read_bits(3)?;
      if bit_length == 7 {
        // This indicates a bit length 7 or more. It's encoded as (bit_length - 3) `1` bits, and
        //  then a `0` bit.
        // So `7` is `0b1110`, and `8` is `0b11110` etc.
        while reader.read_bit()? {
          bit_length += 1;
        }
      }
      bit_lengths[idx] = bit_length;
      idx += 1;

      if idx == 3 && tree_type == BitLengthTreeType::ByteEncodingLength {
        let skip = reader.read_bits(2)?;
        idx += skip as usize;
      }
    }

    Ok(huffman_depths_to_tree(&bit_lengths)?)
  }
}

#[cfg(test)]
mod test {
  use crate::{expand::DecompressError, support::reader::BitBasedBitwiseReader};

  use super::*;

  mod read_bit_length_tree {
    use super::*;
    #[test]
    fn test_read_1_node_encoding_length_tree() {
      /*
       ─ 0x001
      */
      let mut reader =
        BitBasedBitwiseReader::from_bit_string("00000 00001").prevent_read_beyond_data();

      let node = read_bit_length_tree(&mut reader, BitLengthTreeType::ByteEncodingLength).unwrap();
      assert_eq!(node, Node::Leaf(1, 0));

      reader.assert_read_exhausted();
    }
    #[test]
    fn test_read_1_deep_bit_encoding_length_tree() {
      /*
       ┬─0─ 0x002
       └─1─ 0x003
      */
      let mut reader = BitBasedBitwiseReader::from_bit_string("00100 000 000 001 00 001")
        .prevent_read_beyond_data();

      let node = read_bit_length_tree(&mut reader, BitLengthTreeType::ByteEncodingLength).unwrap();
      assert_eq!(node, Node::branch(Node::Leaf(2, 0), Node::Leaf(3, 0)));

      reader.assert_read_exhausted();
    }
    #[test]
    fn test_read_7_deep_bit_encoding_length_tree() {
      /*
       ┬─0─ 0x00B
       └─1─┬─0─ 0x00A
           └─1─┬─0─ 0x009
               └─1─┬─0─ 0x008
                   └─1─┬─0─ 0x002
                       └─1─┬─0─ 0x001
                           └─1─┬─0─ 0x005
                               └─1─ 0x006
      */

      let mut reader = BitBasedBitwiseReader::from_bit_string(
        "01100 000 110 101 10 1110 1110 000 100 011 010 001",
      )
      .prevent_read_beyond_data();

      let node = read_bit_length_tree(&mut reader, BitLengthTreeType::ByteEncodingLength).unwrap();
      assert_eq!(
        node,
        Node::branch(
          Node::Leaf(0x00B, 0),
          Node::branch(
            Node::Leaf(0x00A, 0),
            Node::branch(
              Node::Leaf(0x009, 0),
              Node::branch(
                Node::Leaf(0x008, 0),
                Node::branch(
                  Node::Leaf(0x002, 0),
                  Node::branch(
                    Node::Leaf(0x001, 0),
                    Node::branch(Node::Leaf(0x005, 0), Node::Leaf(0x006, 0)),
                  ),
                ),
              ),
            ),
          ),
        )
      );
      reader.assert_read_exhausted();
    }
    #[test]
    fn test_read_invalid_tree() {
      /*
       ┬─0─ ??
       └─1─┬─0─ 0x000
           └─1─ 0x001
      */

      let mut reader =
        BitBasedBitwiseReader::from_bit_string("00010 010 010").prevent_read_beyond_data();

      let result = read_bit_length_tree(&mut reader, BitLengthTreeType::ByteEncodingLength);
      assert!(
        matches!(result, Err(DecompressError::InvalidBinaryTree)),
        "Incorrect error: {:?}",
        result
      );
      reader.assert_read_exhausted();
    }
  }

  mod read_byte_frequency_tree {
    use super::*;

    #[test]
    fn test_read_1_node_encoding_length_tree() {
      /*
       ─ 0x001
      */
      let mut reader = BitBasedBitwiseReader::from_bit_string("00000 00000 000000000 000000001")
        .prevent_read_beyond_data();

      let node = read_byte_frequency_tree(&mut reader).unwrap();
      reader.assert_read_exhausted();
      assert_eq!(node, Node::Leaf(1, 0));
    }

    #[test]
    fn test_small_bit_tree() {
      /*
       ┬─0─ 0x061
       └─1─┬─0─ 0x063
           └─1─┬─0─ 0x062
               └─1─ 0x1FE
      */
      let mut reader = BitBasedBitwiseReader::from_bit_string(
        "00110 000 000 001 00 011 011 010 111111111 0 001001101 110 10 111 0 110000110 10",
      )
      .prevent_read_beyond_data();
      let node = read_byte_frequency_tree(&mut reader).unwrap();
      assert_eq!(
        node,
        Node::branch(
          Node::Leaf(0x061, 0),
          Node::branch(
            Node::Leaf(0x063, 0),
            Node::branch(Node::Leaf(0x062, 0), Node::Leaf(0x1FE, 0),),
          ),
        )
      );
      reader.assert_read_exhausted();
    }

    #[test]
    fn test_medium_sized_bit_tree() {
      /*
        ┬─0─┬─0─ 0x101
        │   └─1─┬─0─ 0x05A
        │       └─1─ 0x061
        └─1─┬─0─┬─0─ 0x062
            │   └─1─ 0x063
            └─1─┬─0─ 0x064
                └─1─ 0x1FE
      */
      let mut reader = BitBasedBitwiseReader::from_bit_string(
        "00110 000 011 010 01 011 001 111111111 10 001000110 0 110 0011 0 0 0 0 10 010001000 111 10 011101000 0"
      )
      .prevent_read_beyond_data();
      let node = read_byte_frequency_tree(&mut reader).unwrap();
      reader.assert_read_exhausted();
      assert_eq!(
        node,
        Node::branch(
          Node::branch(
            Node::Leaf(0x101, 0),
            Node::branch(Node::Leaf(0x05A, 0), Node::Leaf(0x061, 0)),
          ),
          Node::branch(
            Node::branch(Node::Leaf(0x062, 0), Node::Leaf(0x063, 0),),
            Node::branch(Node::Leaf(0x064, 0), Node::Leaf(0x1FE, 0),),
          ),
        )
      );
    }
  }
}
