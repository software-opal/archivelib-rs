use std::{cmp::Ordering, collections::BinaryHeap};

use crate::huffman::tree::Node;

use super::SortAlgorithm;

#[derive(Debug, PartialEq, Eq)]
pub struct NodeWrapper(Node);

impl PartialOrd for NodeWrapper {
  fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
    Some(self.cmp(other))
  }
}
impl Ord for NodeWrapper {
  fn cmp(&self, other: &Self) -> std::cmp::Ordering {
    self
      .0
      .frequency()
      .cmp(&other.0.frequency())
      .reverse()
      .then_with(|| self.0.cmp(&other.0))
  }
}

#[derive(Debug)]
pub struct ModernSortAlgorithm {}
impl SortAlgorithm for ModernSortAlgorithm {
  type List = BinaryHeap<NodeWrapper>;
  fn initial_sort(&self, nodes: Vec<Node>) -> BinaryHeap<NodeWrapper> {
    // NodeWrapper sort, so the smallest key is at the end.
    BinaryHeap::from_iter(nodes.into_iter().map(NodeWrapper))
  }
  fn pop_smallest_node(&self, nodes: &mut BinaryHeap<NodeWrapper>) -> Option<Node> {
    nodes.pop().map(|r| r.0)
  }
  fn insert_node(&self, nodes: &mut BinaryHeap<NodeWrapper>, new_node: Node) {
    nodes.push(NodeWrapper(new_node));
  }
}

pub const MODERN_SORT_ALGORITHM: ModernSortAlgorithm = ModernSortAlgorithm {};

#[cfg(test)]
mod test {
  use super::*;

  #[test]
  fn test_sort() {
    let mut heap = MODERN_SORT_ALGORITHM.initial_sort(vec![
      Node::Leaf(0, 5),
      Node::Leaf(4, 4),
      Node::Leaf(6, 4),
      Node::Leaf(1, 3),
      Node::Leaf(2, 2),
      Node::Leaf(3, 1),
    ]);
    assert_eq!(
      MODERN_SORT_ALGORITHM.pop_smallest_node(&mut heap),
      Some(Node::Leaf(3, 1))
    );
    assert_eq!(
      MODERN_SORT_ALGORITHM.pop_smallest_node(&mut heap),
      Some(Node::Leaf(2, 2))
    );
    assert_eq!(
      MODERN_SORT_ALGORITHM.pop_smallest_node(&mut heap),
      Some(Node::Leaf(1, 3))
    );
    assert_eq!(
      MODERN_SORT_ALGORITHM.pop_smallest_node(&mut heap),
      Some(Node::Leaf(6, 4))
    );
    assert_eq!(
      MODERN_SORT_ALGORITHM.pop_smallest_node(&mut heap),
      Some(Node::Leaf(4, 4))
    );
    assert_eq!(
      MODERN_SORT_ALGORITHM.pop_smallest_node(&mut heap),
      Some(Node::Leaf(0, 5))
    );
  }
}
