#include "arclib.h"

#include "grenengn.h"
#include "_rc.hpp"
#include "_re.hpp"

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

SimpleStatus al_compress(ALGreenleafCompressionLevels compression_level,
                         ALStorage AL_DLL_FAR &input,
                         ALStorage AL_DLL_FAR &output) {
  bool fail_uncompressible = false;

  ALOpenFiles files(input, output);

  RCompress rc(input, output, compression_level + 10, fail_uncompressible);

  if (rc.mStatus < 0)
    return rc.mStatus.copyToSimple();

  rc.Compress();
  if (rc.mStatus < 0)
    return rc.mStatus.copyToSimple();
  else if (input.mStatus < 0)
    return input.mStatus.copyToSimple();
  else if (output.mStatus < 0)
    return output.mStatus.copyToSimple();
  return SIMPLE_STATUS_SUCCESS();
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

SimpleStatus al_decompress(ALGreenleafCompressionLevels compression_level,
                           ALStorage AL_DLL_FAR &input,
                           ALStorage AL_DLL_FAR &output,
                           long compressed_length) {
  ALOpenFiles files(input, output);

  RExpand re(input, output, compressed_length, compression_level + 10);

  if (re.mStatus < 0)
    return re.mStatus.copyToSimple();
  re.Expand();
  if (re.mStatus < 0)
    return re.mStatus.copyToSimple();
  else if (input.mStatus < 0)
    return input.mStatus.copyToSimple();
  else if (output.mStatus < 0)
    return output.mStatus.copyToSimple();
  return SIMPLE_STATUS_SUCCESS();
}
