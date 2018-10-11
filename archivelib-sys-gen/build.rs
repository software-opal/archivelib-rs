extern crate bindgen;
extern crate cc;

use std::env;
use std::path::PathBuf;

fn main() {
  cc::Build::new()
    .cpp(true) // Switch to C++ library compilation.
    .warnings(false)
    .define("AL_SUN4", None)
    .define("AL_UNIX", None)
    .include("c-lib/include")
    .include("c-lib/include/_custom")
    .files(
      PathBuf::from("c-lib/src/")
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
        }),
    )
    .compile("libarchivelib.a");

  // The bindgen::Builder is the main entry point
  // to bindgen, and lets you build up options for
  // the resulting bindings.
  let bindings = bindgen::Builder::default()
    .header("c-lib/include/all.hpp")
    .header("c-lib/include/compat.hpp")
    .whitelist_type("ALGreenleafEngine")
    .whitelist_type("ALGreenleafCompressionLevels")
    .whitelist_type("ALMemory")
    .whitelist_type("ALStorage")
    .whitelist_type("RCompress")
    .whitelist_type("RExpand")
    .whitelist_function("newALGreenleafEngine")
    .constified_enum("ALGreenleafCompressionLevels")
    .generate()
    .expect("Unable to generate bindings");

  // Write the bindings to the $OUT_DIR/bindings.rs file.
  let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
  bindings
    .write_to_file(out_path.join("bindings.rs"))
    .expect("Couldn't write bindings!");
}
