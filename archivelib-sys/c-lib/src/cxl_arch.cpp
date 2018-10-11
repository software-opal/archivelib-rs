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
// extern "C" int ALArchiveExtract( hALArchive this_object,
//                                  hALEntryList list )
//
// ARGUMENTS:
//
//  this_object  : The handle of (pointer to) an ALArchiveBase object.
//
//  list         : The handle of (pointer to) an ALEntryList object.
//
// RETURNS
//
//  A status code, AL_SUCCESS or < AL_SUCCESS in case of an error.
//
// DESCRIPTION
//
//  This is the wrapper function for ALArchiveBase::Extract().  See the
//  function in ARCHIVEB.CPP for more details.
//
// REVISION HISTORY
//
//   May 22, 1994  2.0A  : First release
//
extern "C" int AL_FUNCTION ALArchiveExtract( hALArchive this_object, hALEntryList list )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase, "ALArchiveExtract" );
    AL_ASSERT_OBJECT( list, ALEntryList, "ALArchiveExtract" );
    return ((ALArchiveBase *) this_object )->Extract( *( (ALEntryList *) list ) );
}

//
// extern "C" int ALArchiveCreate( hALArchive this_object,
//                                 hALEntryList list )
//
// ARGUMENTS:
//
//  this_object  : The handle of (pointer to) an ALArchiveBase object.
//
//  list         : The handle of (pointer to) an ALEntryList object.
//
// RETURNS
//
//  AL_SUCCESS for successful creation, < AL_SUCCESS for a failure.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function that supports the
//  ALArchiveBase::Create(ALEntryList&) function.  For details on the
//  internals, see ARCHIVEB.CPP.  Note that the two arguments
//  are checked for correct type.  The second version of
//  ALArchiveBase::Create() is found below.
//
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALArchiveCreate( hALArchive this_object, hALEntryList list )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase, "ALArchiveCreate" );
    AL_ASSERT_OBJECT( list, ALEntryList, "ALArchiveCreate" );
    return ((ALArchiveBase *) this_object )->Create( *( (ALEntryList *) list ) );
}

//
// extern "C" int ALArchiveCreateFromArchive( hALArchive this_object,
//                                            hALArchive input_archive,
//                                            hALEntryList list )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  This is the archive that is going to be created.
//
//  input_archive : A handle for (pointer to) an ALArchiveBase object.
//                  This is the archive that contains the compressed
//                  objects that are going to be inserted.
//
//  list          : A handle for (pointer to) an ALEntryList object.
//                  It will contains descriptions of all the compressed
//                  objects in input_archive that are going to be inserted
//                  into this_object.
//
// RETURNS
//
//  AL_SUCCESS for successful creation, < AL_SUCCESS for a failure.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ function
//  ALArchiveBase::Create(ALArchiveBase&,ALEntrylist&).
//  For details on how the member function actually works, take a
//  gander at ARCHIVEB.CPP.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALArchiveCreateFromArchive( hALArchive this_object, hALArchive input_archive, hALEntryList list )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase, "ALArchiveCreateFromArchive" );
    AL_ASSERT_OBJECT( input_archive, ALArchiveBase, "ALArchiveCreateFromArchive" );
    AL_ASSERT_OBJECT( list, ALEntryList, "ALArchiveCreateFromArchive" );
    return ((ALArchiveBase *) this_object )->Create( *(ALArchiveBase *)input_archive, *( (ALEntryList *) list ) );
}

//
// extern "C" int ALArchiveAppend( hALArchive this_object,
//                                 hALEntryList list )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  This is the archive that is going to have new objects
//                  appended to it.
//
//  list          : A handle for (pointer to) an ALEntryList object.
//                  It will contains descriptions of all the standalone
//                  objects that are going to be inserted into this_object.
//
// RETURNS
//
//  AL_SUCCESS for a successful append, < AL_SUCCESS for a failure.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ function
//  ALArchiveBase::Append(ALEntrylist&). For details on how the member
//  function actually works, see ARCHIVEB.CPP.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//


extern "C" int AL_FUNCTION ALArchiveAppend( hALArchive this_object, hALEntryList list )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase, "ALArchiveAppend" );
    AL_ASSERT_OBJECT( list, ALEntryList, "ALArchiveAppend" );
    return ((ALArchiveBase *) this_object )->Append( *( (ALEntryList *) list ) );
}

//
// extern "C" int ALArchiveAppendFromArchive( hALArchive this_object,
//                                            hALArchive input_archive,
//                                            hALEntryList list )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  This is the archive that is going to get new stuff
//                  appended to it.
//
//  input_archive : A handle for (pointer to) an ALArchiveBase object.
//                  This is the archive that contains the compressed
//                  objects that are going to be added to this_object.
//
//  list          : A handle for (pointer to) an ALEntryList object.
//                  It will contains descriptions of all the compressed
//                  objects in input_archive that are going to be added
//                  to this_object.
//
// RETURNS
//
//  AL_SUCCESS for a successful append, < AL_SUCCESS for a failure.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ function
//  ALArchiveBase::Append(ALArchiveBase&,ALEntrylist&).
//  For details on how the member function actually works, take a
//  look at ARCHIVEB.CPP.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALArchiveAppendFromArchive( hALArchive this_object, hALArchive input_archive, hALEntryList list )
{
    AL_ASSERT( ((ALArchiveBase *) this_object)->GoodTag(),
               "archive argument is not a valid ALArchiveBase" );
    AL_ASSERT( ((ALArchiveBase *) input_archive)->GoodTag(),
               "input archive argument is not a valid ALArchiveBase" );
    AL_ASSERT( ((ALEntryList *) list)->GoodTag(),
               "list argument is not a valid ALEntryList" );
    return ((ALArchiveBase *) this_object )->Append( *(ALArchiveBase *) input_archive, *( (ALEntryList *) list ) );
}

//
// extern "C" int ALArchiveReadDirectory( hALArchive this_object,
//                                        hALEntryList list )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  This is the archive that is going to have its directory
//                  read into the list.
//
//  list          : A handle for (pointer to) an ALEntryList object.
//                  The list is going to receive descriptions for all
//                  of the compressed objects stored in the archive.
//
// RETURNS
//
//  AL_SUCCESS for a successful read, < AL_SUCCESS for a failure.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ function
//  ALArchiveBase::ReadDirectory(ALEntrylist&).  For details on how
//  the member function actually works, take a look at ARCHIVEB.CPP.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALArchiveReadDirectory( hALArchive this_object, hALEntryList list )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase, "ALArchiveReadDirectory" );
    AL_ASSERT_OBJECT( list, ALEntryList, "ALArchiveReadDirectory" );
    return ((ALArchiveBase *) this_object )->ReadDirectory( *( (ALEntryList *) list ) );
}

//
// extern "C" int ALArchiveWriteDirectory( hALArchive this_object,
//                                         hALEntryList list )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  This is the archive that is going to get the new copy
//                  of the directory written out to it.
//
//  list          : A handle for (pointer to) an ALEntryList object.
//                  The list contains descriptions for all
//                  of the compressed objects stored in the archive.
//
// RETURNS
//
//  AL_SUCCESS for a successful write, < AL_SUCCESS for a failure.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ function
//  ALArchiveBase::WriteDirectory(ALEntrylist&).  For details on how
//  the member function actually works, take a look at ARCHIVEB.CPP.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALArchiveWriteDirectory( hALArchive this_object, hALEntryList list )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase, "ALArchiveWriteDirectory" );
    AL_ASSERT_OBJECT( list, ALEntryList, "ALArchiveWriteDirectory" );
    return ( (ALArchiveBase *) this_object )->WriteDirectory( *( (ALEntryList *) list ) );
}

//
// extern "C" int ALArchiveDelete( hALArchive this_object,
//                                 hALEntryList list,
//                                 hALArchive output_archive )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  This archive will have some of its compressed
//                  objects copied to the output archive.  It will
//                  then be renamed to a backup name, with its contents
//                  unchanged.
//
//  list          : A handle for (pointer to) an ALEntryList object.
//                  All of the marked objects in the list will be
//                  deleted (by not copying) from the output_archive.
//
//  output_archive: This archive will get some of the files from
//                  this_object.  It will then be renamed to have the
//                  original name of this_object.
//
// RETURNS
//
//  AL_SUCCESS for a successful deletion, < AL_SUCCESS for a failure.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ function
//  ALArchiveBase::Delete().  For details on how the member function
//  actually works, take a look at ARCHIVEB.CPP.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALArchiveDelete( hALArchive this_object,
                                             hALEntryList list,
                                             hALArchive output_archive )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase, "ALArchiveDelete" );
    AL_ASSERT_OBJECT( list, ALEntryList, "ALArchiveDelete" );
    AL_ASSERT_OBJECT( output_archive, ALArchiveBase, "IALArchiveDelete" );
    return ((ALArchiveBase *) this_object )->Delete( *( (ALEntryList *) list ), *( (ALArchiveBase *) output_archive ) );
}

//
// extern "C" int ALArchiveSetComment( hALArchive this_object,
//                                     char *comment )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  We are going to change the comment in archive, although
//                  the new comment won't be stored in the archive until
//                  we do a WriteDirectory().
//
//  comment       : An ASCII string that will be the new comment.  Note
//                  that this gets passed properly from both C and VB.
// RETURNS
//
//  AL_SUCCESS for a successful update, < AL_SUCCESS for a failure.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ function
//  ALArchiveBase::SetComment().  For details on how the member function
//  actually works, take a look at ARCHIVEB.CPP.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALArchiveSetComment( hALArchive this_object, char *comment )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase, "ALArchiveSetComment" );
    if ( comment == 0 )
        comment = "";
    return ( (ALArchiveBase *) this_object )->SetComment( comment );
}

//
// extern "C" char * ALArchiveGetComment( hALArchive this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  We are going to get the comment from this archive.
//                  Note that there won't be any comment here until
//                  you have performed a ReadDirectory().
// RETURNS
//
//  A pointer to the new string, which is stored in the ArchiveBase
//  object.  You can print it, copy it, or whatever, but don't you
//  dare change it.
//
// DESCRIPTION
//
//  This is the C wrapper function for the C++ function
//  ALArchiveBase::GetComment().  For details on how the member function
//  actually works, take a look at ARCHIVEB.CPP.  Very important
//  to note that this is for C ONLY!  VB strings take a special
//  return type, see the next function.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" char * AL_FUNCTION ALArchiveGetComment( hALArchive this_object )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase, "ALArchiveGetComment" );
    return (char *) ( (ALArchiveBase *) this_object )->GetComment();
}

//
// extern "C" long ALArchiveGetCommentVB( hALArchive this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  We are going to get the comment from this archive.
//                  Note that there won't be any comment here until
//                  you have performed a ReadDirectory().
// RETURNS
//
//  A pointer (or something, I'm not really sure) to a VB string.
//  The string is created inside VB.EXE (I think), by the
//  special string creation function.
//
// DESCRIPTION
//
//  This is the VB wrapper function for the C++ function
//  ALArchiveBase::GetComment().  For details on how the member function
//  actually works, take a look at ARCHIVEB.CPP.  Very important
//  to note that this is for VB ONLY!  We have to do something
//  special to translate a C character string to a VB native string
//  type, and the two are not interchangeable!!!
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.  The return result from the
//  function has to get ground through the VB string maker before we
//  have something good to return.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Combined a bunch of #ifdefs into a single test
//                          against AL_VB

#if defined( AL_VB )
extern "C" long AL_FUNCTION ALArchiveGetCommentVB( hALArchive this_object )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase, "ALArchiveGetCommentVB" );
    const char _far *status = ( (ALArchiveBase *) this_object )->GetComment();
    if ( status == 0 )
        status = "";
    return ALCreateVBString( status, (unsigned short int) _fstrlen( status ) );
}
#endif

//
// extern "C" hALStorage ALArchiveGetStorage( hALArchive hArchive )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  we are going to get the handle for (pointer to)
//                  the underlying storage object that contains the
//                  archive.
// RETURNS
//
//  A handle for (pointer to) an ALStorage object.  I think in theory
//  it isn't possible for this guy to return a 0.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ function
//  ALArchiveBase::GetStorageObject().  For details on how the member
//  function actually works, take a look at ARCHIVEB.CPP.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" hALStorage AL_FUNCTION ALArchiveGetStorage( hALArchive this_object )
{
    AL_ASSERT( ((ALArchiveBase *) this_object)->GoodTag(),
               "archive argument is not a valid ALArchiveBase" );
    ALStorage *obj = ((ALArchiveBase *) this_object)->GetStorageObject();
    return (hALStorage) obj;
}

//
// extern "C" int ALArchiveFillListBoxWindow( hALArchive this_object,
//                                            HWND window )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  We are going to fill a list box with the names
//                  of the storage objects in this archive.
//
//  window        : The window handle (not the ID!!!) of a list box.
//
// RETURNS
//
//  AL_SUCCESS for good things, <AL_SUCCESS for bad things.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ function
//  ALArchiveBase::FillListBox().  For details on how the member
//  function actually works, take a look at ARCHIVEB.CPP.  Note that
//  even though there is only a single C++ function, we have two
//  different incarnations for the wrapper function.  One that uses
//  a window handle by itself, and another that uses a dialog handle
//  /list box id combo to identify the list box.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

#if defined( AL_WINDOWS_GUI )
extern "C" int AL_FUNCTION ALArchiveFillListBoxWindow( hALArchive this_object,
                                                        HWND window )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase, "ALArchiveFillListBoxWindow" );
    return ((ALArchiveBase *) this_object)->FillListBox( window );
}

//
// extern "C" int ALArchiveFillListBoxDialog( hALArchive this_object,
//                                            HWND dialog,
//                                            int id );
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  We are going to fill a list box with the names
//                  of the storage objects in this archive.
//
//  window        : The window handle of a dialog box that contains
//                  the list box that will be filled.
//
//  id            : The id of the list box control in the dialog.
//
// RETURNS
//
//  AL_SUCCESS for good things, <AL_SUCCESS for bad things.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ function
//  ALArchiveBase::FillListBox().  For details on how the member
//  function actually works, take a look at ARCHIVEB.CPP.  Note that
//  even though there is only a single C++ function, we have two
//  different incarnations for the wrapper function.  One that uses
//  a window handle by itself, and another that uses a dialog handle
//  /list box id combo to identify the list box.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALArchiveFillListBoxDialog( hALArchive this_object,
                                                        HWND dialog,
                                                        int id )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase, "ALArchiveFillListBoxDialog" );
    return ((ALArchiveBase *) this_object )->FillListBox( dialog, id );
}

#endif

//
// extern "C" void deleteALArchive( hALArchive this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  We destroy it in this function.
//
// RETURNS
//
//  No returns from destructors.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ function
//  ALArchiveBase::~ALArchiveBase().  For details on how the member
//  function actually works, take a look at ARCHIVEB.CPP.
//
//  Note that this destructor function is virtual, and should be called
//  to destroy any derived classes as well.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION deleteALArchive( hALArchive this_object )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase, "deleteALArchive()" );
    delete (ALArchiveBase *) this_object;
}

//
// extern "C" int ALArchiveSetError( hALArchive this_object,
//                                   int error,
//                                   char *text )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  We are going to set the archive's status member
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
//  ALName::SetError(), as applied to an ALArchive object.  For more
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

extern "C" int AL_FUNCTION ALArchiveSetError( hALArchive this_object,
                                              int error,
                                              char AL_DLL_FAR *text )
{
    AL_ASSERT_OBJECT( this_object, ALArchive, "ALArchiveSetError" );
    ( (ALArchive *) this_object )->mStatus.SetError( error, text );
    return error;
}

//
// extern "C" int ALArchiveGetStatusCode( hALArchive this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//
// RETURNS
//
//  An integer that contains the current status code for the object.
//  Note that values of < 0 always indicate an error conditions.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ function
//  ALName::GetStatusCode() as implemented for objects of type
//  ALArchiveBase.  For details on how the member
//  function actually works, take a look at OBJNAME.CPP.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALArchiveGetStatusCode( hALArchive this_object )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase , "ALArchiveGetStatusCode" );
    return ( (ALArchiveBase *) this_object )->mStatus.GetStatusCode();
}

//
// extern "C" char *ALArchiveGetStatusString( hALArchive this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  We want to get the string translation of the error
//                  code for this object.
//
// RETURNS
//
//  Always returns a pointer to a short string translation of the
//  current error code.
//
// DESCRIPTION
//
//  This is the C wrapper function for the C++ function
//  ALName::GetStatusString(), as implemented for class ALArchiveBase.
//  Note that we need a completely different function return strings
//  to VB programmers.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" char AL_DLL_FAR * AL_FUNCTION
ALArchiveGetStatusString( hALArchive this_object )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase, "ALArchiveGetStatusString" );
    const char *status = ( (ALArchiveBase *) this_object )->mStatus.GetStatusString();
    if ( status == 0 )
        status = "";
    return (char AL_DLL_FAR *) status;
}

//
// extern "C" long ALArchiveGetStatusStringVB( hALArchive this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  We want to get the string translation of the error
//                  code for this object.
//
// RETURNS
//
//  Always returns a VB handle (pointer?) to a short string translation of
// the current error code for the ALArchiveBase object.
//
// DESCRIPTION
//
//  This is the VB wrapper function for the C++ function
//  ALName::GetStatusString(), as implemented for class ALArchiveBase.
//  Note that we need a completely different function to return strings
//  to C programmers.  In this case, we use a special VB translation routine
//  to convert a C string to one that is nice and easy for VB to use.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Combined a bunch of #ifdefs into a single test
//                          against AL_VB

#if defined( AL_VB )
extern "C" long AL_FUNCTION ALArchiveGetStatusStringVB( hALArchive this_object )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase, "ALArchiveGetStatusStringVB" );
    const char _far *status = ( (ALArchiveBase *) this_object )->mStatus.GetStatusString();
    if ( status == 0 )
        status = "";
    return ALCreateVBString( status, (unsigned short int) _fstrlen( status ) );
}
#endif


//
// extern "C" char * ALArchiveGetStatusDetail( hALArchive this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  We want to get the detailed string describing this
//                  object's current status.
//
// RETURNS
//
//  Always returns a pointer to a status detail message.
//
// DESCRIPTION
//
//  This is the C wrapper function for the C++ function
//  ALName::GetStatusDetail(), as implemented for class ALArchiveBase.
//  Note that we need a completely different function to return strings
//  to VB programmers.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" char AL_DLL_FAR * AL_FUNCTION
ALArchiveGetStatusDetail( hALArchive this_object )
{
    AL_ASSERT_OBJECT( this_object, ALArchiveBase, "ALArchiveGetStatusDetail" );
    const char *status = ( (ALArchiveBase *) this_object )->mStatus.GetStatusDetail();
    if ( status == 0 )
        status = "";
    return (char AL_DLL_FAR *) status;
}

//
// extern "C" long ALArchiveGetStatusDetailVB( hALArchive this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  We want to get the detailed status message for
//                  this object.
//
// RETURNS
//
//  Always returns a VB handle (pointer?) to a translation of
//  the current status detail for the ALArchiveBase object.
//
// DESCRIPTION
//
//  This is the VB wrapper function for the C++ function
//  ALName::GetStatusDetail(), as implemented for class ALArchiveBase.
//  Note that we need a completely different function to return strings
//  to C programmers.  In this case, we use a special VB translation routine
//  to convert a C string to one that is nice and easy for VB to use.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Combined a bunch of #ifdefs into a single test
//                          against AL_VB

#if defined( AL_VB )
extern "C" long AL_FUNCTION ALArchiveGetStatusDetailVB( hALArchive archive )
{
    AL_ASSERT_OBJECT( archive, ALArchiveBase, "ALArchiveGetStatusDetailVB" );
    const char _far *status = ( (ALArchiveBase *) archive)->mStatus.GetStatusDetail();
    if ( status == 0 )
        status = "";
    return ALCreateVBString( status, (unsigned short int) _fstrlen( status ) );
}
#endif

//
// extern "C" int ALArchiveGetVersion( hALArchive this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALArchiveBase object.
//                  We want to get the version of this archive.
//
// RETURNS
//
//  Always returns the integer version of the object.  The current version
//  is 0x100.  Note that this is the version of the Archive, not of