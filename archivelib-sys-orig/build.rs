extern crate bindgen;
extern crate cc;

use std::env;
use std::path::PathBuf;

fn main() {
  // The bindgen::Builder is the main entry point
  // to bindgen, and lets you build up options for
  // the resulting bindings.
  let bindings = bindgen::Builder::default()
    .clang_arg("-xc++")
    .header("c-lib/wrapper.h")
    .allowlist_function("compress")
    .allowlist_function("decompress")
    .allowlist_function("clean")
    .allowlist_function("reverse*")
    .generate()
    .expect("Unable to generate bindings");

  // Write the bindings to the $OUT_DIR/bindings.rs file.
  let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
  bindings
    .write_to_file(out_path.join("bindings.rs"))
    .expect("Couldn't write bindings!");

  let mut files = PathBuf::from("c-lib/src/")
    .read_dir()
    .unwrap()
    .map(|v| v.unwrap().path())
    .filter(|path| path.is_file())
    .filter(|path| {
      if let Some(ext) = path.extension() {
        if "cpp" == ext {
          return true;
        }
      }
      return false;
    })
    .collect::<Vec<_>>();
  files.sort();
  let mut builder = cc::Build::new();
  builder.cpp(true); // Switch to C++ library compilation.
  builder.warnings(false);
  // builder.flag_if_supported("-fsanitize=undefined");
  // builder.flag_if_supported("-fsanitize=address");
  // builder.flag_if_supported("-fsanitize=bounds");
  builder.flag_if_supported("-fstack-protector");
  // builder.flag_if_supported("-fsanitize-memory-track-origins");
  builder.flag_if_supported("-fsanitize=memory");
  if !cfg!(windows) {
    builder.define("AL_UNIX", None);
    builder.define("AL_SUN4", None);
  }

  builder
    .define("AL_CUSTOM", None)
    .define("AL_SYMANTEC", None) // This is needed to compile on OSX
    .include("c-lib/")
    .include("c-lib/include")
    .file("c-lib/api.cpp")
    .file("c-lib/enum_rev.cpp")
    .files(files)
    .compile("libarchivelib.a");
}
