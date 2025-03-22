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
}

