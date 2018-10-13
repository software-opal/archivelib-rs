#include "arclib.h"

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
void  * ALStorage::operator new(size_t size) {
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

 ALStorage::ALStorage(size_t size) : muBufferSize(size) {
  mpcBuffer = 0;
  muBufferValidData = 0;
  muWriteIndex = 0;
  muReadIndex = 0;
  mlFilePointer = 0;
  mlSize = -1L;
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

 ALStorage::~ALStorage() {
  AL_ASSERT(GoodTag(), "~ALStorage: attempting to delete invalid object");
  if (mpcBuffer)
    Close();
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

int  ALStorage::Open() {
  if (mStatus < AL_SUCCESS)
    return mStatus;
  if (muBufferSize != 0)
    mpcBuffer = new uint8_t[muBufferSize];
  muBufferValidData = 0;
  muWriteIndex = 0;
  muReadIndex = 0;
  mlFilePointer = 0;
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
int  ALStorage::Create() {
  if (mStatus < AL_SUCCESS)
    return mStatus;
  mpcBuffer = new uint8_t[muBufferSize];
  muBufferValidData = 0;
  muWriteIndex = 0;
  muReadIndex = 0;
  mlFilePointer = 0;
  mlSize = 0; // If the file has been opened previous, mlSize might be non-zero
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

int  ALStorage::Close() {
  if (mpcBuffer) {
    delete[] mpcBuffer;
    mpcBuffer = 0;
  }
  return mStatus;
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

size_t  ALStorage::ReadBuffer(uint8_t *buf, size_t length) {
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

size_t  ALStorage::WriteBuffer(uint8_t *buf,
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

// PROTECTED MEMBER FUNCTION
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

long  ALStorage::Tell() {
  if (muWriteIndex)
    return mlFilePointer + muWriteIndex;
  else
    return mlFilePointer - muBufferValidData + muReadIndex;
}
