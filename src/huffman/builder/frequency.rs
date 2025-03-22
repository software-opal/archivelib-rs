use core::panic;
use std::{cmp::min, u16, usize};

use crate::huffman::{sorts::SortAlgorithm, tree::Node};

// Bits, Bit length
type Encoding = (u16, usize);

#[derive(PartialEq, PartialOrd, Copy, Clone, Debug)]
pub enum RootNode {
  Leaf(usize, u16),
  Node(u16),
}
impl RootNode {
  pub fn frequency(&self) -> u16 {
    match self {
      RootNode::Leaf(_, freq) => *freq,
      RootNode::Node(freq) => *freq,
    }
  }
}

#[derive(PartialEq, Eq, Debug)]
pub enum EncodingFailure {
  NoNodes,
}

pub fn build_from_frequency(
  leaf_frequency_data: &[u16],
  sort_algorithm: &impl SortAlgorithm,
) -> Result<(RootNode, Vec<Encoding>), EncodingFailure> {
  let leaf_freq_size = leaf_frequency_data.len();
  let (leaves_in_visit_order, root_node) = build_initial_tree(leaf_frequency_data, sort_algorithm)?;

  let root_branch_freq = match root_node {
    Node::Branch(_, _, freq) => freq,
    Node::Leaf(node, freq) => {
      return Ok((RootNode::Leaf(node, freq), vec![(0, 0); leaf_freq_size]));
    }
  };

  // Mapping of leaf indexes to depths.
  let depth_counts = count_leaf_node_depths(&root_node);
  let leaf_depths = assign_depths_to_leaves(leaf_freq_size, leaves_in_visit_order, depth_counts);

  let mut next_value_for_depth = [0_u16; 17];
  for i in 1..16 {
    next_value_for_depth[i + 1] = (next_value_for_depth[i] + depth_counts[i]) << 1;
  }
  let encodings = leaf_depths
    .into_iter()
    .map(|depth: usize| {
      if depth == 0 {
        (0, 0)
      } else {
        let encoding = next_value_for_depth[depth];
        next_value_for_depth[depth] += 1;
        (encoding, depth)
      }
    })
    .collect();

  Ok((RootNode::Node(root_branch_freq), encodings))
}

fn build_initial_tree(
  leaf_frequency_data: &[u16],
  sort_algorithm: &impl SortAlgorithm,
) -> Result<(Vec<usize>, Node), EncodingFailure> {
  let nodes_to_visit: Vec<_> = leaf_frequency_data
    .into_iter()
    .enumerate()
    .filter_map(|(idx, &freq)| {
      if freq > 0 {
        Some(Node::Leaf(idx, freq))
      } else {
        None
      }
    })
    .collect();

  if nodes_to_visit.len() < 2 {
    if let Some(node) = nodes_to_visit.into_iter().next() {
      return Ok((vec![], node));
    } else {
      return Err(EncodingFailure::NoNodes);
    }
  }

  let mut leaves_in_visit_order = Vec::with_capacity(nodes_to_visit.len());
  let mut nodes_to_visit = sort_algorithm.initial_sort(nodes_to_visit);
  let root_node;
  loop {
    let smallest_node = if let Some(node) = sort_algorithm.pop_smallest_node(&mut nodes_to_visit) {
      if let Node::Leaf(idx, _) = node {
        leaves_in_visit_order.push(idx);
      }
      node
    } else {
      // We should always have at least 1 node in the list at this point, so this case is an error.
      return Err(EncodingFailure::NoNodes);
    };
    let next_smallest_node =
      if let Some(node) = sort_algorithm.pop_smallest_node(&mut nodes_to_visit) {
        if let Node::Leaf(idx, _) = node {
          leaves_in_visit_order.push(idx);
        }
        node
      } else {
        root_node = smallest_node;
        break;
      };
    let new_node = Node::branch(smallest_node, next_smallest_node);
    sort_algorithm.insert_node(&mut nodes_to_visit, new_node);
  }
  Ok((leaves_in_visit_order, root_node))
}

fn assign_depths_to_leaves(
  leaf_freq_size: usize,
  mut leaves_in_visit_order: Vec<usize>,
  depth_counts: [u16; 17],
) -> Vec<usize> {
  let mut leaf_depths = vec![0; leaf_freq_size];
  for (depth, &count) in depth_counts.iter().enumerate() {
    for _ in 0..count {
      let leaf = leaves_in_visit_order.pop().unwrap();
      leaf_depths[leaf] = depth;
    }
  }
  leaf_depths
}

fn count_leaf_node_depths(root_node: &Node) -> [u16; 17] {
  let mut depths = [0; 18];
  do_count_leaf_node_depths(&mut depths, root_node, 0);

  if depths[17] != 0 {
    // Tree was over 16 nodes deep, so we will rebalance the tree's depth counts.
    // TODO: Implement tree rebalancing.
    panic!();
  } else {
    let mut fixed_size_depth = [0; 17];
    fixed_size_depth.copy_from_slice(&depths[0..17]);
    return fixed_size_depth;
  }
}
fn do_count_leaf_node_depths(depths: &mut [u16; 18], node: &Node, current_depth: usize) {
  match node {
    Node::Leaf(_, _) => {
      depths[current_depth] += 1;
    }
    Node::Branch(left, right, _) => {
      let next_depth = min(current_depth + 1, 17);
      do_count_leaf_node_depths(depths, &left, next_depth);
      do_count_leaf_node_depths(depths, &right, next_depth);
    }
  }
}

#[cfg(test)]
mod tests {
  use crate::huffman::sorts::{ARCHIVE_LIB_SORT_ALGORITHM, MODERN_SORT_ALGORITHM};

  use super::*;
  #[test]
  fn test_empty_frequency_data() {
    assert_eq!(
      build_from_frequency(&[0, 0, 0], &MODERN_SORT_ALGORITHM),
      Err(EncodingFailure::NoNodes)
    );
  }

  #[test]
  fn test_one_frequency_only() {
    assert_eq!(
      build_from_frequency(&[10, 0, 0], &MODERN_SORT_ALGORITHM),
      Ok((RootNode::Leaf(0, 10), vec![(0, 0); 3]))
    );
  }

  #[test]
  fn test_two_frequencies() {
    assert_eq!(
      build_from_frequency(&[10, 20, 0], &MODERN_SORT_ALGORITHM),
      Ok((RootNode::Node(30), vec![(0, 1), (1, 1), (0, 0)]))
    );
  }
  #[test]
  fn test_empty_frequency_data_with_al_sorting() {
    assert_eq!(
      build_from_frequency(&[0, 0, 0], &ARCHIVE_LIB_SORT_ALGORITHM),
      Err(EncodingFailure::NoNodes)
    );
  }

  #[test]
  fn test_one_frequency_only_with_al_sorting() {
    assert_eq!(
      build_from_frequency(&[10, 0, 0], &ARCHIVE_LIB_SORT_ALGORITHM),
      Ok((RootNode::Leaf(0, 10), vec![(0, 0); 3]))
    );
  }

  #[test]
  fn test_two_frequencies_with_al_sorting() {
    assert_eq!(
      build_from_frequency(&[10, 20, 0], &ARCHIVE_LIB_SORT_ALGORITHM),
      Ok((RootNode::Node(30), vec![(0, 1), (1, 1), (0, 0)]))
    );
  }

  #[test]
  fn test_three_frequencies_with_al_sorting() {
    assert_eq!(
      build_from_frequency(&[10, 20, 30, 0], &ARCHIVE_LIB_SORT_ALGORITHM),
      Ok((
        RootNode::Node(60),
        vec![(0b10, 2), (0b11, 2), (0b0, 1), (0, 0)]
      ))
    );
  }

  #[test]
  fn test_four_frequencies_with_al_sorting() {
    assert_eq!(
      build_from_frequency(&[10, 20, 30, 9000, 0], &ARCHIVE_LIB_SORT_ALGORITHM),
      Ok((
        RootNode::Node(9060),
        vec![(0b110, 3), (0b111, 3), (0b10, 2), (0b0, 1), (0, 0)]
      ))
    );
  }
  #[test]
  fn test_with_archivelib_sort() {
    let mut freq = vec![0; 511];
    let mut expected_encoding = vec![(0, 0); 511];

    // Real frequency data
    freq[0x020] = 4;
    expected_encoding[0x020] = (0b00, 2);
    freq[0x03B] = 1;
    expected_encoding[0x03B] = (0b11110, 5);
    freq[0x041] = 1;
    expected_encoding[0x041] = (0b0110, 4);
    freq[0x042] = 1;
    expected_encoding[0x042] = (0b0111, 4);
    freq[0x049] = 1;
    expected_encoding[0x049] = (0b1000, 4);
    freq[0x061] = 2;
    expected_encoding[0x061] = (0b010, 3);
    freq[0x068] = 1;
    expected_encoding[0x068] = (0b1001, 4);
    freq[0x06D] = 1;
    expected_encoding[0x06D] = (0b1010, 4);
    freq[0x074] = 1;
    expected_encoding[0x074] = (0b1011, 4);
    freq[0x077] = 1;
    expected_encoding[0x077] = (0b1100, 4);
    freq[0x101] = 1;
    expected_encoding[0x101] = (0b1101, 4);
    freq[0x103] = 1;
    expected_encoding[0x103] = (0b1110, 4);
    freq[0x1FE] = 1;
    expected_encoding[0x1FE] = (0b11111, 5);

    assert_eq!(
      build_from_frequency(&freq, &ARCHIVE_LIB_SORT_ALGORITHM),
      Ok((RootNode::Node(17), expected_encoding))
    );
  }
}
