macro_rules! adversarial {
  ($(
    $name:ident ($level: expr, $base: expr, $repeat: expr);
  )+) => (
    $(
      #[test]
      fn $name() {
        use crate::expand_new::expand;
        use crate::do_compress_level;
        use crate::CompressionLevel;
        use crate::support::CorrectLookAheadBitwiseReader;
        use std::iter;

        let level = CompressionLevel::from_compression_level($level).unwrap();
        let base_iter = $base.into_iter();
        let base = {
          let mut b: Vec<u8> = Vec::with_capacity(base_iter.size_hint().0);
          for i in base_iter {
            b.push(i.clone());
          }
          b
        };
        let repeats = $repeat;

        let input_data = iter::repeat(base).take(repeats).flatten().collect::<Vec<_>>();
        let compressed_data = do_compress_level(&input_data, level).unwrap();

        let mut reader = CorrectLookAheadBitwiseReader::from_reader(&compressed_data[..]);
        let mut writer = Vec::with_capacity(input_data.len());
        expand(&mut reader, &mut writer, level).unwrap();
        assert_eq!(writer, input_data);
      }
    )+
  );
}

adversarial! {
  simple(0, [0x00_u8, 0x00, 0x01, 0x01], 1);
  simple_long(0, [0x00_u8, 0x00, 0x01, 0x01], 1024);
  simple_really_long(0, (0..128), 1024);
  simple_really_really_long(0, (0..=255), 1024);
  repetitive(0, vec![1; 50], 1024);
}
