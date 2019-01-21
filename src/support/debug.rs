fn vec_to_nice_debug<T: Debug>(v: Vec<T>) -> String {
  let mut base = "[";
  if let Some(t) = v.first() {
    let mut last = t;
    let mut count = 0;
    for val in v {
      if val == last {
        count += 1;
      } else {
        if base.len() > 1 {
          base += ", ";
        }
        base += format!("{:?} => {}", last, count);
        last = val;
        count = 1;
      }
    }
    if base.len() > 1 {
      base += ", ";
    }
    base += format!("{:?} => {}", last, count);
  }
  base + "]"
}
