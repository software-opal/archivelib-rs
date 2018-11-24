#![allow(
  dead_code,
  mutable_transmutes,
  non_camel_case_types,
  non_snake_case,
  non_upper_case_globals,
  unused_mut
)]
#![feature(libc)]
extern crate libc;
extern "C" {
  #[no_mangle]
  fn calloc(_: libc::c_ulong, _: libc::c_ulong) -> *mut libc::c_void;
  #[no_mangle]
  fn free(__ptr: *mut libc::c_void);
}
pub type size_t = libc::c_ulong;
pub type __int8_t = libc::c_schar;
pub type __uint8_t = libc::c_uchar;
pub type __int16_t = libc::c_short;
pub type __uint16_t = libc::c_ushort;
pub type __int32_t = libc::c_int;
pub type __ssize_t = libc::c_long;
pub type ssize_t = __ssize_t;
pub type int8_t = __int8_t;
pub type int16_t = __int16_t;
pub type int32_t = __int32_t;
pub type ALErrors = libc::c_int;
pub const AL_SUCCESS: ALErrors = 0;
pub const AL_END_OF_FILE: ALErrors = -1;
pub const AL_DUPLICATE_ENTRY: ALErrors = -1176;
pub const AL_GETSEL_ERROR: ALErrors = -1177;
pub const AL_BACKUP_FAILURE: ALErrors = -1178;
pub const AL_LOGIC_ERROR: ALErrors = -1179;
pub const AL_INVALID_ARCHIVE: ALErrors = -1180;
pub const AL_UNKNOWN_STORAGE_OBJECT: ALErrors = -1181;
pub const AL_UNKNOWN_COMPRESSION_TYPE: ALErrors = -1182;
pub const AL_COMPARE_ERROR: ALErrors = -1183;
pub const AL_CRC_ERROR: ALErrors = -1184;
pub const AL_NEED_LENGTH: ALErrors = -1185;
pub const AL_COMPRESSION_TYPE_MISMATCH: ALErrors = -1186;
pub const AL_SERVER_NOT_PRESENT: ALErrors = -1187;
pub const AL_USER_ABORT: ALErrors = -1188;
pub const AL_INTERNAL_ERROR: ALErrors = -1189;
pub const AL_ILLEGAL_PARAMETER: ALErrors = -1190;
pub const AL_DELETE_ERROR: ALErrors = -1191;
pub const AL_WRITE_ERROR: ALErrors = -1192;
pub const AL_READ_ERROR: ALErrors = -1193;
pub const AL_SEEK_ERROR: ALErrors = -1194;
pub const AL_CANT_OPEN_FILE: ALErrors = -1195;
pub const AL_RENAME_ERROR: ALErrors = -1196;
pub const AL_CANT_CREATE_STORAGE_OBJECT: ALErrors = -1197;
pub const AL_CANT_CREATE_ENGINE: ALErrors = -1198;
pub const AL_CANT_ALLOCATE_MEMORY: ALErrors = -1199;
pub const AL_CANT_OPEN_BUFFER: ALErrors = -1200;
pub type uint8_t = __uint8_t;
pub type uint16_t = __uint16_t;
pub type ALStorage = ();
#[derive(Copy, Clone)]
#[repr(C)]
pub struct RExpandData {
  pub input_store: *mut libc::c_void,
  pub output_store: *mut libc::c_void,
  pub error: int32_t,
  pub uncompressed_buffer: *mut uint8_t,
  pub dat_arr180: *mut uint8_t,
  pub dat_arr181: *mut uint8_t,
  pub dat_arr189: *mut uint16_t,
  pub dat_arr190: *mut uint16_t,
  pub dat_arr240: *mut uint16_t,
  pub dat_arr241: *mut uint16_t,
  pub compressed_data_buffer242: *mut uint8_t,
  pub uncompressed_buffer_len: size_t,
  pub dat_arr180_len: size_t,
  pub dat_arr181_len: size_t,
  pub dat_arr189_len: size_t,
  pub dat_arr190_len: size_t,
  pub dat_arr240_len: size_t,
  pub dat_arr241_len: size_t,
  pub compressed_data_buffer242_len: size_t,
  pub compressed_data_index: size_t,
  pub bits_in_buffer172: int16_t,
  pub max_uncompressed_data_size: int16_t,
  pub max_uncompressed_data_size_bitmask: int16_t,
  pub bits182: uint16_t,
  pub error_counter243: int16_t,
  pub items_until_next_header: uint16_t,
  pub tmp_bit_buffer245: uint8_t,
  pub loaded_compressed_data_length246: int16_t,
  pub compressed_data_length248: ssize_t,
}
#[no_mangle]
pub unsafe extern "C" fn create_expand_data(
  mut data: *mut RExpandData,
  mut in_storage: *mut libc::c_void,
  mut out_storage: *mut libc::c_void,
  mut in_length: ssize_t,
  mut compression_level: libc::c_int,
) -> ALErrors {
  (*data).input_store = in_storage;
  (*data).output_store = out_storage;
  (*data).compressed_data_length248 = in_length;
  if compression_level > 14i32 || compression_level < 10i32 {
    return AL_ILLEGAL_PARAMETER;
  } else {
    (*data).max_uncompressed_data_size = (1i32 << compression_level) as int16_t;
    (*data).max_uncompressed_data_size_bitmask =
      ((*data).max_uncompressed_data_size as libc::c_int - 1i32) as int16_t;
    (*data).uncompressed_buffer_len =
      ((*data).max_uncompressed_data_size as libc::c_int + 2i32) as size_t;
    (*data).uncompressed_buffer = calloc(
      (*data).uncompressed_buffer_len,
      ::std::mem::size_of::<int8_t>() as libc::c_ulong,
    ) as *mut uint8_t;
    (*data).dat_arr180_len = (127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32) as size_t;
    (*data).dat_arr180 = calloc(
      (*data).dat_arr180_len,
      ::std::mem::size_of::<uint8_t>() as libc::c_ulong,
    ) as *mut uint8_t;
    (*data).dat_arr181_len = (16i32 + 3i32) as size_t;
    (*data).dat_arr181 = calloc(
      (*data).dat_arr181_len,
      ::std::mem::size_of::<uint8_t>() as libc::c_ulong,
    ) as *mut uint8_t;
    (*data).dat_arr189_len =
      (2i32 * (127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32) - 1i32) as size_t;
    (*data).dat_arr189 = calloc(
      (*data).dat_arr189_len,
      ::std::mem::size_of::<uint16_t>() as libc::c_ulong,
    ) as *mut uint16_t;
    (*data).dat_arr190_len =
      (2i32 * (127i32 * 2i32 + 1i32 + 1i32 + 256i32 - 3i32 + 1i32 + 1i32) - 1i32) as size_t;
    (*data).dat_arr190 = calloc(
      (*data).dat_arr190_len,
      ::std::mem::size_of::<uint16_t>() as libc::c_ulong,
    ) as *mut uint16_t;
    (*data).dat_arr240_len = 4096i32 as size_t;
    (*data).dat_arr240 = calloc(
      (*data).dat_arr240_len,
      ::std::mem::size_of::<uint16_t>() as libc::c_ulong,
    ) as *mut uint16_t;
    (*data).dat_arr241_len = 256i32 as size_t;
    (*data).dat_arr241 = calloc(
      (*data).dat_arr241_len,
      ::std::mem::size_of::<uint16_t>() as libc::c_ulong,
    ) as *mut uint16_t;
    (*data).compressed_data_buffer242_len = 512i32 as size_t;
    (*data).compressed_data_buffer242 = calloc(
      (*data).compressed_data_buffer242_len,
      ::std::mem::size_of::<uint8_t>() as libc::c_ulong,
    ) as *mut uint8_t;
    if (*data).uncompressed_buffer.is_null()
      || (*data).dat_arr180.is_null()
      || (*data).dat_arr181.is_null()
      || (*data).dat_arr189.is_null()
      || (*data).dat_arr190.is_null()
      || (*data).dat_arr240.is_null()
      || (*data).dat_arr241.is_null()
      || (*data).compressed_data_buffer242.is_null()
    {
      return AL_CANT_ALLOCATE_MEMORY;
    } else {
      return AL_SUCCESS;
    }
  };
}
#[no_mangle]
pub unsafe extern "C" fn free_expand_data(mut data: *mut RExpandData) {
  if !(*data).uncompressed_buffer.is_null() {
    free((*data).uncompressed_buffer as *mut libc::c_void);
    (*data).uncompressed_buffer = 0 as *mut uint8_t
  }
  if !(*data).dat_arr180.is_null() {
    free((*data).dat_arr180 as *mut libc::c_void);
    (*data).dat_arr180 = 0 as *mut uint8_t
  }
  if !(*data).dat_arr181.is_null() {
    free((*data).dat_arr181 as *mut libc::c_void);
    (*data).dat_arr181 = 0 as *mut uint8_t
  }
  if !(*data).dat_arr189.is_null() {
    free((*data).dat_arr189 as *mut libc::c_void);
    (*data).dat_arr189 = 0 as *mut uint16_t
  }
  if !(*data).dat_arr190.is_null() {
    free((*data).dat_arr190 as *mut libc::c_void);
    (*data).dat_arr190 = 0 as *mut uint16_t
  }
  if !(*data).dat_arr240.is_null() {
    free((*data).dat_arr240 as *mut libc::c_void);
    (*data).dat_arr240 = 0 as *mut uint16_t
  }
  if !(*data).dat_arr241.is_null() {
    free((*data).dat_arr241 as *mut libc::c_void);
    (*data).dat_arr241 = 0 as *mut uint16_t
  }
  if !(*data).compressed_data_buffer242.is_null() {
    free((*data).compressed_data_buffer242 as *mut libc::c_void);
    (*data).compressed_data_buffer242 = 0 as *mut uint8_t
  };
}
