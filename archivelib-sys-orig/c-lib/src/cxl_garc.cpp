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
// extern "C" hALArchive newALArchive( char *name )
//
// ARGUMENTS:
//
//  name     :  This constructor automatically creates an ALFile object
//              for the archive.  This is the name of the ALFile object.
//
// RETURNS
//
//  A handle for (pointer to) a newly created ALArchive, or 0 if
//  the constructor failed for some reason.
//
// DESCRIPTION
//
//  This is the C/VB translation function for the C++ constructor
//  ALArchive::ALArchive(char *).  See ARCHIVE.CPP for details on what
//  happens in the constructor.
//
//  Unlike most translation functions, this constructor doesn't have to check
//  or cast any arguments before calling its C++ code.  It does however have
//  to cast the result to the correct type before giving it back to the
//  C or VB calling module.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" hALArchive AL_FUNCTION newALArchive( char *name )
{
    ALArchive *archive;

    archive = new ALArchive( name );
    return (hALArchive) archive;
}

//
// extern "C" hALArchive newALArchiveFromStorage( hALStorage storage )
//
// ARGUMENTS:
//
//  storage  :  A handle for (pointer to) a preconstructed storage object.
//              This storage object will be where the archive keeps all of
//              its stuff.  Note that you have to create and destroy this
//              storage object yourself, unlike the previous constructor.
//
// RETURNS
//
//  A handle for (pointer to) a newly created ALArchive, or 0 if
//  the constructor failed for some reason.
//
// DESCRIPTION
//
//  This is the C/VB translation function for the C++ constructor
//  ALArchive::ALArchive(ALStorage&).  See ARCHIVE.CPP for details on what
//  happens in the constructor.
//
//  Like most translation functions, this one checks the type off its single
//  argument (in debug mode), before casting and calling the C++ function.
//  It takes the result from the C++ constructor and casts it back to
//  correct type before giving it back to the C or VB calling module.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" hALArchive AL_FUNCTION newALArchiveFromStorage( hALStorage storage )
{
    AL_ASSERT( ( (ALStorage *) storage)->GoodTag(),
               "storage argument is not a valid ALStorageObject" );
    ALArchive *archive;
    archive = new ALArchive( *(ALStorage *) storage );
    return (hALArchive) archive;
}

//  OBSOLETE FUNCTION
//
// extern "C" int ALArchiveAddFilesToList( hALArchive this_object,
//                                         hALEntryList list,
//                                         char *pattern,
//                                         int traverse )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) a valid ALArchive object.
//                 It isn't used since we are calling a static function.
//
//  list         : A handle for (pointer to) an ALEntryList.  This list
//                 is going to get a bunch of new entries, like it or not.
//
//  pattern      : A list of wild-card file specs, separated by spaces,
//                 commas, or semicolons.
//
//  traverse     : A flag to indicate whether the search engine should
//                 traverse subdirectories when expanding file specs.
//
// RETURNS
//
//  An integer indicating how many entries were added to the list.
//
// DESCRIPTION
//
//  This function has been replaced by the more appropriately named
//  ALEntryListAddWildCardFiles().
//
//  This function is the C/VB translation function to access the C++
//  member function ALArchive::AddWildCardFiles().  For details on how
//  the AddWildCardFiles() function actually works, please check out
//  ARCHIVE.CPP, cuz that is where the real code is.
//
//  Like all the other translation routines, this guy just has to
//  check the arguments to see if they are the correct type (in debug
//  mode only), then cast and call the C++ member function.  The
//  return value from the function is shot directly back to the C or
//  VB calling routine.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALArchiveAddFilesToList( hALArchive, hALEntryList list, char *pattern, int traverse )
{
    AL_ASSERT_OBJECT( list, ALEntryList, "ALArchiveAddFilesToList" );
    return ALArchive::AddWildCardFiles( *( (ALEntryList *) list), pattern, traverse );
}


//
// extern "C" int ALEntryListAddFromDialog( hALEntryList this_object,
//                                          HWND hDlg,
//                                          int id )
//
// ARGUMENTS:
//
//  this_object    : Handle for (pointer to) an ALEntryList object.
//
//  hDlg           : Windows handle for a dialog box containing the
//                   target list box.
//
//  id             : The id of the list box found in the dialog box.
//
// RETURNS
//
//  The number of entries added to the list box.
//
// DESCRIPTION
//
//  This is C translation function that provides access to the C++
//  member function ALArchive::MakeEntriesFromListBox().  Like most of the
//  translation functions, you don't get to see much here.  For detailed
//  information on what happens in the constructor, check ARCHIVE.CPP.
//
//  You might think that this function looks like a member of ALEntryList,
//  not ALArchive.  Yes, it looks like it, but in fact it is a static
//  member of ALArchive.
//
//  This routine first performs type checking on the passed object
//  handle (in debug mode), then calls the member function.  It
//  returns the integer unchanged to the calling procedure.
//
//  You might think that this function looks like a member of ALEntryList,
//  not ALArchive.  Yes, it looks like it, but in fact it is a static
//  member of ALArchive.  The fact that it is static explains why there
//  is no ALArchive object in the argument list.
//
//  You can't use this dude with VB because VB doesn't use Dialog boxes,
//  at least I don't think so.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

#if defined( AL_WINDOWS_GUI )
extern "C" int AL_FUNCTION ALEntryListAddFromDialog( hALEntryList this_object,
                                                     HWND hDlg,
                                                     int id )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListAddFromDialog" );
    return ALArchive::MakeEntriesFromListBox( *(ALEntryList *) this_object, hDlg, id );
}
#endif
//
// extern "C" int ALEntryListAddFromWindow( hALEntryList this_object,
//                                          HWND hwnd )
//
// ARGUMENTS:
//
//  this_object    : Handle for (pointer to) an ALEntryList object.
//
//  hwnd           : Windows handle for a the list box control.
//
// RETURNS
//
//  The number of entries added to the list box.
//
// DESCRIPTION
//
//  This is C translation function that provides access to the C++
//  member function ALArchive::MakeEntriesFromListBox().  Like most of the
//  translation functions, you don't get to see much here.  For detailed
//  information on what happens in the constructor, check ARCHIVE.CPP.
//
//  You might think that this function looks like a member of ALEntryList,
//  not ALArchive.  Yes, it looks like it, but in fact it is a static
//  member of ALArchive.  The fact that it is static explains why there
//  is no ALArchive argument in the picture.
//
//  This routine first performs type checking on the passed object
//  handle (in debug mode), then calls the member function.  It
//  returns the integer unchanged to the calling procedure.
//
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

#if defined( AL_WINDOWS_GUI )
extern "C" int AL_FUNCTION ALEntryListAddFromWindow( hALEntryList this_object,
                                                      HWND hwnd )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListAddFromWindow" );
    return ALArchive::MakeEntriesFromListBox( *(ALEntryList *) this_object, hwnd );
}
#endif

//
// extern "C" int ALEntryListAddWildCardFiles( hALEntryList this_object,
//                                             char *pattern,
//                                             int traverse )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntryList.  This list
//                 is going to get a bunch of new entries, like it or not.
//
//  pattern      : A list of wild-card file specs, separated by spaces,
//                 commas, or semicolons.
//
//  traverse     : A flag to indicate whether the search engine should
//                 traverse subdirectories when expanding file specs.
//
// RETURNS
//
//  An integer indicating how many entries were added to the list.
//
// DESCRIPTION
//
//  This function is the C/VB translation function to access the C++
//  member function ALArchive::AddWildCardFiles().  For details on how
//  the AddWildCardFiles() function actually works, please check out
//  ARCHIVE.CPP, cuz that is where the real code is.  You might notice
//  that there is no ALArchive object passed to this function.  That
//  is because this function is a static member function of ALArchive,
//  we just need its ability to create ALFile objects.
//
//  Like all the other translation routines, this guy just has to
//  check the arguments to see if they are the correct type (in debug
//  mode only), then cast and call the C++ member function.  The
//  return value from the function is shot directly back to the C or
//  VB calling routine.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION
ALEntryListAddWildCardFiles( hALEntryList this_object,
                             char AL_DLL_FAR *pattern,
                             int traverse )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListAddWildCardFiles" );
    return ALArchive::AddWildCardFiles( *( (ALEntryList *) this_object), pattern, traverse );
}

