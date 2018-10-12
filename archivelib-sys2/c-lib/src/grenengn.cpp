#include "arclib.h"

#include "grenengn.h"
#include "_rc.hpp"
#include "_re.hpp"

//
// void * ALGreenleafEngine::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  :  The number of bytes needed to create a new object.
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
void AL_DLL_FAR *AL_PROTO ALGreenleafEngine::operator new(size_t size) {
  return ::new char[size];
}
#endif

//
// ALGreenleafEngine::
// ALGreenleafEngine( short int compression_level = AL_GREENLEAF_LEVEL_2,
//                    short int fail_uncompressible = 0 )
//
// ARGUMENTS:
//
//  compression_level   : This is one of the enumerated types found in ALDEFS.H,
//                        namely AL_GREENLEAF_LEVEL_0 through
//                        AL_GREENLEAF_LEVEL_4.  Level 4 gives the most
//                        compression, but takes up the most memory as well.
//
//  fail_uncompressible : This flag is used to indicate the disposition
//                        of an uncompressible file.  If this flag is set,
//                        the compression of an incompressible file will
//                        be interrupted, and the file will be recompressed
//                        using a straight copy.  Note that this requires
//                        a Seek() operation!
//
// RETURNS
//
//  Nothing, a constructor.
//
// DESCRIPTION
//
//  The constructor for the Greenleaf engine has a pretty simple life.  All
//  it has to do is call the base class constructor, then define a couple of
//  data members.  This is a lightweight object until the compression
//  or expansion routines are invoked, at which time the memory requirements
//  go through the roof.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

AL_PROTO ALGreenleafEngine::ALGreenleafEngine(
    enum ALGreenleafCompressionLevels compression_level,
    bool fail_uncompressible)
    : miCompressionLevel(compression_level) {
  miFailUncompressible = fail_uncompressible;
}

//
// ALGreenleafEngine::~ALGreenleafEngine()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  The destructor for objects of this class doesn't have to do
//  anything.  In debug mode, we at least check for the validity
//  of the object.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

AL_PROTO ALGreenleafEngine::~ALGreenleafEngine() {
  AL_ASSERT(GoodTag(), "~ALGreenleafEngine: attempt to delete invalid object");
}

//
// int ALGreenleafEngine::Compress( ALStorage &input,
//                                  ALStorage &output )
//
// ARGUMENTS:
//
//  input   :  A reference to the storage object that will be compressed.
//
//  output  :  A reference to the storage object that will receive the
//             compressed data.
//
// RETURNS
//
//
//  AL_SUCCESS in the event of a success, an error code < AL_SUCCESS
//  if a failure occurred.
//
// DESCRIPTION
//
//  This is the virtual function that is called to compress data.  The
//  This section of code is really just a front end to the real engine,
//  which is found in _RC.CPP.  The first thing we do here
//  is create an RCompress object, which allocates all of the
//  storage we need to perform the compression.  In a tight memory
//  situation, that may well fail, so we check its status before moving
//  on.  If it succeeded, we can call the low level compression function
//  to do the real work.
//
//  After the compress function returns, we have to check for errors on
//  any of the other objects involved in the compression, and return the
//  cumulative result.
//
//  If the miFailUncompressible option is set, there is always a possiblity
//  that the compressor will return an indication that the file could
//  not be compressed.  If this is the case, we change the compression
//  level in this to AL_GREENLEAF_COPY, then perform a binary copy of
//  the data.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Added proper support for the incompressible
//                          option.

int AL_PROTO ALGreenleafEngine::Compress(ALStorage AL_DLL_FAR &input,
                                         ALStorage AL_DLL_FAR &output) {
  int incompressible;

  ALOpenFiles files(input, output);

  long input_start = input.Tell();
  long output_start = output.Tell();
  RCompress rc(input, output, miCompressionLevel + 10, miFailUncompressible);

  if (rc.mStatus < 0)
    return mStatus = rc.mStatus;
  else
    incompressible = rc.Compress();
  if (rc.mStatus < 0)
    return mStatus = rc.mStatus;
  else if (input.mStatus < 0)
    return mStatus = input.mStatus;
  else if (output.mStatus < 0)
    return mStatus = output.mStatus;
  if (incompressible) {
    input.Seek(input_start);
    output.Seek(output_start);
    miCompressionLevel = AL_GREENLEAF_COPY;
    int c;
    for (;;) {
      c = input.ReadChar();
      if (c < 0)
        break;
      output.WriteChar(c);
    }
    if (input.mStatus < AL_SUCCESS)
      return mStatus = input.mStatus;
    if (output.mStatus < AL_SUCCESS)
      return mStatus = output.mStatus;
  }
  return mStatus;
}

//
// int ALGreenleafEngine::Decompress( ALStorage &input,
//                                    ALStorage &output,
//                                    long compressed_length )
//
// ARGUMENTS:
//
//  input             :  A reference to the storage object that will be
//                       expanded.
//
//  output            :  A reference to the storage object that will receive
//                       the expanded data.
//
//  compressed_length : A long value indicating how long the compressed
//                      object is.  This helps to tell the decompressor
//                      when to quit.
// RETURNS
//
//
//  AL_SUCCESS in the event of a success, an error code < AL_SUCCESS
//  if a failure occurred.
//
// DESCRIPTION
//
//  This is the virtual function that is called to expand a compressed
//  object. This section of code is really just a front end to the real
//  engine, which is found in _RE.CPP.  The first thing we do here
//  is create an RExpand object, which allocates all of the
//  storage we need to perform the decompression.  In a tight memory
//  situation, that may well fail, so we check its status before moving
//  on.  If it succeeded, we can call the low level expansion function
//  to do the real work.
//
//  After the expand function returns, we have to check for errors on
//  any of the other objects involved in the expansion, and return the
//  cumulative result.
//
//  This function now properly supports the incompressible option.  When
//  the type of compression is selectec, via a compression level of
//  AL_GREENLEAF_COPY, we just do a straight binary copy here, instead
//  of calling the actual compressor.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

int AL_PROTO ALGreenleafEngine::Decompress(ALStorage AL_DLL_FAR &input,
                                           ALStorage AL_DLL_FAR &output,
                                           long compressed_length) {
  ALOpenFiles files(input, output);

  if (miCompressionLevel == AL_GREENLEAF_COPY) {
    int c;
    for (; compressed_length; compressed_length--) {
      c = input.ReadChar();
      if (c < 0)
        break;
      output.WriteChar(c);
    }
  } else {
    RExpand re(input, output, compressed_length, miCompressionLevel + 10);

    if (re.mStatus < 0)
      return mStatus = re.mStatus;
    else
      re.Expand();
    if (re.mStatus < 0)
      return mStatus = re.mStatus;
  }
  if (input.mStatus < 0)
    return mStatus = input.mStatus;
  else if (output.mStatus < 0)
    return mStatus = output.mStatus;
  return mStatus;
}
