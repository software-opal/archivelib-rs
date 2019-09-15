extern crate bindgen;
extern crate cc;

use std::env;
use std::path::PathBuf;

fn find_sources(path: PathBuf) -> Vec<PathBuf> {
  let items: Vec<PathBuf> = path
    .read_dir()
    .unwrap()
    .map(|v| v.unwrap().path())
    .collect();
  let mut sources: Vec<PathBuf> = items
    .clone()
    .into_iter()
    .filter(|path| path.is_file())
    .filter(|path| {
      if let Some(ext) = path.extension() {
        "cpp" == ext || "c" == ext
      } else {
        false
      }
    })
    .collect();
  sources.extend(
    items
      .iter()
      .filter(|path| path.is_dir())
      .flat_map(|folder| find_sources(folder.to_path_buf())),
  );
  sources
}

fn main() {
  // The bindgen::Builder is the main entry point
  // to bindgen, and lets you build up options for
  // the resulting bindings.
  let bindings = bindgen::Builder::default()
    .clang_arg("-xc++")
    .clang_arg("-fsanitize=undefined")
    .clang_arg("-fsanitize=address")
    .clang_arg("-fno-omit-frame-pointer")
    .header("c-lib/wrapper.h")
    .whitelist_function("compress")
    .whitelist_function("decompress")
    .whitelist_function("clean2")
    .whitelist_function("reverse*")
    .generate()
    .expect("Unable to generate bindings");

  // Write the bindings to the $OUT_DIR/bindings.rs file.
  let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
  bindings
    .write_to_file(out_path.join("bindings.rs"))
    .expect("Couldn't write bindings!");

  let mut files = find_sources(PathBuf::from("c-lib/src/"));
  files.sort();
  let mut base = cc::Build::new();
  base.warnings(false);
  // base.flag_if_supported("-fsanitize=undefined");
  // base.flag_if_supported("-fsanitize=address");
  // base.flag_if_supported("-fsanitize=bounds");
  base.flag_if_supported("-fstack-protector");
  // base.flag_if_supported("-fsanitize-memory-track-origins");
  base.flag_if_supported("-fsanitize=memory");
  if !cfg!(windows) {
    base.define("AL_UNIX", None);
    base.define("AL_SUN4", None);
  }
  base
    .define("AL_CUSTOM", None)
    .define("AL_SYMANTEC", None) // This is needed to compile on OSX
    .include("c-lib/")
    .include("c-lib/include")
    .files(files)
    .cpp(true)
    .file("c-lib/api.cpp")
    .file("c-lib/enum_rev.cpp")
    .compile("archivelib2");
}
