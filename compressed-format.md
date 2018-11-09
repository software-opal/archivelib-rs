

Using `small_heart_hus::stitch_attrs`

+------------+------------------------+
| bit offset | bits                   |
+------------+------------------------+
|    0x00    | `0000 0000  0000 1101` |
|    0x10    | `0011 1000  0110 1001` |
|    0x20    | `0110 1001  0111 1111` |
|    0x30    | `1100 0011  0110 0101` |
|    0x40    | `1111 0101  1010 0001` |
|    0x50    | `0110 1110  0000 0110` |
|    0x60    | `1001 0010  0001 1101` |
|    0x70    | `0101 0000  0010 1000` |
|    0x80    | `1100 1011  0010 0000` |
|    0x90    | `0000 0000  0000 0010` |
|    0xA0    | `0000 0001  0110 0111` |
|    0xB0    | `0000 0000  0111 0001` |
|    0xC0    | `1111 0101  0101 0001` |
|    0xD0    | `1011 1100  1001 1000` |
|    0xE0    | `0101 1110  ~~~~ ~~~~` |
+------------+------------------------+

| bit range   | value                              | fn              | use                                           |
| `0x00-0x10` | `0000 0000  0000 1101` => `0x000D` | `get_next_item` | Number of values to read after the header.    |
| `0x10-0x15` | `00111` => `0x07`                  | `fn253`         | `bits_to_load219` -- Number of points to load |
| `0x15-0x18` | `000` => `0x0`                     | `fn253`         | `dat_arr181[0]`                               |
| `0x18-0x1B` | `011` => `0x3`                     | `fn253`         | `dat_arr181[1]`                               |
| `0x1B-0x1E` | `010` => `0x2`                     | `fn253`         | `dat_arr181[2]`                               |
| `0x1E-0x20` | `01` => `0x1`                      | `fn253`         | `dat_arr181[3..4] = 0`                        |
| `0x20-0x23` | `011` => `0x3`                     | `fn253`         | `dat_arr181[4]`                               |
| `0x23-0x26` | `010` => `0x2`                     | `fn253`         | `dat_arr181[5]`                               |
| `0x26-0x29` | `010` => `0x2`                     | `fn253`         | `dat_arr181[6]`                               |


| `0x83-0x88` | `01011` => `0x011`                 | `fn253`         | `bits_to_load219` -- Number of points to load |



`fn253(length254, arg220, arg221)`
-------
Calls: `get_next_item` -> `fn253(19, 5, 3);`
Calls: `get_next_item` -> `fn253(15, 5, -1);`

- set `bits_to_load219 = get_bits(arg220)`
- clear `dat_arr181`
- if `bits_to_load219 == 0`
  - clear `dat_arr241`
- else (`bits_to_load219 > 0`)
  - do `bits_to_load219` times
    - set `byte_or_run_length203 = get_bits(3)`
    - if `byte_or_run_length203 == 7`
      - do while `get_bits(1)`
        - increment `byte_or_run_length203`
    - set `data->dat_arr181[idx] = byte_or_run_length203`
    - increment `idx`
    - if `idx == arg221` (i.e. `idx == 3`)
      - set `idx = idx + get_bits(2)`
  - call `fn258(length254, data->dat_arr181, 8, data->dat_arr241, CONST_N149_IS_256)`
