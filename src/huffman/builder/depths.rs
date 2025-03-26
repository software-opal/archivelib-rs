use std::os::unix::raw::dev_t;

use crate::{
  expand_rewrite::{DecompressError, Result},
  huffman::tree::Node,
};

pub fn huffman_depths_to_tree(depths: &[u16]) -> Result<Node> {
  let mut nodes = vec![];

  validate_depths(depths)?;

  for depth in (1..=16).rev() {
    let min_nodes = nodes.len();
    let mut iter = depths
      .iter()
      .enumerate()
      .filter(|(_, d)| **d == depth)
      .map(|(val, _)| Node::Leaf(val, 0))
      .chain(nodes.into_iter());

    nodes = Vec::with_capacity(min_nodes);
    while let Some(left) = iter.next() {
      if let Some(right) = iter.next() {
        nodes.push(Node::branch(left, right));
      } else {
        return Err(DecompressError::InvalidBinaryTree);
      }
    }
  }

  match nodes.pop() {
    None => Err(DecompressError::InvalidBinaryTree),
    Some(node) => {
      assert_eq!(
        nodes.len(),
        0,
        "Binary tree from {:?} has 2 or more roots somehow: {:?}, {:?}",
        depths,
        node,
        nodes
      );
      Ok(node)
    }
  }
}

fn validate_depths(depths: &[u16]) -> Result<()> {
  let mut depth_counter = [0_usize; 16];
  for node_depth in depths {
    depth_counter[*node_depth as usize] += 1;
  }
  let mut previous_nodes = 0;
  for count in depth_counter[1..].into_iter().rev() {
    let this_depth_count = count + previous_nodes;
    if this_depth_count % 2 != 0 {
      return Err(DecompressError::InvalidBinaryTree);
    }
    previous_nodes = this_depth_count / 2;
  }
  if previous_nodes != 1 {
    Err(DecompressError::InvalidBinaryTree)
  } else {
    Ok(())
  }
}

#[cfg(test)]
mod test {

  use super::*;
  #[test]
  fn test_converts_simple_depths_to_tree() {
    let node = huffman_depths_to_tree(&[1, 1]).unwrap();
    assert_eq!(node, Node::branch(Node::Leaf(0, 0), Node::Leaf(1, 0)))
  }
  #[test]
  fn test_converts_more_complex_depths_to_tree() {
    let node = huffman_depths_to_tree(&[1, 2, 2]).unwrap();
    assert_eq!(
      node,
      Node::branch(
        Node::Leaf(0, 0),
        Node::branch(Node::Leaf(1, 0), Node::Leaf(2, 0),),
      )
    )
  }
  #[test]
  fn test_converts_2_deep_balanced_huffman_tree() {
    let node = huffman_depths_to_tree(&[2, 2, 2, 2]).unwrap();
    assert_eq!(
      node,
      Node::branch(
        Node::branch(Node::Leaf(0, 0), Node::Leaf(1, 0),),
        Node::branch(Node::Leaf(2, 0), Node::Leaf(3, 0),),
      )
    )
  }
  #[test]
  fn test_converts_complex_depths_to_tree() {
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
    let node = huffman_depths_to_tree(&[0, 6, 5, 0, 0, 7, 7, 0, 4, 3, 2, 1]).unwrap();
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
    )
  }

  #[test]
  fn test_invalid_tree_from_test_data() {
    match huffman_depths_to_tree(&[1, 3, 1, 5, 6, 4, 3, 2, 2, 0, 6, 3, 0, 0, 0]) {
      Err(DecompressError::InvalidBinaryTree) => {}
      Err(e) => panic!("Incorrect error returned: {:?}", e),
      Ok(_) => panic!("Incorrectly returned a tree"),
    }
  }
  #[test]
  fn test_invalid_tree() {
    match huffman_depths_to_tree(&[1, 1, 1, 1]) {
      Err(DecompressError::InvalidBinaryTree) => {}
      Err(e) => panic!("Incorrect error returned: {:?}", e),
      Ok(_) => panic!("Incorrectly returned a tree"),
    }
  }
}
