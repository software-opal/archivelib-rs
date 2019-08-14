
#include "aldefs.h"
#include <string>

std::string reverseALErrors(int code) {
  switch (code) {
  case AL_CANT_OPEN_BUFFER:
    return "AL_CANT_OPEN_BUFFER";
  case AL_CANT_ALLOCATE_MEMORY:
    return "AL_CANT_ALLOCATE_MEMORY";
  case AL_CANT_CREATE_ENGINE:
    return "AL_CANT_CREATE_ENGINE";
  case AL_CANT_CREATE_STORAGE_OBJECT:
    return "AL_CANT_CREATE_STORAGE_OBJECT";
  case AL_RENAME_ERROR:
    return "AL_RENAME_ERROR";
  case AL_CANT_OPEN_FILE:
    return "AL_CANT_OPEN_FILE";
  case AL_SEEK_ERROR:
    return "AL_SEEK_ERROR";
  case AL_READ_ERROR:
    return "AL_READ_ERROR";
  case AL_WRITE_ERROR:
    return "AL_WRITE_ERROR";
  case AL_DELETE_ERROR:
    return "AL_DELETE_ERROR";
  case AL_ILLEGAL_PARAMETER:
    return "AL_ILLEGAL_PARAMETER";
  case AL_INTERNAL_ERROR:
    return "AL_INTERNAL_ERROR";
  case AL_USER_ABORT:
    return "AL_USER_ABORT";
  case AL_SERVER_NOT_PRESENT:
    return "AL_SERVER_NOT_PRESENT";
  case AL_COMPRESSION_TYPE_MISMATCH:
    return "AL_COMPRESSION_TYPE_MISMATCH";
  case AL_NEED_LENGTH:
    return "AL_NEED_LENGTH";
  case AL_CRC_ERROR:
    return "AL_CRC_ERROR";
  case AL_COMPARE_ERROR:
    return "AL_COMPARE_ERROR";
  case AL_UNKNOWN_COMPRESSION_TYPE:
    return "AL_UNKNOWN_COMPRESSION_TYPE";
  case AL_UNKNOWN_STORAGE_OBJECT:
    return "AL_UNKNOWN_STORAGE_OBJECT";
  case AL_INVALID_ARCHIVE:
    return "AL_INVALID_ARCHIVE";
  case AL_LOGIC_ERROR:
    return "AL_LOGIC_ERROR";
  case AL_BACKUP_FAILURE:
    return "AL_BACKUP_FAILURE";
  case AL_GETSEL_ERROR:
    return "AL_GETSEL_ERROR";
  case AL_DUPLICATE_ENTRY:
    return "AL_DUPLICATE_ENTRY";
  case AL_END_OF_FILE:
    return "AL_END_OF_FILE";
  case AL_SUCCESS:
    return "AL_SUCCESS";
  default:
    return "__unknown__";
  }
}
