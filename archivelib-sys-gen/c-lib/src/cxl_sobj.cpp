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
//  This is the C/VB translation routine that allows you to call the
//  ALStatus::GetStatusDetail() C++ member function for the mStatus data
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
//  of ALStatus::GetStatusDetail().  See STATUS.CPP for more information.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Combined a bunch of #ifdefs into a single test
//                          against AL_VB

extern "C" char AL_DLL_FAR * AL_FUNCTION
ALStorageGetStatusDetail( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetStatusDetail" );
    const char *status = ( (ALStorage *) this_object )->mStatus.GetStatusDetail();
    if ( status == 0 )
        status = "";
    return (char AL_DLL_FAR *) status;
}

#if defined( AL_VB )

extern "C" long AL_FUNCTION 
ALStorageGetStatusDetailVB( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetStatusDetailVB" );
    const char _far *status = ( (ALStorage *) this_object)->mStatus.GetStatusDetail();
    if ( status == 0 )
        status = "";
    return ALCreateVBString( status, (unsigned short int) _fstrlen( status ) );
}

#endif

//
// extern "C" long ALStorageGetUnixTime( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//                  We are going to retrieve the time/date stamp
//                  for this object.
// RETURNS
//
//  The time/date of the object in unix format.  Unix format is
//  the format that is actually stored in an archive directory
//  to record the object's time stamp.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALTimeDate::GetUnixTime(), as applied the mTimeDate data member
//  of the ALStorage object.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
//  You won't learn much about much about the way ALTimeDate::GetUnixTime()
//  works by looking at this function.  See TIMEDATE.CPP for the 
//  details.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" long AL_FUNCTION
ALStorageGetUnixTime( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetUnixTime" );
    return ( (ALStorage *) this_object)->mTimeDate.GetUnixTime();
}

//
// extern "C" long ALStorageToJulian( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//                  We are going to retrieve the time/date stamp
//                  for this object, in Julian date format.
// RETURNS
//
//  The date of the object in julian date format.  
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALTimeDate::ToJulian(), as applied the mTimeDate data member
//  of the ALStorage object.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.  
//
//  You won't learn much about much about the way ALTimeDate::ToJulian()
//  works by looking at this function.  See TIMEDATE.CPP for the 
//  details.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" long AL_FUNCTION
ALStorageToJulian( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageToJulian" );
    return ( (ALStorage *) this_object)->mTimeDate.ToJulian();
}

//
// extern "C" void ALStorageFromJulian( hALStorage this_object, long jdn )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//                  We are going to set the date stamp
//                  for this object, using Julian date format.
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALTimeDate::FromJulian(), as applied the mTimeDate data member
//  of the ALStorage object.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.  
//
//  You won't learn much about much about the way ALTimeDate::FromJulian()
//  works by looking at this function.  See TIMEDATE.CPP for the 
//  details.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION
ALStorageFromJulian( hALStorage this_object, long jdn )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageFromJulian" );
    ( (ALStorage *) this_object)->mTimeDate.FromJulian( jdn );
}

//
// extern "C" void 
// ALStorageSetTimeDateFromStruc( hALStorage this_object, 
//                                struct tm AL_DLL_FAR * time_struct )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//                  We are going to set the time/date stamp
//                  for this object, using C RTL time_struct format
//                  as our source.
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALTimeDate::SetTimeDate(), as applied the mTimeDate data member
//  of the ALStorage object.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.  
//
//  You won't learn much about much about the way ALTimeDate::SetTimeDate()
//  works by looking at this function.  See TIMEDATE.CPP for the
//  details.
//
//  I don't think this function will be of any value to VB programmers.
//  The only place to get this time date structure is from the C run
//  time library.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION 
ALStorageSetTimeDateFromStruc( hALStorage this_object, 
                               struct tm AL_DLL_FAR * time_struct )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageSetTimeDateFromStruc" );
    AL_ASSERT( time_struct != 0, "Null tm struct passed to ALStorageSetTimeDateFromStruc" );
    ( (ALStorage *) this_object )->mTimeDate.SetTimeDate( time_struct );
}

//
// extern "C" void 
// ALStorageGetStrucFromTimeDate( hALStorage this_object, 
//                                struct tm * time_struct )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//                  We are going to get the time/date stamp
//                  for this object, with the result going into
//                  the time date struct used by the C run time library.
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALTimeDate::GetTimeDate(), as applied the mTimeDate data member
//  of the ALStorage object.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.  
//
//  You won't learn much about much about the way ALTimeDate::GetTimeDate()
//  works by looking at this function.  See TIMEDATE.CPP for the 
//  details.
//
//  I don't think this function will be of any value to VB programmers.
//  The only place to get this time date structure is from the C run
//  time library.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION 
ALStorageGetStrucFromTimeDate( hALStorage this_object,
                               struct tm AL_DLL_FAR * time_struct )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetStrucFromTimeDate" );
    AL_ASSERT( time_struct != 0, "Null tm struct passed to ALStorageGetStrucFromTimeDate" );
    ( (ALStorage *) this_object )->mTimeDate.GetTimeDate( time_struct );
}

//
// extern "C" void 
// ALStorageSetTimeDateFromUnix( hALStorage this_object, 
//                               long unix_time )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//                  We are going to set the time/date stamp
//                  for this object, using the Unix format time/date
//                  format as a source.
//
//  unix_time     : The time date in UNIX format.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALTimeDate::SetTimeDate(), as applied to the mTimeDate data member
//  of the ALStorage object.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.  
//
//  You won't learn much about much about the way ALTimeDate::SetTimeDate()
//  works by looking at this function.  See TIMEDATE.CPP for the 
//  details.
//
//  The UNIX time/date format is what ArchiveLib uses internally in archives
//  created using ALArchiveBase.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION 
ALStorageSetTimeDateFromUnix( hALStorage this_object, long unix_time )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageSetTimeDateFromUnix" );
    ( (ALStorage *) this_object )->mTimeDate.SetTimeDate( unix_time );
}

//
// extern "C" int ALStorageIsOpen( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
// RETURNS
//
//  1 if the object is open, 0 if it is closed.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::IsOpen().
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), and a call is made to the member function.
//  The integer result is returned to the calling procedure as is.
//  
//  You won't learn much about much about the way ALStorage::IsOpen()
//  works by looking at this function.  See STORAGE.CPP for the 
//  details.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageIsOpen( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageIsOpen" );
    return ( (ALStorage *) this_object )->IsOpen();
}

//
// extern "C" int ALStorageReadChar( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
// RETURNS
//
//  A byte from 0 to 255 if any valid characters are present, 
//  AL_END_OF_FILE if there aren't any, or an error code < AL_SUCCESS
//  if the file is now in an error state.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::ReadChar().
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), and a call is made to the member function.
//  The integer result is returned to the calling procedure as is.
//  
//  You won't learn much about much about the way ALStorage::ReadChar()
//  works by looking at this function.  See STORAGE.H for the 
//  details.
//  
//  In C++, this function is implemented as an inline function, and it is
//  pretty fast.  Normally it just has to get a byte from the I/O buffer.
//  This isn't the case when using this translation function, so you lose
//  some efficiency.  The best way to solve the efficiency problem is to
//  try to structure your code to use ReadBuffer() instead.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageReadChar( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageReadChar" );
    return ( (ALStorage *) this_object )->ReadChar();
}

//
// extern "C" int ALStorageReadShort( hALStorage this_object, 
//                                    short int *data )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
//  data          : Pointer to a short int that is going to receive
//                  the short int read in from the storage object.
//
// RETURNS
//
//  AL_SUCCESS if the short int was read in properly.  An error code
//  < AL_SUCCESS if something bad happened.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::ReadPortableShort().
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), and a call is made to the member function.
//  The integer result is returned to the calling procedure as is.
//  
//  You won't learn much about much about the way 
//  ALStorage::ReadPortableShort()works by looking at this function.  
//  See STORAGE.CPP for the details.
//  
//  This function is designed for reading data that needs to be stored
//  in a system independent fashion. 
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION 
ALStorageReadShort( hALStorage this_object, short int AL_DLL_FAR *data )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageReadShort" );
    AL_ASSERT( data != 0, "Null pointer passed to function" );
    return ( (ALStorage *) this_object )->ReadPortableShort( *data );
}

//
// extern "C" int ALStorageReadLong( hALStorage this_object, 
//                                   long *data )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
//  data          : Pointer to a long int that is going to receive
//                  the long int read in from the storage object.
//
// RETURNS
//
//  AL_SUCCESS if the long int was read in properly.  An error code
//  < AL_SUCCESS if something bad happened.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::ReadPortableLong().
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), and a call is made to the member function.
//  The integer result is returned to the calling procedure as is.
//  
//  You won't learn much about much about the way 
//  ALStorage::ReadPortableLong()works by looking at this function.  
//  See STORAGE.CPP for the details.
//  
//  This function is designed for reading data that needs to be stored
//  in a system independent fashion. 
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION 
ALStorageReadLong( hALStorage this_object, long AL_DLL_FAR *data )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageReadLong" );
    AL_ASSERT( data != 0, "Null pointer passed to function" );
    return ( (ALStorage *) this_object )->ReadPortableLong( *data );
}

//
// extern "C" int ALStorageWriteLong( hALStorage this_object, 
//                                    long data )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
//  data          : A long int that is going to be written out to
//                  to the storage object in a system independent manner.
//
// RETURNS
//
//  AL_SUCCESS if the long int was written out properly.  An error code
//  < AL_SUCCESS if something bad happened.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::WritePortableLong().
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), and a call is made to the member function.
//  The integer result is returned to the calling procedure as is.
//  
//  You won't learn much about much about the way 
//  ALStorage::WritePortableLong()works by looking at this function.  
//  See STORAGE.CPP for the details.
//  
//  This function is designed for writing data that needs to be stored
//  in a system independent fashion. 
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageWriteLong( hALStorage this_object,
                                               long data )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageWriteLong" );
    return ( (ALStorage *) this_object)->WritePortableLong( data );
}

//
// extern "C" int ALStorageWriteShort( hALStorage this_object, 
//                                     short data )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
//  data          : A short int that is going to be written out to
//                  to the storage object in a system independent manner.
//
// RETURNS
//
//  AL_SUCCESS if the short int was written out properly.  An error code
//  < AL_SUCCESS if something bad happened.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::WritePortableShort().
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), and a call is made to the member function.
//  The integer result is returned to the calling procedure as is.
//  
//  You won't learn much about much about the way 
//  ALStorage::WritePortableShort()works by looking at this function.  
//  See STORAGE.CPP for the details.
//  
//  This function is designed for writing data that needs to be stored
//  in a system independent fashion. 
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageWriteShort( hALStorage this_object, 
                                                short int data )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageWriteShort" );
    return ( (ALStorage *) this_object)->WritePortableShort( data );
}

//
// extern "C" int ALStorageWriteString( hALStorage this_object, 
//                                      char *string )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
//  string        : A string that we are going to write out to the
//                  storage object in a system independent manner.
//
// RETURNS
//
//  AL_SUCCESS if the string was written out properly.  An error code
//  < AL_SUCCESS if something bad happened.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::WriteString().
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), and a call is made to the member function.
//  The integer result is returned to the calling procedure as is.
//  
//  You won't learn much about much about the way 
//  ALStorage::WriteString()works by looking at this function.  
//  See STORAGE.CPP for the details.
//  
//  This function is designed for writing variable length blocks of data
//  to archives in a consistent fashion.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageWriteString( hALStorage this_object, 
                                                 char AL_DLL_FAR *string )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageWriteString" );
    AL_ASSERT( string != 0, "Passing null string in ALStorageWriteString()" );
    return ( (ALStorage *) this_object )->WriteString( string );
}

//
// extern "C" void ALStorageInitCrc32( hALStorage this_object, 
//                                     long seed )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
//  seed          : The initial value for the 32 bit CRC.  In C++, the
//                  default parameter is 0xffffffffL.  This is a good
//                  one to use, and it gives you ARJ/PKWARE compatibility.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::InitCrc32().
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), and a call is made to the member function.
//  
//  You won't learn much about much about the way
//  ALStorage::InitCrc32()works by looking at this function.  
//  See STORAGE.CPP for the details.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION ALStorageInitCrc32( hALStorage this_object, 
                                                long seed )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageInitCrc32" );
    ( (ALStorage *) this_object )->InitCrc32( seed );
}                                                  

//
// extern "C" long ALStorageTell( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
// RETURNS
//
//  A long value indicating where the read/write pointer to the storage
//  object is presently located.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::Tell().  It works a lot like the C RTL tell() or
//  ftell() functions.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), the handle is cast to the correct type, and
//  a call is made to the C++ member function.  The long result from
//  the member function is returned unchanged to the calling C or VB 
//  procedure.
//  
//  You won't learn much about much about the way
//  ALStorage::Tell()works by looking at this function.  
//  See STORAGE.CPP for the details.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" long AL_FUNCTION ALStorageTell( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageTell" );
    return ( (ALStorage *) this_object )->Tell();
}

//
// extern "C" size_t
// ALStorageReadBuffer( hALStorage this_object,
//                      unsigned char *buffer,
//                      size_t length )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
//  buffer        : A pointer to a buffer. Specified as unsigned
//                  char, but the type doesn't really matter.
//
//  length        : The number of bytes you want to read into the buffer.
//
// RETURNS
//
//  A number >= 0 indicates that number of byte were read into the buffer.
//  A number <0 is a status code corresponding to an error.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::ReadBuffer().  It works a lot like the C RTL read() or
//  fread() functions.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), the handle is cast to the correct type, and
//  a call is made to the C++ member function.  The short int result from
//  the member function is returned unchanged to the calling C or VB 
//  procedure.
//  
//  You won't learn much about much about the way 
//  ALStorage::ReadBuffer()works by looking at this function.  
//  See STORAGE.CPP for the details.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" size_t AL_FUNCTION 
ALStorageReadBuffer( hALStorage this_object,
                     unsigned char AL_DLL_FAR *buffer,
                     size_t length )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageReadBuffer" );
    AL_ASSERT( buffer != 0, "Passed a null buffer to ALStorageReadBuffer" );
    return ( (ALStorage * ) this_object )->ReadBuffer( buffer, length );
}

//
// extern "C" size_t
// ALStorageWriteBuffer( hALStorage this_object, 
//                      unsigned char *buffer,
//                      size_t length )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
//  buffer        : A pointer to a buffer. Specified as unsigned
//                  char, but the type doesn't really matter.
//
//  length        : The number of bytes you want to write from the buffer.
//
// RETURNS
//
//  A number >= 0 indicates that number of bytes were written to the object.
//  A number <0 is a status code corresponding to an error.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::WriteBuffer().  It works a lot like the C RTL write() or
//  fwrite() functions.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), the handle is cast to the correct type, and
//  a call is made to the C++ member function.  The short int result from
//  the member function is returned unchanged to the calling C or VB 
//  procedure.
//  
//  You won't learn much about much about the way 
//  ALStorage::WriteBuffer()works by looking at this function.  
//  See STORAGE.CPP for the details.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" size_t AL_FUNCTION 
ALStorageWriteBuffer( hALStorage this_object, 
                      unsigned char AL_DLL_FAR *buffer,
                      size_t length )
                                         
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageWriteBuffer" );
    AL_ASSERT( buffer != 0, "Passed a null buffer to ALStorageWriteBuffer" );
    return ( (ALStorage * ) this_object )->WriteBuffer( buffer, length );
}

//
// extern "C" long ALStorageGetCrc32( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
// RETURNS
//
//  A long value corresponding to the current CRC-32 of the storage
//  object.  A value of -1 indicates that the CRC is unknown.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::GetCrc32().  This is just a simple access routine
//  used to get the value of a private data member.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), the handle is cast to the correct type, and
//  a call is made to the C++ member function.  The long result from
//  the member function is returned unchanged to the calling C or VB 
//  procedure.
//  
//  You won't learn much about much about the way 
//  ALStorage::GetCrc32()works by looking at this function.  
//  See STORAGE.CPP for the details.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" long AL_FUNCTION ALStorageGetCrc32( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetCrc32" );
    return ( (ALStorage *) this_object )->GetCrc32();
}

//
// extern "C" int ALStorageClose( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
// RETURNS
//
//  A value of AL_SUCCESS if the object was closed properly.  A value
//  of < AL_SUCCESS indicates an error state of some sort.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::Close().  This is virtual function that will generally
//  be implemented by a derived class.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), the handle is cast to the correct type, and
//  a call is made to the C++ member function.  The int result from
//  the member function is returned unchanged to the calling C or VB 
//  procedure.
//
//  You won't learn much about much about the way 
//  ALStorage::Close()works by looking at this function.  
//  See MEMSTORE.CPP or FILESTOR.CPP for the details.  The base
//  class version in STORAGE.CPP has good information as well.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageClose( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageClose" );
    return ( (ALStorage *) this_object )->Close();
}

//
// extern "C" int ALStorageCreate( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
// RETURNS
//
//  A value of AL_SUCCESS if the object was created properly.  A value
//  of < AL_SUCCESS indicates an error state of some sort.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::Create().  This is virtual function that will generally
//  be implemented by a derived class.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), the handle is cast to the correct type, and
//  a call is made to the C++ member function.  The int result from
//  the member function is returned unchanged to the calling C or VB 
//  procedure.
//
//  You won't learn much about much about the way 
//  ALStorage::Create()works by looking at this function.  
//  See MEMSTORE.CPP or FILESTOR.CPP for the details.  The base
//  class version in STORAGE.CPP has good information as well.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageCreate( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageCreate" );
    return ( (ALStorage *) this_object )->Create();
}

//
// extern "C" int ALStorageFlushBuffer( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
// RETURNS
//
//  A value of AL_SUCCESS if flush operation worked properly.  A return 
//  of < AL_SUCCESS indicates an error state of some sort.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::FlushBuffer().  This is virtual function that will generally
//  be implemented by a derived class.  FlushBuffer() is used internally
//  by ALStorage to flush the buffer when a write operation overflows
//  the I/O buffer.  It can be called externally to force the buffer out
//  to the physical storage medium.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), the handle is cast to the correct type, and
//  a call is made to the C++ member function.  The int result from
//  the member function is returned unchanged to the calling C or VB 
//  procedure.
//  
//  You won't learn much about much about the way 
//  ALStorage::FlushBuffer()works by looking at this function.  
//  See MEMSTORE.CPP or FILESTOR.CPP for the details.  The base
//  class version in STORAGE.CPP has good information as well.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//
extern "C" int AL_FUNCTION ALStorageFlushBuffer( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageFlushBuffer" );
    return ( (ALStorage *) this_object )->FlushBuffer();
}

//
// extern "C" int ALStorageLoadBuffer( hALStorage this_object,
//                                     long address )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
//  address       : The address that we want to load data from.
//
// RETURNS
//
//  An integer >= 0 if the load operation worked properly.  A return 
//  of < AL_SUCCESS indicates an error state of some sort.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::LoadBuffer().  This is virtual function that will generally
//  be implemented by a derived class.  LoadBuffer() is used internally
//  by ALStorage to refresh the I/O buffer when a read operation underflows
//  the I/O buffer.  It can be called externally to force a load from a
//  specific address, equivalent to a Seek() call.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), the handle is cast to the correct type, and
//  a call is made to the C++ member function.  The int result from
//  the member function is returned unchanged to the calling C or VB 
//  procedure.
//  
//  You won't learn much about much about the way 
//  ALStorage::LoadBuffer()works by looking at this function.
//  See MEMSTORE.CPP or FILESTOR.CPP for the details.  The base
//  class version in STORAGE.CPP has good information as well.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageLoadBuffer( hALStorage this_object,
                                                long address )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageLoadBuffer" );
    return ( (ALStorage *) this_object )->LoadBuffer( address );
}

//
// extern "C" int ALStorageOpen( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
// RETURNS
//
//  AL_SUCCESS if the open operation worked properly.  A return 
//  of < AL_SUCCESS indicates an error state of some sort.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::Open().  This is virtual function that will generally
//  be implemented by a derived class.  Open() opens the physical storage
//  device associated with the ALStorageObject, and reads any storage
//  time date stamp and other attributes.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), the handle is cast to the correct type, and
//  a call is made to the C++ member function.  The int result from
//  the member function is returned unchanged to the calling C or VB 
//  procedure.
//  
//  You won't learn much about much about the way ALStorage::Open()
//  works by looking at this function.  See MEMSTORE.CPP or FILESTOR.CPP 
//  for the details.  The base class version in STORAGE.CPP also has 
//  some useful information.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageOpen( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageOpen" );
    return ( (ALStorage *) this_object )->Open();
}

//
// extern "C" int ALStorageRename( hALStorage this_object, 
//                                 char *new_name, 
//                                 int delete_on_clash )
//
// ARGUMENTS:
//
//  this_object      : A handle for (pointer to) an ALStorage object.
//
//  new_name         : The new name you want to assign to the storage
//                     object.  You can pass a parameter of 0 here to 
//                     indicate that you have already assigned the new
//                     name to the mName member of the storage object.
//
//  delete_on_clash  : If it turns out that the storage object name is
//                     already in use, you have a clash.  This can happen,
//                     for example, when renaming a file to a backup.
//                     Setting this flag indicates that if there is a clash,
//                     you want to delete the clashing file and let
//                     your object use the name.
//
// RETURNS
//
//  AL_SUCCESS if the rename operation worked properly.  AL_RENAME_ERROR
//  may be returned by derived classes if specific problems occur.
//  Additional error codes < AL_SUCCESS may be returned if some other
//  error was detected.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::Rename().  This is virtual function that will generally
//  be implemented by a derived class.  Rename() operates on the operating
//  system level to reassign the name of an object, such as a file.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), the handle is cast to the correct type, and
//  a call is made to the C++ member function.  The int result from
//  the member function is returned unchanged to the calling C or VB 
//  procedure.
//  
//  You won't learn much about much about the way ALStorage::Rename()
//  works by looking at this function.  See FILESTOR.CPP for the details.  
//  MEMSTORE.CPP doesn't really have any useful info, since ALMemory 
//  objects don't have a name defined at the O/S level.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageRename( hALStorage this_object, 
                                            char *new_name,
                                            int delete_on_clash )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageRename" );
    AL_ASSERT( new_name != 0, "NULL new_name in ALStorageRename" );
    return ( (ALStorage *) this_object )->Rename( new_name, delete_on_clash );
}                                             

//
// extern "C" int ALStorageRenameToBackup( hALStorage this_object, 
//                                         int delete_on_clash )
//
// ARGUMENTS:
//
//  this_object      : A handle for (pointer to) an ALStorage object.
//
//  delete_on_clash  : If it turns out that the new storage name is
//                     already in use, you have a clash.  This can happen,
//                     for example, when renaming a file to a backup.
//                     Setting this flag indicates that if there is a clash,
//                     you want to delete the clashing file and let
//                     your object use the name.
//
// RETURNS
//
//  AL_SUCCESS if the rename operation worked properly.  AL_RENAME_ERROR
//  may be returned by derived classes if specific problems occur.
//  Additional error codes < AL_SUCCESS may be returned if some other
//  error was detected.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::RenameToBackup().  This is virtual function that will 
//  generally be implemented by a derived class.  RenameToBackup() 
//  operates on the operating system level to reassign the name of an object, such as a file.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), the handle is cast to the correct type, and
//  a call is made to the C++ member function.  The int result from
//  the member function is returned unchanged to the calling C or VB
//  procedure.
//  
//  You won't learn much about much about the way ALStorage::RenameToBackup()
//  works by looking at this function.  See FILESTOR.CPP for the details.  
//  MEMSTORE.CPP doesn't really have any useful info, since ALMemory 
//  objects don't have a name defined at the O/S level.  It just approves of
//  all name change requests.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageRenameToBackup( hALStorage this_object, 
                                                    int delete_on_clash )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageRenameToBackup" );
    return ( (ALStorage *) this_object )->RenameToBackup( delete_on_clash );
}                                                     

//
// extern "C" int ALStorageSeek( hALStorage this_object,
//                               long address )
//
// ARGUMENTS:
//
//  this_object      : A handle for (pointer to) an ALStorage object.
//
//  address          : The address to seek to within the storage object.
//
// RETURNS
//
//  AL_SUCCESS if the seek operation worked properly.  AL_SEEK_ERROR
//  may be returned by derived classes if specific problems occur.
//  Additional error codes < AL_SUCCESS may be returned if some other
//  error was detected.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::Seek().  This is virtual function that will 
//  generally be implemented by a derived class.  ALStorage::Seek() often
//  operates on the operating system level to seek to a position in a
//  physical or O/S object.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), the handle is cast to the correct type, and
//  a call is made to the C++ member function.  The int result from
//  the member function is returned unchanged to the calling C or VB 
//  procedure.
//  
//  You won't learn much about much about the way ALStorage::Seek()
//  works by looking at this function.  See FILESTOR.CPP and MEMSTORE.CPP
//  for some insight.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//
extern "C" int AL_FUNCTION ALStorageSeek( hALStorage this_object,
                                          long address )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageSeek" );
    return ( (ALStorage *) this_object )->Seek( address );
}
                                           
//
// extern "C" int ALStorageUnRename( hALStorage this_object, 
//                                   int delete_on_clash )
//
// ARGUMENTS:
//
//  this_object      : A handle for (pointer to) an ALStorage object.
//
//  delete_on_clash  : If it turns out that the new storage name is
//                     already in use, you have a clash.  This can happen,
//                     for example, when renaming a file to a backup.
//                     Setting this flag indicates that if there is a clash,
//                     you want to delete the clashing file and let
//                     your object use the name.
//
// RETURNS
//
//  AL_SUCCESS if the rename operation worked properly.  AL_RENAME_ERROR
//  may be returned by derived classes if specific problems occur.
//  Additional error codes < AL_SUCCESS may be returned if some other
//  error was detected.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::UnRename().  This is virtual function that will 
//  generally be implemented by a derived class.  ALStorage::UnRename() 
//  operates on the operating system level to reassign the name of an object, 
//  such as a file.
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), the handle is cast to the correct type, and
//  a call is made to the C++ member function.  The int result from
//  the member function is returned unchanged to the calling C or VB
//  procedure.
//  
//  You won't learn much about much about the way ALStorage::UnRename()
//  works by looking at this function.  See FILESTOR.CPP for the details.  
//  MEMSTORE.CPP doesn't really have any useful info, since ALMemory 
//  objects don't have a name defined at the O/S level.  It just approves of
//  all name change requests.
//  
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageUnRename( hALStorage this_object, 
                                              int delete_on_clash )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageUnRename" );
    return ( (ALStorage *) this_object )->UnRename( delete_on_clash );
}

//
// extern "C" int ALStorageWriteChar( hALStorage this_object, 
//                                    int c )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALStorage object.
//
//  c             : A character to write.
//
// RETURNS
//
//  A byte from 0 to 255 if the write was successful.  Various error
//  codes < AL_SUCCESS if the write failed.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALStorage::WriteChar().
//
//  All that happens here is that the handle argument is checked for correct
//  type (when in debug mode), and a call is made to the member function.
//  The integer result is returned to the calling procedure as is.
//  
//  You won't learn much about much about the way ALStorage:WriteChar()
//  works by looking at this function.  See STORAGE.H for the 
//  details.
//  
//  In C++, this function is implemented as an inline function, and it is
//  pretty fast.  Normally it just has to put a byte into the I/O buffer.
//  This isn't the case when using this translation function, so you lose
//  some efficiency.  The best way to solve the efficiency problem is to
//  try to structure your code to use WriteBuffer() instead.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageWriteChar( hALStorage this_object,
                                               int c )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageWriteChar" );
    return ( (ALStorage *) this_object )->WriteChar( c );
}

//
// extern "C" int ALStorageGetType( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) the storage object that
//                 you want to get the type of.
//
// RETURNS
//
//  The integer storage type, as defined in ALDEFS.H.  AL_MEMORY_OBJECT
//  and AL_FILE_OBJECT are popular choices. AL_UNDEFINED is possible
//  as well.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to access the
//  C++ data member ALStorage::miStorageObjectType. This function checks 
//  its handle argument for correct type (in debug mode), then casts
//  and accesses ALStorage::miStorageObjectType.  It returns the integer
//  result back to the calling procedure unchanged.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALStorage::miStorageObjectType. See STORAGE.H for the details on that.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageGetType( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetType" );
    return ( (ALStorage *) this_object )->miStorageObjectType;
}

//
// extern "C" unsigned short int
// ALStorageGetDosTime( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) the storage object that
//                 you want to get the DOS time for.
//
// RETURNS
//
//  An unsigned int that has the file access time in the format
//  used by _dos_setfileattr().
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to access the
//  C++ member function ALTimeDate::GetDosTime for the mTimeDate
//  member of class ALStorage. This function checks its handle argument for 
//  correct type (in debug mode), then casts and calls 
//  ALTimeDate::GetDosTime.  It returns the short integer result back 
//  to the calling procedure unchanged.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALTimeDate::GetDosTime. See TIMEDATE.CPP for the details on that.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : This function doesn't work very well under UNIX,
//                          so I had to add some #ifdef stuff to exclude
//                          it when it wasn't wanted.

#if !defined( AL_WIN32S ) && !defined( AL_UNIX )

extern "C" unsigned short int AL_FUNCTION
ALStorageGetDosTime( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetDosTime" );
    return ( (ALStorage *) this_object )->mTimeDate.GetDosTime();
}

#endif

//
// extern "C" unsigned short int
// ALStorageGetDosDate( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) the storage object that
//                 you want to get the DOS date for.
//
// RETURNS
//
//  An unsigned int that has the file access date in the format
//  used by _dos_setfileattr().
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to access the
//  C++ member function ALTimeDate::GetDosDate for the mTimeDate
//  member of class ALStorage. This function checks its handle argument for 
//  correct type (in debug mode), then casts and calls
//  ALTimeDate::GetDosDate.  It returns the short integer result back 
//  to the calling procedure unchanged.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALTimeDate::GetDosDate. See TIMEDATE.CPP for the details on that.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : This function doesn't work very well under UNIX,
//                          so I had to add some #ifdef stuff to exclude
//                          it when it wasn't wanted.

#if !defined( AL_WIN32S ) && !defined( AL_UNIX )

extern "C" unsigned short int AL_FUNCTION
ALStorageGetDosDate( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetDosDate" );
    return ( (ALStorage *) this_object )->mTimeDate.GetDosDate();
}

#endif

//
// extern "C" int ALStorageValidTimeDate( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) the storage object that
//                 you want to check for validity.
//
// RETURNS
//
//  A true or false, to indicate whether a valid time has been
//  loaded into the mTimeDate member of the ALStorage object.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to access the
//  C++ member function ALTimeDate::Valid() for the mTimeDate
//  member of class ALStorage. This function checks its handle argument for
//  correct type (in debug mode), then casts and calls
//  ALTimeDate::Valid().  It returns the integer result back
//  to the calling procedure unchanged.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALTimeDate::Valid(). See TIMEDATE.H for the details on that.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALStorageValidTimeDate( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageValidTimeDate" );
    return ( (ALStorage *) this_object )->mTimeDate.Valid();
}


//
// extern "C" DWORD ALStorageGetWin32Attributes( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) the storage object that
//                 you want to get the Win32s file attributes for.
//
// RETURNS
//
//  A DWORD containing the Win32s attributes for the selected
//  storage object.  The attributes are in the format needed
//  by the SetFileAttributes() function.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to access the
//  C++ member function ALFileAttributes::GetWin32Attributes() for the
//  mAttributes member of class ALStorage. This function checks its handle
//  argument for correct type (in debug mode), then casts and calls
//  ALAttributes::GetWin32Attributes().  It returns the long result back
//  to the calling procedure unchanged.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALAttributes::GetWin32Attributes). See FILEATTR.CPP for the details
//  on that.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A  : First release
//

#if defined( AL_WIN32S )
extern "C" DWORD AL_FUNCTION
ALStorageGetWin32Attributes( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetWin32Attributes" );
    return ( (ALStorage *) this_object )->mAttributes.GetWin32Attributes();
}
#endif

//
// extern "C" unsigned short int
// ALStorageGetDosAttributes( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) the storage object that
//                 you want to get the DOS file attributes for.
//
// RETURNS
//
//  A short int containing the DOS attributes for the selected
//  storage object.  The attributes are returned in the format
//  needed by the _dos_setfileattr() function.
//
// DESCRIPTION
//
//  This is the C/VB translation routine that allows you to access the
//  C++ member function ALFileAttributes::GetDosAttributes() for the 
//  mAttributes member of class ALStorage. This function checks its handle
//  argument for correct type (in debug mode), then casts and calls
//  ALAttributes::GetDosAttributes().  It returns the short result back
//  to the calling procedure unchanged.
//
//  The C/VB translation code doesn't offer much insight into the operation
//  of ALAttributes::GetDosAttributes). See FILEATTR.CPP for the details 
//  on that.
//
// REVISION HISTORY
//
//   May 25, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : This function doesn't work very well under UNIX,
//                          so I had to add some #ifdef stuff to exclude
//                          it when it wasn't wanted.

#if !defined( AL_WIN32S ) && !defined( AL_UNIX )
extern "C" unsigned short int AL_FUNCTION
ALStorageGetDosAttributes( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetDosAttributes" );
    return ( (ALStorage *) this_object )->mAttributes.GetDosAttributes();
}
#endif

//
// extern "C" mode_t
// ALStorageGetUnixAttributes( hALStorage this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) the storage object that
//                 you want to get the Unix file attributes for.
//
// RETURNS
//
//  A mode_t word containing the file protection bits packed
//  into Unix format.
//
// DESCRIPTION
//
//  This is the C translation routine that allows you to access the
//  C++ member function ALFileAttributes::GetUnixAttributes() for the
//  mAttributes member of class ALStorage. This function checks its handle
//  argument for correct type (in debug mode), then casts and calls
//  ALAttributes::GetUnixAttributes().  It returns the mode_t result back
//  to the calling procedure unchanged.
//
//  The C translation code doesn't offer much insight into the operation
//  of ALAttributes::GetDosAttributes). See FILEATTR.CPP for the details
//  on that.
//
// REVISION HISTORY
//
//   August 10, 1994 1.0B   : First release
//

#if defined( AL_UNIX )

extern "C" mode_t AL_FUNCTION
ALStorageGetUnixAttributes( hALStorage this_object )
{
    AL_ASSERT_OBJECT( this_object, ALStorage, "ALStorageGetUnixAttributes" );
    return ( (ALStorage *) this_object )->mAttributes.GetUnixAttributes();
}

#endif

