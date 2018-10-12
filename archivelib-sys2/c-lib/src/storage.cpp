#include "arclib.h"
#pragma hdrstop

#include <string.h>

//
// void * ALStorage::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  :  The number of bytes needed to create a new ALStorage object.
//
// RETURNS
//
//  A pointer to the newly allocated storage area, or 0 if no storage
//  was available.
//
// DESCRIPTION
//
//  When using a DLL, it is easy to get into a dangerous situation when
//  creating objects whose ctor and dtor are both in the DLL.  The problem
//  arises because when you create an object using new, the memory for
//  the object will be allocated from the EXE.  However, when you destroy
//  the object using delete, the memory is freed inside the DLL.  Since
//  the DLL doesn't really own that memory, bad things can happen.
//
//  But, you say, won't the space just go back to the Windows heap regardless
//  of who tries to free it?  Maybe, but maybe not.  If the DLL is using
//  a subsegment allocation scheme, it might do some sort of local free
//  before returning the space to the windows heap.  That is the point where
//  you could conceivably cook your heap.
//
//  By providing our own version of operator new inside this class, we
//  ensure that all memory allocation for the class will be done from
//  inside the DLL, not the EXE calling the DLL.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

#if defined(AL_BUILDING_DLL)
void AL_DLL_FAR *AL_PROTO ALStorage::operator new(size_t size) {
  return ::new char[size];
}
#endif

//
// ALStorage::ALStorage( const char *file_name,
//                       size_t size,
//                       const enum ALStorageType object_type,
//                       ALCase name_case )
//
// ARGUMENTS:
//
//  file_name     :  The name to assign to the mName data member of the
//                   newly created storage object.
//
//  size          :  The size of the I/O buffer that is going to be used
//                   for the storage object.  ALFile uses 4096 as a default.
//
//  object_type   :  The type of object, as defined in ALDEFS.H.  Good
//                   values include AL_FILE_OBJECT and AL_MEMORY_OBJECT.
//
//  name_case     :  The case sensitivity of the object name.  For objects
//                   such as ALFile, AL_MIXED is a no-no.  Those objects
//                   need to be forced to convert names to all upper
//                   or all lower, because the operating system considers
//                   file names to be case insensitive.
//
// RETURNS
//
//  Nothing, it is a constructor.
//
// DESCRIPTION
//
//  The constructor for ALStorage gets called from the constructor of
//  derived classes.  It has to initialize all sorts of data members.
//  First, in the initializer list, it sets up the mName data member,
//  as well as muBufferSize and miStorageObjectType.  The latter two
//  data members are set to be const so I can make them public, which
//  means we have to initialize them in the initializer list.
//
//  In the body of the constructor, we initialize a bunch of data members,
//  none of which mean anything at this point.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

AL_PROTO ALStorage::ALStorage(size_t size, const enum ALStorageType object_type)
    : miStorageObjectType(object_type), muBufferSize(size) {
  mpcBuffer = 0;
  muBufferValidData = 0;
  muWriteIndex = 0;
  muReadIndex = 0;
  mlFilePointer = 0;
  miUpdateCrcFlag = 0;
  mlCrc32 = 0xffffffffL;
  mlSize = -1L;
  miCreated = 0;
}

//
// ALStorage::~ALStorage()
//
// ARGUMENTS:
//
//  No arguments for destructors.
//
// RETURNS
//
//  No returns from destructors.
//
// DESCRIPTION
//
//  In debug mode, we first check to make sure we are destroying the
//  right type of object.
//
//  The only thing left to do is free up the I/O buffer if it is still
//  allocated.  This piece of work probably isn't necessary.  Since this
//  is a virtual destructor, we will be called after the destructors
//  for the derived class.  Any derived class that is doing its job
//  will make sure that it calls Close() before destroying itself.  If
//  it doesn't, it will probably be leaving unfinished business behind
//  that we aren't going to be able to deal with here.  Even so, we will
//  be diligent in our attention to detail.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

AL_PROTO ALStorage::~ALStorage() {
  AL_ASSERT(GoodTag(), "~ALStorage: attempting to delete invalid object");
  if (mpcBuffer)
    Close();
}

//
// This giant table is used by the CRC routines.  These are the coefficients
// for calculating the CCITT 32 bit CRC.  I typed these in from memory, so
// I hope they are correct.
//
static unsigned long ccitt_32[256] = {
    0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
    0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
    0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
    0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
    0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
    0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
    0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
    0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
    0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
    0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
    0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
    0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
    0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
    0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
    0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
    0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
    0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
    0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
    0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
    0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
    0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
    0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
    0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
    0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
    0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
    0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
    0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
    0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
    0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
    0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
    0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
    0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
    0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
    0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
    0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
    0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
    0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
    0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
    0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
    0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
    0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
    0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
    0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
    0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
    0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
    0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
    0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
    0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
    0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
    0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
    0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
    0x2d02ef8dL};

//
// void ALStorage::UpdateCrc( size_t count )
//
// ARGUMENTS:
//
//  count  :  The number of characters to process in the I/O buffer.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  If CRC checking has been turned on for the storage object, this
//  routine will be called every time LoadBuffer() or FlushBuffer()
//  are called.  It does CRC checking on a buffer full of data at
//  a time.  Hopefully this means the compiler can optimize the
//  heck out of this code.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

void AL_PROTO ALStorage::UpdateCrc(size_t count) {
  unsigned char *p = (unsigned char *)mpcBuffer;
  while (count-- != 0)
    mlCrc32 = ((mlCrc32 >> 8) & 0x00FFFFFFL) ^
              (ccitt_32[((int)mlCrc32 ^ *p++) & 0xff]);
}

//
// int ALStorage::Open()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  AL_SUCCESS, or AL_CANT_OPEN_BUFFER on memory allocation failure.
//  If the object was already in an error state, it is very possible to
//  get some other error code < 0.
//
// DESCRIPTION
//
//  Any derived class needs to have its own Open() function.  However,
//  the derived class can also call this Open() function in the base
//  class to do some odds and ends for it.  The most important thing it
//  does is allocate the I/O buffer, which is what makes ALStorage a
//  relatively fast way to read and write data.  Although the buffer
//  is in place, there is no data in it, so this guy also sets up the
//  indices and pointers to reflect that.
//
//  Upon exit, all you need to to is start reading or writing, and the
//  whole thing should be ready to go.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

int AL_PROTO ALStorage::Open() {
  if (mStatus < AL_SUCCESS)
    return mStatus;
  if (muBufferSize != 0)
    mpcBuffer = new unsigned char[muBufferSize];
  muBufferValidData = 0;
  muWriteIndex = 0;
  muReadIndex = 0;
  mlFilePointer = 0;
  miUpdateCrcFlag = 0;
  mlCrc32 = 0xffffffffL;
  if (mpcBuffer == 0)
    return mStatus.SetError(AL_CANT_OPEN_BUFFER,
                            "Allocation of buffer failed in Open()");
  return AL_SUCCESS;
}

//
// int ALStorage::Create()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  AL_SUCCESS, or AL_CANT_OPEN_BUFFER on memory allocation failure.
//  If the object was already in an error state, it is very possible to
//  get some other error code < 0.
//
// DESCRIPTION
//
//  This function is nearly identical to ALStorage::Open().
//
//  Any derived class needs to have its own Create() function.  However,
//  the derived class can also call this Create() function in the base
//  class to do some odds and ends for it.  The most important thing it
//  does is allocate the I/O buffer, which is what makes ALStorage a
//  relatively fast way to read and write data.  Although the buffer
//  is in place, there is no data in it, so this guy also sets up the
//  indices and pointers to reflect that.
//
//  Upon exit, all you need to to is start writing, and the
//  whole thing should be ready to go.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : When I create a file now, I set mlSize to 0.  I was
//                         running into trouble when I reused ALMemory objects.
//                         After creating them and closing them, mlSize was
//                         non-zero.  If I went back and created the file
//                         again, I would keep the old mlSize, which was still
//                         non-zero.  Doesn't make sense for newly created
//                         file.
//
int AL_PROTO ALStorage::Create() {
  if (mStatus < AL_SUCCESS)
    return mStatus;
  mpcBuffer = new unsigned char[muBufferSize];
  muBufferValidData = 0;
  muWriteIndex = 0;
  muReadIndex = 0;
  mlFilePointer = 0;
  miUpdateCrcFlag = 0;
  mlSize = 0; // If the file has been opened previous, mlSize might be non-zero
  mlCrc32 = 0xffffffffL;
  miCreated = 1;
  if (mpcBuffer == 0)
    return mStatus.SetError(AL_CANT_OPEN_BUFFER,
                            "Allocation of buffer failed in Open()");
  return AL_SUCCESS;
}

//
// int ALStorage::Close()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  The current integer status of the object.  Hopefully this will be
//  AL_SUCCESS, but it could well be a value < AL_SUCCESS.
//
// DESCRIPTION
//
//  Just like with Open(), must derived classes will have their own
//  versions of Close().  They can call this version to delete the I/O
//  buffer if they feel like it is too hard to do themselves.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

int AL_PROTO ALStorage::Close() {
  if (mpcBuffer) {
    delete[] mpcBuffer;
    mpcBuffer = 0;
  }
  return mStatus;
}

//
// long ALStorage::GetCrc32()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  The current value of the CRC-32.
//
// DESCRIPTION
//
//  This function is used to get the CRC-32 of a storage object.  But it
//  does a little bit more than just give you the CRC.  First, it makes
//  sure the buffers have been flushed, so that the CRC is accurate.  If
//  we didn't do this we might try to get the CRC on an incompletely
//  written file.
//
//  Once we get the CRC,the miUpdateCrcFlag is set to 0, which means
//  that from here on out the value will not be updated.  So retrieving
//  the CRC means you are no longer interested in further calculation.
//  It also means you can trust the value you just read, because it
//  will never be modified again.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

long AL_PROTO ALStorage::GetCrc32() {
  if (IsOpen() && miUpdateCrcFlag)
    FlushBuffer();
  miUpdateCrcFlag = 0;
  return mlCrc32;
}

//
// void ALStorage::InitCrc32( unsigned long seed = 0xffffffffL )
//
// ARGUMENTS:
//
//  seed  :  The long value to start the CRC off at.  There is probably
//           no reason to change this from the default value, although
//           I won't be surprised if someone comes up with one.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  Calling this function kicks off the CRC calculation for a given
//  storage object should be done immediately after the object is
//  opened.  Once the miUpdateCrcFlag is set, the CRC will be updated
//  every time a LoadBuffer() or FlushBuffer() is called.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

void AL_PROTO ALStorage::InitCrc32(unsigned long seed /* = 0xffffffffL */) {
  miUpdateCrcFlag = 1;
  mlCrc32 = seed;
}

//
// size_t ALStorage::ReadBuffer( unsigned char *buf, size_t length )
//
// ARGUMENTS:
//
//  buf    :  The buffer that is going to receive input characters.
//
//  length :  The number of bytes you want to read.
//
// RETURNS
//
//  The number of bytes read in, always.  If this function generates an
//  error, it will be found in the mStatus member.
//
// DESCRIPTION
//
//  We could write a simple version of this function by just calling
//  ReadChar() over and over, but it would be nice to do things
//  a little more efficiently.  Since we have this nice big buffer
//  full of data ready to read, it makes sense to copy big chunks of
//  it in one fell swoop.  That is what this guy does.  It sits in a loop
//  doing a memcpy() followed by LoadBuffer() until all of the data
//  that has been asked for got moved.  As data is read in, we have to
//  update the data member muReadIndex.  Other data members will get
//  updated by LoadBuffer().
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

size_t AL_PROTO ALStorage::ReadBuffer(unsigned char *buf, size_t length) {
  size_t bytes_left_to_read = length;
  size_t buffer_bytes_available;

  while (bytes_left_to_read) {
    buffer_bytes_available = muBufferValidData - muReadIndex;
    if (buffer_bytes_available == 0) {
      if (LoadBuffer(mlFilePointer) < 0)
        return length - bytes_left_to_read;
      buffer_bytes_available = muBufferValidData;
    }
    if (bytes_left_to_read <= buffer_bytes_available) {
      memcpy(buf, mpcBuffer + muReadIndex, bytes_left_to_read);
      muReadIndex += bytes_left_to_read;
      return length;
    } else {
      memcpy(buf, mpcBuffer + muReadIndex, buffer_bytes_available);
      buf += buffer_bytes_available;
      bytes_left_to_read -= buffer_bytes_available;
      muReadIndex += buffer_bytes_available;
      if (LoadBuffer(mlFilePointer) < 0)
        return length - bytes_left_to_read;
    }
  }
  return length;
}

//
// size_t ALStorage::WriteBuffer( const unsigned char *buf,
//                                size_t length )
//
// ARGUMENTS:
//
//  buf    :  The buffer that is contains the output data.
//
//  length :  The number of bytes you want to write.
//
// RETURNS
//
//  The number of bytes written, always.  If this function generates an
//  error, it will be found in the mStatus member.
//
// DESCRIPTION
//
//  We could write a simple version of this function by just calling
//  WriteChar() over and over, but it would be nice to do things
//  a little more efficiently.  Since we have this nice big buffer
//  just waiting for data, it makes sense to copy big chunks to
//  it in one fell swoop.  That is what this guy does.  It sits in a loop
//  doing a memcpy() followed by FlushBuffer() until all of the data
//  that was ready to go has been sent. As data is written, we have to
//  update the data member muWriteIndex.  Other data members will get
//  updated by FlushBuffer().
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

size_t AL_PROTO ALStorage::WriteBuffer(const unsigned char *buf,
                                       size_t length) {
  size_t buffer_bytes_free;
  size_t write_bytes_left = length;

  if (mStatus < 0)
    return 0;
  while (write_bytes_left > 0) {
    buffer_bytes_free = muBufferSize - muWriteIndex;
    if (buffer_bytes_free == 0) {
      if (FlushBuffer() < 0)
        return length - write_bytes_left;
      buffer_bytes_free = muBufferSize;
    }
    if (write_bytes_left <= buffer_bytes_free) {
      memcpy(mpcBuffer + muWriteIndex, buf, write_bytes_left);
      muWriteIndex += write_bytes_left;
      return length;
    } else {
      memcpy(mpcBuffer + muWriteIndex, buf, buffer_bytes_free);
      muWriteIndex += buffer_bytes_free;
      buf += buffer_bytes_free;
      write_bytes_left -= buffer_bytes_free;
      if (FlushBuffer() < 0)
        return length - write_bytes_left;
    }
  }
  return length;
}

//
// int ALStorage::WritePortableShort( short int short_data )
//
// ARGUMENTS:
//
//  short_data  :  A 16 bit int that is going to be written out in
//                 little endian format.
//
// RETURNS
//
//  AL_SUCCESS if all goes well.  Otherwise, some error code < AL_STATUS.
//
// DESCRIPTION
//
//  In order to make sure our archives can be read and written on all sorts
//  of systems, we have a few functions that are used to write numerical
//  data in a portable fashion.  This function writes short integers in
//  little endian format (which is not native Intel format).  The complementary
//  function, ReadPortableShort(), reads short integers back using the
//  same format.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

int AL_PROTO ALStorage::WritePortableShort(short int short_data) {
  WriteChar(short_data >> 8);
  WriteChar(short_data);
  return mStatus;
}

//
// int ALStorage::WritePortableLong( long int long_data )
//
// ARGUMENTS:
//
//  long_data  :  A 32 bit long int that is going to be written out in
//                little endian format.
//
// RETURNS
//
//  AL_SUCCESS if all goes well.  Otherwise, some error code < AL_STATUS.
//
// DESCRIPTION
//
//  In order to make sure our archives can be read and written on all sorts
//  of systems, we have a few functions that are used to write numerical
//  data in a portable fashion.  This function writes long integers in
//  little endian format (which is not native Intel format).  The
//  complementary function, ReadPortableLong(), reads long integers back
//  using the same format.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

int AL_PROTO ALStorage::WritePortableLong(long long_data) {
  WriteChar((int)(long_data >> 24));
  WriteChar((int)(long_data >> 16));
  WriteChar((int)(long_data >> 8));
  WriteChar((int)long_data);
  return mStatus;
}

//
// int ALStorage::ReadPortableShort( short int &short_data )
//
// ARGUMENTS:
//
//  short_data  :  A reference to a 16 bit integer that is going to
//                 have data read in from this storage object.
//
// RETURNS
//
//  AL_SUCCESS if all goes well.  Otherwise, some error code < AL_STATUS.
//
// DESCRIPTION
//
//  In order to make sure our archives can be read and written on all sorts
//  of systems, we have a few functions that are used to read numerical
//  data in a portable fashion.  This function reads short integers in
//  little endian format (which is not native Intel format).  The
//  complementary function, WritePortableShort(), writes short integers out
//  using the same format.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

int AL_PROTO ALStorage::ReadPortableShort(short int &short_data) {
  short_data = (short int)(ReadChar() << 8);
  short_data |= (short int)ReadChar();
  return mStatus;
}

//
// int ALStorage::ReadPortableLong( long int &short_data )
//
// ARGUMENTS:
//
//  long_data  :  A reference to a 32 bit integer that is going to
//                have data read in from this storage object.
//
// RETURNS
//
//  AL_SUCCESS if all goes well.  Otherwise, some error code < AL_STATUS.
//
// DESCRIPTION
//
//  In order to make sure our archives can be read and written on all sorts
//  of systems, we have a few functions that are used to read numerical
//  data in a portable fashion.  This function reads long integers in
//  little endian format (which is not native Intel format).  The
//  complementary function, WritePortableLong(), writes long integers out
//  using the same format.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

int AL_PROTO ALStorage::ReadPortableLong(long &long_data) {
  long_data = (long)ReadChar() << 24;
  long_data |= (long)ReadChar() << 16;
  long_data |= (long)ReadChar() << 8;
  long_data |= ReadChar();
  return mStatus;
}

//
// int ALStorage::WriteString( const char *string_data )
//
// ARGUMENTS:
//
//  string_data  :  A string to be written out in our portable format.
//
// RETURNS
//
//  AL_SUCCESS if things work, or an error code < AL_SUCCESS if an error
//  occurs writing the data out.
//
// DESCRIPTION
//
//  We write random length data to archive directories using this special
//  format, which is a 16 bit int describing the length of the data,
//  followed by the data itself.  All of the storage objects and compression
//  engines write their own private data out using this format.  This
//  means that even if another class doesn't understand the content of data
//  stored in this format, at least it knows how to read it in so as to
//  move past it.
//
//  This function won't write just any random data, it is specifically
//  oriented towards C strings.  This means it is mostly used to write
//  file names and comments.  Their are a few places where classes
//  write private data that isn't kept in C strings, they just manually
//  write the length with WritePortableShort(), followed by the data.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

int AL_PROTO ALStorage::WriteString(const char *string_data) {
  short unsigned int len;
  if (string_data != 0)
    len = (short unsigned int)strlen(string_data);
  else
    len = 0;
  WritePortableShort(len);
  if (len)
    WriteBuffer((unsigned char *)string_data, len);
  return mStatus;
}

// PROTECTED MEMBER FUNCTION
//
// char * ALStorage::ReadString()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  A pointer to a string.  This string has been allocated by the library,
//  which can cause a problem if you are using a DLL.  If an EXE tried
//  to free a string pointer allocated by the DLL, havoc would result.
//  Because of this hassle, this is a protected function.
//
//  The solution to this is to write a new version of this that returns
//  an ALName object.  I thought of that, but too late.
//
// DESCRIPTION
//
//  This function is used internally by ArchiveLib.  It is used to read
//  random length blocks of data out of archives (or other storage objects).
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

char AL_DLL_FAR *AL_PROTO ALStorage::ReadString() {
  short int len;

  if (ReadPortableShort(len) < 0)
    return 0;
  char *new_string = new char[len + 1];
  if (new_string) {
    ReadBuffer((unsigned char *)new_string, len);
    new_string[len] = '\0';
    return new_string;
  } else {
    mStatus.SetError(AL_CANT_ALLOCATE_MEMORY,
                     "Error allocating buffer space in call "
                     "to ReadString() for ALStorage object");
    return 0;
  }
}

//
// long ALStorage::Tell()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  A long integer indicating the current position of the read/write
//  pointer for the file.
//
// DESCRIPTION
//
//  Because we are using buffered I/O here, figuring out the current
//  position of the read write pointer is just a tiny bit more complicated
//  than just checking a pointer.  We have to find the physical location of
//  the file pointer, then add in any offset created by the presence of
//  data in the I/O buffer.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

long AL_PROTO ALStorage::Tell() {
  if (muWriteIndex)
    return mlFilePointer + muWriteIndex;
  else
    return mlFilePointer - muBufferValidData + muReadIndex;
}

//
// int ALStorage::WriteStorageObjectData( ALStorage * archive )
//
// ARGUMENTS:
//
//  archive : A pointer to the storage object where we are going to
//            write the private data.
//
// RETURNS
//
//  AL_SUCCESS if things went okay, otherwise an error code < AL_SUCCESS.
//
// DESCRIPTION
//
//  All storage objects have the ability to create a private data block
//  that will be stored along with the directory when creating an archive.
//  None of the classes predefined in ArchiveLib use this data block, which
//  means they use this function instead of providing their own virtual
//  substitute. This function writes a private data block of exactly 0
//  bytes in length.  Our internal storage format means that a block
//  of 0 bytes length takes 2 bytes to store.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

int AL_PROTO ALStorage::WriteStorageObjectData(ALStorage *archive) {
  return archive->WritePortableShort(0);
}

//
// int ALStorage::ReadStorageObjectData( ALStorage * archive )
//
// ARGUMENTS:
//
//  archive : A pointer to the storage object where we are going to
//            read in the private data..
//
// RETURNS
//
//  AL_SUCCESS if things went okay, otherwise an error code < AL_SUCCESS.
//
// DESCRIPTION
//
//  All storage objects have the ability to create a private data block
//  that will be stored along with the directory when creating an archive.
//  None of the classes predefined in ArchiveLib use this data block, which
//  means they use this function instead of providing their own virtual
//  substitute. This function reads a private data block of exactly 0
//  bytes in length.  Our internal storage format means that a block
//  of 0 bytes length takes 2 bytes to store.
//
//  In debug mode, we get really bent out of shape if this data block
//  doesn't look exactly like we expect it to.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

int AL_PROTO ALStorage::ReadStorageObjectData(ALStorage *archive) {
  short int temp;
  int status = archive->ReadPortableShort(temp);
  AL_ASSERT(temp == 0, "ReadStorageObjectData: stored data is not null");
  return status;
}

//
//  char * ALStorage::ReadCopyright()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  A pointer to a copyright notice.
//
// DESCRIPTION
//
//  It is a good idea for us to have a copyright notice embedded in the
//  library.  Hopefully, this notice will show up in any executables linked
//  using this library, or in the DLL they link to.
//
// REVISION HISTORY
//
//   August 10, 1994
