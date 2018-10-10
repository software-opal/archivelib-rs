extern crate cpp_to_rust_generator;

use cpp_to_rust_generator::common::cpp_build_config::CppBuildConfigData;
use cpp_to_rust_generator::common::cpp_lib_builder::{BuildType, CppLibBuilder};
use cpp_to_rust_generator::common::errors::fancy_unwrap;
use cpp_to_rust_generator::common::file_utils::create_dir;
use cpp_to_rust_generator::common::file_utils::{create_dir_all, PathBufWithAdded};
use cpp_to_rust_generator::common::target;
use cpp_to_rust_generator::common::utils::{add_env_path_item, run_command};
use cpp_to_rust_generator::config::{CacheUsage, Config, CrateProperties};
use std::path::PathBuf;
use std::process::Command;

fn cpp_lib_source() -> PathBuf {
  let mut path = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
  path.push("c-lib");
  assert!(path.exists());
  path
}

fn build_cpp_lib() -> PathBuf {
  let cpp_lib_source_dir = cpp_lib_source();
  let path = PathBuf::from(env!("CARGO_MANIFEST_DIR")).with_added("test_full_run");
  create_dir_all(&path).unwrap();
  let build_dir = path.with_added("build");
  let install_dir = path.with_added("install");
  if !build_dir.exists() {
    create_dir(&build_dir).unwrap();
  }
  if !install_dir.exists() {
    create_dir(&install_dir).unwrap();
  }
  fancy_unwrap(
    CppLibBuilder {
      cmake_source_dir: cpp_lib_source_dir,
      build_dir: build_dir,
      build_type: BuildType::Release,
      install_dir: install_dir,
      num_jobs: None,
      cmake_vars: Vec::new(),
    }
    .run(),
  );
  path
}

fn main() {
  let path = build_cpp_lib();
  let crate_dir = path.with_added("crate");
  let cpp_install_lib_dir = path.with_added("install").with_added("lib");
  assert!(cpp_install_lib_dir.exists());
  let crate_properties = CrateProperties::new("rust_libarchive", "0.0.0");

  let mut config = Config::new(&crate_dir, path.with_added("cache"), crate_properties);
  config.add_include_directive("all.hpp");
  config.set_write_dependencies_local_paths(false);
  let include_path = cpp_lib_source().with_added("include");
  // let crate_template_path = {
  //   let mut path = PathBuf::from(env!("CARGO_MANIFEST_DIR"));
  //   path.push("test_assets");
  //   path.push("archivelib");
  //   path.push("crate");
  //   path
  // };
  assert!(include_path.exists());
  config.add_include_path(&include_path);
  config.add_target_include_path(&include_path);

  {
    let mut data = CppBuildConfigData::new();
    data.add_linked_lib("archivelib");
    config
      .cpp_build_config_mut()
      .add(target::Condition::True, data);
  }
  {
    let mut data = CppBuildConfigData::new();
    data.add_compiler_flag("-fPIC");
    data.add_compiler_flag("-std=gnu++11");
    data.add_compiler_flag("-fpermissive");
    data.add_compiler_flag("-Wall");
    config
      .cpp_build_config_mut()
      .add(target::Condition::Env(target::Env::Msvc).negate(), data);
  }
  if target::current_env() == target::Env::Msvc {
    config.add_cpp_parser_argument("-std=c++14");
  } else {
    config.add_cpp_parser_argument("-std=gnu++11");
  }
  config.add_cpp_parser_argument("-fpermissive");
  config.add_cpp_parser_argument("-Wall");
  // config.set_crate_template_path(&crate_template_path);
  config.set_cache_usage(CacheUsage::None);
  fancy_unwrap(config.exec());
  assert!(crate_dir.exists());

  for cargo_cmd in &["update", "build", "test", "doc"] {
    let mut command = Command::new("cargo");
    command.arg(cargo_cmd);
    command.arg("-v");
    if *cargo_cmd != "update" {
      // command.arg("-j1");
    }
    if *cargo_cmd == "test" {
      command.arg("--");
      command.arg("--nocapture");
    }
    command.current_dir(&crate_dir);
    command.env("CPP_TO_RUST_INCLUDE_PATHS", &include_path);
    command.env("CPP_TO_RUST_LIB_PATHS", &cpp_install_lib_dir);
    command.env(
      "PATH",
      add_env_path_item("PATH", vec![cpp_install_lib_dir.clone()]).unwrap(),
    );
    command.env(
      "LD_LIBRARY_PATH",
      add_env_path_item("LD_LIBRARY_PATH", vec![cpp_install_lib_dir.clone()]).unwrap(),
    );
    run_command(&mut command).unwrap();
  }
}
