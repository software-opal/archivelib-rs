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

#if !defined( AL_WIN32S ) && !defined( AL_UNIX )
#include <dos.h>
#endif

#include "fileattr.h"

//
// void * ALFileAttributes::operator new( size_t size )
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
//  By providing our own version of operator new inside this class, we
//  ensure that all memory allocation for the class will be done from
//  inside the DLL, not the EXE calling the DLL.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//

#if defined( AL_BUILDING_DLL )
void AL_DLL_FAR * AL_PROTO ALFileAttributes::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif

//
// ALFileAttributes::ALFileAttributes()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  Constructor, no return.
//
// DESCRIPTION
//
//  This is the only constructor for class ALFileAttributes.  It does
//  nothing more than set all of the bits to 0.  I was thinking that
//  it might be a good idea to set them to -1 instead, indicating that
//  they are presently in an unknown state.  I would want to set them
//  to 0 when I called Create(), and read them in when I call Open().
//  Just thinking out loud.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Added 10 new bits to support UNIX file
//                          attributes.  9 of the bits are the actual
//                          UNIX bits, the tenth is a flag to indicate
//                          whether the bits are from UNIX or DOS.
//
AL_PROTO ALFileAttributes::ALFileAttributes()
{
    miReadOnly        = 0;
    miSystem          = 0;
    miHidden          = 0;
    miArchive         = 0;
    miUnixBitsPresent = 0;
    miUserRead        = 0;
    miUserWrite       = 0;
    miUserExecute     = 0;
    miOtherRead       = 0;
    miOtherWrite      = 0;
    miOtherExecute    = 0;
    miGroupRead       = 0;
    miGroupWrite      = 0;
    miGroupExecute    = 0;
}

//
// ALFileAttributes::~ALFileAttributes()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  Destructor, no returns.
//
// DESCRIPTION
//
//  The destructor doesn't have to worry about freeing any dynamic
//  storage or anything like that, so it gets to do a great big nothing.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//

AL_PROTO ALFileAttributes::~ALFileAttributes()
{
}

//
// void ALFileAttributes::SetFromPackedAttributes( short int attributes )
//
// ARGUMENTS:
//
//  attributes  : A set of attributes that have been packed into a
//                short int using our internal bit ordering:
//
//                X X X X X X X X X X X 0 A H S R
//
//                Note that if we are using UNIX, the packed attributes
//                stored differently:
//
//                X X gx gw gr ox ow or ux uw ur 1 X X X X
//
// RETURNS
//
//  Nothing, a void function.
//
// DESCRIPTION
//
//  The packed format is how we actually store file attributes in an
//  archive.  When we read in the directory from an archive, we can
//  use this function to apply the packed attribute bits to a storage
//  object.  All it has to do is set the 14 bits internal to the
//  class.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//
//   August 10, 1994 1.0B : Added the 9 UNIX bits and the flag bit.
//
void AL_PROTO ALFileAttributes::
SetFromPackedAttributes( short int attributes )
{
    miReadOnly        = ( attributes & 1 ) != 0;
    miSystem          = ( attributes & 2 ) != 0;
    miHidden          = ( attributes & 4 ) != 0;
    miArchive         = ( attributes & 8 ) != 0;
    miUnixBitsPresent = ( attributes & 16 ) != 0;
    miUserRead        = ( attributes & 32 ) != 0;
    miUserWrite       = ( attributes & 64 ) != 0;
    miUserExecute     = ( attributes & 128 ) != 0;
    miOtherRead       = ( attributes & 256 ) != 0;
    miOtherWrite      = ( attributes & 512 ) != 0;
    miOtherExecute    = ( attributes & 1024 ) != 0;
    miGroupRead       = ( attributes & 2048 ) != 0;
    miGroupWrite      = ( attributes & 4096 ) != 0;
    miGroupExecute    = ( attributes & 8192 ) != 0;
}

//
// void ALFileAttributes::SetFromWin32Attributes( DWORD win32_attributes )
//
// ARGUMENTS:
//
//  attributes  : The set of file attributes as defined for Win32s.
//                These are the attributes you get back from the
//                GetFileAttributes() function call.
//
// RETURNS
//
//  Nothing, a void function.
//
// DESCRIPTION
//
//  When the ALFile class opens a file under Win32s, it reads in
//  the file attributes with a call to GetFileAttributes.  It can then store
//  those file attributes in the ALFileAttributes member of ALStorage
//  by calling this function.
//
//  Converting the data to our internal format is simply a matter of picking
//  bits out of a DWORD.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//
//   August 10, 1994 1.0B : Added the support for the UNIX bits.  Note
//                          that we consider Win32 protection bits to
//                          be identical to DOS protection bits.

#if defined( AL_WIN32S )

void AL_PROTO ALFileAttributes::
SetFromWin32Attributes( DWORD win32_attributes )
{
    miUnixBitsPresent = 0;
    miReadOnly = ( win32_attributes & FILE_ATTRIBUTE_READONLY ) != 0;
    miSystem = ( win32_attributes & FILE_ATTRIBUTE_SYSTEM ) != 0;
    miHidden = ( win32_attributes & FILE_ATTRIBUTE_HIDDEN ) != 0;
    miArchive = ( win32_attributes & FILE_ATTRIBUTE_ARCHIVE ) != 0;
}

#endif

//
// void ALFileAttributes::SetFromDosAttributes( unsigned dos_attributes )
//
// ARGUMENTS:
//
//  attributes  : The set of file attributes as defined for MS-DOS.
//                These are the attributes you get back from the
//                _dos_getfileattr() function call.
//
// RETURNS
//
//  Nothing, a void function.
//
// DESCRIPTION
//
//  When the ALFile class opens a file under MS-DOS or Windows, it reads in
//  the file attributes with a call to _dos_getfileattr().  It can then store
//  those file attributes in the ALFileAttributes member of ALStorage
//  by calling this function.
//
//  Converting the data to our internal format is simply a matter of picking
//  bits out of an unsigned int
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A   : First release
//
//   July 7, 1994  1.0B   : Conditional compile used to turn this routine
//                          off under UNIX.
//
//   August 10, 1994 1.0B : Added suppport for the UNIX file protection bits.
//

#if !defined( AL_WIN32S ) && !defined( AL_UNIX )

void AL_PROTO ALFileAttributes::SetFromDosAttributes( unsigned dos_attributes )
{
    miUnixBitsPresent = 0;
    miReadOnly = ( dos_attributes & _A_RDONLY ) != 0;
    miSystem = ( dos_attributes & _A_SYSTEM ) != 0;
    miHidden = ( dos_attributes & _A_HIDDEN ) != 0;
    miArchive = ( dos_attributes & _A_ARCH ) != 0;
}
#endif

//
// unsigned short int ALFileAttributes::PackedAttributes()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  An unsigned short int containing the 14 file attribute bits, packed
//  into the format we use to store file attributes in an archive:
//
//
//                X X gx gw gr ox ow or ux uw ur UNIX A H S R
//
// DESCRIPTION
//
//  When it comes time to write an archive directory out to disk, we need
//  to store the file attributes in a consistent format.  This format
//  is our internal packed attribute format.  You can get the file attributes
//  in this packed attribute format with a call to this function.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Added support for the UNIX bits.
//

unsigned short int AL_PROTO ALFileAttributes::PackedAttributes()
{
    int result = 0;

    result |= miReadOnly ? 1 : 0;
    result |= miSystem ? 2 : 0;
    result |= miHidden ? 4 : 0;
    result |= miArchive ? 8 : 0;
    result |= miUnixBitsPresent ? 16 : 0;
    result |= miUserRead ? 32 : 0;
    result |= miUserWrite ? 64 : 0;
    result |= miUserExecute ? 128 : 0;
    result |= miOtherRead ? 256 : 0;
    result |= miOtherWrite  ? 512 : 0;
    result |= miOtherExecute ? 1024 : 0;
    result |= miGroupRead ? 2048 : 0;
    result |= miGroupWrite ? 4096 : 0;
    result |= miGroupExecute ? 8192 : 0;
    return (unsigned short int ) result;
}

//
// unsigned short int ALFileAttributes::GetDosAttributes()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  The set of file attributes, packed into the order that MS-DOS
//  expects them for the _dos_setfileattr() function call.
//
// DESCRIPTION
//
//  When we are closing a file that we have to set the time stamp and
//  attributes for, this function is used to get the attributes.
//  Under MS-DOS at least.  All it needs to do is repack some bits
//  into the order that MS-DOS expects.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A   : First release
//
//   July 7, 1994  1.0B   : Conditional compilation used to turn this off
//                          under UNIX.
//
//   August 10, 1994 1.0B : Modified this routine so that it just returns
//                          0 if the attributes are in UNIX format.
//

#if !defined( AL_WIN32S ) && !defined( AL_UNIX )

unsigned short int AL_PROTO ALFileAttributes::GetDosAttributes()
{
    int result = 0;
    if ( !miUnixBitsPresent ) {
        result |= miReadOnly ? _A_RDONLY : 0;
        result |= miSystem ? _A_SYSTEM : 0;
        result |= miHidden ? _A_HIDDEN : 0;
        result |= miArchive ? _A_ARCH : 0;
    }
    return (unsigned short int) result;
}
#endif //#if !defined( AL_WIN32S )

//
// DWORD ALFileAttributes::GetWin32Attributes()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  The set of file attributes, packed into the order that Win32s
//  expects them for the SetFileAttributes() function call.
//
// DESCRIPTION
//
//  When we are closing a file that we have to set the time stamp and
//  attributes for, this function is used to get the attributes.
//  Under Win32s at least.  All it needs to do is repack some bits
//  into the order that Win32s expects.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   August 10, 1994 1.0B : Modified this routine so that it just returns
//                          0 if the attributes are in UNIX format.
//

#if defined( AL_WIN32S )

DWORD AL_PROTO ALFileAttributes::GetWin32Attributes()
{
    DWORD result = 0;
    if ( !miUnixBitsPresent ) {
        result |= miReadOnly ? FILE_ATTRIBUTE_READONLY : 0;
        result |= miSystem ? FILE_ATTRIBUTE_SYSTEM : 0;
        result |= miHidden ? FILE_ATTRIBUTE_HIDDEN : 0;
        result |= miArchive ? FILE_ATTRIBUTE_ARCHIVE : 0;
    }
    return result;
}
#endif //#if !defined( AL_WIN32S )

//
// void ALFileAttributes::SetFromUnixAttributes( mode_t attributes )
//
// ARGUMENTS:
//
//  attributes  : The set of file attributes as defined for UNIX.
//                These are the attributes you get back from the
//                stat() function call.
//
// RETURNS
//
//  Nothing, a void function.
//
// DESCRIPTION
//
//  When the ALFile class opens a file under UNIX, it reads in
//  the file attributes with a call to stat().  It can then store
//  those file attributes in the ALFileAttributes member of ALStorage
//  by calling this function.
//
//  Converting the data to our internal format is simply a matter of picking
//  bits out of the mode_t word.
//
// REVISION HISTORY
//
//   August 10, 1994 1.0B : First released.
//
//

#if defined AL_UNIX
void AL_PROTO ALFileAttributes::SetFromUnixAttributes( mode_t mode )
{
    miUnixBitsPresent = 1;
    miUserRead        = ( mode & S_IRUSR ) ? 1 : 0;
    miUserWrite       = ( mode & S_IWUSR ) ? 1 : 0;
    miUserExecute     = ( mode & S_IXUSR ) ? 1 : 0;
    miOtherRead       = ( mode & S_IROTH ) ? 1 : 0;
    miOtherWrite      = ( mode & S_IWOTH ) ? 1 : 0;
    miOtherExecute    = ( mode & S_IXOTH ) ? 1 : 0;
    miGroupRead       = ( mode & S_IRGRP ) ? 1 : 0;
    miGroupWrite      = ( mode & S_IWGRP ) ? 1 : 0;
    miGroupExecute    = ( mode & S_IXGRP ) ? 1 : 0;
}

#endif

//
// mode_t ALFileAttributes::GetUnixAttributes()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  The set of file attributes, packed into the order that UNIX
//  expects them.
//
// DESCRIPTION
//
//  When we are closing a file that we have to set the time stamp and
//  attributes for, this function is used to get the attributes.
//  All it needs to do is repack some bits into the order that UNIX expects.
//
//  Note that if the attribute is presently stored in DOS format, this
//  function just returns the default protection mask.
//
// REVISION HISTORY
//
//   August 10, 1994 1.0B : First released.
//

#ifdef AL_UNIX

mode_t AL_PROTO ALFileAttributes::GetUnixAttributes()
{
    mode_t mode = 0;
    if ( !miUnixBitsPresent ) {
        mode = umask( 0 );
        umask( mode );
    } else {
        mode |= miUserRead ? S_IRUSR : 0;
        mode |= miUserWrite ? S_IWUSR : 0;
        mode |= miUserExecute ? S_IXUSR : 0;
        mode |= miOtherRead ? S_IROTH : 0;
        mode |= miOtherWrite ? S_IWOTH : 0;
        mode |= miOtherExecute ? S_IXOTH : 0;
        mode |= miGroupRead ? S_IRGRP : 0;
        mode |= miGroupWrite ? S_IWGRP : 0;
        mode |= miGroupExecute ? S_IXGRP : 0;
    }
    return mode;
}

#endif

