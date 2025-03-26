#[derive(PartialEq, Eq, Debug, PartialOrd, Ord)]
pub enum Node {
  Leaf(usize, u16),
  Branch(Box<Node>, Box<Node>, u16),
}

impl Node {
  pub fn branch(left: Node, right: Node) -> Node {
    let freq = left.frequency() + right.frequency();
    Self::Branch(Box::new(left), Box::new(right), freq)
  }

  pub fn value(&self) -> Option<usize> {
    match self {
      Self::Leaf(value, _) => Some(*value),
      Self::Branch(_, _, _) => None,
    }
  }

  pub fn frequency(&self) -> u16 {
    match self {
      Node::Leaf(_, freq) => *freq,
      Node::Branch(_, _, freq) => *freq,
    }
  }

  pub fn parse_value(&self, iter: &mut impl Iterator<Item = bool>) -> Option<usize> {
    match self {
      Node::Leaf(value, _) => Some(*value),
      Node::Branch(left, right, _) => iter.next().and_then(|v| match v {
        false => left.parse_value(iter),
        true => right.parse_value(iter),
      }),
    }
  }
}

#[cfg(test)]
mod test {
  use super::*;

  #[test]
  fn test_parse_value() {
    let node = Node::branch(
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
    );

    assert_eq!(node.parse_value(&mut [false].into_iter()), Some(0x00B));
    assert_eq!(
      node.parse_value(&mut [true, true, false].into_iter()),
      Some(0x009)
    );
    assert_eq!(node.parse_value(&mut [true].into_iter()), None);
    assert_eq!(
      node.parse_value(&mut [true, false].into_iter()),
      Some(0x00A)
    );
  }
}
