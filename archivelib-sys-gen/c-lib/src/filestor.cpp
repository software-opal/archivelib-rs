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

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

//
// Sun lumps all this stuff into unistd.h.  I suspect nobody else will,
// so this might be better off as #ifdef AL_SUN4.  We will see when
// we tackle the next UNIX system.
//
#ifdef AL_UNIX
  #include <unistd.h>
  #include <utime.h>
#else
  #include <io.h>
  #include <dos.h>
#endif

#include "filestor.h"

//
// void * ALFile::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  :  The number of bytes needed to create a new ALFile object.
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

#if defined( AL_BUILDING_DLL )
void AL_DLL_FAR * AL_PROTO ALFile::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif

//
// ALFile::ALFile( const char *file_name = "",
//                 int buffer_size = 4096,
//                 ALCase name_case  = AL_LOWER)
//
// ARGUMENTS:
//
//  file_name    : The initial file name of the ALFile object you are 
//                 creating.  By default, this is a blank string, which
//                 will get converted to a temporary name before opening
//                 the actual disk file.
//
//  buffer_size  : The size of the object's I/O buffer.  The default of 4096
//                 should give very good performance.
//
//  name_case    : This parameter determines whether the file names will
//                 always be converted to upper case, lower case, or left
//                 in mixed case.  Under MS-DOS, you shouldn't use mixed
//                 case, because the O/S file naming convention is case
//                 insensitive.  ArchiveLib will think "TEMP.BAK" and 
//                 "temp.bak" are different, when they really aren't.
//
// RETURNS
//
//  Nothing, this is a constructor.
//
// DESCRIPTION
//
//  This constructor is used to create a new ALFile object, which will
//  usually be treated as an ALStorage object by ArchiveLib functions.
//  It is important to note that not much happens during construction of
//  this object, the real activity happens after you call the Open()
//  function.  Just creating this object *does not* create a file on disk!
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

AL_PROTO ALFile::ALFile( const char AL_DLL_FAR *file_name /* = "" */,
                         int buffer_size /* = 4096 */,
                         ALCase name_case /* = AL_LOWER */)
// Note: if non-msdos, change case parameter to AL_MIXED
    : ALStorage( file_name, buffer_size, AL_FILE_OBJECT, name_case ) {
    miHandle = -1;
}

//
// ALFile::~ALFile()
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
//  The destructor for an ALFile object doesn't have to do much work.
//  The base class destructor will take care of freeing the I/O buffer,
//  and any other loose ends.  All we have to do here is make sure
//  the file gets closed, and that its buffers get flushed to the disk
//  file.
//
//  Note that in debug mode, the destructor also checks this for the 
//  correct class type.  This helps flag erroneous or duplicated 
//  destructor calls.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

AL_PROTO ALFile::~ALFile()
{
    AL_ASSERT( GoodTag(), "~ALFile: attempting to delete invalid object" );
    if ( miHandle != -1 )
        Close();
}

//
// int ALFile::LoadBuffer( long address )
//
// ARGUMENTS:
//
//  address  : The long offset into the physical storage object.  A
//             seek/read combination will be executed at this location,
//             so that subsequent calls to read data will start at
//             the given address.
//
// RETURNS
//
//  AL_SUCCESS, AL_SEEK_ERROR, AL_END_OF_FILE, or possibly another 
//  error code < AL_SUCCESS.
//
// DESCRIPTION
//
//  This function is used in the library whenever a byte needs to be read
//  that isn't present in the current I/O buffer.  It has to use
//  the C RTL function lseek() to go to the correct position in the library.
//  If that works, it uses the C RTL function read() to read in an I/O
//  buffer full of data.
//
//  After that operation is performed, muReadIndex is set to 0, indicating
//  that the next read from the I/O buffer will take place at location 0.
//  mlFilePointer is set to address plus the number of bytes read, so
//  we know where the next read from the file will take place.  And 
//  muBufferValidData is set to the count of bytes read in from this
//  location.  That lets us know how far we can read in the I/O buffer
//  before we run out of space.
//
//  Note that if CRC checking has been turned on, we will update the
//  current working CRC value with the new data that has been read
//  in from the buffer.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : Added a very minor #ifdef for UNIX.
//

int AL_PROTO ALFile::LoadBuffer( long address )
{
    if ( mStatus < AL_SUCCESS )
        return mStatus;
    if ( mlFilePointer != address ) {
        long result = lseek( miHandle, address, SEEK_SET );
        if ( result == -1L )
            return mStatus.SetError( AL_SEEK_ERROR,
                                       "Seek failure on %s.  errno = %d",
                                       mName.GetName(),
                                       errno );
    }
#ifdef AL_UNIX
    int result = read( miHandle, (char *) mpcBuffer, muBufferSize );
#else
    int result = read( miHandle, mpcBuffer, muBufferSize );
#endif
    if ( result == 0 )
        return AL_END_OF_FILE;
    if ( result < 0 )
        return mStatus.SetError( AL_READ_ERROR,
                                   "Read failure on %s.  errno = %d",
                                   mName.GetName(),
                                   errno );
    if ( miUpdateCrcFlag )
        UpdateCrc( result );
    muReadIndex = 0; //Reading can resume at this location in the I/O buffer
    mlFilePointer += result;
    muBufferValidData = result;
    YieldTime();
    return result;
}

//
// int ALFile::FlushBuffer()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  An integer status value, AL_SUCCESS, AL_WRITE_ERROR, or possibly some
//  status code < AL_SUCCESS.
//
// DESCRIPTION
//
//  This function is the counterpart to LoadBuffer(). It gets called
//  when a write operation is poised to overflow the I/O buffer.  This
//  means we need to flush the buffer out to disk, then reset some
//  data members.
//
//  Unlike LoadBuffer(), this function doesn't have an address argument,
//  so we don't have to perform a seek().  Instead, the data will be
//  written out to the current position of the file pointer.  If the
//  write is successful, muWriteIndex is set to 0, indicating that the
//  next write to the I/O buffer can go to position 0.  mlFilePointer is
//  incremented by the length of the write, so we know where the next read
//  or write will occur.  Finally, muBufferValidData is set to 0, indicating
//  that there is no data in the I/O buffer that has been written, and
//  there is no data that can be read.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//
//   July 7, 1994  1.0B  : Added a very minor #ifdef for UNIX.
//

int AL_PROTO ALFile::FlushBuffer()
{
    if ( mStatus < 0 )
        return mStatus;
    if ( muWriteIndex != 0 ) {
        if ( miUpdateCrcFlag )
            UpdateCrc( muWriteIndex );
#ifdef AL_UNIX
        int result = write( miHandle, (char *) mpcBuffer, muWriteIndex );
#else
        int result = write( miHandle, mpcBuffer, muWriteIndex );
#endif
        muWriteIndex = 0;
        if ( result == -1L )
            return mStatus.SetError( AL_WRITE_ERROR,
                                     "Write failure on %s.  errno = %d",
                                     mName.GetName(),
                                     errno );
        mlFilePointer += result;
    }
    muReadIndex = 0;
    muBufferValidData = 0;
    YieldTime();
    return AL_SUCCESS;
}

//
// int ALFile::Seek( long address )
//
// ARGUMENTS:
//
//  address  :  The address in the physical disk to seek to.
//
// RETURNS
//
//  AL_SUCCESS, AL_SEEK_ERROR, or possibly some other status code < AL_SUCCESS.
//
// DESCRIPTION
//
//  This is a function the user can call to position the read/write pointer
//  to a new location in the disk file.  If there is any data that has been
//  written to the I/O buffer, it gets flushed first.  After that, we do
//  a seek, and update mlFilePointer to reflect the new reality.  Note that
//  the other important data members will have been updated by FlushBuffer().
//
//  And no, this guy doesn't do a LoadBuffer().  Which is fine if you are
//  going to do a bunch of writes afterwards.  If you are going to read data
//  immediately after Seek(), you would have been better of calling
//  LoadBuffer().
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

int AL_PROTO ALFile::Seek( long address )
{
    FlushBuffer();
    if ( mStatus < 0 )
        return mStatus;
    if ( mlFilePointer != address ) {
        long result = lseek( miHandle, address, SEEK_SET );
        if ( result == -1L )
            return mStatus.SetError( AL_SEEK_ERROR,
                                     "Seek failure on %s.  errno = %d",
                                     mName.GetName(),
                                     errno );
    }
    mlFilePointer = address;
    return AL_SUCCESS;
}

//
// int ALFile::Open()
//
// ARGUMENTS:
//
//  None. 
//
// RETURNS
//
//  AL_CANT_OPEN_FILE, AL_SUCCESS, or possibly some other error code
//  < AL_SUCCESS.
//
// DESCRIPTION
//
//  This is an important function, because it converts the ALFile
//  object from a dinky little unimportant object, to a big massive
//  thing that is ready to do serious work.
//
//  The first thing we do here is see if we can open the file.  We try
//  to open it with READ/WRITE privileges, but we give up and drop back
//  to READ only if that doesn't work out.
//
//  We then call the base class ALStorage::Open() who takes care of
//  allocating buffers and initializing data members.
//
//  Finally, we have to get the protection attributes and time date
//  stamps for the file.  After those are stored off, the file is ready
//  for abuse.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : Several changes to this routine for UNIX.  A couple
//                         of them are very minor #ifdefs to handle different
//                         values for constants.  But there is still one giant
//                         chunk missing.  If you look down a few lines, you
//                         will see that the code to get the file attributes
//                         has not been filled in yet.
//
int AL_PROTO ALFile::Open()
{
    if ( mStatus < AL_SUCCESS )
        return mStatus;
#ifdef AL_UNIX
    miHandle = open( mName, O_RDWR );
#else
    miHandle = open( mName, O_BINARY | O_RDWR );
#endif

    if ( miHandle == -1 && errno == EACCES )
#ifdef AL_UNIX
        miHandle = open( mName, O_RDONLY );
#else
        miHandle = open( mName, O_BINARY | O_RDONLY );
#endif
    if ( miHandle == -1 )
        return mStatus.SetError( AL_CANT_OPEN_FILE,
                                 "File open failure.  Open of %s returned "
                                 "errno = %d",
                                 mName.GetName(),
                                 errno );

    ALStorage::Open();
    struct stat buf;
    struct tm *tblock;
    if ( stat( mName, &buf ) == -1 )
        return mStatus.SetError( AL_CANT_OPEN_FILE,
                                 "Couldn't get time, date, and size "
                                 "information for %s.  errno = %d.",
                                 mName.GetName(),
                                 errno );
    mlSize = buf.st_size;
    tblock = localtime( &buf.st_mtime );
    mTimeDate.SetTimeDate( tblock );
#if defined( AL_UNIX )
    mAttributes.SetFromUnixAttributes( buf.st_mode );
#elif defined( AL_WIN32S )
    DWORD attributes = GetFileAttributes( mName );
    if ( attributes == 0xFFFFFFFF )
        return mStatus.SetError( AL_CANT_OPEN_FILE,
                                 "Couldn't get Win32 file attribute "
                                 "information for %s.  GetLastError = %d.",
                                 mName.GetName(),
                                 GetLastError() );
    mAttributes.SetFromWin32Attributes( attributes );
#else
    unsigned attributes;
    if ( _dos_getfileattr( mName, &attributes ) != 0 )
        return mStatus.SetError( AL_CANT_OPEN_FILE,
                                 "Couldn't get DOS attribute "
                                 "information for %s.  errno = %d.",
                                 mName.GetName(),
                                 errno );
    mAttributes.SetFromDosAttributes( attributes );
#endif
    return AL_SUCCESS;
}

//
// void ALFile::MakeTempName( int i )
//
// ARGUMENTS:
//
//  i    : A numeric argument that can somehow be incorporated into 
//         the temporary file name.  Create() will call this function
//         while incrementing this number in an attempt to find a unique 
//         name.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This function is called by Create() and other functions when they
//  decide they need to cook up a temporary file name.  The single parameter
//  i is incremented by the calling program so that repeated calls should
//  eventually produce a unique name.
//
//  All this function does to create that unique name is perform a sprintf()
//  into a buffer using a simple template.  The result is copied into the
//  mName member, and is ready to be tried out.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

void AL_PROTO ALFile::MakeTempName( int i )
{
    char name[ 21 ];

    sprintf( name, "~al~%03d.tmp", i );
    mName = name;
}

//
// int ALFile::Create()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  AL_SUCCESS, AL_CANT_OPEN_FILE, or possibly some other error code
//  < AL_SUCCESS.
//
// DESCRIPTION
//
//  This function is used to create a new file storage object.  Since
//  we are creating a new object, we must be able to open it with read
//  access.  We will also rudely obliterate any existing file.
//
//  The first thing we do here is call the base class Create() function.
//  It takes care of setting up the I/O buffer and initializing the
//  data members used to support the class.
//
//  Next, function checks to see if we have a valid filename.  If 
//  not, a  search is made for a valid temporary file name.  In either
//  case, the file is then opened with R/W access, in O_CREAT
//  mode, obliterating any existing file with the same name.
//
//  Once the file is open, everything is ready to go, and you can write
//  to the file at will.  Don't expect much to happen if you try to
//  read, however.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : Added a very minor #ifdef to handle UNIX.
//
int AL_PROTO ALFile::Create()
{
    ALStorage::Create();
    if ( mStatus < AL_SUCCESS )
        return mStatus;
    if ( (char *) mName == 0 || strlen( mName ) == 0 ) {
        for ( int i = 0 ; i < 999 ; i++ ) {
            MakeTempName( i );
            miHandle = open( mName,
#ifdef AL_UNIX
                             O_CREAT | O_RDWR | O_EXCL,
#else
                             O_CREAT | O_RDWR | O_BINARY | O_EXCL,
#endif
                             S_IREAD | S_IWRITE );
            if ( miHandle != -1 )
                break;
            else if ( errno != EEXIST && errno != EACCES ) {
                mStatus.SetError( AL_CANT_OPEN_FILE,
                                  "Temporary file creation failure.  "
                                  "Open of %s returned errno = %d",
                                  mName.GetName(),
                                  errno );
                mName = "";
                return AL_CANT_OPEN_FILE;
            }
        }
        if ( i == 1000 ) {
            mStatus.SetError( AL_CANT_OPEN_FILE,
                              "Temporary file creation failure.  "
                              "Tried 1000 times to open %s "
                              "(or a name something like that).",
                              mName.GetName() );
            mName = "";
            return AL_CANT_OPEN_FILE;
        }
    } else {
        miHandle = open( mName,
#ifdef AL_UNIX
                         O_CREAT | O_RDWR | O_TRUNC,
#else
                         O_CREAT | O_RDWR | O_BINARY | O_TRUNC,
#endif
                         S_IREAD | S_IWRITE );
    }
    if ( miHandle == -1 )
        return mStatus.SetError( AL_CANT_OPEN_FILE,
                                 "File creation failure.  "
                                 "Open of %s returned errno = %d",
                                 mName.GetName(),
                                 errno );
    return AL_SUCCESS;
}

//
// int ALFile::Close()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  Any status code, hopefully AL_SUCCESS.
//
// DESCRIPTION
//
//  This function is called when you are done accessing a file, and want 
//  to free up its resources.  The first thing it does is check to see
//  if the file was ever actually opened.  If it was, we flush the output
//  buffer, then calculate and store the file length.  Finally, we close
//  the disk file, then call the base class Close() function to clean up
//  the buffers and deal with other miscellaneous dirty work.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : Sun UNIX doesn't have filelength, so I had to
//                         improvise.  We could probably use the same
//                         code under DOS and Windows, and get rid of an
//                         #ifdef.
//

int AL_PROTO ALFile::Close()
{
    if ( miHandle == -1 )
        return mStatus;
    FlushBuffer();
#if defined( AL_UNIX )
    mlSize = lseek( miHandle, 0, SEEK_END );
#else
    mlSize = filelength( miHandle );
#endif
    if ( miCreated && mTimeDate.Valid() ) {
#if defined( AL_UNIX )
    struct utimbuf times;
    times.actime = mTimeDate.GetUnixTime();
    times.modtime = mTimeDate.GetUnixTime();
    utime( (char *) mName, &times );
#elif defined( AL_WIN32S )
// Can you do this under NT?  I don't know how.    
#else
        _dos_setftime( miHandle, mTimeDate.GetDosDate(), mTimeDate.GetDosTime() );
#endif
    }
    close( miHandle );
    miHandle = -1;
    ALStorage::Close();
    if ( miCreated && mTimeDate.Valid() ) {
#if defined( AL_UNIX )
        chmod( (char *) mName, mAttributes.GetUnixAttributes() );
#elif defined( AL_WIN32S )
        SetFileAttributes( mName, mAttributes.GetWin32Attributes() );
#else
        _dos_setfileattr( mName, mAttributes.GetDosAttributes() );
#endif
    }
    return mStatus;
}

//
// int ALFile::RenameToBackup( int delete_on_clash = 1 )
//
// ARGUMENTS:
//
//  delete_on_clash  : If this flag is set, it means that we will overwrite
//                     an existing file with this file if the names clash.
//                     For example, if I am renaming TEMP.DAT to TEMP.BAK,
//                     and a TEMP.BAK already exists, I will delete it
//                     before renaming if this arg is set.
//
// RETURNS
//
//  AL_SUCCESS or AL_RENAME_ERROR.
//
// DESCRIPTION
//
//  This function is a quick way to rename a storage object. The new
//  name created is the default name, which usually means changing the
//  file extension to ".BAK", from whatever it was.
//
//  You don't see it here, but both the mName member and the physical file
//  name are both updated.  That all happens in the Rename() function.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

int AL_PROTO ALFile::RenameToBackup( int delete_on_clash /* = 1 */ )
{
    mName.ChangeExtension();
    return Rename( 0, delete_on_clash );
}

//
// int ALFile::Rename( const char *new_name /* = 0 */, 
//                     int delete_on_clash /* = 1 */ )
//
// ARGUMENTS:
//
//  new_name         : A character pointer to a new file name.  If a name is
//                     defined here, the file is renamed to this new value.
//                     If this value is 0, it means that we expect that
//                     the mName member has already been updated with a
//                     new name.  In this case, the old name of the
//                     file is renamed to the new name.
// 
//  delete_on_clash  : If this flag is set, it means that we will overwrite
//                     an existing file with this file if the names clash.
//                     For example, if I am renaming TEMP.DAT to TEMP.BAK,
//                     and a TEMP.BAK already exists, I will delete it
//                     before renaming if this arg is set.
//
// RETURNS
//
//  AL_SUCCESS or AL_RENAME_ERROR.
//
// DESCRIPTION
//
//  This virtual function provides a way to rename a storage object's
//  physical implementation.  It first updates the mName member if a
//  new_name argument is provided.  After that, we unlink the clash file
//  if one exists, then do a simple rename of mName.mszOldName to
//  new_name.
//
//  Note that this function does a lot of error checking in debug mode.
//  It also does a little error checking in release mode.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : Sun UNIX doesn't have stricmp(), so I have to
//                         substitute strcasecmp().
//
int AL_PROTO ALFile::Rename( const char AL_DLL_FAR *new_name /* = 0 */,
                             int delete_on_clash /* = 1 */ )
{
    AL_ASSERT( miHandle == -1, "Rename: attempting to rename open file" );
    AL_ASSERT( mName.GetName() != 0, "Rename: attempting to rename file with null name" );
    AL_ASSERT( strlen( mName ) > 0, "Rename: attempting to rename file with 0 length name" );

    int status;
    const char *real_old_name;
    const char *real_new_name;
    if ( new_name ) {
        real_old_name = mName.GetSafeName();
        real_new_name = new_name;
    } else {
        real_old_name = mName.GetSafeOldName();
        real_new_name = mName.GetSafeName();
    }
#if !defined( AL_WIN32S )
    const char *p = strchr( real_new_name, '.' );
    if ( p && strlen( p ) > 4 )
        return mStatus.SetError( AL_RENAME_ERROR,
                            "Error trying to rename %s.  It has a long "
                            "extension, which could lead to inadvertent "
                            "deletion of a file when trying to rename.",
                            real_old_name );
#endif
    if ( delete_on_clash ) {
        if ( mName.mCase == AL_MIXED )
            status = strcmp( real_new_name, real_old_name );
        else
#ifdef AL_UNIX
            status = strcasecmp( (char *) real_new_name, real_old_name );
#else
            status = stricmp( real_new_name, real_old_name );
#endif
        if ( status == 0 )
            return mStatus.SetError( AL_RENAME_ERROR,
                                     "Error attempting to rename %s to %s.  "
                                     "Can't rename to the same name!",
                                     real_new_name,
                                     real_old_name );
        status = unlink( real_new_name );
        if ( status != 0 && errno != ENOENT )
            return mStatus.SetError( AL_RENAME_ERROR,
                                     "Error deleting %s before renaming %s.  "
                                     "errno = %d",
                                     real_new_name,
                                     real_old_name,
                                     errno );
    }
    status = rename( real_old_name, real_new_name );
    if ( status != 0 )
        return mStatus.SetError( AL_RENAME_ERROR,
                                 "Error renaming %s to %s.  errno = %d",
                                 real_old_name,
                                 real_new_name,
                                 errno );
    if ( new_name != 0 )
        mName = new_name;
    return AL_SUCCESS;
}

//
// int ALFile::UnRename( int delete_on_clash /* = 1 */ )
//
// ARGUMENTS:
//
//  delete_on_clash  : If this flag is set, it means that we will overwrite
//                     an existing file with this file if the names clash.
//                     For example, if I am renaming TEMP.BAK to TEMP.DAT,
//                     and a TEMP.DAT already exists, I will delete it
//                     before renaming if this arg is set.
//
// RETURNS
//
//  AL_SUCCESS or AL_RENAME_ERROR.
//
// DESCRIPTION
//
//  This virtual function provides a way to undo a previous renaming of
//  a storage object's physical name.  We can do this because the mName
//  member of ALStorage keeps track both of the current name of the file,
//  and the old name.  In this case, we just rename the current name to
//  the old name.  Then we update the mName member so it is accurate.
//
//  Note that this function does a lot of error checking in debug mode.
//  It also does a little error checking in release mode.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

int AL_PROTO ALFile::UnRename( int delete_on_clash /* = 1 */ )
{
    AL_ASSERT( miHandle == -1, "UnRename: attempting to rename open file" );
    AL_ASSERT( mName.GetName() != 0, "UnRename: attempting to rename file with null name" );
    AL_ASSERT( mName.GetOldName() != 0, "UnRename: attempting to rename file with null old name" );
    AL_ASSERT( strlen( mName ) > 0, "UnRename: attempting to rename file with 0 length name" );
    AL_ASSERT( strlen( mName.GetOldName() ) > 0, "UnRename: attempting to rename file with 0 length old name" );

    int status;

    if ( delete_on_clash ) {
        status = unlink( mName.GetOldName() );
        if ( status != 0 && errno != ENOENT )
            return mStatus.SetError( AL_RENAME_ERROR,
                                     "Error deleting %s before renaming %s.  "
                                     "errno = %d",
                                     mName.GetOldName(),
                                     mName.GetName(),
                                     errno );
    }
    status = rename( mName, mName.GetOldName() );
    if ( status != 0 && errno != ENOENT )
        return mStatus.SetError( AL_RENAME_ERROR,
                                 "Error renaming %s to %s.  errno = %d",
                                 mName.GetName(),
                                 mName.GetOldName(),
                                 errno );
    ALStorage::mName = mName.GetOldName();
    return AL_SUCCESS;
}

//
// int ALFile::Delete()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  AL_DELETE_ERROR or AL_SUCCESS.
//
// DESCRIPTION
//
//  This function is called to delete the physical object associated with
//  a file.  This simply means calling the unlink() function for the
//  given name.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

int AL_PROTO ALFile::Delete()
{
    AL_ASSERT( miHandle == -1, "Delete: attempting to delete open file" );
    AL_ASSERT( mName.GetName() != 0, "Delete: attempting to delete file with null name"  );
    AL_ASSERT( strlen( mName ) > 0, "Delete: attempting to delete file with 0 length name" );

    int status = unlink( mName );
    if ( status != 0 )
        return mStatus.SetError( AL_DELETE_ERROR,
                                 "Error deleting file %s, errno = %d ",
                                 mName.GetName(),
                                 errno );
    return AL_SUCCESS;
}


