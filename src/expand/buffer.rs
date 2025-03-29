use std::{collections::VecDeque, io::Write};

use super::{DecompressError, Result};

pub struct ExpandHistoryBuffer<W: Write> {
  history_bytes: VecDeque<u8>,
  buffer_size: usize,
  writer: W,
}

impl<W: Write> ExpandHistoryBuffer<W> {
  pub fn new(writer: W, buffer_size: usize) -> Self {
    Self {
      writer,
      buffer_size,
      history_bytes: VecDeque::with_capacity(buffer_size),
    }
  }

  pub fn write_byte(&mut self, byte: u8) -> Result<()> {
    self.write_bytes(&[byte])
  }

  pub fn write_run(&mut self, run_len: usize, run_offset: usize) -> Result<()> {
    if run_offset > self.history_bytes.len() {
      return Err(DecompressError::InvalidRunOffset(run_offset));
    }
    match run_offset {
      0 => {
        // Optimise a 0 run offset, meaning copy on the last character a specific number of times.
        if let Some(fill) = self.history_bytes.back() {
          self.write_bytes(&vec![*fill; run_len])
        } else {
          Err(DecompressError::InvalidRunOffset(run_offset))
        }
      }
      _ => {
        let run_bytes = self.history(run_len, run_offset)?;
        if run_bytes.len() == run_len {
          self.write_bytes(&run_bytes)
        } else {
          let mut bytes = run_len;
          let max = run_bytes.len();

          while bytes > 0 {
            let written = bytes.min(max);
            self.write_bytes(&run_bytes[..written])?;
            bytes -= written;
          }
          Ok(())
        }
      }
    }
  }

  fn write_bytes(&mut self, bytes: &[u8]) -> Result<()> {
    self.writer.write_all(bytes)?;

    let remaining_spaces = self.buffer_size - self.history_bytes.len();
    if remaining_spaces >= bytes.len() {
      self.history_bytes.extend(bytes);
    } else if bytes.len() < self.buffer_size {
      self.history_bytes.drain(..(bytes.len() - remaining_spaces));
      self.history_bytes.extend(bytes);
    } else if bytes.len() >= self.buffer_size {
      self.history_bytes.clear();
      self
        .history_bytes
        .extend(bytes[(bytes.len() - self.buffer_size)..].into_iter());
    }
    Ok(())
  }

  fn history(&self, length: usize, offset: usize) -> Result<Vec<u8>> {
    if offset >= self.history_bytes.len() {
      Err(DecompressError::InvalidRunOffset(offset))
    } else if length == 0 {
      Ok(vec![])
    } else {
      let buffer_len = self.history_bytes.len();
      let start = buffer_len - offset - 1;
      if length == 1 {
        Ok(vec![self.history_bytes[start]])
      } else {
        Ok(
          self
            .history_bytes
            .iter()
            .skip(start)
            .take(length)
            .copied()
            .collect(),
        )
      }
    }
  }
}

#[cfg(test)]
mod test {
  use core::panic;

  use super::*;

  #[test]
  fn test_write_more_than_buffer_size_bytes() {
    let mut output = vec![];
    let mut buffer = ExpandHistoryBuffer::new(&mut output, 32);
    buffer.write_bytes(&[0xAA; 31]).unwrap();
    buffer.write_bytes(&[0xBB]).unwrap();
    buffer.write_bytes(&[0xCC; 2]).unwrap();
    assert_eq!(buffer.history_bytes.len(), 32);
    assert_eq!(output[30..], [0xAA, 0xBB, 0xCC, 0xCC]);
  }

  #[test]
  fn test_write_run_offset_out_of_bound() {
    let mut output = vec![];
    let mut buffer = ExpandHistoryBuffer::new(&mut output, 8);
    buffer.write_bytes(&[0xAA, 0xBB, 0xCC]).unwrap();
    match buffer.write_run(3, 3) {
      Err(DecompressError::InvalidRunOffset(3)) => {}
      Err(e) => panic!("Incorrect error: {:?}", e),
      Ok(_) => panic!("Incorrectly passed"),
    }
  }

  #[test]
  fn test_write_run_data() {
    let mut output = vec![];
    let mut buffer = ExpandHistoryBuffer::new(&mut output, 8);
    buffer.write_bytes(&[0xAA, 0xBB, 0xCC]).unwrap();
    buffer.write_run(3, 2).unwrap();
    buffer.write_run(60, 5).unwrap();

    assert_eq!(buffer.history_bytes.len(), 8);
    assert_eq!(
      output,
      [0xAA, 0xBB, 0xCC]
        .into_iter()
        .cycle()
        .take(66)
        .collect::<Vec<_>>()
    );
  }

  #[test]
  fn test_some_runs() {
    let mut output = vec![];
    let mut buffer = ExpandHistoryBuffer::new(&mut output, 8);
    buffer.write_bytes(&[00, 11, 22, 33]).unwrap();
    buffer.write_bytes(&[44, 55, 66, 77]).unwrap();
    buffer.write_run(3, 3).unwrap();
    buffer.write_run(3, 3).unwrap();

    assert_eq!(
      output,
      [
        00, 11, 22, 33, // Write 1
        44, 55, 66, 77, // Write 2
        44, 55, 66, // Run 1
        77, 44, 55 // Run 2
      ]
    );
  }
  #[test]
  fn test_runs_lon() {
    let mut output = vec![];
    let mut buffer = ExpandHistoryBuffer::new(&mut output, 8);
    buffer.write_bytes(&[44, 55, 66, 77]).unwrap();
    buffer.write_run(9, 2).unwrap();
    buffer.write_run(3, 3).unwrap();

    assert_eq!(
      output,
      [
        44, 55, 66, 77, // Write 1
        55, 66, 77, 55, 66, 77, 55, 66, 77, // Run 1
        77, 55, 66, // Run 2
      ]
    )
  }
}
