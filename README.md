# archivelib-rs

This is a Rust port of the 'archivelib' compression algorithm written by Greenleaf Software in 1994. The code is based of version 1.0B, released August 10, 1994 and was ported from the code found in [this repo](https://github.com/softwarepublico/lightbase/tree/master/LBS/LIBS/AL101).

## Library goals

The goals of porting the code from C to Rust:

- Provide a pure Rust implementation.

  This will allow users to build this code wherever Rust can be built, from WASM to Embedded. And it will allow the code to be transpiled into other languages(like Python).
- Provide a 'safe' implementation.

  The implementation must be memory safe and crash free, especially if it is to be used in limited environments.
- Must perfectly replicate the original library, warts and all

  It is important the port acts the same as the original version for well defined code paths(the happy paths). For undefined behaviours(like reading past the end of an array) it is fine for the Rust library to return an error instead of trying to replicate the undefined behaviour.
- Non-algorithmic deviation is permitted

  The usage of the original library allows outputs up to 65,536 bytes; erroring for outputs beyond that. The Rust version, by default, also has this limit; but it is possible to output larger files by using a custom `ArchivelibConfig`.


## The story

The story begins when I decided that I wanted to port [EmbroiderModder](https://github.com/Embroidermodder/Embroidermodder) to Rust, or more specifically the reading and writing of various formats. In order to work with two common embriodery machine formats, `hus` and `vip`, I would need to work with a compression library called archivelib. A version exists in the EmbroiderModder git tree, and after several attempts to port the code I realised that I needed to find a version closer to the original.

### From C++ to Rust

The original source code was found in [this repo](https://github.com/softwarepublico/lightbase/tree/master/LBS/LIBS/AL101) and it has been copied into `archivelib-sys-orig/c-lib/`. The compression algorithm itself is obfuscated C++ code, and can be viewed in [`_rc.cpp`](archivelib-sys-orig/c-lib/src/_rc.cpp) and [`_re.cpp`](archivelib-sys-orig/c-lib/src/_re.cpp) along with a number of supporting files included as they were found(where ever possible). Minimal changes have been made to the original source code, apart from fixing one or two bugs that impacted my ability to effectively fuzz the code(such as [this change](https://github.com/software-opal/archivelib-rs/commit/4fb94991c9205683feeef40ee6966c5853b1a070) to prevent double-free).

From there, most of the unnecessary code was pruned, the obfuscated C++ was formatted and split out and the long process of understanding; cleaning and tidying the code began:
- Variables were renamed from `_266` to something more useful like `run_start226`.
- Refactored from using a C++ class to a more functional style, passing around a struct.
- Trying to remove as much C/C++ 'magic'(like pointer math).
- Generate test cases for each function using some known-good data.

This allowed the code to be converted into Rust and continue to be refactored and improved.

### Let the games begin

Once a mostly working port was achieved, it became important to test that the port was correct. I started off by simply trying to compress, and then decompress a given input and assert that it produced the same output. This uncovered some bugs; but I was sure there were more. So I ended up down a rabbit hole trying to fuzz the original C++ version of the library(which has *many* memory safety problems) and making sure that my Rust version performed the same; just without panicking.

In order to do this effectively I ended up having to build a separate C++ CLI for the original library to allow me to use all the memory-protection options(like `no-stack-arrays` to perform bounds checking on stack array accesses).

### The library today

The Rust library is relatively well tested and doesn't(read: shouldn't) panic, even where the original would crash. The API needs more documentation and a larger think about how the library is to be used.

### Further work

I saw somewhere, that this may just be zlib with a different block size. I would like to investigate this claim, just to put my mind at ease.
