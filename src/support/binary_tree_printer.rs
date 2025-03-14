
pub fn print_tree(root_node: u32, node_start_at: i16,  left_nodes: &[u16], right_nodes: &[u16]) {
  eprintln!("{:#05X}", root_node);
  do_print_tree("", root_node as u16, node_start_at as u16, left_nodes, right_nodes);
}

fn do_print_tree(prefix: &str, root_node: u16, node_start_at: u16,  left_nodes: &[u16], right_nodes: &[u16]) {
  let sub_node_prefix = format!("{} │", prefix);
  
  let left = left_nodes[root_node as usize];
  eprintln!("{} ┝ {:#05X}", prefix, left);
  if left >= node_start_at {
    do_print_tree(&sub_node_prefix, left, node_start_at, left_nodes, right_nodes);
  }
  let right = right_nodes[root_node as usize];
  eprintln!("{} ┕ {:#05X}", prefix, right);
  if right >= node_start_at {
    do_print_tree(&sub_node_prefix, right, node_start_at, left_nodes, right_nodes);
  }
}
