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

#include "al.h"
#include "alcxl.h"

// Windows:
//
// extern "C" hALStorage newALMemory( char *buffer_name,
//                                    char AL_HUGE *user_buffer,
//                                    DWORD user_buffer_size )
//
// MS-DOS:
//
// extern "C" hALStorage newALMemory( char *buffer_name,
//                                    char *user_buffer,
//                                    int user_buffer_size )
// ARGUMENTS:
//
//  name             : A character string containing an arbitrary name
//                     name for the memory buffer.
//
//  user_buffer      : If you want to supply a buffer of your own, pass it
//                     here.  Otherwise, set it to 0, and the ALMemory
//                     class will allocate storage as needed.
//
//  user_buffer_size : If you are supplying a buffer, you have to say how
//                     long it is.
//
// RETURNS
//
//  A handle for (pointer to) a newly created ALMemory object.
//
// DESCRIPTION
//
//  This C/VB translation function provides access to the ALMemory
//  constructor.  Note that the constructor has the same name under
//  MS-DOS and Windows, but it does have slightly different capabilities.
//  Because of this, the Windows version also has different argument types.
//
//  This function passes all of its arguments to the C++ constructor
//  in unchanged form.  It then takes the return from the function, casts
//  it to a C/VB acceptable type, and returns it otherwise unchanged.
//
//  This function is like all of the other translation routines in that
//  it is fairly uninformative.  To get the real information about what
//  is happening in the constructor, take a look at the source for the
//  C++ functions in MEMSTORE.CPP.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

#if defined( AL_WINDOWS_MEMORY )
extern "C" hALStorage AL_FUNCTION
newALMemory( char AL_DLL_FAR *buffer_name,
             char AL_HUGE *user_buffer,
             DWORD user_buffer_size )
{
    if ( user_buffer_size == 0 )
        return (hALStorage) new ALMemory( buffer_name );
    else
        return (hALStorage) new ALMemory( buffer_name, user_buffer, user_buffer_size );
}

#else
extern "C" hALStorage AL_FUNCTION newALMemory( char AL_DLL_FAR *buffer_name,
                                               char AL_DLL_FAR *user_buffer,
                                               int user_buffer_size )
{
    if ( user_buffer_size == 0 )
        return (hALStorage) new ALMemory( buffer_name );
    else
        return (hALStorage) new ALMemory( buffer_name, user_buffer, user_buffer_size );
}

#endif

//
// extern "C" int ALMemoryGetBufferOwner( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object  :  A handle for (pointer to) an ALMemory object.
//
// RETURNS
//
//  An integer flag indicating whether the user owns the buffer.
//
// DESCRIPTION
//
//  This C/VB translation function provides access to the C++ data member
//  ALMemory::mfUserOwnsBuffer.
//
//  This function first tests its only argument for correct type (when in
//  debug mode), then casts and accesses the data member.  The value of
//  the data member is then returned unchanged to the calling C or VB
//  procedure.
//
//  This function is like all of the other translation routines in that
//  it is fairly uninformative.  To get the real information about what
//  is happening in the constructor, take a look at the source for the
//  C++ functions in MEMSTORE.CPP.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALMemoryGetBufferOwner( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALMemory, "ALMemoryGetBufferOwner" );
    return ( (ALMemory *) this_object )->mfUserOwnsBuffer;
}

//
// extern "C" void ALMemorySetBufferOwner( hALStorage this_object,
//                                         int user_owns_buffer )
//
// ARGUMENTS:
//
//  this_object      :  A handle for (pointer to) an ALMemory object.
//
//  user_owns_buffer : The new setting of the flag.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This C/VB translation function provides access to the C++ data member
//  ALMemory::mfUserOwnsBuffer.  Why would you want to change this
//  data member?  Well, if the ALMemory object is currently the buffer
//  owner, the buffer will be deleted when the ALMemory object is destroyed.
//  You can set the owner of the buffer to be you, the user, and copy
//  the pointer to it.  Then you get to keep it long after the ALMemory
//  object is done with it.
//
//  This function first tests its only argument for correct type (when in
//  debug mode), then casts and modifies the data member.
//
//  This function is like all of the other translation routines in that
//  it is fairly uninformative.  To get the real information about what
//  is happening in the constructor, take a look at the source for the
//  C++ functions in MEMSTORE.CPP.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION ALMemorySetBufferOwner( hALStorage this_object,
                                                    int user_owns_buffer )
{
    AL_ASSERT_OBJECT( this_object, ALMemory, "ALMemorySetBufferOwner" );
    ( (ALMemory *) this_object )->mfUserOwnsBuffer = user_owns_buffer;
}

// WINDOWS ONLY PROCEDURE
//
// extern "C" UINT ALMemoryGetHandle( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object  :  A handle for (pointer to) an ALMemory object.
//
// RETURNS
//
//  A UINT windows memory handle.
//
// DESCRIPTION
//
//  This C/VB translation function provides access to the C++ data member
//  ALMemory::mhUserMemoryHandle.  Under C++, this is a public data
//  member that the programmer is free to access or modify.
//
//  This function first tests its only argument for correct type (when in
//  debug mode), then casts and accesses the data member.  The value of
//  the data member is then returned unchanged to the calling C or VB
//  procedure.
//
//  This function is like all of the other translation routines in that
//  it is fairly uninformative.  To get the real information about what
//  is happening in the constructor, take a look at the source for the
//  C++ functions in MEMSTORE.CPP.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

#if defined( AL_WINDOWS_MEMORY )
extern "C" UINT AL_FUNCTION ALMemoryGetHandle( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALMemory, "ALMemorySetBufferOwner" );
    return (UINT) ( (ALMemory *) this_object )->mhUserMemoryHandle;
}
#endif

// WINDOWS Version
//
// extern "C" long ALMemoryGetBufferSize( hALStorage this_object )
//
// MS-DOS Real Mode version
//
// extern "C" size_t ALMemoryGetBufferSize( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object  :  A handle for (pointer to) an ALMemory object.
//
// RETURNS
//
//  The size of the memory buffer being uses as a storage object.  The
//  type of the the size differs between Windows and Real mode DOS, simply
//  because Windows can access a lot more memory, especially when
//  using Win32s.
//
// DESCRIPTION
//
//  This C/VB translation function provides access to the C++ data member
//  ALMemory::muUserBufferSize.  Under C++, this is a public data
//  member that the programmer is free to access or modify.
//
//  This function first tests its only argument for correct type (when in
//  debug mode), then casts and accesses the data member.  The value of
//  the data member is then returned unchanged to the calling C or VB
//  procedure.
//
//  This function is like all of the other translation routines in that
//  it is fairly uninformative.  To get the real information about what
//  is happening in the constructor, take a look at the source for the
//  C++ functions in MEMSTORE.CPP.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

#if defined( AL_WINDOWS_MEMORY )
extern "C" long AL_FUNCTION ALMemoryGetBufferSize( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALMemory, "ALMemoryGetBufferSize" );
    return ( (ALMemory *) this_object )->muUserBufferSize;
}

#else

extern "C" size_t AL_FUNCTION ALMemoryGetBufferSize( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALMemory, "ALMemoryGetBufferSize" );
    return ( (ALMemory *) this_object )->muUserBufferSize;
}
#endif

// WINDOWS Version
//
// extern "C" char AL_HUGE * ALMemoryGetBuffer( hALStorage this_object )
//
// MS-DOS Real Mode version
//
// extern "C" char *ALMemoryGetBuffer( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object  :  A handle for (pointer to) an ALMemory object.
//
// RETURNS
//
//  A pointer to the memory buffer being uses as a storage object.  The
//  type of the the pointer differs between the various modes of our
//  library.  Under Real Dos, it is just a normal pointer.  Under Win16
//  it is a _huge pointer.  Under Win32s, it is a flat model pointer.
//
// DESCRIPTION
//
//  This C/VB translation function provides access to the C++ data member
//  ALMemory::mpcUserBuffer.  Under C++, this is a public data
//  member that the programmer is free to access or modify.
//
//  This function first tests its only argument for correct type (when in
//  debug mode), then casts and accesses the data member.  The value of
//  the data member is then returned unchanged to the calling C or VB
//  procedure.
//
//  This function is like all of the other translation routines in that
//  it is fairly uninformative.  To get the real information about what
//  is happening in the constructor, take a look at the source for the
//  C++ functions in MEMSTORE.CPP.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

#if defined( AL_WINDOWS_MEMORY )
extern "C" char AL_HUGE * AL_FUNCTION ALMemoryGetBuffer( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALMemory, "ALMemoryGetBuffer" );
    return ( (ALMemory *) this_object )->mpcUserBuffer;
}

#else

extern "C" char AL_DLL_FAR *AL_FUNCTION ALMemoryGetBuffer( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALMemory, "ALMemoryGetBuffer" );
    return ( (ALMemory *) this_object )->mpcUserBuffer;
}

#endif

//
// extern "C" long ALMemoryCopyBufferVB( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object  :  A handle for (pointer to) an ALMemory object.
//
// RETURNS
//
//  A VB string that contains the contents of the memory object.
//  Note that the memory object is still there, unchanged, but now
//  you can easily get at it using VB.
//
// DESCRIPTION
//
//  This VB translation function provides access to the data stored
//  in the buffer of an ALMemory object.  It does this by creating a
//  VB string with a copy of the data.  We don't do any checking here,
//  so it is possible to abort VB if the string is too large.
//
//  This function was sort of a missing link in our ability to handle
//  memory objects in VB.  It was always easy to convert a VB string to
//  an ALMemory object, but we didn't have any good way to make the
//  reverse trip.
//
// REVISION HISTORY
//
//   July 7, 1994  1.0B  : First release
//
//   August 10, 1994 1.0B : Combined a bunch of #ifdefs into a single test
//                          against AL_VB

#if defined( AL_VB )

extern "C" long AL_FUNCTION ALMemoryCopyBufferVB( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALMemory, "ALMemoryCopyBufferVB" );
    ( (ALMemory *) this_object )->FlushBuffer();
    return ALCreateVBString( (char _far *) ( (ALMemory *) this_object )->mpcUserBuffer,
                             (unsigned short int) ( (ALMemory *) this_object )->GetSize() );
}

#endif

