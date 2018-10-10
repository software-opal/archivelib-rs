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

#include "archive.h"
#include "grenengn.h"
#include "copyengn.h"
#include "filestor.h"
#include "memstore.h"
#include "wildcard.h"

//
// void * ALArchive::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  :  The number of bytes needed to create a new ALArchive object.
//
// RETURNS
//
//  A pointer to the newly allocated storage area, or 0 if no storage
//  was available.
//
// DESCRIPTION
//
//  When using a DLL, it is easy to create a dangerous situation when 
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
//   May 23, 1994  1.0A  : First release
//

#if defined( AL_BUILDING_DLL )
void AL_DLL_FAR * AL_PROTO ALArchive::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif

//
// ALArchive::ALArchive( const char *file_name )
//
// ARGUMENTS:
//
//  name  :  The name of the ALFile object that is going to hold the
//           ALArchive.
//
// RETURNS
//
//  Nothing, this is a constructor.
//
// DESCRIPTION
//
//  This is one of two constructors for ALArchive.  (The other one 
//  follows immediately in this source file.)  It is used
//  when the archive you are working with or are going to create will
//  reside in an ALFile object, which should be often.  It simply
//  calls the constructor for the base class with the appropriately
//  created ALFile object and returns.  It doesn't have to initialize
//  any data members of its own, so life is really simple.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

AL_PROTO ALArchive::ALArchive( const char AL_DLL_FAR *file_name )
    : ALArchiveBase( new ALFile( file_name ), 1 )
{
}

//
// ALArchive::ALArchive( ALStorage &so )
//
// ARGUMENTS:
//
//  so  : A storage object that will used as the storage for the archive.
//
// RETURNS
//
//  Nothing, this is a constructor.
//
// DESCRIPTION
//
//  This is one of two constructors for ALArchive.  (The other one 
//  immediately precedes this function in the same source file.)  It is used
//  when the archive you are working with or are going to create will
//  reside in an object that you have already constructed.
//  It doesn't have to initialize any data members of its own, so 
//  all it does is call the base class constructor from an
//  initializer list, and then return.
//
//  The storage object used for this archive is your responsibility to delete.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

AL_PROTO ALArchive::ALArchive( ALStorage AL_DLL_FAR &so )
    : ALArchiveBase( &so, 0 )
{
}

//
// ALArchive::~ALArchive()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  Nothing, destructor.
//
// DESCRIPTION
//
//  This destructor has an easy life of it, since it has no data
//  members to clean up after.  Instead, it leaves all of the heavy
//  lifting to the base class destructor.  In debug mode we at least
//  do *something*, which is just to verify that this is the correct
//  type of object.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

AL_PROTO ALArchive::~ALArchive()
{
    AL_ASSERT( GoodTag(), "~ALArchive: attempt to delete invalid object" );
}

//
// ALCompressionEngine *ALArchive::CreateCompressionEngine( int engine_type )
//
// ARGUMENTS:
//
//  engine_type  : An integer from ALDEFS.H that defines the type of compression
//                 to be created.
//
// RETURNS
//
//  A pointer to a newly created compression engine.  If things go bad,
//  a value of 0 is possible.
//
// DESCRIPTION
//
//  ALArchiveBase doesn't know how to create compression engines or
//  storage objects.  That knowledge is left up to derived classes like
//  this one.  When extracting objects from an archive, this class has
//  to be able to create a compression engine with no more information
//  than the integer engine type stored in the archive directory.
//  This is where we do it.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

ALCompressionEngine AL_DLL_FAR *
AL_PROTO ALArchive::CreateCompressionEngine( int engine_type )
{
    switch ( engine_type ) {
        case AL_COMPRESSION_COPY :
            return new ALCopyEngine();
        case AL_COMPRESSION_GREENLEAF :
            return new ALGreenleafEngine();
        default :
            mStatus.SetError( AL_UNKNOWN_COMPRESSION_TYPE,
                                "Unknown compression type (%d) found in archive",
                                engine_type );
            break; //Break instead of return because of bogus warnings
    }
    return 0;
}

//
// ALStorage * ALArchive::CreateStorageObject( const char *name,
//                                             int object_type )
//
// ARGUMENTS:
//
//  name        : The name of the storage object to be created.
//
//  object_type : The object type, from ALStorageType in STORAGE.H.
//
// RETURNS
//
//  A pointer to a newly storage object.  If things go bad,
//  a value of 0 is possible.
//
// DESCRIPTION
//
//  ALArchiveBase doesn't know how to create compression engines or
//  storage objects.  That knowledge is left up to derived classes like
//  this one.  When extracting objects from an archive, this class has
//  to be able to create a storage object with no more information
//  than the integer object type stored in the archive directory.
//  This is where we do it.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

ALStorage AL_DLL_FAR * AL_PROTO
ALArchive::CreateStorageObject( const char AL_DLL_FAR *name,
                                int object_type )
{
    switch ( object_type ) {
        case AL_MEMORY_OBJECT :
           return new ALMemory( name );
        case AL_FILE_OBJECT :
            return new ALFile( name );
        default :
            mStatus.SetError( AL_UNKNOWN_STORAGE_OBJECT,
                              "Unknown storage object type (%d) "
                              "found in archive",
                              object_type );

            break;
    }
    return 0;
}

// STATIC MEMBER FUNCTION
//
// int ALArchive::AddWildCardFiles(
//                      ALEntryList & list,
//                      const char *wild_spec = "*.*",
//                      int traverse_flag  = 0,
//                      short int compression_level = AL_GREENLEAF_LEVEL_2,
//                      short int fail_uncompressible = 0 )
//
// ARGUMENTS:
//
//  list                :  The list that is going to receive the newly created
//                         ALEntry objects.
//
//  wild_spec           :  The wild card file spec to expand into a list of
//                         files.  Note that this string can contain multiple
//                         file specs, separated by commas or spaces.
//
//  traverse_flag       :  A flag to indicate whether wild card expansion
//                         should traverse subdirectories.
//
//  compression_level   :  The compression level that will be used to create
//                         compression engines for the new entries.
//
//  fail_uncompressible :  The fail_uncompressible flag that will be used
//                         when creating the compression engines.
//
// RETURNS
//
//  The number of entries created by the wild card expansion.
//
// DESCRIPTION
//
//  It is handy to have a function that will create a bunch of ALEntry
//  objects and add them to a list of your choosing.  We can't do this
//  in the base class of ALArchiveBase, because it doesn't know anything
//  about specific storage objects or compression engines.  So the
//  logical place to turn is to this derived class, which does know about
//  ALFile and ALMemory.  
//
//  The only bad thing here is that this is a static public functions, 
//  so it is kind of confusing.  You might think that this ought to be
//  a member function of ALEntryList.  The problem is, we don't want
//  ALEntryList to know about specific engines or compression classes,
//  because then they would get linked in to any application created
//  with ArchiveLib.
//
//  So anyway, this function creates a bunch of new ALEntry objects,
//  with the storage object being an instance of class ALFile, and the
//  compression engine being an object of class ALGreenleafEngine.  It
//  then adds the new ALEntry to the specified ALEntryList, so you don't
//  have to do any work at all.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Patch B.  I wanted to allow users calling this
//                          function to set the fail_uncompressible flag
//                          in all the engines created here.  It was added
//                          as a default parameter to this function, which
//                          is set to 0 by default.  This adds the capability
//                          without breaking any existing code.
//
int AL_PROTO ALArchive::
AddWildCardFiles( ALEntryList AL_DLL_FAR & list,
                  const char AL_DLL_FAR *wild_spec /* = "*.*" */,
                  int traverse_flag /* = 0 */,
                  short int compression_level /* = AL_GREENLEAF_LEVEL_2 */,
                  short int fail_uncompressible /* = 0 */ )
{
    AL_ASSERT( wild_spec != 0, "AddWildCardFiles: null parameter for wild_spec" );
    ALWildCardExpander files( wild_spec, traverse_flag );
    int count = 0;

    char *new_name;
    while ( ( new_name = files.GetNextFile() ) != 0 ) {
        new ALEntry( list,
                     new ALFile( new_name ),
                     new ALGreenleafEngine( compression_level, fail_uncompressible ) );
        count++;
    }
    return count;
}

//
// int ALArchive::MakeEntriesFromListBox( ALEntryList &list,
//                                        HWND hDlg,
//                                        int list_box /* = -1 */ )
//
// ARGUMENTS:
//
//  list     :  The list that is going to get the new entries.
//
//  hDlg     :  The handle of the dialog box that contains the list box
//              control.  If you aren't using a dialog box, you can pass
//              the handle of the list box control directly, and set
//              the list_box parameter to -1.
//
//  list_box :  The id of the list box control in the dialog box.  If this
//              parameter is set to -1, it means that hDlg doesn't refer
//              to a dialog, it refers to the actual handle of a list box
//              control.
//
// RETURNS
//
//  The number of new entries created from the list box.
//
// DESCRIPTION
//
//  This function goes through a list box, and picks out all the
//  highlighted entries.  It creates a new ALEntry object for each
//  of the marked entires, using ALFile and ALGreenleafEngine objects.
//  For purposes of orthogonality, I  probably should have had a
//  compression_level parameter here, but I neglected to include that.
//  We could do it in the next release using a default parameter and
//  nobody would even notice.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

#if defined( AL_WINDOWS_GUI )

int AL_PROTO ALArchive::
MakeEntriesFromListBox( ALEntryList AL_DLL_FAR &list,
                        HWND hDlg,
                        int list_box /* = -1 */ )
{
    HWND window;

    if ( list_box != -1 )
        window = GetDlgItem( hDlg, (short int) list_box );
    else
        window = hDlg;
    int count = (WORD) SendMessage( window, LB_GETSELCOUNT, 0, 0L );
    if ( count == LB_ERR )
        return AL_GETSEL_ERROR;
    int *items = new int[ count ];
    if ( items == 0 )
        return AL_CANT_ALLOCATE_MEMORY;
#ifdef AL_FLAT_MODEL
    if ( count != SendMessage( window, LB_GETSELITEMS, (short int) count, (LPARAM) items ) ) {
#else
    if ( count != SendMessage( window, LB_GETSELITEMS, (short int) count, (LPARAM)(int _far *)  items ) ) {
#endif
        delete items;
        return AL_GETSEL_ERROR;
    }
    for ( WORD i = 0 ; i < (WORD) count ; i++ ) {
        WORD length = (WORD) SendMessage( window, LB_GETTEXTLEN, (short int) items[ i ], 0L );
        if ( length > 0 ) {
            char *name = new char[ length + 1 ];
            if ( name ) {
                if ( SendMessage( window, LB_GETTEXT, (short int) items[ i ], (LPARAM)( (LPSTR) name ) ) >= 0 ) {
                    new ALEntry( list,
                                 new ALFile( name ),
                                 new ALGreenleafEngine() );
                }
                delete name;
                SendMessage( window,
                             LB_SETSEL,
                             0,
                             items[ i ] );
            }
        }
    }
    delete items;
    return count;
}

#endif //#ifdef AL_WINDOWS_GUI
