use std::collections::VecDeque;

use crate::huffman::tree::Node;

use super::SortAlgorithm;

#[derive(Debug)]
pub struct ArchiveLibSortAlgorithm {}
impl ArchiveLibSortAlgorithm {
  fn shuffle_smallest_to_front(&self, values: &mut VecDeque<Node>, start_idx: usize) {
    let max_idx = values.len() - 1;
    let start_frequency = values[start_idx].frequency();
    let mut last_idx = start_idx;
    loop {
      let mut current_idx = 2 * last_idx;
      if current_idx > max_idx {
        break;
      }
      if current_idx < max_idx
        && values[current_idx].frequency() > values[current_idx + 1].frequency()
      {
        current_idx += 1
      }
      if start_frequency <= values[current_idx].frequency() {
        break;
      }
      values.swap(last_idx, current_idx);
      last_idx = current_idx
    }
  }
}
impl SortAlgorithm for ArchiveLibSortAlgorithm {
  type List = VecDeque<Node>;
  fn initial_sort(&self, nodes: Vec<Node>) -> VecDeque<Node> {
    let mut deque = VecDeque::from(nodes);
    // Push a null-node to the front of the list so the real nodes start at index 1.
    deque.push_front(Node::Leaf(usize::MAX, u16::MAX));
    for idx in (1..=(deque.len() / 2)).rev() {
      self.shuffle_smallest_to_front(&mut deque, idx);
    }
    deque
  }
  fn pop_smallest_node(&self, nodes: &mut VecDeque<Node>) -> Option<Node> {
    if nodes.len() <= 1 {
      return None;
    }
    self.shuffle_smallest_to_front(nodes, 1);
    // The original implementation copied the last node over the first and reduced the length by
    //  one. We need to emulate this to prevent issues.
    nodes.swap(1, nodes.len() - 1);
    let val = nodes.pop_back();
    val
  }
  fn insert_node(&self, nodes: &mut VecDeque<Node>, new_node: Node) {
    let null_node = nodes.pop_front().unwrap();
    // Pull the node we swapped to the front off again so we can put it on the back, which matches
    //  the original implementation's logic.
    let front_node = nodes.pop_front();
    nodes.push_front(new_node);
    nodes.push_front(null_node);
    if let Some(node) = front_node {
      nodes.push_back(node);
    }
  }
}

pub const ARCHIVE_LIB_SORT_ALGORITHM: ArchiveLibSortAlgorithm = ArchiveLibSortAlgorithm {};

#[cfg(test)]
mod tests {
  use super::*;

  fn nodes_from_freq(freqs: impl IntoIterator<Item = u16>) -> Vec<Node> {
    freqs
      .into_iter()
      .enumerate()
      .map(|(idx, freq)| Node::Leaf(idx, freq))
      .collect()
  }

  #[test]
  fn test_initial_sorting() {
    let algo = ArchiveLibSortAlgorithm {};
    let nodes = nodes_from_freq(1..20);
    let deque = algo.initial_sort(nodes);
    assert_eq!(deque.get(1), Some(&Node::Leaf(0, 1)));
  }

  #[test]
  fn test_removals() {
    let algo = ArchiveLibSortAlgorithm {};
    let nodes = nodes_from_freq(1..3);
    let mut deque = algo.initial_sort(nodes);
    assert_eq!(algo.pop_smallest_node(&mut deque), Some(Node::Leaf(0, 1)));
    assert_eq!(algo.pop_smallest_node(&mut deque), Some(Node::Leaf(1, 2)));
    assert_eq!(algo.pop_smallest_node(&mut deque), None);
  }
  #[test]
  fn test_removals_in_reverse_order() {
    let algo = ArchiveLibSortAlgorithm {};
    let nodes = nodes_from_freq((300..=305).rev());
    let mut deque = algo.initial_sort(nodes);
    assert_eq!(algo.pop_smallest_node(&mut deque), Some(Node::Leaf(5, 300)));
    assert_eq!(algo.pop_smallest_node(&mut deque), Some(Node::Leaf(4, 301)));
    assert_eq!(algo.pop_smallest_node(&mut deque), Some(Node::Leaf(3, 302)));
    assert_eq!(algo.pop_smallest_node(&mut deque), Some(Node::Leaf(2, 303)));
    assert_eq!(algo.pop_smallest_node(&mut deque), Some(Node::Leaf(1, 304)));
    assert_eq!(algo.pop_smallest_node(&mut deque), Some(Node::Leaf(0, 305)));
    assert_eq!(algo.pop_smallest_node(&mut deque), None);
  }
  #[test]
  fn test_removals_then_inserts() {
    let algo = ArchiveLibSortAlgorithm {};
    let nodes = nodes_from_freq(1..3);
    let mut deque = algo.initial_sort(nodes);
    assert_eq!(algo.pop_smallest_node(&mut deque), Some(Node::Leaf(0, 1)));
    assert_eq!(algo.pop_smallest_node(&mut deque), Some(Node::Leaf(1, 2)));
    algo.insert_node(&mut deque, Node::Leaf(10, 9000));
    assert_eq!(
      algo.pop_smallest_node(&mut deque),
      Some(Node::Leaf(10, 9000))
    );
    assert_eq!(algo.pop_smallest_node(&mut deque), None);
  }
  #[test]
  fn test_large_dataset() {
    let algo = ArchiveLibSortAlgorithm {};
    let nodes = nodes_from_freq((1..=300).rev());
    let mut deque: VecDeque<Node> = algo.initial_sort(nodes);
    assert_eq!(algo.pop_smallest_node(&mut deque), Some(Node::Leaf(299, 1)));
    assert_eq!(algo.pop_smallest_node(&mut deque), Some(Node::Leaf(298, 2)));
    algo.insert_node(&mut deque, Node::Branch(
      Box::new(Node::Leaf(299, 1)),
      Box::new(Node::Leaf(298, 2)),
      3
    ));
    assert_eq!(
      algo.pop_smallest_node(&mut deque),
      Some(Node::Branch(
        Box::new(Node::Leaf(299, 1)),
        Box::new(Node::Leaf(298, 2)),
        3
      ))
    );
    assert_eq!(algo.pop_smallest_node(&mut deque), Some(Node::Leaf(297, 3)));
  }

  #[test]
  fn test_quirks() {
    let algo = ArchiveLibSortAlgorithm {};
    let nodes = nodes_from_freq((300..=306).rev());
    let mut nodes = algo.initial_sort(nodes);

    assert_eq!(nodes.get(1), Some(&Node::Leaf(6, 300)));
    let (back_idx, back_freq) = if let Some(Node::Leaf(back_idx, back_freq)) = nodes.back() {
      (*back_idx, *back_freq)
    } else {
      panic!()
    };

    assert_eq!(algo.pop_smallest_node(&mut nodes), Some(Node::Leaf(6, 300)));
    assert_eq!(nodes.get(1), Some(&Node::Leaf(back_idx, back_freq)));

    algo.insert_node(&mut nodes, Node::Leaf(10, 9000));
    assert_eq!(nodes.get(1), Some(&Node::Leaf(10, 9000)));
    assert_eq!(nodes.back(), Some(&Node::Leaf(back_idx, back_freq)));

    assert_eq!(algo.pop_smallest_node(&mut nodes), Some(Node::Leaf(5, 301)));
    assert_eq!(nodes.get(1), Some(&Node::Leaf(10, 9000)));
  }
  #[test]
  fn test_insert_quirks_on_short_data() {
    let algo = ArchiveLibSortAlgorithm {};
    let mut nodes = algo.initial_sort(vec![Node::Leaf(1, 2)]);

    assert_eq!(nodes.get(1), Some(&Node::Leaf(1, 2)));
    algo.pop_smallest_node(&mut nodes);
    algo.insert_node(&mut nodes, Node::Leaf(10, 9000));
    assert_eq!(nodes.get(1), Some(&Node::Leaf(10, 9000)));
  }
}
