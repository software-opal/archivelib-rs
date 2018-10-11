
#include "all.hpp"

/*
 * inline int ALStorage::ReadChar()
 *
 * ARGUMENTS:
 *
 *  None.
 *
 * RETURNS
 *
 *  Either the next character available from the I/O buffer, or
 *  AL_END_OF_FILE.
 *
 * DESCRIPTION
 *
 *  This is an inline function that is able to quickly do buffered I/O.
 *  By utilizing an I/O buffer we can make this routine very fast, since
 *  it doesn't have to call a virtual function.  The virtual function
 *  only has to be called when LoadBuffer() gets called.
 *
 *  Different compilers have different abilities to make this code inline,
 *  so sometimes it needs to be tinkered with.  If you see anything in here
 *  that looks funny, that probably explains why.
 *
 * REVISION HISTORY
 *
 *   May 26, 1994  1.0A  : First release
 *
 */

inline int AL_PROTO ALStorage::ReadChar()
{
    int result;

    AL_ASSERT( muWriteIndex == 0, "ReadChar(): Attempt to read while in write mode" ); /*Can't read if I've done a write!*/
    result = muBufferValidData - muReadIndex;
    if ( result <= 0 )
        result = LoadBuffer( mlFilePointer );
    AL_ASSERT( mpcBuffer != 0, "ReadChar(): Attempt to read from closed file" );    /*Potential disaster*/
    if ( result < 0 )
        return result;
    else
        return mpcBuffer[ muReadIndex++ ] & 0xff;
}

/*
 * inline int ALStorage::WriteChar( int c )
 *
 * ARGUMENTS:
 *
 *  c  : The character that is going to be written.
 *
 * RETURNS
 *
 *  Either the character that we just wrote out, or an error < AL_SUCCESS.
 *
 * DESCRIPTION
 *
 *  This is an inline function that is able to quickly do buffered I/O.
 *  By utilizing an I/O buffer we can make this routine very fast, since
 *  it doesn't have to call a virtual function.  The virtual function
 *  only has to be called when FlushBuffer() gets called.
 *
 *  Different compilers have different abilities to make this code inline,
 *  so sometimes it needs to be tinkered with.  If you see anything in here
 *  that looks funny, that probably explains why.
 *
 * REVISION HISTORY
 *
 *   May 26, 1994  1.0A  : First release
 *
 */

inline int AL_PROTO ALStorage::WriteChar( int c )
{
    int result;

/*    assert( muReadIndex == 0 ); */ /* Can't write if I've done a read */
    AL_ASSERT( mpcBuffer != 0, "WriteChar(): Attempt to write to closed file" );   /* Disaster! */
    result = muBufferSize - muWriteIndex;
    if ( result <= 0 )
        result = FlushBuffer();
    if ( result < 0 )
        return mStatus;
    else
        return mpcBuffer[ muWriteIndex++ ] = (char) c;
}
