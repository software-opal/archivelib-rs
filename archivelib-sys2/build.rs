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
    .header("c-lib/wrapper.h")
    .whitelist_function("compress")
    .whitelist_function("decompress")
    .whitelist_function("clean")
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
  base
    .warnings(true)
    .define("AL_CUSTOM", None)
    .define("AL_SUN4", None)
    .define("AL_UNIX", None)
    .define("AL_DISABLE_NEW", None)
    .define("NDEBUG", None)
    .include("c-lib/")
    .include("c-lib/include");
  let mut c_build = base.clone();
  let mut cpp_build = base.clone();

  for file in files {
    match file.extension().unwrap().to_str().unwrap() {
      "c" => c_build.file(file),
      "cpp" => cpp_build.file(file),
      _ => unreachable!(),
    };
  }
  c_build.compile("archivelib_c");
  cpp_build
    .cpp(true)
    .file("c-lib/api.cpp")
    .file("c-lib/enum_rev.cpp")
    .compile("archivelib");
}
