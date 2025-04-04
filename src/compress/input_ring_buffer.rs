use std::io::Read;

use crate::{
  compress::Result,
  consts::{MAX_RUN_LENGTH, MIN_RUN_LENGTH},
};

use super::byte_run_hash_table::ByteRunHashTable;

fn read_all(reader: &mut impl Read, target: &mut [u8]) -> Result<usize> {
  let mut idx = 0;
  loop {
    match reader.read(&mut target[idx..]) {
      Ok(0) => return Ok(idx),
      Ok(n) => idx += n,
      Err(ref e) if e.kind() == std::io::ErrorKind::Interrupted => {}
      Err(e) => return Err(e.into()),
    }
  }
}

fn read_one(reader: &mut impl Read) -> Result<Option<u8>> {
  let mut dat = [0];
  match read_all(reader, &mut dat)? {
    0 => Ok(None),
    1 => Ok(Some(dat[0])),
    _ => unreachable!(),
  }
}

#[derive(Debug, PartialEq, Eq)]
enum BufferState {
  NotLoaded,
  InitialLoad,
  RestOfData,
}

pub struct InputRingBuffer<R: Read> {
  reader: R,
  nominal_size: usize,
  buffer: Vec<u8>,
  pub remaining_data: usize,
  buffer_end_position: usize,
  buffer_position: usize,

  data_to_advance: usize,
  buffer_state: BufferState,

  byte_run_hash: ByteRunHashTable,
}

impl<R: Read> InputRingBuffer<R> {
  pub fn new(reader: R, buffer_size: usize) -> Self {
    Self {
      reader,
      nominal_size: buffer_size,
      buffer: vec![0; buffer_size + MAX_RUN_LENGTH + 2],
      remaining_data: 0,
      buffer_end_position: 0,
      buffer_position: 0,

      data_to_advance: 0,
      buffer_state: BufferState::NotLoaded,

      byte_run_hash: ByteRunHashTable::new(buffer_size),
    }
  }

  pub(crate) fn ensure_buffer_filled(&mut self) -> Result<()> {
    match self.buffer_state {
      BufferState::NotLoaded => {
        let size = read_all(&mut self.reader, &mut self.buffer[0..self.nominal_size])?;

        self.remaining_data = size;
        self.buffer_end_position = size % self.nominal_size;

        self.byte_run_hash.record_byte(self.buffer[0]);
        self.byte_run_hash.record_byte(self.buffer[1]);
        self.byte_run_hash.record_byte(self.buffer[2]);

        self.buffer_state = BufferState::InitialLoad;
        return Ok(());
      }
      BufferState::InitialLoad => {
        while self.remaining_data > MAX_RUN_LENGTH + 4 {
          if self.data_to_advance == 0 {
            return Ok(());
          }

          self.byte_run_hash.insert_byte_hash(self.buffer_position);
          self.buffer_position = (self.buffer_position + 1) % self.nominal_size;
          self
            .byte_run_hash
            .record_byte(self.buffer[self.buffer_position + 2]);

          self.data_to_advance -= 1;
          self.remaining_data -= 1;
        }

        while self.remaining_data < 256 {
          let byte = match read_one(&mut self.reader)? {
            None => break,
            Some(n) => n,
          };
          self.buffer[self.buffer_end_position] = byte;
          if (self.buffer_end_position) < 256 - 1 {
            self.buffer[self.buffer_end_position + self.nominal_size] = byte;
          }

          self
            .byte_run_hash
            .clear_entry_at_position(self.buffer_end_position);
          self.buffer_end_position = (self.buffer_end_position + 1) % self.nominal_size;

          self.remaining_data += 1
        }

        self.buffer_state = BufferState::RestOfData;
      }
      _ => {}
    }

    // This loop advances the ring buffer by 1, replacing the front element with a byte from the
    //  file.
    while self.data_to_advance > 0 {
      let byte = match read_one(&mut self.reader)? {
        None => break,
        Some(n) => n,
      };
      self.buffer[self.buffer_end_position] = byte;
      if (self.buffer_end_position) < 256 - 1 {
        self.buffer[self.buffer_end_position + self.nominal_size] = byte;
      }
      self.buffer_end_position = (self.buffer_end_position + 1) % self.nominal_size;

      self
        .byte_run_hash
        .clear_entry_at_position(self.buffer_position);
      self.byte_run_hash.insert_byte_hash(self.buffer_position);
      self.buffer_position = (self.buffer_position + 1) % self.nominal_size;
      self
        .byte_run_hash
        .record_byte(self.buffer[self.buffer_position + 2]);
      self.data_to_advance -= 1;
    }

    // This loop advances our position in the buffer, and is only used when we approach the end of
    //  the buffer.
    while self.data_to_advance > 0 {
      self.byte_run_hash.insert_byte_hash(self.buffer_position);
      self.buffer_position = (self.buffer_position + 1) % self.nominal_size;
      self
        .byte_run_hash
        .record_byte(self.buffer[self.buffer_position + 2]);
      self.remaining_data -= 1;
      self.data_to_advance -= 1;
    }

    Ok(())
  }

  pub fn find_longest_run(&self) -> Option<(usize, usize)> {
    assert_eq!(
      self.data_to_advance, 0,
      "Cannot operate on buffer whilst data is yet to be loaded"
    );
    let start_position = self.buffer_position;
    let mut longest_run = None;

    for test_position in self.byte_run_hash.possible_run_positions() {
      let mut run_length = 0;
      while run_length < MAX_RUN_LENGTH {
        // QUIRK: This code can read beyond `remaining_data`. It is important that it does this so
        // it selects the same run position as the original code, ensuring bit-wise equivalence.
        if self.buffer[start_position + run_length] != self.buffer[test_position + run_length] {
          break;
        }
        run_length += 1;
      }
      if run_length >= MIN_RUN_LENGTH && run_length > longest_run.unwrap_or((0, 0)).0 {
        let offset = if self.buffer_position < (test_position + 1) {
          self.nominal_size + start_position - 1 - test_position
        } else {
          start_position - test_position - 1
        };

        if offset >= self.nominal_size {
          break;
        } else {
          longest_run = Some((run_length, offset));
        }
      }
    }

    longest_run
      .map(|(run_len, off)| (run_len.min(self.remaining_data), off))
      .filter(|(run_len, _)| *run_len >= MIN_RUN_LENGTH)
  }

  pub fn advance_by(&mut self, count: usize) {
    self.data_to_advance += count;
  }

  pub fn next_byte(&self) -> Option<u8> {
    assert_eq!(
      self.data_to_advance, 0,
      "Cannot operate on buffer whilst data is yet to be loaded"
    );
    if self.remaining_data > 0 {
      Some(self.buffer[self.buffer_position])
    } else {
      None
    }
  }
}

#[cfg(test)]
mod test {
  use super::InputRingBuffer;

  #[test]
  fn test_find_longest_run_reads_off_end_of_buffer_when_finding_runs() {
    let mut buffer = InputRingBuffer::new(&[0, 0, 0, 0, 0x39, 0, 0, 0][..], 0x1000);
    buffer.ensure_buffer_filled().unwrap();

    buffer.advance_by(1);
    buffer.ensure_buffer_filled().unwrap();
    let longest = buffer.find_longest_run();
    assert_eq!(longest, Some((3, 0)));

    buffer.advance_by(4);
    buffer.ensure_buffer_filled().unwrap();

    // For the love of pete, this reads off the "end" of the array :sob:
    let longest = buffer.find_longest_run();
    assert_eq!(longest, Some((3, 4)));
  }

  #[test]
  fn test_find_longest_run_without_nul_bytes() {
    let mut buffer = InputRingBuffer::new(
      &[0x01, 0x01, 0x01, 0x01, 0x39, 0x01, 0x01, 0x01][..],
      0x1000,
    );
    buffer.ensure_buffer_filled().unwrap();

    buffer.advance_by(1);
    buffer.ensure_buffer_filled().unwrap();
    let longest = buffer.find_longest_run();
    assert_eq!(longest, Some((3, 0)));

    buffer.advance_by(4);
    buffer.ensure_buffer_filled().unwrap();

    let longest = buffer.find_longest_run();
    assert_eq!(longest, Some((3, 3)));
  }

  #[test]
  fn test_prevents_too_short_runs_from_returning_as_a_real_run() {
    let mut buffer = InputRingBuffer::new(&[0xFF, 0x00, 0x00, 0x00][..], 0x1000);
    buffer.ensure_buffer_filled().unwrap();

    buffer.advance_by(1);
    buffer.ensure_buffer_filled().unwrap();
    let longest = buffer.find_longest_run();
    assert_eq!(longest, None);

    buffer.advance_by(1);
    buffer.ensure_buffer_filled().unwrap();
    let longest = buffer.find_longest_run();
    assert_eq!(longest, None);
  }
}
