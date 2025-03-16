pub fn print_tree(root_node: u32, node_start_at: i32, left_nodes: &[u16], right_nodes: &[u16]) {
  eprintln!("{:#05X}", root_node);
  do_print_tree(
    "",
    root_node as u16,
    node_start_at as u16,
    left_nodes,
    right_nodes,
  );
}

fn do_print_tree(
  prefix: &str,
  root_node: u16,
  node_start_at: u16,
  left_nodes: &[u16],
  right_nodes: &[u16],
) {
  eprintln!("┐");

  let left = left_nodes[root_node as usize];

  eprint!("{}├─0─", prefix);
  if left >= node_start_at {
    let sub_node_prefix = format!("{}│   ", prefix);
    do_print_tree(
      &sub_node_prefix,
      left,
      node_start_at,
      left_nodes,
      right_nodes,
    );
  } else {
    eprintln!(" {:#05X}", left);
  }
  let right = right_nodes[root_node as usize];
  eprint!("{}└─1─", prefix);
  if right >= node_start_at {
    let sub_node_prefix = format!("{}    ", prefix);
    do_print_tree(
      &sub_node_prefix,
      right,
      node_start_at,
      left_nodes,
      right_nodes,
    );
  } else {
    eprintln!(" {:#05X}", right);
  }
}

fn build_tree_from_encoding(
  node_start_at: i32,
  bit_depth: &[u8],
  bit_values: &[u16],
) -> (usize, Vec<u16>, Vec<u16>) {
  let mut last_node: usize = node_start_at as usize;
  let root_node: usize = node_start_at as usize;
  let mut left_nodes = vec![0_u16; root_node * 2];
  let mut right_nodes = vec![0_u16; root_node * 2];
  for i in 0..(node_start_at as usize) {
    let depth = bit_depth[i];
    if depth > 0 {
      let value = bit_values[i];
      let mut node = root_node;
      for d in (1..depth).rev() {
        node = if value & (1 << d) == 0 {
          if left_nodes[node] == 0 {
            (last_node += 1);
            left_nodes[node] = last_node as u16;
          }
          left_nodes[node] as usize
        } else {
          if right_nodes[node] == 0 {
            (last_node += 1);
            right_nodes[node] = last_node as u16;
          }
          right_nodes[node] as usize
        };
      }
      if value & 1 == 0 {
        left_nodes[node] = i as u16;
      } else {
        right_nodes[node] = i as u16;
      }
    }
  }
  (root_node, left_nodes, right_nodes)
}

pub fn print_tree_from_encoding(node_start_at: i32, bit_depth: &[u8], bit_values: &[u16]) {
  let (root_node, left_nodes, right_nodes) =
    build_tree_from_encoding(node_start_at, bit_depth, bit_values);
  print_tree(root_node as u32, node_start_at, &left_nodes, &right_nodes);
}

#[cfg(test)]
mod tests {
  use super::*;

  #[test]
  fn test_build_tree_from_encoding_1_deep() {
    let bit_depth = [0, 1, 1];
    let bit_values = [0, 0b1, 0b0];
    let node_start_at = 3;

    let (root_node, left_nodes, right_nodes) =
      build_tree_from_encoding(node_start_at, &bit_depth, &bit_values);

    assert_eq!(left_nodes[root_node], 2);
    assert_eq!(right_nodes[root_node], 1);
  }

  #[test]
  fn test_build_tree_from_encoding_2_deep() {
    let bit_depth = [0, 2, 2, 2, 2];
    let bit_values = [0, 0b11, 0b10, 0b01, 0b00];
    let node_start_at = 5;

    let (root_node, left_nodes, right_nodes) =
      build_tree_from_encoding(node_start_at, &bit_depth, &bit_values);

    // 0b00
    assert_eq!(left_nodes[left_nodes[root_node] as usize], 4);
    // 0b01
    assert_eq!(right_nodes[left_nodes[root_node] as usize], 3);
    // 0b10
    assert_eq!(left_nodes[right_nodes[root_node] as usize], 2);
    // 0b11
    assert_eq!(right_nodes[right_nodes[root_node] as usize], 1);
  }
  #[test]
  fn test_build_tree_from_encoding_lopsided_3_deep() {
    let bit_depth = [0, 1, 2, 3, 3];
    let bit_values = [0, 0b0, 0b10, 0b110, 0b111];
    assert_eq!(bit_depth.len(), bit_values.len());
    let node_start_at = bit_depth.len();

    let (root_node, left_nodes, right_nodes) =
      build_tree_from_encoding(node_start_at as i32, &bit_depth, &bit_values);

    // 0b00
    assert_eq!(left_nodes[root_node], 1);
    // 0b01
    assert_eq!(left_nodes[right_nodes[root_node] as usize], 2);
    // 0b10
    assert_eq!(
      left_nodes[right_nodes[right_nodes[root_node] as usize] as usize],
      3
    );
    // 0b11
    assert_eq!(
      right_nodes[right_nodes[right_nodes[root_node] as usize] as usize],
      4
    );
  }
}
