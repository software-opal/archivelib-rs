pub fn create_expand_data(
  &mut self,
  mut in_storage: *mut libc::c_void,
  mut out_storage: *mut libc::c_void,
  mut in_length: ssize_t,
  mut compression_level: libc::c_int,
) -> ALErrors {
  (self).input_store = in_storage;
  (self).output_store = out_storage;
  (self).compressed_data_length248 = in_length;
  if compression_level > 14i32 || compression_level < 10i32 {
    return AL_ILLEGAL_PARAMETER;
  } else {
    (self).max_uncompressed_data_size = (1i32 << compression_level) as i16;
    (self).max_uncompressed_data_size_bitmask =
      ((self).max_uncompressed_data_size as libc::c_int - 1i32) as i16;
    (self).uncompressed_buffer_len =
      ((self).max_uncompressed_data_size as libc::c_int + 2i32) as size_t;
    (self).uncompressed_buffer = calloc(
      (self).uncompressed_buffer_len,
      ::std::mem::size_of::<i8>() as libc::c_ulong,
    ) as *mut u8;
    (self).dat_arr180_len = (127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32) as size_t;
    (self).dat_arr180 = calloc(
      (self).dat_arr180_len,
      ::std::mem::size_of::<u8>() as libc::c_ulong,
    ) as *mut u8;
    (self).dat_arr181_len = (16i32 + 3i32) as size_t;
    (self).dat_arr181 = calloc(
      (self).dat_arr181_len,
      ::std::mem::size_of::<u8>() as libc::c_ulong,
    ) as *mut u8;
    (self).dat_arr189_len =
      (2i32 * (127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32) - 1i32) as size_t;
    (self).dat_arr189 = calloc(
      (self).dat_arr189_len,
      ::std::mem::size_of::<u16>() as libc::c_ulong,
    ) as *mut u16;
    (self).dat_arr190_len =
      (2i32 * (127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32) - 1i32) as size_t;
    (self).dat_arr190 = calloc(
      (self).dat_arr190_len,
      ::std::mem::size_of::<u16>() as libc::c_ulong,
    ) as *mut u16;
    (self).dat_arr240_len = 4096i32 as size_t;
    (self).dat_arr240 = calloc(
      (self).dat_arr240_len,
      ::std::mem::size_of::<u16>() as libc::c_ulong,
    ) as *mut u16;
    (self).dat_arr241_len = 256i32 as size_t;
    (self).dat_arr241 = calloc(
      (self).dat_arr241_len,
      ::std::mem::size_of::<u16>() as libc::c_ulong,
    ) as *mut u16;
    (self).compressed_data_buffer242_len = 512i32 as size_t;
    (self).compressed_data_buffer242 = calloc(
      (self).compressed_data_buffer242_len,
      ::std::mem::size_of::<u8>() as libc::c_ulong,
    ) as *mut u8;
    if (self).uncompressed_buffer.is_null()
      || (self).dat_arr180.is_null()
      || (self).dat_arr181.is_null()
      || (self).dat_arr189.is_null()
      || (self).dat_arr190.is_null()
      || (self).dat_arr240.is_null()
      || (self).dat_arr241.is_null()
      || (self).compressed_data_buffer242.is_null()
    {
      return AL_CANT_ALLOCATE_MEMORY;
    } else {
      return AL_SUCCESS;
    }
  };
}
pub fn free_expand_data(&mut self) {
  if !(self).uncompressed_buffer.is_null() {
    free((self).uncompressed_buffer as *mut libc::c_void);
    (self).uncompressed_buffer = 0 as *mut u8
  }
  if !(self).dat_arr180.is_null() {
    free((self).dat_arr180 as *mut libc::c_void);
    (self).dat_arr180 = 0 as *mut u8
  }
  if !(self).dat_arr181.is_null() {
    free((self).dat_arr181 as *mut libc::c_void);
    (self).dat_arr181 = 0 as *mut u8
  }
  if !(self).dat_arr189.is_null() {
    free((self).dat_arr189 as *mut libc::c_void);
    (self).dat_arr189 = 0 as *mut u16
  }
  if !(self).dat_arr190.is_null() {
    free((self).dat_arr190 as *mut libc::c_void);
    (self).dat_arr190 = 0 as *mut u16
  }
  if !(self).dat_arr240.is_null() {
    free((self).dat_arr240 as *mut libc::c_void);
    (self).dat_arr240 = 0 as *mut u16
  }
  if !(self).dat_arr241.is_null() {
    free((self).dat_arr241 as *mut libc::c_void);
    (self).dat_arr241 = 0 as *mut u16
  }
  if !(self).compressed_data_buffer242.is_null() {
    free((self).compressed_data_buffer242 as *mut libc::c_void);
    (self).compressed_data_buffer242 = 0 as *mut u8
  };
}
