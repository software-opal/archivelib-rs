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
// extern "C" hALEntry newALEntry( hALEntryList list,
//                                 hALStorage storage,
//                                 hALEngine engine )
//
// ARGUMENTS:
//
//  list     : A handle for (pointer to) the ALEntryList that is
//             going get this ALEntry.
//
//  storage  : A handle for (pointer to) the storage object that is
//             being described in this ALEntry object.
//
//  engine   : A handle for (pointer to) the compression engine that
//             will be/was used to compress/expand the storage object.
//
// RETURNS
//
//  A handle for (pointer to) a freshly constructed ALEntry object. In the
//  case of a really horrible error, this might be a 0.
//
// DESCRIPTION
//
//  This C/VB function provides a translation layer so that those two
//  languages can access the C++ constructor ALEntry::ALEntry().
//  For details on what exactly happens in the ALEntry ctor, check out
//  the source module in ARCENTRY.CPP.
//
//  Like most of the translation layer functions, this guy works by simply
//  performing type checking on the arguments (in debug mode), and calling
//  the function.  The result is then cast properly and returned to the
//  calling routine.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" hALEntry AL_FUNCTION newALEntry( hALEntryList list,
                                            hALStorage storage,
                                            hALEngine engine )
{
    AL_ASSERT_OBJECT( list, ALEntryList, "newALEntry" );
    AL_ASSERT_OBJECT( storage, ALStorage, "newALEntry" );
    AL_ASSERT_OBJECT( engine, ALCompressionEngine, "newALEntry" );
    return (hALEntry) new ALEntry( *( (ALEntryList *) list ),
                                   (ALStorage * ) storage,
                                   (ALCompressionEngine *) engine );
}

//
// extern "C" void deleteALEntry( hALEntry this_object )
//
// ARGUMENTS:
//
//  this_object : A handle for (pointer to) a valid ALEntry object.
//
// RETURNS
//
//  Nothing, this is a destructor.
//
// DESCRIPTION
//
//  This C/VB function provides a translation layer so that those two
//  languages can access the C++ destructor ALEntry::~ALEntry().
//  For details on what exactly happens in the ALEntry dtor, check out
//  the source module in ARCENTRY.CPP.
//
//  Like most of the translation layer functions, this guy works by simply
//  performing type checking on the arguments (in debug mode), and calling
//  the function.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION deleteALEntry( hALEntry entry )
{
    AL_ASSERT_OBJECT( entry, ALEntry, "deleteALEntry" );
    delete (ALEntry *) entry;
}

//
// extern "C" int ALEntryDuplicate( hALEntry this_object, hALEntryList list )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntry object.
//
//  list         : A handle for (pointer to) an ALEntryList object.
//
// RETURNS
//
//  1 if the entry is duplicated somewhere in the list, 0 if not.
//
// DESCRIPTION
//
//  This function provides a translation layer for C and VB to access the
//  C++ member function ALEntry::Duplicate().  For more information on
//  what this member function actually does, see ARCENTRY.CPP.
//
//  Like most of the translation functions, this routine checks the
//  arguments for correct type (in debug mode), then casts this_object
//  to the desired class, and calls the member function.  The return
//  from the member function is passed right back to the calling routine.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION
ALEntryDuplicate( hALEntry this_object, hALEntryList list )
{
    AL_ASSERT_OBJECT( this_object, ALEntry, "ALEntryDuplicate" );
    AL_ASSERT_OBJECT( list, ALEntryList, "ALEntryDuplicate" );
    return ( (ALEntry *) this_object )->Duplicate( * (ALEntryList *) list );
}


//
// extern "C" hALEntry ALEntryGetNextEntry( hALEntry this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntry object.
//
// RETURNS
//
//  A valid handle for (pointer to) an ALEntry if there are more
//  objects in the list.  If not, 0.
//
// DESCRIPTION
//
//  This function provides a translation layer for C and VB to access the
//  C++ member function ALEntry::GetNextEntry().  For more information on
//  what this member function actually does, see ARCENTRY.CPP.
//
//  Like most of the translation functions, this routine checks the
//  arguments for correct type (in debug mode), then casts this_object
//  to the desired class, and calls the member function.  The return
//  from the member function is passed right back to the calling routine
//  after being cast to the appropriate C/VB type.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" hALEntry AL_FUNCTION ALEntryGetNextEntry( hALEntry this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntry, "ALEntryGetNextEntry" );
    return (hALEntry) ( ((ALEntry *) this_object )->GetNextEntry() );
}

//
// extern "C" void ALEntrySetMark( hALEntry this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntry object.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This function provides a translation layer for C and VB to access the
//  C++ member function ALEntry::SetMark().  For more information on
//  what this member function actually does, see ARCENTRY.CPP.
//
//  Like most of the translation functions, this routine checks the
//  arguments for correct type (in debug mode), then casts this_object
//  to the desired class, and calls the member function.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION ALEntrySetMark( hALEntry this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntry, "ALEntrySetMark" );
    ((ALEntry *) this_object )->SetMark();
}

//
// extern "C" void ALEntryClearMark( hALEntry this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntry object.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This function provides a translation layer for C and VB to access the
//  C++ member function ALEntry::ClearMark().  For more information on
//  what this member function actually does, see ARCENTRY.CPP.
//
//  Like most of the translation functions, this routine checks the
//  arguments for correct type (in debug mode), then casts this_object
//  to the desired class, and calls the member function.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION ALEntryClearMark( hALEntry this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntry, "ALEntryClearMark" );
    ((ALEntry *) this_object )->ClearMark();
}

//
// extern "C" void ALEntrySetMarkState( hALEntry this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntry object.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This function provides a translation layer for C and VB to access the
//  C++ member function ALEntry::SetMarkState().  For more information on
//  what this member function actually does, see ARCENTRY.CPP.
//
//  Like most of the translation functions, this routine checks the
//  arguments for correct type (in debug mode), then casts this_object
//  to the desired class, and calls the member function.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION ALEntrySetMarkState( hALEntry this_object,
                                                 short int new_state )
{
    AL_ASSERT_OBJECT( this_object, ALEntry, "ALEntrySetMarkState" );
    ((ALEntry *) this_object )->SetMarkState( new_state );
}

//
// extern "C" hALStorage ALEntryGetStorage( hALEntry this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntry object.
//
// RETURNS
//
//  A valid handle for (pointer to) an ALStorage object. It is possible
//  to get a return value of 0, since an ALEntry is not required to have
//  a valid ALStorage object attached to it.
//
// DESCRIPTION
//
//  This function provides a translation layer for C and VB to access the
//  C++ data member ALEntry::mpStorageObject.  For more information on
//  what this data member actually does, see ARCENTRY.CPP.
//
//  Like most of the translation functions, this routine checks the
//  arguments for correct type (in debug mode), then casts this_object
//  to the desired class, and gets the data member.  The data member
//  is passed right back to the calling routine after being cast to the
//  appropriate C/VB type.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" hALStorage AL_FUNCTION ALEntryGetStorage( hALEntry this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntry, "ALEntryGetStorage" );
    return (hALStorage) ( (ALEntry *) this_object )->mpStorageObject;
}

//
// extern "C" void ALEntrySetStorage( hALEntry this_object,
//                                    hALStorage storage )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntry object.
//
//  storage      : A handle for (pointer to) an ALStorage object that
//                 will now be attached to the ALEntry object.  Note
//                 that a value of 0 is acceptable.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This function provides a translation layer for C and VB to access the
//  C++ data member ALEntry::mpStorageObject.  For more information on
//  what this data member actually does, see ARCENTRY.CPP.
//
//  Like most of the translation functions, this routine checks the
//  arguments for correct type (in debug mode), then casts this_object
//  to the desired class, and accesses the data member.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION ALEntrySetStorage( hALEntry this_object,
                                               hALStorage storage )
{
    AL_ASSERT_OBJECT( this_object, ALEntry, "ALEntrySetStorage" );
    if ( storage != 0 )
        AL_ASSERT_OBJECT( storage, ALStorage, "ALEntrySetStorage" );
    ( (ALEntry *) this_object )->mpStorageObject = (ALStorage *) storage;
}

//
// extern "C" int ALEntrySetComment( hALEntry this_object, char *comment )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntry object.
//
//  comment      : A pointer to a character string that is going to be
//                 copied into the ALEntry object.
//
// RETURNS
//
//  AL_SUCCESS if things went okay, or a code < AL_SUCCESS if not.
//
// DESCRIPTION
//
//  This function provides a translation layer for C and VB to access the
//  C++ data member ALEntry::mpStorageObject.  For more information on
//  what this data member actually does, see ARCENTRY.CPP.
//
//  Like most of the translation functions, this routine checks the
//  arguments for correct type (in debug mode), then casts this_object
//  to the desired class, and accesses the data member.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION
ALEntrySetComment( hALEntry this_object, char *comment )
{
    AL_ASSERT_OBJECT( this_object, ALEntry, "ALEntrySetComment" );
    if ( comment == 0 )
        comment = (char *)"";
    return ( (ALEntry *) this_object )->SetComment( comment );
}

//
// extern "C" long ALEntryGetCompressedSize( hALEntry this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntry object.
//
// RETURNS
//
//  A long value, indicating the compressed size.  Note that this information
//  will only be available after reading a directory, or performing an
//  operation that compresses the file.
//
// DESCRIPTION
//
//  This function provides a translation layer for C and VB to access the
//  C++ member ALEntry::GetCompressedSize.  For more information on
//  what this member function actually does, see ARCENTRY.CPP.
//
//  Like most of the translation functions, this routine checks the
//  arguments for correct type (in debug mode), then casts this_object
//  to the desired class, and calls the member function.  The return data
//  from the member function is passed back directly to the calling module.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" long AL_FUNCTION ALEntryGetCompressedSize( hALEntry this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntry, "ALEntryGetCompressedSize" );
    return ( (ALEntry *) this_object )->GetCompressedSize();
}

//
// extern "C" long ALEntryGetCrc32( hALEntry this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntry object.
//
// RETURNS
//
//  A long value, indicating the object's CRC.  Note that this information
//  will only be available after reading a directory, or performing an
//  operation that compresses the file.
//
// DESCRIPTION
//
//  This function provides a translation layer for C and VB to access the
//  C++ member function ALEntry::GetCrc32().  For more information on
//  what this member function actually does, see ARCENTRY.CPP.
//
//  Like most of the translation functions, this routine checks the
//  arguments for correct type (in debug mode), then casts this_object
//  to the desired class, and calls the member function.  The return data
//  from the member function is passed back directly to the calling module.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" long AL_FUNCTION ALEntryGetCrc32( hALEntry this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntry, "ALEntryGetCrc32" );
    return ( (ALEntry *) this_object )->GetCrc32();
}

//
// extern "C" int ALEntryGetMark( hALEntry this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntry object.
//
// RETURNS
//
//  A 1 or 0, indicating the object's mark state.
//
// DESCRIPTION
//
//  This function provides a translation layer for C and VB to access the
//  C++ member function ALEntry::GetMark().  For more information on
//  what this member function actually does, see ARCENTRY.CPP.
//
//  Like most of the translation functions, this routine checks the
//  arguments for correct type (in debug mode), then casts this_object
//  to the desired class, and calls the member function.  The return data
//  from the member function is passed back directly to the calling module.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALEntryGetMark( hALEntry this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntry, "ALEntryGetMark" );
    return ( (ALEntry *) this_object )->GetMark();
}

//
// extern "C" int ALEntryCompressionRatio( hALEntry this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntry object.
//
// RETURNS
//
//  A number usually ranging from 0 to 100.  Less than 0 indicates
//  an error.
//
// DESCRIPTION
//
//  This function provides a translation layer for C and VB to access the
//  C++ member function ALEntry::CompressionRatio().  For more information on
//  what this member function actually does, see ARCENTRY.CPP.
//
//  Like most of the translation functions, this routine checks the
//  arguments for correct type (in debug mode), then casts this_object
//  to the desired class, and calls the member function.  The return data
//  from the member function is passed back directly to the calling module.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALEntryCompressionRatio( hALEntry this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntry, "ALEntryCompressionRatio" );
    return ( (ALEntry *) this_object )->CompressionRatio();
}

//
// extern "C" char *ALEntryGetComment( hALEntry this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntry object.
//
// RETURNS
//
//  A pointer to a C character string.  The pointer is the value of the
//  string pointer embedded in the class object.
//
// DESCRIPTION
//
//  This function provides a translation layer for C to access the
//  C++ member function ALEntry::GetComment().  For more information on
//  what this member function actually does, see ARCENTRY.CPP.
//
//  Like most of the translation functions, this routine checks the
//  arguments for correct type (in debug mode), then casts this_object
//  to the desired class, and calls the member function.  The return data
//  from the member function is passed back directly to the calling module.
//
//  C strings aren't happy in VB, so there is a special function just to
//  convert strings to vB format.  Don't call this function from
//  Visual Basic, there is an easier way!
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" char *AL_FUNCTION ALEntryGetComment( hALEntry this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntry, "ALEntryGetComment" );
    return (char *) ( (ALEntry *) this_object )->GetComment();
}

//
// extern "C" long ALEntryGetCommentVB( hALEntry this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntry object.
//
// RETURNS
//
//  A Visual Basic character string.  The string is a copy of the
//  string pointer embedded in the class object.
//
// DESCRIPTION
//
//  This function provides a translation layer for VB to access the
//  C++ member function ALEntry::GetComment().  For more information on
//  what this member function actually does, see ARCENTRY.CPP.
//
//  Like most of the translation functions, this routine checks the
//  arguments for correct type (in debug mode), then casts this_object
//  to the desired class, and calls the member function.  The return data
//  from the member function is passed back directly to the calling module.
//
//  C strings aren't happy in VB, so this function converts the C
//  character pointer to a super-special VB string.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Combined a bunch of #ifdefs into a single test
//                          against AL_VB

#if defined( AL_VB )
extern "C" long AL_FUNCTION ALEntryGetCommentVB( hALEntry this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntry, "ALEntryGetComment" );
    char _far *p = (char _far *) ( (ALEntry *) this_object )->GetComment();
    return ALCreateVBString( p, (unsigned short int) _fstrlen( p ) );
}
#endif

//
// extern "C" hALEngine ALEntryGetEngine( hALEntry this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntry object.
//
// RETURNS
//
//  A valid handle for (pointer to) an ALCompressionEngine object. It is
//  possible to get a return value of 0, since an ALEntry is not required to
//  have a valid ALCompressionEngine object attached to it.
//
// DESCRIPTION
//
//  This function provides a translation layer for C and VB to access the
//  C++ data member ALEntry::mpCompressionEngine.  For more information on
//  what this data member actually does, see ARCENTRY.CPP.
//
//  Like most of the translation functions, this routine checks the
//  arguments for correct type (in debug mode), then casts this_object
//  to the desired class, and gets the data member.  The data member
//  is passed right back to the calling routine after being cast to the
//  appropriate C/VB type.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" hALEngine AL_FUNCTION ALEntryGetEngine( hALEntry this_object )
{
    AL_ASSERT_OBJECT( this_object, ALEntry, "ALEntryGetEngine" );
    return (hALEngine) ( (ALEntry *) this_object )->mpCompressionEngine;
}

//
// extern "C" void ALEntrySetEngine( hALEntry this_object, hALEngine engine )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALEntry object.
//
//  engine       : A handle for (pointer to) an ALcompressionEngine
//                 will now be attached to the ALEntry object. Note
//                 that a value of 0 is acceptable.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This function provides a translation layer for C and VB to access the
//  C++ data member ALEntry::mpCompressionEngine.  For more information on
//  what this data member actually does, see ARCENTRY.CPP.
//
//  Like most of the translation functions, this routine checks the
//  arguments for correct type (in debug mode), then casts this_object
//  to the desired class, and accesses the data member.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION ALEntrySetEngine( hALEntry this_object, hALEngine engine )
{
    AL_ASSERT_OBJECT( this_object, ALEntry, "ALEntrySetEngine" );
    if ( engine != 0 )
        AL_ASSERT_OBJECT( engine, ALCompressionEngine, "ALEntrySetEngine" );
    ( (ALEntry *) this_object )->mpCompressionEngine = (ALCompressionEngine *) engine;
}
