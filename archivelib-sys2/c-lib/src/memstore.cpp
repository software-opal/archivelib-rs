#include "arclib.h"

#include "memstore.h"

#include <stdlib.h>
#include <cstring>
#include <iostream>

//
// ALMemory::ALMemory( const char *buffer_name = "",
//                     char *user_buffer = 0,
//                     int user_buffer_size = 0,
//                     ALCase name_case = AL_MIXED )
//
// ARGUMENTS:
//
//  user_buffer  : If you want the ALMemory class to automatically allocate
//                 a buffer for you, and grow it as necessary, just leave
//                 this pointer set to 0.  If you want to use your own buffer,
//                 which won't have the ability to grow, pass a pointer to
//                 it in this parameter.  Note that under Windows 16 this
//                 is a huge pointer, meaning it can span segments, and
//                 access potentially 16 Mbytes of memory.
//
//  user_buffer_size : If you are passing a pointer to your own buffer,
//                     you need to indicate how large it is here.  Under
//                     Windows this is a DWORD instead of a size_t.
//
// RETURNS
//
//  Nothing, it is a constructor.
//
// DESCRIPTION
//
//  This constructor calls the base class constructor in an initializer
//  list, which takes care of most of the dirty work right away.  After that
//  is done, all the constructor has to do is initialize a few data members.
//  That should be self-explanatory.  Remember that if the user doesn't
//  supply a buffer, we are going to allocate it for her, but not until
//  there is actually a demand for memory.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//
ALMemory::ALMemory(uint8_t *user_buffer /* = 0 */,
                   int user_buffer_size /* = 0 */)
    : ALStorage(MEMORY_BLOCK_BYTES / 2) {
  if (user_buffer != 0) {
    mpcUserBuffer = user_buffer;
    mfUserOwnsBuffer = 1;
    muUserBufferSize = user_buffer_size;
  } else {
    mfUserOwnsBuffer = 0;
    mpcUserBuffer = 0;
    muUserBufferSize = 0;
  }
}

//
// ALMemory::~ALMemory()
//
// ARGUMENTS:
//
//  None, you don't get any for a destructor.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  The destructor has just one thing it has to do before this object
//  goes away.  If the buffer that it has been using all along doesn't
//  belong to the user, then it is the class's responsibility to get
//  rid of it.  We do so here, using one of two methods, depending on
//  whether we are under MS-DOS or Windows.
//
//  Note also that we check the GoodTag() function when in Debug mode.
//  That will help catch really bad mistakes, such as trying to delete
//  an object that is not even an ALMemory object, maybe a beer can.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

ALMemory::~ALMemory() {
  if (!mfUserOwnsBuffer) {
    if (mpcUserBuffer) {
      free(mpcUserBuffer);
      mpcUserBuffer = 0;
    }
  }
}

//
// int ALMemory::LoadBuffer( long address )
//
// ARGUMENTS:
//
//  address  : An offset that we need to load data from.
//
// RETURNS
//
//  AL_SEEK_ERROR if we try to read past the end of file.  AL_END_OF_FILE
//  if we just run out of data.  Otherwise an int indicating how many bytes
//  are now in the buffer.
//
// DESCRIPTION
//
//  This is a virtual function the ALStorage functions rely on when reading
//  data.  Anytime someone tries to do a ReadChar() or ReadBuffer(), and
//  it turns out that the I/O buffer has been exhausted, this function
//  is called.
//
//  The simple job of this function is to read as many bytes as possible out
//  of the giant memory block allocated for the ALMemory object, and stick
//  it into the I/O buffer, which caches it for calls to ReadChar()
//  and friends.
//
//  This works fine unless you try to go past the end of the buffer,
//  since there is nothing there we flag that as an error.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A   : First release
//
//   August 12, 1994 1.0B : When I failed in LoadBuffer because I was out
//                          of data, I was leaving muReadIndex at its old
//                          value.  A subsequent call to ReadBuffer()
//                          would then cause a GPF under Windows, because
//                          muValidData was 0 and muReadIndex was > 0.

int ALMemory::LoadBuffer(long address) {
  if (mStatus < AL_SUCCESS)
    return mStatus;
  if (mlFilePointer != address) {
    if (mlFilePointer > (long)muUserBufferSize)
      return mStatus.SetError(AL_SEEK_ERROR, "Attempt to read past end of the "
                                             "buffer in ALMemory");
  }
  long load = muUserBufferSize - address;
  if (load > (long)muBufferSize)
    muBufferValidData = muBufferSize;
  else
    muBufferValidData = (size_t)load;
  if (muBufferValidData <= 0) {
    muReadIndex = 0;
    return AL_END_OF_FILE;
  }
  memcpy(mpcBuffer, mpcUserBuffer + (size_t)address, muBufferValidData);
  muReadIndex = 0; // Reading can resume at this location
  mlFilePointer += muBufferValidData;
  return muBufferValidData;
}

//
// int ALMemory::Delete()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  Always returns AL_SUCCESS.
//
// DESCRIPTION
//
//  This function is supposed to delete the underlying physical object.
//  This makes a lot of sense with files, because you are essentially
//  emulating the MS-DOS command line DEL function.  With memory
//  objects things aren't quite as clear.  So we destroy the buffer,
//  and that's that.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

int ALMemory::Delete() {
  if (!mfUserOwnsBuffer) {
    free(mpcUserBuffer);
    mpcUserBuffer = 0;
  }
  return AL_SUCCESS;
}

//
// int ALMemory::Seek( long address )
//
// ARGUMENTS:
//
//  address  :  The address in the memory object to go to.  The read and
//              write pointers will now be repositioned to this point.
//
// RETURNS
//
//  AL_SEEK_ERROR if we can't get to that point in the buffer.  Otherwise
//  AL_SUCCESS.
//
// DESCRIPTION
//
//  This function acts just like the seek() function in the C runtime
//  library.  It flushes the current I/O buffers, and then moves the file
//  read and write pointers to a new spot, specified by the address.  if
//  there is no memory there, you will get an error.  Note that this
//  makes it not quite like the C run time library, since it can create
//  new space with a seek().  But I don't think we need that ability yet.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

int ALMemory::Seek(long address) {
  FlushBuffer();
  if (mStatus < 0)
    return mStatus;

  if (mlFilePointer != address) {
    if (address > muUserBufferSize)
      return mStatus.SetError(AL_SEEK_ERROR, "Attempt to seek past end of the "
                                             "buffer in ALMemory");
  }
  mlFilePointer = address;
  return AL_SUCCESS;
}

//
// int ALMemory::GrowUserBuffer( long minimum_new_size )
//
// ARGUMENTS:
//
//  minimum_new_size :    The absolute minimum new size you need the buffer
//                        to grow to.  This amount is usually determined by
//                        a pending I/O request.  For example, if the current
//                        size of the buffer is 1000, and you have a 1 byte
//                        data block to write at 1000, the minimum new size
//                        will be 1001.
//
// RETURNS
//
//  AL_CANT_ALLOCATE_MEMORY, if we just can't get it.  AL_SUCCESS if we can.
//
// DESCRIPTION
//
//  When you are trying to write to the memory object, and you have hit
//  the end of the currently allocated area, it would seem like a good
//  time to allocate more.  When that situation occurs, this function is
//  called.  If the user owns the buffer, we don't have the option of asking
//  the O/S or RTL for more memory, because we don't even know if the user
//  memory is on the heap or what.  But if we own the memory we know how
//  to ask for more.
//
//  The strategy for asking for more memory is pretty simple.  Normally,
//  we ask for another 16K.  If that fails, we fall back to asking for
//  just enough memory to cover our current I/O request.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

int ALMemory::GrowUserBuffer(long minimum_new_size) {
  if (mStatus < AL_SUCCESS)
    return mStatus;
  if (mfUserOwnsBuffer)
    return mStatus.SetError(AL_CANT_ALLOCATE_MEMORY,
                            "Attempt to write past the end of a "
                            "user owned buffer for ALMemory");
  if (minimum_new_size >= 65535L)
    return mStatus.SetError(AL_CANT_ALLOCATE_MEMORY,
                            "Attempt to allocate a huge buffer "
                            "of %ld bytes for ALMemory",
                            minimum_new_size);
  long trial_size = muUserBufferSize + MEMORY_BLOCK_BYTES;
  if (trial_size >= 65000U)
    trial_size = 65000U;
  if (trial_size >= minimum_new_size) {
    uint8_t *new_buf = (uint8_t *)realloc(mpcUserBuffer, (size_t)trial_size);
    if (new_buf) {
      mpcUserBuffer = new_buf;
      muUserBufferSize = (size_t)trial_size;
      return AL_SUCCESS;
    }
  }
  uint8_t *new_buf =
      (uint8_t *)realloc(mpcUserBuffer, (size_t)minimum_new_size);
  if (new_buf) {
    mpcUserBuffer = new_buf;
    muUserBufferSize = (size_t)trial_size;
    return AL_SUCCESS;
  }
  return mStatus.SetError(AL_CANT_ALLOCATE_MEMORY,
                          "Allocation failure when attempting to "
                          "allocate a buffer "
                          "of %ld bytes for ALMemory",
                          minimum_new_size);
}

//
// int ALMemory::FlushBuffer()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  AL_CANT_ALLOCATE_MEMORY, if we run out.  Otherwise, AL_SUCCESS.
//
// DESCRIPTION
//
//  This routine is called when the I/O buffer is filled up. It means
//  you have filled up the cache with what is usually 4K bytes of data.
//  This routine is also called if you have hot data in the I/O buffer
//  and you decide to do a seek(), or a read().
//
//  All we have to do here is take the hot data in the I/O buffer and
//  write it out to our massive memory object.  The big complication is
//  that sometimes the memory object isn't big enough, so while we are
//  all busy trying to do this, we have to ask for more data at the
//  same time.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Slight mod to make a compiler happy, syntactic
//                          change only.
//
int ALMemory::FlushBuffer() {
  if (mStatus < 0)
    return mStatus;
  //
  // If the write index is 0, we can skip all this stuff, because there
  // is nothing in the buffer to flush out.
  //
  if (muWriteIndex != 0) {
    if ((long)(muWriteIndex + mlFilePointer) > (long)muUserBufferSize)
      if (GrowUserBuffer(muWriteIndex + mlFilePointer) < 0)
        return mStatus;
    memcpy(mpcUserBuffer + (size_t)mlFilePointer, mpcBuffer, muWriteIndex);
    mlFilePointer += muWriteIndex;
    muWriteIndex = 0;
    if (mlSize < mlFilePointer)
      mlSize = mlFilePointer;
  }
  muReadIndex = 0;
  muBufferValidData = 0;
  return AL_SUCCESS;
}

//
// int ALMemory::Close()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  AL_SUCCESS, or various error codes that filter on down from other
//  routines.
//
// DESCRIPTION
//
//  Close() is supposed to do the same thing as fclose() in the run
//  time library.  The most important thing we are concerned about is
//  that the I/O buffer gets freed up by the base class, so this suddenly
//  might not be a giant heavyweight object any more.
//
//  After freeing things up in the base class, we check to see if
//  we have allocated more space than we really need.  If so, we do
//  a realloc() of some sort to give space back to the O/S.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//
//   July 6, 1994  1.0B  : Michael Meadows pointed out that I was calling
//                         realloc() whether the user owned the buffer or
//                         not.  This could be very bad.
//

int ALMemory::Close() {
  if (mpcBuffer == 0)
    return mStatus;
  FlushBuffer();
  ALStorage::Close();
  //
  // If we aren't using all our space, give back the extra.
  //
  if (!mfUserOwnsBuffer && mlSize < (long)muUserBufferSize) {
    if (mlSize == 0) {
      free(mpcUserBuffer);
      mpcUserBuffer = NULL;
      muUserBufferSize = 0;
    } else {
      uint8_t *new_buf = (uint8_t *)realloc(mpcUserBuffer, (size_t)mlSize);
      if (new_buf)
        mpcUserBuffer = new_buf;
      muUserBufferSize = (size_t)mlSize;
    }
  }
  return mStatus;
}

//
// int ALMemory::Create()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  AL_SUCCESS, AL_CANT_ALLOCATE_MEMORY, or various error codes that
//  filter on down from other routines.
//
// DESCRIPTION
//
//  This is like creating a new file.  If there isn't a memory buffer
//  already assigned to this object, we create one, with an initial
//  allocation of 16Kbytes.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//
//   July 7, 1994  1.0A  : Fixed a memory leak that could be created by
//                         calling Create() for a memory object that had
//                         already allocated some space.

int ALMemory::Create() {
  ALStorage::Create();
  if (mStatus < AL_SUCCESS)
    return mStatus;
  if (mfUserOwnsBuffer)
    return AL_SUCCESS; // If the user supplied the buffer, we take what's
                       // available
  if (mpcUserBuffer)
    return AL_SUCCESS; // If a buffer was already created somewhere down the
                       // line, we won't do it again.
  mpcUserBuffer = (uint8_t *)calloc(MEMORY_BLOCK_BYTES, sizeof(uint8_t));
  muUserBufferSize = MEMORY_BLOCK_BYTES;
  if (mpcUserBuffer == 0)
    return mStatus.SetError(AL_CANT_ALLOCATE_MEMORY,
                            "Allocation failure when attempting to "
                            "create a buffer "
                            "of %ld bytes for ALMemory "
                            "in Create()",
                            MEMORY_BLOCK_BYTES);
  return AL_SUCCESS;
}

//
// int ALMemory::Open()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  AL_SUCCESS, AL_CANT_OPEN_FILE, or various error codes that
//  filter on down from other routines.
//
// DESCRIPTION
//
//  This is like opening an existing file.  Since there is supposed to be
//  an existing memory buffer already, we gripe if we can't find one.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

int ALMemory::Open() {
  ALStorage::Open();
  if (mStatus < AL_SUCCESS)
    return mStatus;
  if (mpcUserBuffer == 0)
    return mStatus.SetError(AL_CANT_OPEN_FILE, "Attempt to open ALMemory "
                                               "with no buffer allocated");
  else
    mlSize = (long)muUserBufferSize;
  return AL_SUCCESS;
}
