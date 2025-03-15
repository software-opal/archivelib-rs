# How the dickens does `archive_lib` compress stuff?

The compression looks at a sliding window of bytes, looking for runs of the next bytes in the prior data. This is then stored into a temporary buffer using [`LZSS`](https://en.wikipedia.org/wiki/Lempel%E2%80%93Ziv%E2%80%93Storer%E2%80%93Szymanski) compression. Then we build a number of huffman tables and use them to actually perform the writing.

## `LZSS` compression

Implemented in `compress` (`_184`) and `write_byte_or_run_into_buffer` (`_202`). With data being stored in `byte_run_length_buffer` (`_165`), and frequency data stored in `byte_run_length_frequency` (`_191`) and `run_offset_bit_count_frequency` (`_193`).

We would loop over each byte:
- Does this byte start a run of bytes in the data we've just seen?
  - If so, write a 'run' indicating the length and (negative) offset of the start of the run. Note
     that `position - offset + length` could be greater than `position` if the data repeats.
    We also write a `1` in the bit field indicating that this position represents a run.
  - Otherwise write the byte itself.

For example for the input `ABABAB`; the algorithm would write the bytes `A` and `B`; then a run of length 4 at offset (-)2; and we would set the 6th bit in the bit-field to indicate that the 3rd item is a run(as opposed to a byte).

### Worked example

So given the input `I am what I am; ABABABAB`, we would write the following into `byte_run_length_buffer` (`_165`):

```
0b0000_0000, 0x49, 0x20, 0x61, 0x6D, 0x20, 0x77, 0x68, 0x61, 
0b0010_0001, 0x74, 0x20, 0x01, 0x09, 0x00, 0x3B, 0x20, 0x41, 0x42, 0x03, 0x01, 0x00, 
0b1000_0000, 0xFE,
```

Looking at each line, we can see the first entry is written in binary, this is a bit-flag, indicating if the given value is a byte(`0`) or a run (`1`). So the first line contains 0 runs, the 2nd contains 2, and the third contains 1.

- `0b0000_0000, 0x49, 0x20, 0x61, 0x6D, 0x20, 0x77, 0x68, 0x61`
  
  Here we've written the first 8 bytes as-is, so `I am wha`.
- `0b0010_0001, 0x74, 0x20, 0x01, 0x09, 0x00, 0x3B, 0x20, 0x41, 0x42, 0x03, 0x01, 0x00`
  
  Here we've written:
  - 2 bytes `t `
  - A run with data `0x01, 0x09, 0x00`, which copies the bytes `I am`. The byte meaning is:
    - A run of length `4` (`3 + 0x01`). Runs must be 3 bytes long so a 3 byte run is written with
       `0x00` to give a few more values to work with.
    - An start offset of `-10` (`1 + 0x09 + 0x00 << 16`). The offset starts at `-1` allowing `0x00`
       to point to the byte prior.
  - 4 bytes `; AB`
  - A run with data `0x03, 0x01, 0x00`, which copies `ABABAB`. The byte meaning is:
    - A run length of `6` (`3 + 0x03`).
    - An offset of `-2` (`1 + 0x01 + 0x00 << 16`). Note that this offset is shorter than the run
       length, meaning we're relying on some of the earlier copied bytes.

- `0b1000_0000, 0xFE`
  
  Here we've just written the EOF flag, usually referred as `0x1FE`.

During the compression we are also keeping track of byte/run frequencies and offset bit count
 frequencies. These values are used in subsequent steps to build huffman trees, and then a huffman
 encoding.

The frequency tables contain the following:

- `byte_run_length_frequency` (`_191`), which is used to build a huffman.
  - `[0x020] => 4` -- 4 space bytes.
  - `[0x03B] => 1` -- 1 semicolon byte.
  - `[0x041] => 1` -- 1 `A` byte
  - `[0x042] => 1` -- 1 `B` byte
  - `[0x049] => 1` -- 1 `I` byte, the 2nd `I` is part of a run.
  - `[0x061] => 2` -- 2 `a` bytes
  - `[0x068] => 1` -- 1 `h` byte
  - `[0x06D] => 1` -- 1 `m` byte
  - `[0x074] => 1` -- 1 `t` byte
  - `[0x077] => 1` -- 1 `w` byte
  - `[0x101] => 1` -- A run length of `4` for the `I am` repeat.
  - `[0x103] => 1` -- A run length of `6` for the `ABABAB` repeat. 
  - `[0x1FE] => 1` -- EOF flag.
- `run_offset_bit_count_frequency` (`_193`), which is used to build h
  - `[0] => 1` -- For the EOF flag (`0x1FE`), which has an offset of `0`.
  - `[1] => 1` -- For the `ABABAB` repeat, which has an offset of `0x0001` (indicating start 2 bytes
                   back).
  - `[4] => 1` -- For the `I am` repeat, which has an offset of `0x0009` (indicating start 10 bytes
                   back).

During this compression pass other data is written, specifically:
 - The byte and run length frequencies into the `byte_run_length_frequency` (`_191`) frequency table
    for use in building a huffman table in the 2nd compression step.
 - The offset bit lengths into `run_offset_bit_count_frequency` (`_193`), so a offset of `0x0004`
    would increment the `3` bit length; and `0x00FF` would increment the `8` bit length.

## Building a byte length frequency huffman encoding

Using the byte frequency data generated above, we can now produce a huffman tree from which to derive our huffman encoding. Note that the implementation doesn't actually use the huffman tree to determine the encoding, but more on that later.

The huffman tree is generated in `build_huffman_encoding` (`_211`).

Of note, any value in the tree greater than or equal to `0x1FF` (I.E. `EOF flag + 1`) represents a branch in the tree, and anything less than that value represents a leaf node.

```
Tree              | Value/Node | Frequency
┐                       0x20A  - 17
├─0-┐                   0x208  - 8
│   ├─0-┐               0x205  - 4
│   │   ├─0-          0x061    - 2
│   │   └─1-┐           0x1FF  - 2
│   │       ├─0-      0x03B    - 1
│   │       └─1-      0x1FE    - 1
│   └─1-              0x020    - 4
└─1-┐                   0x209  - 9
    ├─0-┐               0x206  - 4
    │   ├─0-┐           0x201  - 2
    │   │   ├─0-      0x074    - 1
    │   │   └─1-      0x101    - 1
    │   └─1-┐           0x202  - 2
    │       ├─0-      0x049    - 1
    │       └─1-      0x077    - 1
    └─1-┐               0x207  - 5
        ├─0-┐           0x203  - 2
        │   ├─0-      0x041    - 1
        │   └─1-      0x103    - 1
        └─1-┐           0x204  - 3
            ├─0-      0x068    - 1
            └─1-┐       0x200  - 2
                ├─0-  0x042    - 1
                └─1-  0x06D    - 1
```

As part of this process, the leaf nodes are also inserted into `values_in_tree` in the order they were inserted.

Now we assign each value a depth based on it's position in the `values_in_tree` map:

- `[0x020] => 2` -- 4 space bytes.
- `[0x03B] => 5` -- 1 semicolon byte.
- `[0x041] => 4` -- 1 `A` byte
- `[0x042] => 4` -- 1 `B` byte
- `[0x049] => 4` -- 1 `I` byte, the 2nd `I` is part of a run.
- `[0x061] => 3` -- 2 `a` bytes
- `[0x068] => 4` -- 1 `h` byte
- `[0x06D] => 4` -- 1 `m` byte
- `[0x074] => 4` -- 1 `t` byte
- `[0x077] => 4` -- 1 `w` byte
- `[0x101] => 4` -- A run length of `4` for the `I am` repeat.
- `[0x103] => 4` -- A run length of `6` for the `ABABAB` repeat. 
- `[0x1FE] => 5` -- EOF flag.

And then using these depths we are able to build a huffman encoding for this data:

- `[0x020] => 0b00   ` -- 4 space bytes.
- `[0x03B] => 0b11110` -- 1 semicolon byte.
- `[0x041] => 0b0110 ` -- 1 `A` byte
- `[0x042] => 0b0111 ` -- 1 `B` byte
- `[0x049] => 0b1000 ` -- 1 `I` byte, the 2nd `I` is part of a run.
- `[0x061] => 0b010  ` -- 2 `a` bytes
- `[0x068] => 0b1001 ` -- 1 `h` byte
- `[0x06D] => 0b1010 ` -- 1 `m` byte
- `[0x074] => 0b1011 ` -- 1 `t` byte
- `[0x077] => 0b1100 ` -- 1 `w` byte
- `[0x101] => 0b1101 ` -- A run length of `4` for the `I am` repeat.
- `[0x103] => 0b1110 ` -- A run length of `6` for the `ABABAB` repeat. 
- `[0x1FE] => 0b11111` -- EOF flag.



Or a tree that looks like this:
```
┐
├─0─┐
│   ├─0─ 0x020
│   └─1─┐
│       ├─0- 0x061 
│       └─1─┐
│           ├─0- 0x041
│           └─1─ 0x042
└─1─┐
    ├─0─┐
    │   ├─0─┐
    │   │   ├─0─ 0x049
    │   │   └─1─ 0x068 
    │   └─1─┐
    │       ├─0- 0x06D 
    │       └─1─ 0x074 
    └─1─┐
        ├─0─┐
        │   ├─0─ 0x077
        │   └─1─ 0x101 
        └─1─┐
            ├─0- 0x103 
            └─1─┐
                ├─0- 0x03B
                └─1─ 0x1FE
```
Note how the tree has the same size as before, but the values are moved to place the higher frequency values at the start and lower frequencies at the bottom.

In the generated huffman tree, `0x035` has an encoding of `0b0010`, but in this sorted tree, it has an encoding of `0b11110`. This doesn't change the effectiveness of the tree, as `0x042` (the value at `0b11110` in the originally generated tree) has the same frequency (`1`).

## Actually writing information to the bit buffer

The first two bytes will be the frequency of the root node, or the total frequency of all the leaves in `byte_run_length_frequency`.

In our example, this is `17` or `0x0011`; which is written in big-endian order, so `0x00 0x11` are written to the file.



The next bits that are written are:

Writing 5 bits from 0b01000
Writing 3 bits from 0b011
Writing 3 bits from 0b010
Writing 3 bits from 0b010
Writing 2 bits from 0b01
Writing 3 bits from 0b101
Writing 3 bits from 0b101
Writing 3 bits from 0b010
Writing 3 bits from 0b100

Writing by 0x43
Writing by 0x49
Writing by 0xB5










