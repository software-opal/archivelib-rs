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

### When to stop

Our goal is to fill the buffer with a multiple of 8 calls as close to the buffer size (of `8192`) as possible. Given the maximum number of bytes written in each iteration is `25`(1 bit flag + 8 * 3 bytes for runs & offsets), the code choses a limit of `8162` (`8192 - 24 - 1 - 5`). If the buffer is over that size, then we dump the buffer out to the file using the algorithm described below.

If instead we reach the end of the file, we add the run length `0x1FE` and offset `0` to the buffer; and then we finalise the compression by writing the values in the buffer using the same mechanism as if the buffer had filled.

### Worked example

So given the input `I am what I am; ABABABAB`, we would write the following into `byte_run_length_buffer` (`_165`):

```
0b0000_0000, 0x49, 0x20, 0x61, 0x6D, 0x20, 0x77, 0x68, 0x61, 
0b0010_0001, 0x74, 0x20, 0x01, 0x09, 0x00, 0x3B, 0x20, 0x41, 0x42, 0x03, 0x01, 0x00, 
0b1000_0000, 0xFE, 0x00, 0x00
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

## Writing the LZSS butter to the file's bit buffer

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

### Encoding the Huffman tables

Our goal in this section is to write out enough information so that the other side can recreate the huffman encoding we just generated.

#### Encoding a 1-node byte/run length huffman table. 

If the byte/run length huffman table has only 1 node, implying a single value. Then rather than doing all the complicated bits below, we instead just make the following write calls:

- 5 bits of all zeros `0b0_0000`
- 5 bits of all zeros `0b0_0000`
- 9 bits of all zeros `0b0_0000_0000`
- 9 bits of the `root_node_value`(I.E. it's entire value)

So if the only value in the table is `0x1AB`, then we would have written the following 28 bits:

```
0000_0000 0000_0000 0001_1010 1011
└───┬┘└─┬──┘└┬────────┘└────────┬┘
 writes of `0x00`             0x1AB
```

At this point we skip down 

#### Bit length Huffman table

Now we build a second huffman table based on the data in the first table. Here we're trying to encode the bit lengths, and the distances between sequential values.

To do this, we use the bit length array from the prior steps:

From 0 to the largest value in the array (in our case `0x1FE`) we:
- If the current value has an encoding (I.E. the bit length is non-zero)
  - Increment the frequency of `2 + bit_length`(so the range `(2+1)..=(2+16)`, or `3..19`).
- Otherwise, count the number of blank entries between this index and the next index with a value.
  - If the distance is `<= 2`, increment the `0` frequency by the distance (so a distance of `2` increments twice).
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
 Tree                      | Frequency
┐                          - 26
├─0─┐                      - 10
│   ├─0─ 0x001             - 5
│   └─1─ 0x002             - 5
└─1─┐                      - 16
    ├─0─ 0x006             - 9
    └─1─┐                  - 7
        ├─0─ 0x000         - 3
        └─1─┐              - 4  
            ├─0─ 0x007     - 2
            └─1─┐          - 2
                ├─0─ 0x004 - 1
                └─1─ 0x005 - 1
```

Now we start writing out this huffman table's information.

##### Encoding a 1-node bit length huffman tree

If the generated bit length tree has only 1 node, then the data is not written using `_218`, it is
 instead written directly in `_207`. Specifically we write:

- 5 bits of zeros `0b00000`
- 5 bits of the root node's value

For example, if the only value was `0x005`, then we would have written the following 10 bits:

```
0000_0001 01
└──┬─┘└───┬┘
 0x00   0x005
```

##### Writing bit length information

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

This means we've now written the following 44 bits into to the file:

```
0000_0000 0001_0001 0100_0011 0100_1001 1011_0101 0100
└────────┬────────┘ └┬───┘└┬┘ └┬┘└┬─┘└┤ └┬┘└┬─┘└┬─┘└┬┘
  byte/run length    │     └┬──┴──┘   │  └──┴┬───┴───┘
  total frequency    │  bit lengths   │  bit lengths
                     │            gap size
              used length of bit
                 length table
```

Or in hexadecimal: `0x00 0x11 0x43 0x89 0xB5 0x4_`

#### Are we there yet? No. Time to write out the byte/run length encoding

In this section we're outputting enough information to let the decompressor rebuild the byte/run length huffman tree. 

##### Encoding process

We're going to loop through every value from `0` up to and including the largest value in our table.

- If the current value is present in the byte/run length huffman encoding:
  - Write the bits defined in the bit length huffman table by `bit_length + 2`.
- Otherwise (I.E. the current value is not in the huffman coding):
  - Count the number of values until the next value that is in the huffman encoding
  - Advance the value by that number, so the next loop will be of a value in the encoding
  - If the gap is `1`(the smallest possible)
    - Write the bits defined in the bit length huffman table for `0x000`.
  - If the gap is `2`
    - Write the bits defined in the bit length huffman table for `0x000` twice.
  - If the gap is `18` or less then:
    - Write the bits defined in the bit length huffman table for `0x001`.
    - Write `gap_size - 3` in 4 bits.
  - If the gap is `19` then we effectively write the `1` bit gap, then an `18` bit gap; or:
    - Write the bits defined in the bit length huffman table for `0x000`.
    - Write the bits defined in the bit length huffman table for `0x001`.
    - Write `15` (`gap_size - 3 - 1`) in 4 bits.
  - If the gap is more that 19:
    - Write the bits defined in the bit length huffman table for `0x002`.
    - Write `gap_size - 20` in 9 bits.

This effectively means the output structure follows this Regex `(value)*(gap?value+)+`; I.E. we start with 0 or more values, then repeating as many time as necessary: a single gap followed by 1 or more values.

##### Writing data from our example

As with writing tree above, the first 9 bits we write represent the length of the table(or max value + 1). In our case, the largest value is `0x1FE`, so we would write `0x1FF` or `0b1_1111_1111`.

Now we will write the bit lengths and gap lengths using the huffman encoding we generated above. As a refresher the encodings are: `0x000 => 0b110`, `0x001 => 0b00`, `0x002 => 0b01`, `0x004 => 0b11110`, `0x005 => 0b11111`, `0x006 => 0b10` and `0x007 => 0b1110`.

- We start with a gap of 32(`0x000` to `0x020`) which is greater than 19, so we write:
  - The huffman encoding for `0x002`: `0b01`
  - The gap size minus 20, so `32 - 20 = 12`: `0b0_0000_1100`
- Now we have a value at `0x020`, with a bit length of `2`, so we write:
  - The huffman encoding for the bit length plus 2(`0x004`): `0b11110`
- Now we have a gap of 26(`0x021` to `0x03B`), which is also greater than 19, so we write:
  - The huffman encoding for `0x002`: `0b01`
  - The gap size minus 20, so `26 - 20 = 6`: `0b0_0000_0110`
- Now we have a value at `0x03B`, with a bit length of `5`, so we write:
  - The huffman encoding for the bit length plus 2(`0x007`): `0b1110`
- Now we have a gap of 4(`0x03C` to `0x041`), which is between 3 and 18, so we write:
  - The huffman encoding for `0x001`: `0b00`
  - The gap size minus 3: `0b0010`
- Now we have a value at `0x041`, with a bit length of `4`, so we write:
  - The huffman encoding for the bit length plus 2(`0x006`): `0b10`
- Now we have a value at `0x042`, with a bit length of `4`, so we write:
  - The huffman encoding for the bit length plus 2(`0x006`): `0b10`
- [... next writes elided for brevity ...]
- Now we have a value at `0x101`, with a bit length of `4`, so we write:
  - The huffman encoding for the bit length plus 2(`0x006`): `0b10`
- Now we have a gap of 1(`0x102` to `0x103`), which is less than 3 so we write:
  - The huffman encoding for `0x000`: `0b110`
- Now we have a value at `0x103`, with a bit length of `4`, so we write:
  - The huffman encoding for the bit length plus 2(`0x006`): `0b10`
- A gap of `250`(`0x104` to `0x1FE`), which is greater than 19 so we write:
  - The huffman encoding for `0x002`: `0b01`
  - The gap size minus 20, so `250 - 20 = 230`: `0b0_1110_0110`
- And the final value at `0x1FE`, with a bit length of `5`, so we write:
  - The huffman encoding for the bit length plus 2(`0x007`): `0b1110`

This means we've now written the following bits into to the file:

```
1111_1111 1010_0000 1100_1111 0010_0000 0110_1110 0000_1010 1000_0011
└─────────┘└┘└─────────┘ └────┘└┘└─────────┘ └──┘ └┘└───┘└┘ └┘└┘ └──┘

1001_0000 0001_1111 1100_0011 1000_0001 1000_0011
└┘└┘ └─────────┘└────┘└┘└──┘└─┘└┘└───┘└─┘└┘└────┘

1011_0110 1001_0011 1010_1101 1010_0101 1100_1101 110
└┘└──┘└─┘ └┘└┘ └─────────┘└┘└──┘└┘ └┘└─────────┘└───┘
```

<!-- Sorry no annotations this time. -->

With this information we can now rebuild the byte/run length huffman encoding. Does this mean we can actually get to writing the data? No. Not yet, we've got one more huffman encoding to generate and output.


### What's the time? Offset run length huffman encoding time!

The final huffman table is for the offset length bit lengths. This encodes the number of bits that each run offset uses, allowing the writing of offsets to take less overall bits.

We've been counting these during our `LZSS` encoding, storing them in `_193`. We then generate a huffman tree using the same method as above.

In our example, our bit lengths are `[1, 1, 0, 0, 1, ...]`, which produces the following huffman tree and encoding:

```
┐
├─0─ 0x001
└─1─┐
    ├─0─ 0x000
    └─1─ 0x004
```

#### What if it's a 1-node huffman encoding?

As above, we will write out `0` using 5 bits, and then the root node's value using 5 bits.

For example, if our only node is the bit length `0x003`, then we would write the following bits: `0b00000 0b00011`.

#### Writing offset bit length tree data

This generates and gets output the same way as the byte/run bit length encoding, just without the 2 bit write after writing the bit information for`0x002`. So we will write the following bits:

- The length of values(or max value + 1), so `0x005`, using 5 bits: `0b00101`
- The depth of the `0x000` bit length using 3 bits: `0b010`
- [...]
- The depth of the `0x004` bit length using 3 bits: `0b010`

This means we've now written the following 19 bits into to the file:
```
0010_1010 0010_00000 010
└────┘└─┘ └─┘└──┘└─┘ └─┘
```

### Finally, finally writing out the data in the LZSS buffer

It is finally time for us to write out the LZSS buffer. 

#### The writing process

As you recall, the LZSS buffer is written in 8 write chunks, with the first byte being a bit-flag, and the next 8-24 bytes represent the bytes or the run lengths.

Here we'll loop using two different indexes, the loop index, and the array offset. The loop count is used to track when we need to reload the bit fields; and the array index is advanced every time we've dealt with a byte in the buffer.

So looping over the range `0..frequency` we go through each one:

- If the current loop count is a multiple of `8`, store the bit field and advance the array index.
- Otherwise, bit-shift the bit-field left by 1. This puts the current bit in the bit-flag in the highest bit.
- If the highest bit of the bit-field is `0`
  - Look up the value in the the byte/run length huffman coding
  - Write the huffman encoding.
  - Advance the array offset
- If the highest bit of the bit-field is `1`
  - Look up `0x100 + value` in the byte/run length huffman coding
  - Write the huffman encoding.
  - Advance the array offset
  - Reconstruct the run offset's value (`arr[idx] + (arr[idx] << 8)`).
  - Count the number of bits in the offset
  - Look up the bit count in the offset bit length huffman coding
  - Write the huffman coding
  - If the bit count is greater than 1
    - Write the offset value using `bit_count - 1` calculated(I.E. skipping the most significant bit, which is implied by the bit length).
  - Advance the array offset by 2

#### Writing data from our example

As a refresher, our byte/run length huffman tree is

```
┬─0─┬─0─ 0x020
│   └─1─┬─0- 0x061 
│       └─1─┬─0- 0x041
│           └─1─ 0x042
└─1─┬─0─┬─0─┬─0─ 0x049
    │   │   └─1─ 0x068 
    │   └─1─┬─0- 0x06D 
    │       └─1─ 0x074 
    └─1─┬─0─┬─0─ 0x077
        │   └─1─ 0x101 
        └─1─┬─0- 0x103 
            └─1─┬─0- 0x03B
                └─1─ 0x1FE
```

Our run offset bit length huffman tree is

```
┬─0─ 0x001
└─1─┬─0─ 0x000
    └─1─ 0x004
```


And this is the data we have in the LZSS buffer

```
0b0000_0000, 0x49, 0x20, 0x61, 0x6D, 0x20, 0x77, 0x68, 0x61, 
0b0010_0001, 0x74, 0x20, 0x01, 0x09, 0x00, 0x3B, 0x20, 0x41, 0x42, 0x03, 0x01, 0x00, 
0b1000_0000, 0xFE, 0x00, 0x00
```

- `0b0000_0000` -- All bytes, so we just look up the values and write their encodings
  - `0x049` -- `0b1000`
  - `0x020` -- `0b00`
  - `0x061` -- `0b010`
  - `0x06D` -- `0b1010`
  - `0x020` -- `0b00`
  - `0x077` -- `0b1100`
  - `0x068` -- `0b1001`
  - `0x061` -- `0b010`
- `0b0010_0001` -- Positions 3 & 8 are runs
  - `0x074` -- `0b1011`
  - `0x020` -- `0b00`
  - Run with bytes `0x01, 0x09, 0x00`
    - Run length `0x01`, so look up `0x101` -- `0b1101`
    - Reconstruct offset `0x0009`:
      - Look up the offset bit length (`4`) -- `0b11`
      - Write the offset using the bit length bits -- `0b1001`
  - `0x03B` -- `0b11110`
  - `0x020` -- `0b00`
  - `0x041` -- `0b0110`
  - `0x042` -- `0b0111`
  - Run with bytes `0x03, 0x01, 0x00`
    - Run length `0x03`, so look up `0x103` -- `0b1110`
    - Reconstruct offset `0x0001`:
      - Look up the offset bit length (`1`) -- `0b0`
      - Because the bit length is 1 or less, we write 0 bytes of the offset. The offset's bit length is implied from the length we've written
- `0b1000_0000` -- Position 1 is a run(technically the EOF flag)
  - Run length `0xFE, 0x00, 0x00`
    - Run length `0xFE`, so look up `0x1FE` -- `0b11111`
    - Reconstruct offset `0x0000`:
      - Look up the offset bit length (`0`) -- `0b10`
      - Because the bit length is 1 or less, we write 0 bytes of the offset. The offset's bit length is implied from the length we've written.

Which means we've now written 

```
1000_0001 0101_0001 1001_0010 1010_1100 1101_1100 1111_1000 0110_0111 1110_0111 1110
└──┘ └┘└──┘└───┘└┘└───┘└───┘└──┘└───┘└┘ └──┘ └┘└──┘└────┘└┘ └──┘ └──┘ └──┘ │└────┘└┘
```

### Tidying up 

Finally we need to clear the byte/run length and offset bit count frequency buffers, so the next iteration starts fresh.

## What next?

If there's more data in the input file, then the process starts again from the top, building up the LZSS buffer.

Otherwise, At this point, we've loaded and compressed all the the input file, and written all but the last 0-7 bits to the output file.

We finalise the compression by writing enough bits to complete the last byte and we write that out to the output file. And we're done.

So to summarise:
- Repeat until all data is read
  - Compress the input data using LZSS into a temporary buffer
  - Generate some huffman encodings to use when writing the LZSS buffer's data
  - Generate a huffman table to write the byte/run length huffman encoding
  - Write the 3 huffman tables out
  - Write the LZSS buffer to the file, encoding the bytes, run lengths, and offsets according to the huffman encoding.
