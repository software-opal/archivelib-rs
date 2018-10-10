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
// extern "C" hALEntryList newALEntryList( hALMonitor monitor )
//
// ARGUMENTS:
//
//  monitor  : A handle for (pointer to) an ALMonitor object.
//             The monitor will stay attached to the list, and will
//             be used to provide feedback during all archiving operations.
//
// RETURNS
//
//  A handle for (pointer to) an ALEntryList object.  It is possible to get
//  back a 0 if the constructor fails for lack of memory. Not likely
//  though.
//
// DESCRIPTION
//
//  This is C/VB translation function that provides access to the C++
//  constructor ALEntryList::ALEntryList().  Like most of the translation
//  functions, you don't get to see much here.  For detailed information
//  on what happens in the constructor, check ARCENTRY.CPP.
//
//  This routine first performs type checking on all the passed object
//  handles (in debug mode), then calls the constructor.  The returned
//  object pointer is then cast to a C/VB compatible type and returned
//  to the calling routine.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" hALEntryList AL_FUNCTION newALEntryList( hALMonitor monitor )
{
    if ( monitor != 0 )
        AL_ASSERT(  ((ALMonitor *) monitor)->GoodTag(),
                    "monitor argument is not a valid ALMonitor" );
    ALEntryList *list = new ALEntryList( (ALMonitor *) monitor );
    return (hALEntryList) list;
}

//
// extern "C" hALEntry ALEntryListGetFirstEntry( hALEntryList this_object )
//
// ARGUMENTS:
//
//  this_object : Handle for (pointer to) an ALEntryList object.
//
// RETURNS
//
//  A handle for (pointer to) an ALEntry object.  If there are no
//  entries in the list you will get back a 0.
//
// DESCRIPTION
//
//  This is C/VB translation function that provides access to the C++
//  member function ALEntryList::GetFirstEntry().  This routine is
//  the first one called when iterating through an entire list.
//
//  Like most of the translation functions, you don't get to see much here.  
//  For detailed information on what happens in the member function, check 
//  ARCENTRY.CPP.
//
//  This routine first performs type checking on the passed object
//  handle (in debug mode), then calls the member function.  The returned
//  object pointer is then cast to a C/VB compatible type and returned
//  to the calling routine.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" hALEntry AL_FUNCTION 
ALEntryListGetFirstEntry( hALEntryList this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListGetFirstEntry" );
    return (hALEntry) ( ((ALEntryList *) this_object )->GetFirstEntry() );

}

//
// extern "C" void ALEntryListUnmarkDuplicates( hALEntryList this_object,
//                                              hALEntryList list,
//                                              char *error_message )
//
// ARGUMENTS:
//
//  this_object    : Handle for (pointer to) an ALEntryList object.
//
//  list           : Handle for (pointer to) another ALEntryList object.
//                   (can be the same one, no problem).
//
//  error_message  : A character string giving an error message that
//                   will be attached to each entry that turns out
//                   to be a duplicate and gets unmarked.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This is C/VB translation function that provides access to the C++
//  member function ALEntryList::UnmarkDuplicates().  This routine is
//  used to remove the marks from any ALEntry objects in the list that
//  are duplicated in another list.
//
//  Like most of the translation functions, you don't get to see much here.  
//  For detailed information on what happens in the member function, check 
//  ARCENTRY.CPP.
//
//  This routine first performs type checking on the passed object
//  handles (in debug mode), then calls the member function.  
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION 
ALEntryListUnmarkDuplicates( hALEntryList this_object,
                             hALEntryList list,
                             char *error_message )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListUnmarkDuplicates" );
    AL_ASSERT_OBJECT( list, ALEntryList, "ALEntryListUnmarkDuplicates" );
    ( (ALEntryList *) this_object )->UnmarkDuplicates( * (ALEntryList *) list, error_message );
}

//
// extern "C" int ALEntryListSetMarksFromDialog( hALEntryList this_object, 
//                                               HWND hDlg, 
//                                               int id )
//
// ARGUMENTS:
//
//  this_object    : Handle for (pointer to) an ALEntryList object.
//
//  HDlg           : Windows handle for a dialog box containing a list box
//                   control.
//
//  id             : The id of the list box in the dialog.  The routine
//                   will set marks in the ALEntryList based on the
//                   names are set in the list box.
//
// RETURNS
//
//  The number of entries marked in the list box.
//
// DESCRIPTION
//
//  This is C translation function that provides access to the C++
//  member function ALEntryList::SetMarksFromListBox().  This function is
//  used to set marks in an ALEntryList based on selections made
//  in a list box.
//
//  Like most of the translation functions, you don't get to see much here.  
//  For detailed information on what happens in the member function, check 
//  ARCENTRY.CPP.
//
//  Note that since this routine looks to a dialog for its input,
//  it isn't useful to VB.
//
//  This routine first performs type checking on the passed object
//  handle (in debug mode), then calls the member function.  It
//  returns the integer unchanged to the calling procedure.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

#if defined( AL_WINDOWS_GUI )

extern "C" int AL_FUNCTION 
ALEntryListSetMarksFromDialog( hALEntryList this_object, HWND hDlg, int id )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListSetMarksFromDialog" );
    return ( (ALEntryList *) this_object)->SetMarksFromListBox( hDlg, id );
}

#endif

//
// extern "C" int ALEntryListSetMarksFromWindow( hALEntryList this_object, 
//                                               HWND hWnd )
//
// ARGUMENTS:
//
//  this_object    : Handle for (pointer to) an ALEntryList object.
//
//  hWnd           : Windows handle for a list box that contains names
//                   that are marked.
// RETURNS
//
//  The number of entries marked in the list box.
//
// DESCRIPTION
//
//  This is C/VB translation function that provides access to the C++
//  member function ALEntryList::SetMarksFromListBox().  This function is
//  called to transfer the marks a user makes in a list box into
//  the ALEntryList.
//
//  Like most of the translation functions, you don't get to see much here.  
//  For detailed information on what happens in the member function, check 
//  ARCENTRY.CPP.
//
//  This routine first performs type checking on the passed object
//  handle (in debug mode), then calls the member function.  It
//  returns the integer unchanged to the calling procedure.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

#if defined( AL_WINDOWS_GUI )

extern "C" int AL_FUNCTION 
ALEntryListSetMarksFromWindow( hALEntryList this_object, HWND hWnd )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListSetMarksFromWindow" );
    return ( (ALEntryList *) this_object )->SetMarksFromListBox( hWnd );
}

#endif


//
// extern "C" int ALEntryListFillListBoxWindow( hALEntryList this_object,
//                                              HWND hWnd )
//
// ARGUMENTS:
//
//  this_object    : Handle for (pointer to) an ALEntryList object.
//
//  hWnd           : Windows handle for a list box that is going to get
//                   filled up.
// RETURNS
//
//  The number of entries placed in the list box.
//
// DESCRIPTION
//
//  This is C/VB translation function that provides access to the C++
//  member function ALEntryList::SetMarksFromListBox().  This function
//  is used to transfer the selections a user made in a list box into
//  marks in the list.
//
//  Like most of the translation functions, you don't get to see much here.  
//  For detailed information on what happens in the member function, check 
//  ARCENTRY.CPP.
//
//  This routine first performs type checking on the passed object
//  handle (in debug mode), then calls the member function.  It
//  returns the integer unchanged to the calling procedure.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

#if defined( AL_WINDOWS_GUI )

extern "C" int AL_FUNCTION 
ALEntryListFillListBoxWindow( hALEntryList this_object,
                              HWND hWnd )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListFillListBoxWindow" );
    return ( (ALEntryList *) this_object )->FillListBox( hWnd );
}

#endif

//
// extern "C" int ALEntryListFillListBoxDialog( hALEntryList this_object,
//                                              HWND hDlg,
//                                              int list_box_id )
//
// ARGUMENTS:
//
//  this_object    : Handle for (pointer to) an ALEntryList object.
//
//  HDlg           : Windows handle for a dialog box containing a list box
//                   control.
//
//  id             : The id of the list box in the dialog.  The routine
//                   will fill up this list box with the names of all
//                   the marked files in this_object.
//
// RETURNS
//
//  The number of entries added to the list box.
//
// DESCRIPTION
//
//  This is C translation function that provides access to the C++
//  member function ALEntryList::FillListBox().  This routine is called
//  to fill a list box with the names of storage objects in the list.
//
//  Like most of the translation functions, you don't get to see much here.  
//  For detailed information on what happens in the member function, check 
//  ARCENTRY.CPP.
//
//  Note that since this routine wants to use a list box that is 
//  embedded in a dialog box, it isn't much use to VB.
//
//  This routine first performs type checking on the passed object
//  handle (in debug mode), then calls the member function.  It
//  returns the integer unchanged to the calling procedure.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

#if defined( AL_WINDOWS_GUI )

extern "C" int AL_FUNCTION 
ALEntryListFillListBoxDialog( hALEntryList this_object,
                              HWND hDlg,
                              int list_box_id )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListFillListBoxDialog" );
    return ( (ALEntryList *) this_object )->FillListBox( hDlg, list_box_id );
}
#endif

//
// extern "C" void deleteALEntryList( hALEntryList this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntryList object.
//
// RETURNS
//
//  None, a destructor.
//
// DESCRIPTION
//
//  This is the C/VB translation routine for the ALEntryList destructor.
//  You don't see much exciting code in this routine, so if you want to
//  see exactly what is going on in the destructor, take a look at
//  ARCENTRY.CPP.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION deleteALEntryList( hALEntryList this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "deleteALEntryList" );
    delete (ALEntryList *) this_object;
}

//
// extern "C" int ALEntryListSetMarks( hALEntryList this_object, 
//                                     char *pattern )
//
// ARGUMENTS:
//
//  this_object    : Handle for (pointer to) an ALEntryList object.
//
//  pattern        : A regular expression (wildcard) that will be used as 
//                   a pattern to set marks in the list.
//
// RETURNS
//
//  The number of entries marked in the list.
//
// DESCRIPTION
//
//  This is C/VB translation function that provides access to the C++
//  member function ALEntryList::SetMarks().  This function is used
//  to set marks for ALEntry objects in the list that match the wild
//  card pattern.
//
//  Like most of the translation functions, you don't get to see much here.  
//  For detailed information on what happens in the member function, check 
//  ARCENTRY.CPP.
//
//  This routine first performs type checking on the passed object
//  handle (in debug mode), then calls the member function.  It
//  returns the integer unchanged to the calling procedure.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALEntryListSetMarks( hALEntryList this_object, 
                                                char AL_DLL_FAR *pattern )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListSetMarks" );
    return ( (ALEntryList *) this_object )->SetMarks( pattern );
}

//
// extern "C" int ALEntryListClearMarks( hALEntryList this_object, 
//                                       char *pattern )
//
// ARGUMENTS:
//
//  this_object    : Handle for (pointer to) an ALEntryList object.
//
//  pattern        : A regular expression (wildcard) that will be used as 
//                   a pattern to clear marks in the list.
//
// RETURNS
//
//  The number of entries cleared in the list.
//
// DESCRIPTION
//
//  This is C/VB translation function that provides access to the C++
//  member function ALEntryList::ClearMarks().  It is used to clear
//  the marks of entries in the list that match the wildcard pattern.
//
//  Like most of the translation functions, you don't get to see much here.  
//  For detailed information on what happens in the member function, check 
//  ARCENTRY.CPP.
//
//  This routine first performs type checking on the passed object
//  handle (in debug mode), then calls the member function.  It
//  returns the integer unchanged to the calling procedure.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALEntryListClearMarks( hALEntryList this_object, 
                                                  char AL_DLL_FAR *pattern )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListClearMarks" );
    return ( (ALEntryList *) this_object )->ClearMarks( pattern );
}

//
// extern "C" int ALEntryListDeleteUnmarked( hALEntryList this_object )
//
// ARGUMENTS:
//
//  this_object    : Handle for (pointer to) an ALEntryList object.
//
//
// RETURNS
//
//  The number of entries deleted from the list.
//
// DESCRIPTION
//
//  This is C/VB translation function that provides access to the C++
//  member function ALEntryList::DeleteUnmarked().  This function is used
//  to delete ALEntry objects from the list.
//
//  Like most of the translation functions, you don't get to see much here.  
//  For detailed information on what happens in the member function, check 
//  ARCENTRY.CPP.
//
//  This routine first performs type checking on the passed object
//  handle (in debug mode), then calls the member function.  It
//  returns the integer unchanged to the calling procedure.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION 
ALEntryListDeleteUnmarked( hALEntryList this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListDeleteUnmarked" );
    return ( (ALEntryList *) this_object )->DeleteUnmarked();
}

//
// extern "C" int ALEntryListToggleMarks( hALEntryList this_object )
//
// ARGUMENTS:
//
//  this_object    : Handle for (pointer to) an ALEntryList object.
//
// RETURNS
//
//  The number of entries whose marks were toggled.
//
// DESCRIPTION
//
//  This is C/VB translation function that provides access to the C++
//  member function ALEntryList::ToggleMarks().  This function is used
//  toggle the marks of *every* entry in the list.
//
//  Like most of the translation functions, you don't get to see much here.  
//  For detailed information on what happens in the member function, check 
//  ARCENTRY.CPP.
//
//  This routine first performs type checking on the passed object
//  handle (in debug mode), then calls the member function.  It
//  returns the integer unchanged to the calling procedure.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALEntryListToggleMarks( hALEntryList this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListToggleMarks" );
    return ( (ALEntryList *) this_object )->ToggleMarks();
}

//
// extern "C" int ALEntryListGetStatusCode( hALEntryList this_object )
//
// ARGUMENTS:
//
//  this_object    : Handle for (pointer to) an ALEntryList object.
//
// RETURNS
//
//  The current status integer for the ALEntryList object.
//
// DESCRIPTION
//
//  This is a C/VB translation function that provides access to the C++
//  member function ALStatus::GetStatusCode(), for the mStatus data
//  member for the ALEntryList object.  
//
//  Like most of the translation functions, you don't get to see much here.  
//  For detailed information on what happens in the member function, check 
//  ARCENTRY.CPP.
//
//  This routine first performs type checking on the passed object
//  handle (in debug mode), then calls the member function.  It
//  returns the status integer unchanged to the calling procedure.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION 
ALEntryListGetStatusCode( hALEntryList this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListGetStatusCode" );
    return ( (ALEntryList *) this_object )->mStatus.GetStatusCode();
}

//
// extern "C" char * ALEntryListGetStatusString( hALEntryList this_object )
//
// ARGUMENTS:
//
//  this_object    : Handle for (pointer to) an ALEntryList object.
//
// RETURNS
//
//  The current status string translation for the ALEntryList object.
//
//  Note that this return type is just not very good for VB people.
//  They should use the VB specific function to get a real VB string
//  back.
//
// DESCRIPTION
//
//  This is C translation function that provides access to the C++
//  member function ALStatus::GetStatusString(), for the mStatus data
//  member for the ALEntryList object.  This is the short string 
//  translation, not the status detail.
//
//  Like most of the translation functions, you don't get to see much here.  
//  For detailed information on what happens in the member function, check 
//  ARCENTRY.CPP.
//
//  This routine first performs type checking on the passed object
//  handle (in debug mode), then calls the member function.  It
//  then casts and returns the status string.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" char AL_DLL_FAR * AL_FUNCTION 
ALEntryListGetStatusString( hALEntryList this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListGetStatusString" );
    const char *status = ( (ALEntryList *) this_object)->mStatus.GetStatusString();
    if ( status == 0 )
        status = "";
    return (char AL_DLL_FAR *) status;
}

//
// extern "C" char * ALEntryListGetStatusDetail( hALEntryList this_object )
//
// ARGUMENTS:
//
//  this_object    : Handle for (pointer to) an ALEntryList object.
//
// RETURNS
//
//  The current status string detail for the ALEntryList object.
//
//  Note that this return type is just not very good for VB people.
//  They should use the VB specific function to get a real VB string
//  back.
//
// DESCRIPTION
//
//  This is C translation function that provides access to the C++
//  member function ALStatus::GetStatusDetail(), for the mStatus data
//  member for the ALEntryList object.  This is the detailed message,
//  not to be confused with the short translation.
//
//  Like most of the translation functions, you don't get to see much here.  
//  For detailed information on what happens in the member function, check 
//  ARCENTRY.CPP.
//
//  This routine first performs type checking on the passed object
//  handle (in debug mode), then calls the member function.  It
//  then casts and returns the status string.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" char AL_DLL_FAR * AL_FUNCTION 
ALEntryListGetStatusDetail( hALEntryList this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListGetStatusString" );
    const char *status = ( (ALEntryList *) this_object )->mStatus.GetStatusDetail();
    if ( status == 0 )
        status = "";
    return (char AL_DLL_FAR *) status;
}

//
// extern "C" long ALEntryListGetStatusStringVB( hALEntryList this_object )
//
// ARGUMENTS:
//
//  this_object    : Handle for (pointer to) an ALEntryList object.
//
// RETURNS
//
//  The current status string translation for the ALEntryList object.
//
//  Note that this function performs a translation of the string type to
//  a VB string.  It won't do you much good to call this from a C program.
//
// DESCRIPTION
//
//  This is VB translation function that provides access to the C++
//  member function ALStatus::GetStatusString(), for the mStatus data
//  member for the ALEntryList object.  This is the short string 
//  translation, not the status detail.
//
//  Like most of the translation functions, you don't get to see much here.  
//  For detailed information on what happens in the member function, check 
//  ARCENTRY.CPP.
//
//  This routine first performs type checking on the passed object
//  handle (in debug mode), then calls the member function.  It
//  then converts and returns the status string.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Combined a bunch of #ifdefs into a single test
//                          against AL_VB

#if defined( AL_VB )

extern "C" long AL_FUNCTION 
ALEntryListGetStatusStringVB( hALEntryList this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryLisGetStatusStringVB" );
    const char _far *status = ( (ALEntryList *) this_object )->mStatus.GetStatusString();
    if ( status == 0 )
        status = "";
    return ALCreateVBString( status, (unsigned short int) _fstrlen( status ) );
}
#endif


//
// extern "C" long ALEntryListGetStatusDetailVB( hALEntryList this_object )
//
// ARGUMENTS:
//
//  this_object    : Handle for (pointer to) an ALEntryList object.
//
// RETURNS
//
//  The current status detail string for the ALEntryList object.
//
//  Note that this function performs a translation of the string type to
//  a VB string.  It won't do you much good to call this from a C program.
//
// DESCRIPTION
//
//  This is VB translation function that provides access to the C++
//  member function ALStatus::GetStatusDetail(), for the mStatus data
//  member for the ALEntryList object.  This is the detailed status
//  string, not the short status translation.
//
//  Like most of the translation functions, you don't get to see much here.  
//  For detailed information on what happens in the member function, check 
//  ARCENTRY.CPP.
//
//  This routine first performs type checking on the passed object
//  handle (in debug mode), then calls the member function.  It
//  then converts and returns the status string.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Combined a bunch of #ifdefs into a single test
//                          against AL_VB

#if defined( AL_VB )

extern "C" long AL_FUNCTION 
ALEntryListGetStatusDetailVB( hALEntryList this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntryList, "ALEntryListGetStatusDetailVB" );
    const char _far *status = ( (ALEntryList *) this_object )->mStatus.GetStatusDetail();
    if ( status == 0 )
        status = "";
    return ALCreateVBString( status, (unsigned short int) _fstrlen( status ) );
}

#endif
