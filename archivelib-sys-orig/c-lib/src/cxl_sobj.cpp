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

//
// extern "C" void deleteALStorage( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) the storage object that
//                 is going to be destroyed.
//
// RETURNS
//
//  Nothing, this is a destructor.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to access the
//  C++ destructor ALStorage::~ALStorage().  This function checks its
//  single handle argument for correct type (in debug mode), then casts
//  and calls the dtor.
//
//  Note that since the destructor is virtual, you will actually be
//  going to the destructor for your derive class.
//
//  The ALStorage destructor does a fair amount of work, so you might
//  want to check it out in STORAGE.CPP.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION deleteALStorage( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "deleteALStorage" );
    delete (ALStorage *) this_object;
}

//
// extern "C" void ALStorageSetMonitor( hALStorage this_object,
//                                      hALMonitor monitor )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) the storage object that
//                 is have a new monitor assigned to it.
//
//  monitor      : A handle for (pointer to) an ALMonitor object.  This
//                 object is going to be assigned to the storage object.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to access the
//  C++ data member ALStorage::mpMonitor.  This function checks its
//  two handle arguments for correct type (in debug mode), then casts
//  and assigns.
//
//  Normally, assignment of monitor objects to storage objects is done
//  inside the member functions of ALArchiveBase. However, if you want
//  to use a monitor for some operation you are performing on your own,
//  such as a batch file copy, you will have to use this function (along
//  with a couple of others) to get things to work properly.
//
//  Note that 0 is a valid value for a monitor.
//
//  To see how the monitor actually works, check out ARCHIVEB.CPP,
//  and BARGRAPH.CPP or WINMON.CPP.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION ALStorageSetMonitor( hALStorage this_object,
                                                 hALMonitor monitor )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageSetMonitor" );
    if ( monitor )
        AL_ASSERT_OBJECT( monitor, ALMonitor, "ALStorageSetMonitor" );
    ( (ALStorage *) this_object )->mpMonitor = (ALMonitor *) monitor;
}

//
// extern "C" int ALStorageDelete( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) the storage object that
//                 is going to have its underlying physical object deleted.
//
// RETURNS
//
//  AL_SUCCESS, or some other error code.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to access the
//  C++ member function ALStorage::Delete().  This function checks its
//  handle argument for correct type (in debug mode), then casts
//  and calls Delete().
//
//  Delete() is used to delete the underlying physical object associated
//  with a storage object, for example, a disk file.  The implementation
//  of this function will vary widely across derived classes.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALStorage::Delete(). See FILESTOR.CPP and MEMSTORE.CPP for an
//  in-depth look at the whole deal.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageDelete( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageDelete" );
    return ( (ALStorage *) this_object)->Delete();
}

//
// extern "C" void ALStorageSetName( hALStorage this_object,
//                                   char *object_name )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) the storage object that
//                 is going to have its name changed.
//
//  object_name  : The new name.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to access the
//  C++ member function ALName::operator =(char*), for the mName member
//  of objects of class ALStorage.  This function checks its single
//  handle argument for correct type (in debug mode), then casts
//  and assigns.
//
//  This function changes the name in the ALStorage object, but it doesn't
//  change the underlying name of the physical object (if there is one.)
//  To do that, you need to call ALStorageRename().
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALName. See OBJNAME.CPP for the details on that.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION ALStorageSetName( hALStorage this_object,
                                              char *object_name )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageSetName" );
    ( (ALStorage *) this_object )->mName = object_name;
}

//
// extern "C" int ALStorageCompare( hALStorage this_object,
//                                  hALStorage test_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) the storage object that
//                 is going to be half of the comparison.
//
//  test_object  : A handle for (pointer to) the storage object that
//                 is going to be the other half of the comparison.
//
// RETURNS
//
//  AL_SUCCESS if they match, AL_COMPARE_ERROR if they don't, some other
//  code < AL_SUCCESS if a bad thing happened.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to access the
//  C++ member function ALStorage::Compare(). This function checks its two
//  handle arguments for correct type (in debug mode), then casts
//  and calls ALStorage::Compare().
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALStorage::Compare(). See STORCMP.CPP for the details on that.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageCompare( hALStorage this_object,
                                             hALStorage test_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageCompare" );
    AL_ASSERT_OBJECT( test_object, ALStorage, "ALStorageCompare" );
    return ( (ALStorage *) this_object )->Compare( *(ALStorage *) test_object );
}

//
// extern "C" long ALStorageGetSize( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) the storage object that
//                 you want to get this size of.
//
// RETURNS
//
//  The long size of the storage object if it is known.  -1 if it
//  isn't known.  Some other error code < 0 if things went haywire.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to access the
//  C++ member function ALStorage::GetSize(). This function checks its
//  handle argument for correct type (in debug mode), then casts
//  and calls ALStorage::GetSize().  It returns the long integer
//  result back to the calling procedure unchanged.
//
//  GetSize() is not a virtual function, it simply provides access to
//  the mlSize member of ALStorage.  However, mlSize will be updated
//  many of the member functions of ALStorage and derived classes.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALStorage::GetSize(). See STORAGE.H for the details on that.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//

extern "C" long AL_FUNCTION ALStorageGetSize( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetSize" );
    return ( (ALStorage *) this_object)->GetSize();
}

//
// extern "C" unsigned short int ALStoragePackedAttributes(
//                            hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) the storage object that
//                 has the packed attributes you are interested int.
//
// RETURNS
//
//  An unsigned short containing the packed attributes of the storage
//  object.  The attributes are packed in our own internal format
//  (which is documented).
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to access the
//  C++ member function ALFileAttributes::PackedAttributes() for the
//  mAttributes data member of class ALStorage. This function checks its
//  handle argument for correct type (in debug mode), then casts
//  and calls the C++ function.  It returns the unsigned short
//  result back to the calling procedure unchanged.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALFileAttributes::PackedAttributes. See FILEATTR.H for the details
//  on that.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//

extern "C" unsigned short int AL_FUNCTION
ALStoragePackedAttributes( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStoragePackedAttributes" );
    return ( (ALStorage *) this_object)->mAttributes.PackedAttributes();
}

//
// extern "C" void
// ALStorageSetFromUnixAttributes( hALStorage this_object,
//                                 mode_t attributes )
//
// ARGUMENTS:
//
//  this_object    : A handle for (pointer to) the storage object whose
//                   attributes you want to set.
//
//  attributes     : A set of file attributes in a mode_t word used by UNIX.
//
// RETURNS
//
//  Nothing, a void guy.
//
// DESCRIPTION
//
//  This is the C translation routine that allows you to set the
//  value of the ALStorage data member mAttributes by way of the
//  ALFileAttributes::SetFromUnixAttributes() member function.  This function
//  checks the handle argument for correct type (in debug mode), then casts
//  and calls the C++ function.
//
//  The C translation code doesn't offer much insight into the operation
//  of ALFileAttributes::SetFromUnixAttributes(). See FILEATTR.CPP for the
//  details on that.
//
// REVISION HISTORY
//
//   August 10, 1994 1.0B : First release, added with Patch B.
//
#if defined( AL_UNIX )
extern "C" void AL_FUNCTION
ALStorageSetFromUnixAttributes( hALStorage this_object,
                                mode_t attributes )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageSetFromUnixAttributes" );
    ( (ALStorage *) this_object)->mAttributes.SetFromUnixAttributes( attributes );
}
#endif

//
// extern "C" void
// ALStorageSetFromWin32Attributes( hALStorage this_object,
//                                  DWORD attributes )
//
// ARGUMENTS:
//
//  this_object    : A handle for (pointer to) the storage object whose
//                   attributes you want to set.
//
//  attributes     : A set of file attributes in the DWORD format used by
//                   Win32 function calls.
//
// RETURNS
//
//  Nothing, a void guy.
//
// DESCRIPTION
//
//  This is the C translation routine that allows you to set the
//  value of the ALStorage data member mAttributes by way of the
//  ALFileAttributes::SetFromWin32Attributes() member function.  This function
//  checks the handle argument for correct type (in debug mode), then casts
//  and calls the C++ function.
//
//  The C translation code doesn't offer much insight into the operation
//  of ALFileAttributes::SetFromWin32Attributes(). See FILEATTR.CPP for the
//  details on that.
//
// REVISION HISTORY
//
//   August 10, 1994 1.0B : First release, added with Patch B.
//

#if defined( AL_WIN32S )

extern "C" void AL_FUNCTION
ALStorageSetFromWin32Attributes( hALStorage this_object,
                                 DWORD attributes )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageSetFromWin32Attributes" );
    ( (ALStorage *) this_object)->mAttributes.SetFromWin32Attributes( attributes );
}

#endif

//
// extern "C" void
// ALStorageSetFromDosAttributes( hALStorage this_object,
//                                unsigned short int dos_attributes )
//
// ARGUMENTS:
//
//  this_object    : A handle for (pointer to) the storage object whose
//                   attributes you want to set.
//
//  dos_attributes : A set of file attributes in the format you normally
//                   get from MS-DOS functions such as _dos_getfileattr().
// RETURNS
//
//  Nothing, a void guy.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to set the
//  value of the ALStorage data member mAttributes by way of the
//  ALFileAttributes::SetFromDosAttributes() member function.  This function
//  checks the handle argument for correct type (in debug mode), then casts
//  and calls the C++ function.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALFileAttributes::SetFromDosAttributes(). See FILEATTR.CPP for the
//  details on that.
//
//  Doesn't compile in AL_FLAT_MODEL, because file attributes are handled
//  completely differently in Win32s/NT.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : This function doesn't work very well under UNIX,
//                          so I had to add some #ifdef stuff to exclude
//                          it when it wasn't wanted.
//
#if !defined( AL_WIN32S ) && !defined( AL_UNIX )
extern "C" void AL_FUNCTION ALStorageSetFromDosAttributes( hALStorage this_object, unsigned short int dos_attributes )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageSetFromDosAttributes" );
    ( (ALStorage *) this_object)->mAttributes.SetFromDosAttributes( dos_attributes );
}
#endif

//
// extern "C" void
// ALStorageSetFromPackedAtts( hALStorage this_object,
//                             unsigned short int packed_attributes )
//
// ARGUMENTS:
//
//  this_object       : A handle for (pointer to) the storage object whose
//                      attributes you want to set.
//
//  packed_attributes : A set of file attributes in the format used
//                      internally by ArchiveLib.
// RETURNS
//
//  Nothing, a void guy.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to set the
//  value of the ALStorage data member mAttributes by way of the
//  ALFileAttributes::SetFromPackedAttributes() member function.  This
//  function checks the handle argument for correct type (in debug mode),
//  then casts and calls the C++ function.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALFileAttributes::SetFromPackedAttributes(). See FILEATTR.CPP for the
//  details on that.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION ALStorageSetFromPackedAtts( hALStorage this_object, unsigned short int packed_attributes )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageSetFromPackedAtts" );
    ( (ALStorage *) this_object )->mAttributes.SetFromPackedAttributes( packed_attributes );
}

//
// extern "C" int ALStorageWildCardMatch( hALStorage this_object,
//                                        char *pattern )
//
// ARGUMENTS:
//
//  this_object     : A handle for (pointer to) the storage object whose
//                      name you want to test.
//
//  pattern         : A regular expression that will be tested for a match.
//
// RETURNS
//
//  1 for a match, 0 for not.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to call the
//  ALName::WildCardMatch() C++ member function for the mName data
//  member of class ALStorage.  This function checks the handle argument for
//  correct type (in debug mode), then casts and calls the C++ function.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALName::WildCardMatch().  See OBJNAME.CPP for more information.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageWildCardMatch( hALStorage this_object, char AL_DLL_FAR *pattern )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageWildCardMatch" );
    return ( (ALStorage *) this_object )->mName.WildCardMatch( pattern );
}

// C TRANSLATION FUNCTION
//
// extern "C" char * ALStorageChangeExtension( hALStorage this_object,
//                                             char *new_extension )
// VB TRANSLATION FUNCTION
//
// extern "C" long ALStorageChangeExtensionVB( hALStorage this_object,
//                                             char *new_extension )
// ARGUMENTS:
//
//  this_object     : A handle for (pointer to) the storage object whose
//                      name you want to test.
//
//  new_extension   : A new three letter (maybe) extension you want
//                    to apply to the object name.
//
// RETURNS
//
//  A string pointer (or VB string) containing the file name after the
//  new extension has been applied to it.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to call the
//  ALName::ChangeExtension() C++ member function for the mName data
//  member of class ALStorage.  This function checks the handle argument for
//  correct type (in debug mode), then casts and calls the C++ function.
//
//  Note that the VB version of this function is almost identical.  However,
//  instead of returning a pointer to a character string, this routine calls
//  ALVBCreateString() to build a VB string, which it returns to the
//  calling module.  Don't use the VB function from C, it will blow up.
//  Don't use the C function from VB, because it returns a string pointer,
//  which VB doesn't know how to deal with.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALName::ChangeExtension().  See OBJNAME.CPP for more information.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Combined a bunch of #ifdefs into a single test
//                          against AL_VB

extern "C" char AL_DLL_FAR * AL_FUNCTION
ALStorageChangeExtension( hALStorage this_object,
                          char AL_DLL_FAR *new_extension )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageChangeExtension" );
    return ( (ALStorage *) this_object )->mName.ChangeExtension( new_extension );
}

#if defined( AL_VB )

extern "C" long AL_FUNCTION
ALStorageChangeExtensionVB( hALStorage this_object,
                            char AL_DLL_FAR *new_extension )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageChangeExtensionVB" );
    char _far * p = ( (ALStorage *) this_object )->mName.ChangeExtension( new_extension );
    return ALCreateVBString( p, (unsigned short int) _fstrlen( p ) );
}

#endif

// C TRANSLATION FUNCTION
//
// extern "C" char * ALStorageChangeTrailingChar( hALStorage this_object,
//                                                char new_char )
//
// VB TRANSLATION FUNCTION
//
// extern "C" long ALStorageChangeTrailingCharVB( hALStorage this_object,
//                                                char new_char )
//
// ARGUMENTS:
//
//  this_object     : A handle for (pointer to) the storage object whose
//                      name you want to test.
//
//  new_char        : A new final character you want to apply to the
//                    object name.  Often a "funny" character, like '~'.
//
// RETURNS
//
//  A string pointer (or VB string) containing the file name after the
//  new final character has been applied to it.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to call the
//  ALName::ChangeTrailingChar() C++ member function for the mName data
//  member of class ALStorage.  This function checks the handle argument for
//  correct type (in debug mode), then casts and calls the C++ function.
//
//  Note that the VB version of this function is almost identical.  However,
//  instead of returning a pointer to a character string, this routine calls
//  ALVBCreateString() to build a VB string, which it returns to the
//  calling module.  Don't use the VB function from C, it will blow up.
//  Don't use the C function from VB, because it returns a string pointer,
//  which VB doesn't know how to deal with.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALName::ChangeTrailingChar().  See OBJNAME.CPP for more information.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Combined a bunch of #ifdefs into a single test
//                          against AL_VB

extern "C" char AL_DLL_FAR * AL_FUNCTION
ALStorageChangeTrailingChar( hALStorage this_object, char new_char )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageChangeTrailingChar" );
    return ( (ALStorage *) this_object )->mName.ChangeTrailingChar( new_char );
}

#if defined( AL_VB )

extern "C" long AL_FUNCTION
ALStorageChangeTrailingCharVB( hALStorage this_object, char new_char )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageChangeTrailingChar" );
    char _far * p = ( (ALStorage *) this_object )->mName.ChangeTrailingChar( new_char );
    return ALCreateVBString( p, (unsigned short int) _fstrlen( p ) );
}

#endif

// C TRANSLATION FUNCTION
//
// extern "C" char * ALStorageGetName( hALStorage this_object )
//
// VB TRANSLATION FUNCTION
//
// extern "C" long ALStorageGetNameVB( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object     : A handle for (pointer to) the storage object whose
//                    name you want.
//
// RETURNS
//
//  A string pointer (or VB string) containing the file name.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to call the
//  ALName::GetSafeName() C++ member function for the mName data
//  member of class ALStorage.  This function checks the handle argument for
//  correct type (in debug mode), then casts and calls the C++ function.
//
//  Note that the VB version of this function is almost identical.  However,
//  instead of returning a pointer to a character string, this routine calls
//  ALVBCreateString() to build a VB string, which it returns to the
//  calling module.  Don't use the VB function from C, it will blow up.
//  Don't use the C function from VB, because it returns a string pointer,
//  which VB doesn't know how to deal with.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALName::GetSafeName().  See OBJNAME.CPP for more information.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Combined a bunch of #ifdefs into a single test
//                          against AL_VB

extern "C" char AL_DLL_FAR * AL_FUNCTION
ALStorageGetName( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetName" );
    return (char AL_DLL_FAR *) ( (ALStorage *) this_object )->mName.GetSafeName();
}

#if defined( AL_VB )

extern "C" long AL_FUNCTION ALStorageGetNameVB( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetName" );
    const char _far *p = ( (ALStorage *) this_object )->mName.GetSafeName();
    return ALCreateVBString( p, (unsigned short int) _fstrlen( p ) );
}

#endif

// C TRANSLATION FUNCTION
//
// extern "C" char * ALStorageGetOldName( hALStorage this_object )
//
// VB TRANSLATION FUNCTION
//
// extern "C" long ALStorageGetOldNameVB( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object     : A handle for (pointer to) the storage object whose
//                    old name you want.
//
// RETURNS
//
//  A string pointer (or VB string) containing the old file name.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to call the
//  ALName::GetSafeOldName() C++ member function for the mName data
//  member of class ALStorage.  This function checks the handle argument for
//  correct type (in debug mode), then casts and calls the C++ function.
//
//  Note that the VB version of this function is almost identical.  However,
//  instead of returning a pointer to a character string, this routine calls
//  ALVBCreateString() to build a VB string, which it returns to the
//  calling module.  Don't use the VB function from C, it will blow up.
//  Don't use the C function from VB, because it returns a string pointer,
//  which VB doesn't know how to deal with.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALName::GetSafeOldName().  See OBJNAME.CPP for more information.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Combined a bunch of #ifdefs into a single test
//                          against AL_VB

extern "C" char AL_DLL_FAR * AL_FUNCTION
ALStorageGetOldName( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetOldName" );
    return (char AL_DLL_FAR *) ( (ALStorage *) this_object )->mName.GetSafeOldName();
}

#if defined( AL_VB )

extern "C" long AL_FUNCTION ALStorageGetOldNameVB( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetOldNameVB" );
    const char _far *p = ( (ALStorage *) this_object )->mName.GetSafeOldName();
    return ALCreateVBString( p, (unsigned short int) _fstrlen( p ) );
}

#endif

//
// extern "C" int ALStorageGetStatusCode( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object     : A handle for (pointer to) the storage object whose
//                    status code you want.
//
// RETURNS
//
//  The integer value of the status code.  Values of AL_SUCCESS are good,
//  values < AL_SUCCESS are bad.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to call the
//  ALStatus::GetStatusCode() C++ member function for the mStatus data
//  member of class ALStorage.  This function checks the handle argument for
//  correct type (in debug mode), then casts and calls the C++ function.
//
//  There really isn't too much to know about this function.  It just
//  returns the current integer status code to the calling routine.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageGetStatusCode( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetStatusCode" );
    return ( (ALStorage *) this_object)->mStatus.GetStatusCode();
}

//
// extern "C" int ALStorageSetError( hALArchive this_object,
//                                   int error,
//                                   char *text )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//                  We are going to set the object's status member
//                  so that it is in an error state.
//
//  error         : The error code to apply to the object.  Values from
//                  ALDEFS.H are good, but it really doesn't matter as
//                  long as you use a negative number.
//
//  text          : The text of the error message you want to associate with
//                  this error.
//
// RETURNS
//
//  Returns the error code that you passed it.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALName::SetError(), as applied to an ALStorage object.  For more
//  details on how the function actually works, check out OBJNAME.CPP.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageSetError( hALStorage this_object,
                                              int error,
                                              char AL_DLL_FAR *text )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageSetError" );
    ( (ALStorage *) this_object )->mStatus.SetError( error, text );
    return error;
}

// C TRANSLATION FUNCTION
//
// extern "C" char * ALStorageGetStatusString( hALStorage this_object )
//
// VB TRANSLATION FUNCTION
//
// extern "C" long ALStorageGetStatusStringVB( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object     : A handle for (pointer to) the storage object whose
//                    status string you want to get.
//
// RETURNS
//
//  A string pointer (or VB string) containing the status string.  This
//  is the short translation string, not the detailed message.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to call the
//  ALStatus::GetStatusString() C++ member function for the mStatus data
//  member of class ALStorage.  This function checks the handle argument for
//  correct type (in debug mode), then casts and calls the C++ function.
//
//  Note that the VB version of this function is almost identical.  However,
//  instead of returning a pointer to a character string, this routine calls
//  ALVBCreateString() to build a VB string, which it returns to the
//  calling module.  Don't use the VB function from C, it will blow up.
//  Don't use the C function from VB, because it returns a string pointer,
//  which VB doesn't know how to deal with.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALStatus::GetStatusString().  See STATUS.CPP for more information.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Combined a bunch of #ifdefs into a single test
//                          against AL_VB

extern "C" char AL_DLL_FAR * AL_FUNCTION
ALStorageGetStatusString( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetStatusString" );
    const char *status = ( (ALStorage *) this_object)->mStatus.GetStatusString();
    if ( status == 0 )
        status = "";
    return (char AL_DLL_FAR *) status;
}

#if defined( AL_VB )

extern "C" long AL_FUNCTION ALStorageGetStatusStringVB( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetStatusStringVB" );
    const char _far *status = ( (ALStorage *) this_object)->mStatus.GetStatusString();
    if ( status == 0 )
        status = "";
    return ALCreateVBString( status, (unsigned short int) _fstrlen( status ) );
}

#endif

// C TRANSLATION FUNCTION
//
// extern "C" char * ALStorageGetStatusDetail( hALStorage this_object )
//
// VB TRANSLATION FUNCTION
//
// extern "C" long ALStorageGetStatusDetailVB( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object     : A handle for (pointer to) the storage object whose
//                    status detail string you want to get.
//
// RETURNS
//
//  A string pointer (or VB string) containing the status string.  This
//  is the detailed status message, not the short translated string.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that