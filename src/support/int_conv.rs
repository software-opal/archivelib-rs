#[macro_export]
macro_rules! cast_trunc {
  ($e:ident as $t:ty) => {
    $crate::cast_trunc!(($e) as $t)
  };
  (($e:expr) as $t:ty) => {{
    use std::convert::TryFrom;
    let a = $e;
    #[allow(unused_assignments)]
    let mut b = a;
    b = <$t>::max_value().into();
    match <$t>::try_from(a & b) {
      Ok(v) => v,
      Err(_) => {
        unreachable!(
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

#[macro_export]
macro_rules! cast {
  ($e:ident as $t:ty) => {
    $crate::cast!(($e) as $t)
  };
  (($e:expr) as $t:ty) => {{
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
