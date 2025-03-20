mod archivelib;
pub use self::archivelib::ArchiveLibSortAlgorithm;

mod modern;
pub use self::modern::ModernSortAlgorithm;

use super::tree::Node;

pub trait SortAlgorithm {
  type List;
  fn initial_sort(&self, nodes: Vec<Node>) -> Self::List;
  fn pop_smallest_node(&self, nodes: &mut Self::List) -> Option<Node>;
  fn insert_node(&self, nodes: &mut Self::List, new_node: Node);
}
