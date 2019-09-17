#![feature(test)]
#![feature(concat_idents)]

extern crate test;
use archivelib::{do_compress_level, do_decompress_level, CompressionLevel};
use test::Bencher;

macro_rules! compress {
  ($name: ident, $data: expr) => {
    mod $name {
      use super::*;
      compress! { level_0, $data, CompressionLevel::Level0}
      compress! { level_1, $data, CompressionLevel::Level1}
      compress! { level_2, $data, CompressionLevel::Level2}
      compress! { level_3, $data, CompressionLevel::Level3}
      compress! { level_4, $data, CompressionLevel::Level4}
    }
  };
  ($name: ident, $data: expr, $level: expr) => {
    #[bench]
    fn $name(b: &mut Bencher) {
      let data: Vec<u8> = {
        let src = $data;
        src.to_vec()
      };
      b.bytes = data.len() as u64;
      b.iter(|| do_compress_level(&data, $level));
    }
  };
}

compress! {compress_large, [0xa5; 16384]}
compress! {compress_small, [0xa5; 2048]}
compress! {compress_random, {
  use rand::RngCore;
  let mut buffer = [0; 16384];
  rand::thread_rng().fill_bytes(&mut buffer);
  buffer
}}

// #[bench]
// fn bench_compress_large_zeros(b: &mut Bencher) {
//   let data = [0xa5; 16384];
//   b.iter(|| add_two(2));
// }
