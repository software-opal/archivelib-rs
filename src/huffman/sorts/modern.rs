use std::{cmp::Reverse, collections::BinaryHeap};

use crate::huffman::tree::Node;

use super::SortAlgorithm;

#[derive(Debug)]
pub struct ModernSortAlgorithm {}
impl SortAlgorithm for ModernSortAlgorithm {
  type List = BinaryHeap<Reverse<Node>>;
  fn initial_sort(&self, nodes: Vec<Node>) -> BinaryHeap<Reverse<Node>> {
    // Reverse sort, so the smallest key is at the end.
    BinaryHeap::from_iter(nodes.into_iter().map(Reverse))
  }
  fn pop_smallest_node(&self, nodes: &mut BinaryHeap<Reverse<Node>>) -> Option<Node> {
    nodes.pop().map(|r| r.0)
  }
  fn insert_node(&self, nodes: &mut BinaryHeap<Reverse<Node>>, new_node: Node) {
    nodes.push(Reverse(new_node));
  }
}

pub const MODERN_SORT_ALGORITHM: ModernSortAlgorithm = ModernSortAlgorithm {};
