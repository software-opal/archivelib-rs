
#include "aldefs.h"
#include <string>

std::string reverseALErrors(int code) {
  switch (code) {
  AL_CANT_OPEN_BUFFER:
    return "AL_CANT_OPEN_BUFFER";
  AL_CANT_ALLOCATE_MEMORY:
    return "AL_CANT_ALLOCATE_MEMORY";
  AL_CANT_CREATE_ENGINE:
    return "AL_CANT_CREATE_ENGINE";
  AL_CANT_CREATE_STORAGE_OBJECT:
    return "AL_CANT_CREATE_STORAGE_OBJECT";
  AL_RENAME_ERROR:
    return "AL_RENAME_ERROR";
  AL_CANT_OPEN_FILE:
    return "AL_CANT_OPEN_FILE";
  AL_SEEK_ERROR:
    return "AL_SEEK_ERROR";
  AL_READ_ERROR:
    return "AL_READ_ERROR";
  AL_WRITE_ERROR:
    return "AL_WRITE_ERROR";
  AL_DELETE_ERROR:
    return "AL_DELETE_ERROR";
  AL_ILLEGAL_PARAMETER:
    return "AL_ILLEGAL_PARAMETER";
  AL_INTERNAL_ERROR:
    return "AL_INTERNAL_ERROR";
  AL_USER_ABORT:
    return "AL_USER_ABORT";
  AL_SERVER_NOT_PRESENT:
    return "AL_SERVER_NOT_PRESENT";
  AL_COMPRESSION_TYPE_MISMATCH:
    return "AL_COMPRESSION_TYPE_MISMATCH";
  AL_NEED_LENGTH:
    return "AL_NEED_LENGTH";
  AL_CRC_ERROR:
    return "AL_CRC_ERROR";
  AL_COMPARE_ERROR:
    return "AL_COMPARE_ERROR";
  AL_UNKNOWN_COMPRESSION_TYPE:
    return "AL_UNKNOWN_COMPRESSION_TYPE";
  AL_UNKNOWN_STORAGE_OBJECT:
    return "AL_UNKNOWN_STORAGE_OBJECT";
  AL_INVALID_ARCHIVE:
    return "AL_INVALID_ARCHIVE";
  AL_LOGIC_ERROR:
    return "AL_LOGIC_ERROR";
  AL_BACKUP_FAILURE:
    return "AL_BACKUP_FAILURE";
  AL_GETSEL_ERROR:
    return "AL_GETSEL_ERROR";
  AL_DUPLICATE_ENTRY:
    return "AL_DUPLICATE_ENTRY";
  AL_END_OF_FILE:
    return "AL_END_OF_FILE";
  AL_SUCCESS:
    return "AL_SUCCESS";
  default:
    return "__unknown__";
  }
}
