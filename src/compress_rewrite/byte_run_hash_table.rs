const BYTE_RUN_HASH_BITS: usize = 12;
const BYTE_RUN_HASH_SIZE: usize = 1 << BYTE_RUN_HASH_BITS;
const BYTE_RUN_HASH_BITMASK: usize = BYTE_RUN_HASH_SIZE - 1;

pub const MAX_RUN_COPY_CHECK_ATTEMPTS: usize = 128;

pub struct ByteRunHashTable {
  buffer_size: usize,
  hash_table: Vec<Option<usize>>,
  inverse_table: Vec<Option<usize>>,

  current_hash: usize,
}

impl ByteRunHashTable {
  pub fn new(buffer_size: usize) -> Self {
    // Buffer size's bits must not toggle any bits in the lower 8 bits of the byte hash, otherwise
    //  it'll throw off the hash.
    assert_eq!(buffer_size & 0xFF, 0);
    Self {
      buffer_size,
      hash_table: vec![None; buffer_size + BYTE_RUN_HASH_SIZE],
      inverse_table: vec![None; buffer_size + BYTE_RUN_HASH_SIZE],
      current_hash: 0,
    }
  }

  pub fn next_byte_hash(&self, existing_hash: usize, next_byte: u8) -> usize {
    return self.buffer_size + ((existing_hash << 4) ^ (usize::from(next_byte)))
      & BYTE_RUN_HASH_BITMASK;
  }

  pub fn insert_byte_hash(&mut self, position: usize) {
    let byte_hash = self.current_hash;
    if let Some(previous_position) = self.hash_table[byte_hash] {
      // We have a value for this hash, and we're about to update what position is in there, so
      //  update the inverse table so we can still find the previous position's entry in the hash
      //  table when we go to clear it.
      self.inverse_table[previous_position] = Some(position);
      self.hash_table[position] = Some(previous_position)
    } else {
      assert_eq!(
        self.hash_table[position], None,
        "Possibly invalid hash table."
      );
    }
    self.inverse_table[position] = Some(byte_hash);
    self.hash_table[byte_hash] = Some(position);
  }

  pub fn clear_entry_at_position(&mut self, position: usize) {
    if let Some(hash_table_idx) = self.inverse_table[position] {
      self.hash_table[hash_table_idx] = None;
      self.inverse_table[position] = None;
    }
  }

  pub fn possible_run_positions(&self) -> impl Iterator<Item = usize> {
    let mut next_position = self.hash_table[self.current_hash];
    std::iter::from_fn(move || {
      if let Some(position) = next_position {
        next_position = self.hash_table[position];
        Some(position)
      } else {
        None
      }
    })
    .take(MAX_RUN_COPY_CHECK_ATTEMPTS)
  }

  pub(crate) fn record_byte(&mut self, byte: u8) {
    self.current_hash = self.next_byte_hash(self.current_hash, byte);
  }
}

#[cfg(test)]
mod test {

  use super::*;

  #[test]
  fn test_hash_table() {
    let mut table = ByteRunHashTable::new(1 << 10);

    // 'abc'
    table.record_byte(97);
    table.record_byte(98);
    table.record_byte(99);

    assert_eq!(table.current_hash, 2883);

    table.insert_byte_hash(0);
    assert_eq!(table.hash_table[2883], Some(0));
    assert_eq!(table.inverse_table[0], Some(2883));

    table.clear_entry_at_position(0);
    assert_eq!(table.hash_table[2883], None);
    assert_eq!(table.inverse_table[0], None);
  }

  #[test]
  fn test_multiple_inserts() {
    let mut table = ByteRunHashTable::new(1 << 10);

    // 'abc'
    table.record_byte(97);
    table.record_byte(98);
    table.record_byte(99);

    assert_eq!(table.current_hash, 2883);

    table.insert_byte_hash(0);
    assert_eq!(table.hash_table[2883], Some(0));
    assert_eq!(table.inverse_table[0], Some(2883));

    table.insert_byte_hash(1);
    assert_eq!(table.hash_table[2883], Some(1));
    assert_eq!(table.hash_table[1], Some(0));
    assert_eq!(table.inverse_table[1], Some(2883));
    assert_eq!(table.inverse_table[0], Some(1));

    table.insert_byte_hash(2);
    assert_eq!(table.hash_table[2883], Some(2));
    assert_eq!(table.hash_table[2], Some(1));
    assert_eq!(table.hash_table[1], Some(0));
    assert_eq!(table.inverse_table[2], Some(2883));
    assert_eq!(table.inverse_table[1], Some(2));
    assert_eq!(table.inverse_table[0], Some(1));
  }

  #[test]
  fn test_removing() {
    let mut table = ByteRunHashTable::new(1 << 10);

    // 'abc'
    table.record_byte(97);
    table.record_byte(98);
    table.record_byte(99);

    table.insert_byte_hash(0);
    table.insert_byte_hash(1);
    table.insert_byte_hash(2);

    assert_eq!(table.inverse_table[0], Some(1));
    assert_eq!(table.hash_table[1], Some(0));
    table.clear_entry_at_position(0);
    assert_eq!(table.inverse_table[0], None);
    assert_eq!(table.hash_table[1], None);

    assert_eq!(table.inverse_table[1], Some(2));
    assert_eq!(table.hash_table[2], Some(1));
    table.clear_entry_at_position(1);
    assert_eq!(table.inverse_table[1], None);
    assert_eq!(table.hash_table[2], None);

    assert_eq!(table.inverse_table[2], Some(2883));
    assert_eq!(table.hash_table[2883], Some(2));
    table.clear_entry_at_position(2);
    assert_eq!(table.inverse_table[2], None);
    assert_eq!(table.hash_table[2883], None);
  }

  fn test_possible_run_positions() {
    let mut table = ByteRunHashTable::new(1 << 10);

    // 'abc'
    table.record_byte(97);
    table.record_byte(98);
    table.record_byte(99);

    table.insert_byte_hash(0);
    table.insert_byte_hash(1);
    table.insert_byte_hash(2);

    assert_eq!(
      table.possible_run_positions().collect::<Vec<_>>(),
      [2, 1, 0]
    );
  }
}
