This folder contains a series of fuzzing targets. From the root of the repo run:

    cargo fuzz run {fuzz_decompress,fuzz_compress,fuzz_decompress_correctness,fuzz_compress_correctness}

Note that the `_correctness` ones will compare the results with that from `archivelib_sys`. The plain ones will just use the input as in on the Rust version.

Currently `fuzz_decompress` panics on an empty input.

Additional fuzzing opts
-----------------------

Usage: `cargo-fuzz run [FLAGS] [OPTIONS] <TARGET> [CORPUS]... [-- <ARGS>...]`

- Flags:
  - `-O` -- Turn on release optimisations
- Options:
  - `-j <JOBS>` -- Run over more processes; especially useful if you have lots of cores and don't know what to do with them.
- Corpus:
  - `fuzz/known_inputs` -- These are generated from the files in `test_data`; and have caused crashes in the past.



Adding more test data
---------------------

New test data can be created by running `hus_vip_to_test_case.py` with a series of `.hus` and `.vip` files as arguments. This will create 2 sets of files:

- `src/test/match_sys/staging/*.rs` -- These are `match_sys` test cases that ensure the given compressed data is decompressed matching the `_sys` library. You should move them into `src/test/match_sys` and add them to the `src/test/match_sys/mod.rs`.
- `fuzz/known_inputs/*` -- These are the components, named by the SHA1 of the contents.
