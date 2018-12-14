
#include "r_compress.hpp"

void fn224(RCompressData *data, uint16_t _204) {
  DEBUG_FILE_HANDLE(fs, data);
  fs << "{\"ptr\": " << (intptr_t)(data);
  WRITE_HEX(fs, "_204", _204);
  WRITE_DATA_ARRAY(fs, data, dat_arr181, uint8_t);
  WRITE_DATA_ARRAY(fs, data, dat_arr194, uint16_t);

  uint16_t byte_or_run_length203, _457;
  byte_or_run_length203 = 0;
  _457 = _204;
  while (_457) {
    byte_or_run_length203++;
    _457 >>= 1;
  }
  fs << ",'output[0]': {bit_count: " << ((int) data->dat_arr181[byte_or_run_length203]) << ", bits: " <<  data->dat_arr194[byte_or_run_length203] << "}";
  write_bits_to_buffer(data, data->dat_arr181[byte_or_run_length203],
                       data->dat_arr194[byte_or_run_length203]);
  if (byte_or_run_length203 > 1){
    fs << ",'output[1]': {bit_count: " << (byte_or_run_length203 - 1) << ", bits: " <<  _204 << "}";
    write_bits_to_buffer(data, byte_or_run_length203 - 1, _204);
  }
  fs << "}";
}
