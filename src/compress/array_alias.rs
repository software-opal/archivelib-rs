pub trait ArrayAlias<P> {
  type Item;

  fn get(&self, parent: &P, index: usize) -> Self::Item;
  fn set(&mut self, parent: &mut P, index: usize, item: Self::Item);
  fn shift(&mut self, parent: &P, delta: isize) -> usize;
  fn set_offset(&mut self, parent: &P, offset: usize) -> usize;
  fn offset(&mut self, parent: &P) -> usize;
  fn slice_copy(&self, parent: &P) -> Box<[Self::Item]>;

  fn apply<F>(&mut self, parent: &mut P, index: usize, apply: F)
  where
    F: FnOnce(&mut P, Self::Item) -> Self::Item,
  {
    let val = self.get(parent, index);
    let new_val = apply(parent, val);
    self.set(parent, index, new_val);
  }
}

#[macro_export]
macro_rules! array_alias_enum {
  (
    $(
      pub enum<$($generic_k:tt: $generic_v:tt),*> $name:ident {
        type Parent = $parent:ty;
        type Item = $item:ty;
        $(
          $key:ident => $target_arr:ident;
        )*
      }
    )+
  ) => (
    $(
      #[allow(dead_code)]
      #[derive(Debug, PartialEq)]
      pub enum $name<'a> {
        $(
          $key(usize),
        )*
        Custom(usize, &'a mut [$item]),
      }
      impl<'a, $($generic_k: $generic_v),*> ArrayAlias<$parent> for $name<'a> {
        type Item = $item;
        fn shift(&mut self, parent: &$parent, delta:isize) -> usize {
            let new_offset = match self {
            $(
              $name::$key(ref mut idx) => cast!((cast!((*idx) as isize) + delta) as usize),
            )*
            $name::Custom(ref mut idx, _) => cast!((cast!((*idx) as isize) + delta) as usize),
          }
          ; self.set_offset(parent, new_offset)
        }
        fn offset(&mut self, _parent: &$parent) -> usize {
          match self {
            $(
              $name::$key(ref mut idx) => {*idx},
            )*
            $name::Custom(ref mut idx, _) => {*idx},
          }
        }
        fn set_offset(&mut self, _parent: &$parent, offset: usize) -> usize {
          match self {
            $(
              $name::$key(ref mut idx) => {*idx = offset; *idx},
            )*
            $name::Custom(ref mut idx, _) => {*idx = offset; *idx},
          }
        }
        fn slice_copy(&self, parent: &$parent) -> Box<[Self::Item]> {
          match self {
            $(
              $name::$key(ref idx) => parent.$target_arr[*idx..].to_vec().into_boxed_slice(),
            )*
            $name::Custom(idx, arr) => arr[*idx..].to_vec().into_boxed_slice(),
          }
        }
        fn get(&self, parent: &$parent, index: usize) -> Self::Item {
          match self {
            $(
              $name::$key(ref idx) => parent.$target_arr[idx + index],
            )*
            $name::Custom(idx, arr) => arr[idx + index],
          }
        }
        fn set(&mut self, parent: &mut $parent, index: usize, item: Self::Item) {
          match self {
            $(
              $name::$key(ref idx) => parent.$target_arr[idx + index] = item,
            )*
            $name::Custom(ref idx, arr) => arr[idx + index] = item,
          }
        }
      }
    )+
  );
}
