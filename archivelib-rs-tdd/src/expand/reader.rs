use crate::support::{BitwiseRead, Result};

trait BitwiseReadAheadRead {
  fn read_ahead(&mut self, bits: usize) -> Result<u128>;
  fn consume(&mut self, bits: usize) -> Result<u128>;
}

struct BitwiseReadAheadReader<R: BitwiseRead> {
  inner: R,
  bit_buffer: u128,
  bit_buffer_len: usize,
  bit_buffer_pos: usize,
}

impl<R: BitwiseRead> BitwiseReadAheadReader<R> {
    pub fn new(reader: R) -> Self {
        BitwiseReadAheadReader {
            inner: reader,
            bit_buffer: 0,
            bit_buffer_len: 0,
            bit_buffer_pos: 0,
        }
    }
}

impl<R: BitwiseRead> BitwiseReadAheadRead for BitwiseReadAheadReader<R> {
    fn read_ahead(&mut self, bits: usize) -> Result<u128> {
        assert!(self.bit_buffer_pos + bits <= 128);
        let bitmask = if bits == 128 {
            u128::max_value()
        } else {
            (1u128 << bits) - 1
        };

        if bits == 0 {
            return 0;
        } else if self.bit_buffer_pos + bits <= self.bit_buffer_len {
            // Simple case. All bits are in the buffer already
            let out = self.bit_buffer.wrapping_shl(self.bit_buffer_pos) & bitmask;
        }
    }
    fn consume(&mut self, bits: usize) -> Result<u128> {}
}
