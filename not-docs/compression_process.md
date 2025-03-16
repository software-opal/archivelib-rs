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
       to point to the byte prior. And note it is written in little endian order (for bonus confusion).
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

## Writing to the bit buffer

This is performed in `_207`, which takes the data loaded above and writes it out to the buffer.

### Building a byte length frequency huffman encoding

Using the byte frequency data generated above, we can now produce a huffman tree from which to derive our huffman encoding. Note that the implementation is a little weird with how it assigns the nodes to their encodings, but more on that later.

The huffman tree is generated in `build_huffman_encoding` (`_211`) and the resulting encoding is stored in `_192` and `_180`.

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

Now we assign each value a depth based on it's position in the `values_in_tree` map, for this table, this is stored in `_180`:

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

And then using these depths we are able to build a huffman encoding for this data. For this encoding, this is stored in `_192` :

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

### Writing our first bits.

The first two bytes will be the frequency of the root node, or the total frequency of all the leaves in `byte_run_length_frequency`.

In our example, this is `17` or `0x0011`; which is written in big-endian order, so `0x00 0x11` are written to the file.

Note: If the above table only has 1 node, then 

### Huffman table 2, electric boogaloo

Now we build a second huffman table based on the data in the first table. Here we're trying to encode the bit lengths, and the distances between sequential values.

To do this, we use the bit length array from the prior steps:

From 0 to the largest value in the array (in our case `0x1FE`) we:
- If the current value has an encoding (I.E. the bit length is non-zero)
  - Increment the frequency of `2 + bit_length`(so the range `(2+1)..=(2+16)`, or `3..19`).
- Otherwise, count the number of blank entries between this index and the next index with a value.
  - If the distance is `<= 2`, increment the `0` frequency
  - If the distance is `<=18`, increment the `1` frequency
  - If the distance is `19` increment both `0` and `1` frequencies
  - Otherwise increment the `2` frequency.

So given the bit length input `[0, 2, 0, 0, 0, 0, 2, 1, 0, 0]`, we would iterate over the range `0..8` (I.E. stopping at the last index with a bit length). And our iterations would look like:

- `i = 0`:
  - `0` bit length, therefore count the number of values until a value with a bit length. This gives us `1`
  - Increment the `[0]` frequency
- `i = 1`:
  - `2` bit length, therefore increment the `[4]` frequency.
- `i = 2`:
  - `0` bit length, therefore count until the next value. This gives us `4`.
  - Increment the `[1]` frequency
- `i = 6`:
  - `2` bit length, therefore increment the `[4]` frequency.
- `i = 7`:
  - `1` bit length, therefore increment the `[3]` frequency.

If we now apply this to the huffman table generated above we get:
- `i = 0x000` -- `32` (`0x020`) gap, so increment `[2]`
- `i = 0x020` -- Bit length of `2`, so increment `[4]`
- `i = 0x021` -- `26` (`0x01A`) gap, so increment `[2]`
- `i = 0x03B` -- Bit length of `5`, so increment `[7]`
- etc.

At the end we end up with the following data: `[03, 05, 05, 00, 01, 01, 09, 02, ...]`. Which we can then turn into a huffman encoding stored in `_181` and `_194` using the method described above:

```
┐
├─0─┐
│   ├─0─ 0x001
│   └─1─ 0x002
└─1─┐
    ├─0─ 0x006
    └─1─┐
        ├─0─ 0x000
        └─1─┐
            ├─0─ 0x007
            └─1─┐
                ├─0─ 0x004
                └─1─ 0x005
```

Now we start writing out this huffman table's information.

#### Writing bit length information

> *Note on 1-node huffman tree*
>
> If the generated bit length tree has only 1 node, then the data is not written using `_218`, it is
>  instead written directly in `_207`. Specifically we write:
>  - 5 bits of zeros `0b00000`
>  - 5 bits of the root node's value
>
> For example, if the only value was `0x005`, then we would only write 10 bits `0b00000 0b00101`,
>  skipping over this entire section.

We now call `_218` with the constants `19, 5, 3`:
- `19` is the length of the huffman table we generated (so `16 + 3`).
- `5` is the bit length required to print the length (so 5 bits of `0b1_0011`).
- `3` is a magic number represents the value in the encoding where the meaning shifts from gaps to bit lengths. Presumably this saves some space in the encoded result.

The first thing we write is the largest value in the bit length table, plus one. In our case the largest value is `0x007`, so we would write `0x008`. And we write this out using `5` bits, so `0b01000`. 

Now we're going to write out the lengths of the huffman encoding in order starting with value `0x000`, going until `0x007`(a range of `0..8`).

We'll do this using 3 bits for each entry:

- `0x000` -- represented by 3 bits, so `0b011`
- `0x001` -- represented by 3 bits, so `0b010`
- `0x002` -- represented by 3 bits, so `0b010`

Now we've written out the "gaps" part of the values, we now write out 2 bits representing the gap until the next value(up to 3), skipping over those entries. Because our next value is `0x004`, we skip over one and write out `0b01`. If our next value was more than 3 away, say `0x008`, then we would write out `0b11` and continue writing at at `0x006` (so we'd write `0b000` and `0b000`; representing the depths for `0x006` and `0x007` respectively).

Then we continue until we've reached the last value

- `0x004` -- represented by 5 bits, so `0b101`
- `0x005` -- represented by 5 bits, so `0b101`
- `0x006` -- represented by 5 bits, so `0b010`
- `0x007` -- represented by 5 bits, so `0b100`

This means we've now written the following bits to the file:

```
0000_0000 0001_0001 0100_0011 0100_1001 1011_0101 0100
└────────┬────────┘ └┬───┘└┬┘ └┬┘└┬─┘└┤ └┬┘└┬─┘└┬─┘└┬┘
  byte/run length    │     └┬──┴──┘   │  └──┴┬───┴───┘
  total frequency    │  bit lengths   │  bit lengths
                     │            gap size
              used length of bit
                 length table
```











