#![feature(libc)]
extern crate libc;
extern "C" {
  #[no_mangle]
  fn ALStorage_ReadBuffer(
    storage: *mut libc::c_void,
    buffer: *mut uint8_t,
    length: size_t,
  ) -> size_t;
}

#[no_mangle]
pub unsafe extern "C" fn expand_read_bits(
  mut data: *mut RExpandData,
  mut bits_to_load219: uint8_t,
) {
  /*
  Reads `bits_to_load219` bits into the LSB side of `data->bits182`.
  */
  while bits_to_load219 as libc::c_int > (*data).bits_in_buffer172 as libc::c_int {
    // This loop loads 1 new byte into `data->tmp_bit_buffer245`(the temporary
    // buffer)
    bits_to_load219 =
      (bits_to_load219 as libc::c_int - (*data).bits_in_buffer172 as libc::c_int) as uint8_t;
    // Rotate in the remaining bits from the tmp_bit_buffer.
    (*data).bits182 = ((((*data).bits182 as libc::c_int)
      << (*data).bits_in_buffer172 as libc::c_int)
      + ((*data).tmp_bit_buffer245 as libc::c_int
        >> 8i32 - (*data).bits_in_buffer172 as libc::c_int)) as uint16_t;
    if (*data).loaded_compressed_data_length246 as libc::c_int <= 0i32 {
      (*data).compressed_data_index = 0i32 as size_t;
      if (*data).compressed_data_length248 >= 0i32 as libc::c_long
        && (*data).compressed_data_length248 < 512i32 as libc::c_long
      {
        (*data).loaded_compressed_data_length246 = ALStorage_ReadBuffer(
          (*data).input_store,
          (*data).compressed_data_buffer242,
          (*data).compressed_data_length248 as size_t,
        ) as int16_t;
        (*data).compressed_data_length248 -=
          (*data).loaded_compressed_data_length246 as libc::c_long
      } else {
        (*data).loaded_compressed_data_length246 = ALStorage_ReadBuffer(
          (*data).input_store,
          (*data).compressed_data_buffer242,
          512i32 as size_t,
        ) as int16_t
      }
      if (*data).loaded_compressed_data_length246 as libc::c_int <= 0i32 {
        (*data).error_counter243 += 1
      }
    }
    (*data).tmp_bit_buffer245 = *(*data)
      .compressed_data_buffer242
      .offset((*data).compressed_data_index as isize);
    (*data).compressed_data_index = (*data).compressed_data_index.wrapping_add(1);
    (*data).loaded_compressed_data_length246 -= 1;
    (*data).bits_in_buffer172 = 8i32 as int16_t
  }
  (*data).bits_in_buffer172 =
    ((*data).bits_in_buffer172 as libc::c_int - bits_to_load219 as libc::c_int) as int16_t;
  (*data).bits182 = ((((*data).bits182 as libc::c_int) << bits_to_load219 as libc::c_int)
    + ((*data).tmp_bit_buffer245 as libc::c_int >> 8i32 - bits_to_load219 as libc::c_int))
    as uint16_t;
  (*data).tmp_bit_buffer245 =
    (((*data).tmp_bit_buffer245 as libc::c_int) << bits_to_load219 as libc::c_int) as uint8_t;
}
#[no_mangle]
pub unsafe extern "C" fn expand_get_bits(
  mut data: *mut RExpandData,
  mut bits_to_load219: uint8_t,
) -> uint16_t {
  let mut bits: uint16_t = 0;
  bits =
    ((*data).bits182 as libc::c_int >> 2i32 * 8i32 - bits_to_load219 as libc::c_int) as uint16_t;
  expand_read_bits(data, bits_to_load219);
  return bits;
}
