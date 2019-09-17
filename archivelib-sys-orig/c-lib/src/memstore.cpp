/*
Copyright 1990-2008 Light Infocon Tecnologia S/A

Este arquivo é parte do programa LightBase - Banco de Dados Textual Documental

O LightBase é um software livre; você pode redistribui-lo e/ou modifica-lo dentro
dos termos da Licença Pública Geral GNU como publicada pela Fundação do Software
Livre (FSF); na versão 2 da Licença.

Este programa é distribuído na esperança que possa ser útil, mas SEM NENHUMA
GARANTIA; sem uma garantia implícita de ADEQUAÇÃO a qualquer MERCADO ou APLICAÇÃO
EM PARTICULAR. Veja a Licença Pública Geral GNU para maiores detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU versao 2, sob o
título "LICENCA.txt", junto com este programa, se não, escreva para a Fundação do
Software Livre(FSF) Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/


#include "arclib.h"
#pragma hdrstop

#include "memstore.h"

#include <stdlib.h>  // might be using malloc()!

//
// void * ALMemory::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  :  The amount of storage that needs to be allocated for
//           this object.
//
// RETURNS
//
//  A pointer to the storage.
//
// DESCRIPTION
//
//  When using the DLL version of ArchiveLib, it is a good idea to
//  allocate the storage for objects from inside the DLL, since they
//  will be freed inside the DLL.  If we don't have the new operator
//  for a class, its storage will be allocated from the EXE before
//  the constructor code is called.  Then, when it is time to free
//  the storage, the delete operator will be called inside the DLL.
//  Not good, right?
//
//
//  Very important:  this new operator is called to allocate the
//  storage for the ALMemory object itself.  This has nothing to do
//  with the storage buffer that the memory object will be using
//  later on.  In other words, this new operator is responsible for
//  no more than a couple of dozen bytes, not potentially hundreds
//  of Kbytes.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

#if defined( AL_BUILDING_DLL )
void AL_DLL_FAR * AL_PROTO ALMemory::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif

// WINDOWS version:
//
// ALMemory::ALMemory( const char *buffer_name = "",
//                     char AL_HUGE *user_buffer = 0,
//                     DWORD user_buffer_size = 0,
//                     ALCase name_case = AL_MIXED )
//
// MS-DOS real mode version :
//
// ALMemory::ALMemory( const char *buffer_name = "",
//                     char *user_buffer = 0,
//                     int user_buffer_size = 0,
//                     ALCase name_case = AL_MIXED )
//
// ARGUMENTS:
//
//  buffer_name  : An arbitrary name assigned to the buffer.  Buffer
//                 names don't have to be unique, because buffers aren't
//                 named at the operating system level.  But if you are
//                 going to insert the storage object into an archive, the
//                 name needs to be unique so that you will be able to
//                 extract it properly.
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
//  name_case    : This decides whether you want the file name to be
//                 case sensitive when making comparisons.  MS-DOS
//                 file names are case-insensitive.  You can make memory
//                 buffers either mixed case, forced upper, or forced
//                 lower.  The default of mixed case means that comparisons
//                 will be case sensitive, which is fine.
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
#if defined( AL_WINDOWS_MEMORY )
//
// The Windows and MS-DOS constructors are nearly identical.
//
AL_PROTO ALMemory::ALMemory( const char AL_DLL_FAR *buffer_name /* = "" */,
                             char AL_HUGE *user_buffer /* = 0 */,
                             DWORD user_buffer_size /* = 0 */,
                             ALCase name_case /* = AL_MIXED */)
    : ALStorage( buffer_name, 4096, AL_MEMORY_OBJECT, name_case )
{
    if ( user_buffer != 0 ) {
        mpcUserBuffer = user_buffer;
        mfUserOwnsBuffer = 1;
        muUserBufferSize = user_buffer_size;
    } else {
        mfUserOwnsBuffer = 0;
        mpcUserBuffer = 0;
        muUserBufferSize = 0;
    }
    mhUserMemoryHandle = 0;
}

#else // #if defined( AL_WINDOWS_MEMORY )

AL_PROTO ALMemory::ALMemory( const char AL_DLL_FAR *buffer_name /* = "" */,
                             char AL_DLL_FAR *user_buffer /* = 0 */ ,
                             int user_buffer_size /* = 0 */,
                             ALCase name_case /* = AL_MIXED */ )
    : ALStorage( buffer_name, 4096, AL_MEMORY_OBJECT, name_case )
{
    if ( user_buffer != 0 ) {
        mpcUserBuffer = user_buffer;
        mfUserOwnsBuffer = 1;
        muUserBufferSize = user_buffer_size;
    } else {
        mfUserOwnsBuffer = 0;
        mpcUserBuffer = 0;
        muUserBufferSize = 0;
    }
}
#endif

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

AL_PROTO ALMemory::~ALMemory()
{
    AL_ASSERT( GoodTag(), "~ALMemory: attempting to delete invalid object" );
    if ( !mfUserOwnsBuffer ) {
        if ( mpcUserBuffer ) {
#if defined( AL_WINDOWS_MEMORY )
            GlobalUnlock( (HGLOBAL) mhUserMemoryHandle );
            GlobalFree( (HGLOBAL) mhUserMemoryHandle );
            mhUserMemoryHandle= 0;
#else
            free( mpcUserBuffer );
#endif
            mpcUserBuffer = 0;
        }
    }
    AL_ASSERT( GoodTag(), "~ALMemory: attempting to delete invalid object" );
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

int AL_PROTO ALMemory::LoadBuffer( long address )
{
    if ( mStatus < AL_SUCCESS )
        return mStatus;
    if ( mlFilePointer != address ) {
        if ( mlFilePointer > (long) muUserBufferSize )
            return mStatus.SetError( AL_SEEK_ERROR,
                                     "Attempt to read past end of the "
                                     "buffer in ALMemory %s",
                                     mName.GetName() );
    }
    long load = muUserBufferSize - address;
    if ( load > (long) muBufferSize )
        muBufferValidData = muBufferSize;
    else
        muBufferValidData = (size_t) load;
    if ( muBufferValidData <= 0 ) {
        muReadIndex = 0;
        return AL_END_OF_FILE;
    }
#if defined( AL_WINDOWS_MEMORY )
//
// Some problems passing huge arrays to memcpy, got to do it inline instead
// I think Microsoft says memcpy() will work with huge pointers as long
// as you don't try to use the inline optimizations, but I say why take
// chances...
//
// Another note: AL_HUGE is _huge for win16, but blank for win32.
//
    char AL_HUGE *temp = mpcUserBuffer + address;
    for ( unsigned i = 0 ; i < muBufferValidData ; i++ )
        mpcBuffer[ i ] = *temp++;
//    memcpy( mpcBuffer, mpcUserBuffer +          address, muBufferValidData );
#else
    memcpy( mpcBuffer, mpcUserBuffer + (size_t) address, muBufferValidData );
#endif
    if ( miUpdateCrcFlag )
        UpdateCrc( muBufferValidData );
    muReadIndex = 0; //Reading can resume at this location
    mlFilePointer += muBufferValidData;
    YieldTime();
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

int AL_PROTO ALMemory::Delete()
{
    if ( !mfUserOwnsBuffer ) {
#if defined( AL_WINDOWS_MEMORY )
        GlobalUnlock( (HGLOBAL) mhUserMemoryHandle );
        GlobalFree( (HGLOBAL) mhUserMemoryHandle );
        mhUserMemoryHandle= 0;
#else
        free( mpcUserBuffer );
#endif
        mpcUserBuffer = 0;
    }
    return AL_SUCCESS;
}

//
// int ALMemory::Rename( const char *new_name /* = 0 */,
//                       int /* delete_on_clash = 1 */ )
//
// ARGUMENTS:
//
//  new_name        : The new name of the buffer.
//
//  delete_on_clash : This argument makes sense with files.  What it says
//                    is that if you try to rename BOB.DAT to BOB.BAK,
//                    and it turns out that there is another BOB.BAK, should
//                    you delete the other one?  With memory buffers,
//                    there is no clash, cause the OS doesn't care about
//                    unique names.  So we ignore this parm.
//
// RETURNS
//
//  Always returns AL_SUCCESS.
//
// DESCRIPTION
//
//  This function is supposed to rename the underlying physical object.
//  But in the case of memory buffers, the underlying physical object
//  doesn't actually have a name, so this is really just a local rename.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

int AL_PROTO ALMemory::Rename( const char AL_DLL_FAR *new_name /* = 0 */,
                               int /* delete_on_clash = 1 */ )
{
    if ( new_name )
        mName = new_name;
    return AL_SUCCESS;
}

//
// int ALMemory::RenameToBackup( int delete_on_clash /* = 1 */ )
//
// ARGUMENTS:
//
//  delete_on_clash : This argument makes sense with files.  What it says
//                    is that if you try to rename BOB.DAT to BOB.BAK,
//                    and it turns out that there is another BOB.BAK, should
//                    you delete the other one?  With memory buffers,
//                    there is no clash, cause the OS doesn't care about
//                    unique names.  So it doesn't matter what value you
//                    pass to the Rename() function, it is going to be
//                    ignored.
//
// RETURNS
//
//  Always returns AL_SUCCESS.
//
// DESCRIPTION
//
//  This function renames an object.  But instead of making you sweat in
//  order to come up with a new name, it just uses the default name
//  that we use to assign a backup name.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

int AL_PROTO ALMemory::RenameToBackup( int delete_on_clash /* = 1 */ )
{
    mName.ChangeExtension();
    return Rename( 0, delete_on_clash );
}

//
// int ALMemory::UnRename( int /* delete_on_clash = 1 */)
//
// ARGUMENTS:
//
//  delete_on_clash : This argument makes sense with files.  What it says
//                    is that if you try to rename BOB.DAT to BOB.BAK,
//                    and it turns out that there is another BOB.BAK, should
//                    you delete the other one?  With memory buffers,
//                    there is no clash, cause the OS doesn't care about
//                    unique names.  So we just ignore it here.
//
// RETURNS
//
//  Always returns AL_SUCCESS.
//
// DESCRIPTION
//
//  If you decide you didn't really want to rename an object after all, you
//  can call this function to get the old name back!
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

int AL_PROTO ALMemory::UnRename( int /* delete_on_clash = 1 */)
{
    AL_ASSERT( mName.GetName() != 0, "UnRename: trying to rename with a null name" );
    AL_ASSERT( mName.GetOldName() != 0, "UnRename: trying to rename with a null old name" );
    AL_ASSERT( strlen( mName ) > 0, "UnRename: trying to rename with a zero length name"  );
    AL_ASSERT( strlen( mName.GetOldName() ) > 0, "UnRename: trying to rename with a zero length old name" );

    ALStorage::mName = mName.GetOldName();
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

int AL_PROTO ALMemory::Seek( long address )
{
    FlushBuffer();
    if ( mStatus < 0 )
        return mStatus;

    if ( mlFilePointer != address ) {
        if ( mlFilePointer > (long) muUserBufferSize )
            return mStatus.SetError( AL_SEEK_ERROR,
                                     "Attempt to read past end of the "
                                     "buffer in ALMemory %s",
                                     mName.GetName() );
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
//  just enough memory to cover our current I/O request.  Asking for
//  this memory is sufficiently different under real mode dos and protected
//  mode windows that we have two completely different routines, separated
//  only by #ifdefs.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

#if defined( AL_WINDOWS_MEMORY )
int AL_PROTO ALMemory::GrowUserBuffer( long minimum_new_size )
{
    if ( mStatus < AL_SUCCESS )
        return mStatus;
    if ( mfUserOwnsBuffer )
        return mStatus.SetError( AL_CANT_ALLOCATE_MEMORY,
                                   "Attempt to write past the end of a "
                                   "user owned buffer for ALMemory "
                                   "%s",
                                   mName.GetSafeName() );
    long trial_size = muUserBufferSize + 16384;
    GlobalUnlock( (HGLOBAL) mhUserMemoryHandle );
    HGLOBAL new_handle = GlobalReAlloc( (HGLOBAL) mhUserMemoryHandle, trial_size, GMEM_MOVEABLE );
    if ( new_handle == 0 ) {
        trial_size = minimum_new_size;
        new_handle = GlobalReAlloc( (HGLOBAL) mhUserMemoryHandle, trial_size, GMEM_MOVEABLE );
    }
    if ( new_handle == 0 ) {
        mpcUserBuffer = (char AL_HUGE *) GlobalLock( (HGLOBAL) mhUserMemoryHandle );
        return mStatus.SetError( AL_CANT_ALLOCATE_MEMORY,
                                   "Allocation failure when attempting to "
                                   "allocate a buffer "
                                   "of %ld bytes for ALMemory "
                                   "%s",
                                   minimum_new_size,
                                   mName.GetSafeName() );
    }
    mpcUserBuffer = (char AL_HUGE *) GlobalLock( new_handle );
    mhUserMemoryHandle = new_handle;
    muUserBufferSize = trial_size;
    return AL_SUCCESS;
}
#else // #ifdef AL_WINDOWS_MEMORY

int AL_PROTO ALMemory::GrowUserBuffer( long minimum_new_size )
{
    if ( mStatus < AL_SUCCESS )
        return mStatus;
    if ( mfUserOwnsBuffer )
        return mStatus.SetError( AL_CANT_ALLOCATE_MEMORY,
                                   "Attempt to write past the end of a "
                                   "user owned buffer for ALMemory "
                                   "%s",
                                   mName.GetSafeName() );
    if ( minimum_new_size >= 65535L )
        return mStatus.SetError( AL_CANT_ALLOCATE_MEMORY,
                                   "Attempt to allocate a huge buffer "
                                   "of %ld bytes for ALMemory "
                                   "%s",
                                   minimum_new_size,
                                   mName.GetSafeName() );
    long trial_size = muUserBufferSize + 16384;
    if ( trial_size >= 65000U )
        trial_size = 65000U;
    if ( trial_size >= minimum_new_size ) {
        char *new_buf = (char *) realloc( mpcUserBuffer, (size_t) trial_size );
        if ( new_buf ) {
            mpcUserBuffer = new_buf;
            muUserBufferSize = (size_t) trial_size;
            return AL_SUCCESS;
        }
    }
    char *new_buf = (char *) realloc( mpcUserBuffer, (size_t) minimum_new_size );
    if ( new_buf ) {
        mpcUserBuffer = new_buf;
        muUserBufferSize = (size_t) trial_size;
        return AL_SUCCESS;
    }
    return mStatus.SetError( AL_CANT_ALLOCATE_MEMORY,
                               "Allocation failure when attempting to "
                               "allocate a buffer "
                               "of %ld bytes for ALMemory "
                               "%s",
                               minimum_new_size,
                               mName.GetSafeName() );
}
#endif

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
int AL_PROTO ALMemory::FlushBuffer()
{
    if ( mStatus < 0 )
        return mStatus;
//
// If the write index is 0, we can skip all this stuff, because there
// is nothing in the buffer to flush out.
//
    if ( muWriteIndex != 0 ) {
        if ( miUpdateCrcFlag )
            UpdateCrc( muWriteIndex );
        if ( (long) ( muWriteIndex + mlFilePointer ) > (long) muUserBufferSize )
            if ( GrowUserBuffer( muWriteIndex + mlFilePointer ) < 0 )
                return mStatus;
#if defined( AL_WINDOWS_MEMORY )
//
// Can't use memcpy with huge pointers, at least not with the optimized
// versions.
//
        char AL_HUGE *temp = mpcUserBuffer + mlFilePointer;
        for ( unsigned int i = 0 ; i < muWriteIndex ; i++ )
            *temp++ = mpcBuffer[ i ];
//        memcpy( mpcUserBuffer +          mlFilePointer, mpcBuffer, muWriteIndex );
#else
        memcpy( mpcUserBuffer + (size_t) mlFilePointer, mpcBuffer, muWriteIndex );
#endif
        mlFilePointer += muWriteIndex;
        muWriteIndex = 0;
        if ( mlSize < mlFilePointer )
            mlSize = mlFilePointer;
    }
    muReadIndex = 0;
    muBufferValidData = 0;
    YieldTime();
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

int AL_PROTO ALMemory::Close()
{
    if ( mpcBuffer == 0 )
        return mStatus;
    FlushBuffer();
    ALStorage::Close();
//
// If we aren't using all our space, give back the extra.
//
    if ( !mfUserOwnsBuffer && mlSize < (long) muUserBufferSize ) {
#if defined( AL_WINDOWS_MEMORY )
        GlobalUnlock( (HGLOBAL) mhUserMemoryHandle );
        HGLOBAL new_handle = GlobalReAlloc( (HGLOBAL) mhUserMemoryHandle, mlSize, GMEM_MOVEABLE );
        if ( new_handle != 0 )
            mhUserMemoryHandle = new_handle;
        mpcUserBuffer = (char AL_HUGE *) GlobalLock( (HGLOBAL) mhUserMemoryHandle );
        muUserBufferSize = mlSize;
#else
        if (mlSize == 0) {
          // 2019 -- Added this if/else block to prevent double free.
          //         Passing `0` into realloc will free the memory, then return
          //         null. This won't clear the reference to the buffer meaning
          //         ALMemory's deconstructor tries to free it again.
          free(mpcUserBuffer);
          mpcUserBuffer = NULL;
        } else {
        char *new_buf = (char *) realloc( mpcUserBuffer, (size_t) mlSize );
        if ( new_buf )
            mpcUserBuffer = new_buf;
        }
        muUserBufferSize = (size_t) mlSize;
#endif
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

int AL_PROTO ALMemory::Create()
{
    ALStorage::Create();
    if ( mStatus < AL_SUCCESS )
        return mStatus;
    if ( (char *) mName == 0 || strlen( mName ) == 0 )
        mName = "AL.TMP";
    if ( mfUserOwnsBuffer )
        return AL_SUCCESS;  //If the user supplied the buffer, we take what's available
    if ( mpcUserBuffer )
        return AL_SUCCESS; //If a buffer was already created somewhere down the
                           //line, we won't do it again.
#if defined( AL_WINDOWS_MEMORY )
    mhUserMemoryHandle = GlobalAlloc( GMEM_MOVEABLE, 16384 );
    if ( mhUserMemoryHandle ) {
        mpcUserBuffer = (char AL_HUGE *) GlobalLock( (HGLOBAL) mhUserMemoryHandle );
        muUserBufferSize = 16384;
    } else {
        mpcUserBuffer = 0;
        return mStatus.SetError( AL_CANT_ALLOCATE_MEMORY,
                                 "Allocation failure when attempting to "
                                 "create a buffer "
                                 "of %ld bytes for ALMemory "
                                 "%s in Create()",
                                 16384,
                                 mName.GetSafeName() );
    }
#else
    mpcUserBuffer = (char *) calloc( 16384 , sizeof(char));
    muUserBufferSize = 16384;
    if ( mpcUserBuffer == 0 )
        return mStatus.SetError( AL_CANT_ALLOCATE_MEMORY,
                                 "Allocation failure when attempting to "
                                 "create a buffer "
                                 "of %ld bytes for ALMemory "
                                 "%s in Create()",
                                 16384,
                                 mName.GetSafeName() );
#endif
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

int AL_PROTO ALMemory::Open()
{
    ALStorage::Open();
    if ( mStatus < AL_SUCCESS )
        return mStatus;
    if ( mpcUserBuffer == 0 )
        return mStatus.SetError( AL_CANT_OPEN_FILE,
                                   "Attempt to open ALMemory %s "
                                   "with no buffer allocated",
                                   mName.GetSafeName() );
    else
        mlSize = (long) muUserBufferSize;
    return AL_SUCCESS;
}


