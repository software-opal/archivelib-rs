#[macro_export]
macro_rules! cast {
  ($e:ident as $t:ty) => {
    $crate::cast!(($e) as $t)
  };
  (($e:expr_2021) as $t:ty) => {{
    use std::convert::TryFrom;
    let a = $e;
    match <$t>::try_from(a) {
      Ok(v) => v,
      Err(_) => {
        panic!(
          "Conversion of {}(=={}) to {} failed at {}:{}:{}",
          stringify!($e),
          a,
          stringify!($t),
          file!(),
          line!(),
          column!()
        );
      }
    }
  }};
}
