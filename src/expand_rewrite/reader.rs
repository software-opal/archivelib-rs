use std::io::Write;

use super::Result;

trait BitwiseRead {

}



struct Extractor<R: BitwiseRead, W: Write> {
  reader: R,
  writer: W,
  history_bytes: Vec<u8>
}

impl<R: BitwiseRead, W: Write>  Extractor<R, W> {
  pub fn new(reader: R, writer: W) -> Self {
    Self {
      reader, writer, history_bytes: vec![]
    }
  }

  pub fn extract(&mut self) -> Result<()> {
    while self.extract_chunk()? {    }

    Ok(())
  }

  pub fn extract_chunk(&mut self) -> Result<bool> {
    // let lzss_entries = self.reader.read_bits(16);

    // let byte_tree = self.load_byte_tree()?;
    // let offset_tree = self.load_offset_tree()?;


    // for _ in ..lzss_entries {
    //   let byte = self.load_lzss_entry(byte_tree, offset_tree);
    // //  if let LzssEntry::
    // }

    
    return Ok(false);
  }
}

