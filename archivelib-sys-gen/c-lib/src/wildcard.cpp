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

#include <stdlib.h>
#ifdef __BORLANDC__
#include <dir.h>
#endif

#include "wildcard.h"
#ifdef AL_UNIX
#include <sys/stat.h>
#endif

//
// void * ALWildCardExpander::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  :  The number of bytes needed to create a new ALWildCardExpander
//           object.
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
void AL_DLL_FAR * AL_PROTO ALWildCardExpander::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif

//
// ALWildCardExpander::ALWildCardExpander( const char *file_list,
//                                         int traverse_flag = 0,
//                                         ALCase name_case = AL_LOWER ) 
//
// ARGUMENTS:
//
//  file_list       : A list of wild card file specifications, separated
//                    by commas, semicolons, or spaces, maybe looking 
//                    something like this: "*.CPP, BOB.DAT, *.*"
//
//  traverse_flag   : A flag that indicates whether you want to traverse
//                    all subdirectories under the current path.
//
//  name_case       : An indicator of whether you want all the returned
//                    file names forced to a certain case.
//
// RETURNS
//
//  No returns.
//
// DESCRIPTION
//
//  The constructor for the expander has to set up a bunch of data members
//  that will all be used during the expansion process.  The mCase
//  member is easy to understand.  All of the objname objects that
//  we create are going to be force to a certain case by this
//  using this data member.  miTraverseFlag is just our copy of the
//  input parameter.  And the mState variable keeps track of what we
//  are doing in between function calls.  We set it to GET_NEXT_WILD_NAME,
//  which means we will be doing that the first time we get called.
// 
//  mInputLine is where we keep a copy of the list of wild card file
//  specifications passed by the calling program.  Each time we take
//  a new file name out of mInputLine, we remove it from the ALName
//  object, making mInputLine just a little shorter.
//
//  The mResultFileName member is the storage area where we keep a copy
//  of the file name created by the expander.  This is our local copy,
//  when it gets returned to the calling program they need to make
//  their own copy of it and leave ours alone.
//
//  Every time we get asked to get a new file, the very first thing
//  we do is check to see if the mpNextExpander member is pointing
//  to a new expander object.  If it is, we ask him to provide
//  the next file name, instead of giving it ourselves.  When he
//  doesn't have any file names left to give, we destroy him and
//  set that pointer back to 0.  Here in the constructor, the smart
//  thing to do is set him to 0 for starters.
//
//  The final data member differs between NT and DOS.  The structure
//  NT uses to expand directories is store in mFindFileHandle.  The
//  DOS version is stored in mpFfblk.  Both of these are presently
//  in an invalid state, but will get initialized when the user
//  calls the member function.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : Added support for wild card expansion under
//                         UNIX.  UNIX uses an opendir()/readir() loop to
//                         expand wild cards, instead of the MS-DOS
//                         get_first()/get_next() strategy.  So our class
//                         under UNIX keeps a pointer to the open directory.
//                         We initialize it to 0 here which tells the destructor
//                         that we don't have a directory open.  If the
//                         destructor is called with a non-zero value in
//                         mpDirectory, it will try to close the directory.
//

AL_PROTO ALWildCardExpander::ALWildCardExpander(
          const char AL_DLL_FAR *file_list,
          int traverse_flag /* = 0 */,
          ALCase name_case /* = AL_LOWER */ )
    : mCase( name_case ),
      mResultFileName( "", name_case )
{
    mInputLine = file_list;
    mState = GET_NEXT_WILD_NAME;
    mpNextExpander = 0;
    miTraverseFlag = traverse_flag;
#if defined( AL_UNIX )
    mpDirectory = 0;
#elif defined( AL_WIN32S )
    mFindFileHandle = INVALID_HANDLE_VALUE;
#else
    mpFfblk = new find_t;
#endif
}

//
// ALWildCardExpander::~ALWildCardExpander()
//
// ARGUMENTS:
//
//  None, destructors don't get any.
//
// RETURNS
//
//  None, destructor.
//
// DESCRIPTION
//
//  There are a couple of big deals we need to worry about in the
//  destructor an ALWildCardExpander.  First, we have to worry about
//  any additional handlers we created to search subdirectories.  If
//  this destructor is being called before our search is done, we
//  may have some of those expander objects just hanging around out
//  there.  We take care of the by checking the mpNextExpander member.
//  If it isn't set to 0, we delete the dynamically created expander.
//
//  Under NT we also have to worry about our mpFindFileHandle.  Under
//  NT, the file expansion algorithm isn't just a get first/get next
//  deal.  Instead, it is get first/get next/terminate.  The termination
//  is done using the FindClose() call.  If we still had a search in progress
//  we call that function.
//
//  Under DOS, we just have to delete the dynamically created
//  mpFfblk structure.  I wanted to make that a data member of this
//  class, instead of a pointer, but one of our compilers wasn't happy
//  about putting this C struct in a class, it complained about something.
//  So, to expedite, we made it a pointer.
//
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : Added support for UNIX.  In the destructor,
//                         I have to close any open directory that was
//                         being scanned.
//
AL_PROTO ALWildCardExpander::~ALWildCardExpander()
{
    if ( mpNextExpander )
        delete mpNextExpander;
#if defined( AL_UNIX )
    if ( mpDirectory )
        closedir( mpDirectory );
#elif defined( AL_WIN32S )
    if ( mFindFileHandle != INVALID_HANDLE_VALUE )
        FindClose( mFindFileHandle );
#else
    if ( mpFfblk )
        delete mpFfblk;
#endif
}

// PROTECTED MEMBER FUNCTION
//
// int ALWildCardExpander::GetNextWildName()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  1 if it got a new file spec, 0 if it didn't.
//
// DESCRIPTION
//
//  This function is called internally to get the next file spec out of
//  the input line.  This is simply a matter of parsing past all the 
//  delimiter characters.  The resulting file spec is stored in
//  data member mFullWildName.  That member will be the one used to
//  kick off the next wild card search.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : Under Sun UNIX I was having quite a tough time
//                         figuring out which constant to use for this
//                         value.  This hack will work, but I don't know
//                         if it is good.

#ifdef AL_UNIX
  #ifndef _MAX_PATH
    #define _MAX_PATH 255
  #endif
#endif

int AL_PROTO ALWildCardExpander::GetNextWildName()
{
    char wild_spec[ _MAX_PATH ];
    int i = 0;
    char *p = mInputLine;

    for ( ; ; p++ ) {
        int c = *p;
        if ( c != ' ' && c != ',' && c != '\t' )
            break;
    }
    for ( ; ; p++ ) {
        int c = *p;
        if ( c == ' ' || c == ',' || c == '\t' || c == '\0' )
            break;
        wild_spec[ i++ ] = (char) c;
        if ( i >= ( _MAX_PATH - 2 ) )
            return 0;
    }
    wild_spec[ i++ ] = '\0';
    if ( i <= 1 )
         return 0;
    mFullWildName = wild_spec;
    mInputLine = p;
    return 1;
}

//
// char * ALWildCardExpander::GetNextFile()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  In the event that this routine is able to come up with a new
//  file name, it returns a character pointer to the name, which
//  is kept in member variable mResultFileName.  If no new file
//  name could be cooked up, we return a 0, which means you are
//  done.
//
// DESCRIPTION
//
//  There are two wild card expander routines.  One for NT, and one
//  for DOS.  They are both very similar in structure, but they weren't
//  quite close enough to combine into a single routine.  However, the
//  both share a common structure, which is being described here.
//
//  The ALWildCardExpander has what amounts to six different internal
//  states.  They are:
//
//              Searching subdirectories, using another object
//
//              Extracting the next wild spec from the input line
//
//              Expanding the wild card to get the first matching file
//
//              Expanding the wild card to get the next matching file
//
//              Looking for the first subdirectory
//
//              Looking for the next subdirectory
//
//  For the most part, we keep track of the state using the mState
//  variable.  However, we keep track of whether we are searching
//  subdirectories by examining the pointer to the next expander.  If
//  it is non-null, it means we are in that state.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : Added support for UNIX.  This meant creating a
//                         completely new version of this routine.  It would
//                         have been possible with conditional compilation, but
//                         things started looking nasty when I tried that approach.
//

#if defined( AL_UNIX )

char AL_DLL_FAR * AL_PROTO ALWildCardExpander::GetNextFile()
{
    struct dirent *entry;
    struct stat buf;
    ALName *temp;
//
// I sit in this loop forever, until something causes me to return.  It is
// possible to iterate through the loop many times before returning.
//
    for ( ; ; ) {
//
// If I am presently searcing a subdirectory, my mpNextExpander member will
// be pointing to another expander.  If that is the case, I call him to get
// the next file.
//
        if ( mpNextExpander ) {
            char *p = mpNextExpander->GetNextFile();
            if ( p )
                return p;          // Return the name if he found one
            delete mpNextExpander; // If not, he is toast
            mpNextExpander = 0;
        }
        switch ( mState ) {
//
// This is where I get the next wild spec from the input line.  If
// there aren't any more, I return 0, because we are done.  If there
// is one, I set up the member variable that will be used in the
// rest of the search, and set up the state so that next I will get
// get the first file name.
//
            case GET_NEXT_WILD_NAME :
                if ( GetNextWildName() == 0 )
                    return 0;
                mWildPathOnly = mFullWildName;
                mWildPathOnly.StripFileName();
                mWildNameOnly = mFullWildName;
                mWildNameOnly.StripPath();
                mState = GET_FIRST_FILE_NAME;
                break;
//
// Once I have a wild spec, time to start getting file names.
// Under NT and MS-DOS, I can get a file name right away with a
// call to some sort of get_first() routine.  Under UNIX, things
// are a little different.  I just open the directory in this step,
// and don't start pulling files out until the next state.  So this
// case *never* returns to the calling program.
//
            case GET_FIRST_FILE_NAME :
                mpDirectory = opendir( mWildPathOnly );
                if ( mpDirectory == 0 ) {
                    if ( miTraverseFlag )
                        mState = GET_FIRST_DIRECTORY;
                    else
                        mState = GET_NEXT_WILD_NAME;
                    break;
                }
                mState = GET_NEXT_FILE_NAME;
                break;
//
// Time to get another file name with readdir().  If there aren't
// any more, I clean up, and either get the next name for the input
// line or start searching subdirectories. If there was a name, I return
// it to the calling procedure. Unlike MS-DOS, I can't ask the directory
// to just give me files, and not directories.  So I have to check to
// see if I got a directory.  If I did, I just blow it off, because I'm
// not going to worry about it until later, when I am traversing.
//
            case GET_NEXT_FILE_NAME :
                entry = readdir( mpDirectory );
                if ( entry == 0 ) {
                    closedir( mpDirectory );
                    mpDirectory = 0;
                    if ( miTraverseFlag )
                        mState = GET_FIRST_DIRECTORY;
                    else
                        mState = GET_NEXT_WILD_NAME;
                   break;
                }
                mResultFileName = ALName( mWildPathOnly + entry->d_name );
                stat( mResultFileName, &buf );
                if ( S_ISDIR( buf.st_mode ) )
                    break;
                if ( ALName( entry->d_name ).WildCardMatch( (const char *) mWildNameOnly ) == 0 )
                    break;
                return mResultFileName;
//
// To start traversing subdirectories, we have to execute another opendir().
// We don't get a file name back from this call, instead that will happen
// on the next pass through the loop in our next state.
//
// Things are a little tricky here, because the file name isn't quite right
// here.  If it is a blank, I want to do an opendir on ".".  In general, I
// think under UNIX it is always going to be a good idea to append a "."
// to the path name, indicating that I want to open the directory at the
// end of that path.
//
            case GET_FIRST_DIRECTORY :
                mpDirectory = opendir( mWildPathOnly + "." );
                if ( mpDirectory == 0 ) {
                    mState = GET_NEXT_WILD_NAME;
                    break;
                }
                mState = GET_NEXT_DIRECTORY;
                break;
//
// I make a call to readdir() to get the next directory name.  Unfortunately,
// I might get an ordinary file, not a directory.  If it turns out to be
// a file, I just blow it off.  But if it is a directory, I create a new
// expander, and set mpNextExpander to point to it.  That means when I
// start at the top of the loop I will call *his* version of GetNextFile(),
// meaning I will pull files out of that directory next.
//
            case GET_NEXT_DIRECTORY :
                entry = readdir( mpDirectory );
                if ( entry == 0 ) {
                    closedir( mpDirectory );
                    mState = GET_NEXT_WILD_NAME;
                    break;
                }
//
// I create the temp because g++ has trouble if I try to pass it
// directly.
//
                temp = new ALName( mWildPathOnly + entry->d_name + "/" );
                stat( (char *) *temp, &buf );
                if ( S_ISDIR( buf.st_mode ) ) {
                    if ( strcmp( entry->d_name, ".." ) == 0 )
                        break;
                    if ( strcmp( entry->d_name, "." ) == 0 )
                        break;
                    *temp = mWildPathOnly + entry->d_name + "/" + (char *) mWildNameOnly;
                    mpNextExpander = new ALWildCardExpander( (char *) *temp, 1, mCase );
               }
               delete temp;
               break;
            default :
                return 0;
        }
    }
}

#elif defined( AL_WIN32S )

//
// This is the NT version.  It has to use FindFirstFile() and
// FindNextFile() to get file names.  Note that this is implemented
// as a giant loop.  This means we may go through several states
// inside this routine until we finally come up with a filename.
//

char AL_DLL_FAR * AL_PROTO ALWildCardExpander::GetNextFile()
{
    for ( ; ; ) {
//
// If the pointer to the next expander is set, it means we are working
// on a subdirectory, so I have to let him do the work.  If the subdirectory
// search fails, I continue right back where I was when interrupted.
//
        if ( mpNextExpander ) {
            char *p = mpNextExpander->GetNextFile();
            if ( p )
                return p;          // Return the name if he found one
            delete mpNextExpander; // If not, he is toast
            mpNextExpander = 0;
        }
        switch ( mState ) {
//
// This is where I get the next wild spec from the input line.  If
// there aren't any more, I return 0, because we are done.  If there
// is one, I set up the member variable that will be used in the
// rest of the search, and set up the state so that next I will get
// get the first file name.
//
            case GET_NEXT_WILD_NAME :
                if ( GetNextWildName() == 0 )
                    return 0;
                mWildPathOnly = mFullWildName;
                mWildPathOnly.StripFileName();
                mWildNameOnly = mFullWildName;
                mWildNameOnly.StripPath();
                mState = GET_FIRST_FILE_NAME;
                break;
//
// Once I have a wild spec, time to start getting file names.
// FindFirstFile() does it for me.  if there aren't any files, I
// either go on to search directories, or go the the next wild
// name in the input line.  If there is a name, I return it to
// the calling procedure.
//
            case GET_FIRST_FILE_NAME :
                mFindFileHandle = FindFirstFile( mFullWildName, &mFindFileData );
                if ( mFindFileHandle == INVALID_HANDLE_VALUE ) {
                    if ( miTraverseFlag )
                        mState = GET_FIRST_DIRECTORY;
                    else
                        mState = GET_NEXT_WILD_NAME;
                    break;
                }
                mState = GET_NEXT_FILE_NAME;
                if ( mFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
                    break;
                mResultFileName = ALName( mWildPathOnly + mFindFileData.cFileName );
                return mResultFileName;
//
// Time to get another file name with FindNextFile().  If there aren't
// any more, I clean up, and either get the next name for the input
// line or start searching subdirectories. If there was a name, I return
// it to the calling procedure.
//
            case GET_NEXT_FILE_NAME :
                if ( !FindNextFile( mFindFileHandle, &mFindFileData ) ) {
                    FindClose( mFindFileHandle );
                    mFindFileHandle = INVALID_HANDLE_VALUE;
                    if ( miTraverseFlag )
                        mState = GET_FIRST_DIRECTORY;
                    else
                        mState = GET_NEXT_WILD_NAME;
                    break;
                }
                if ( mFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
                    break;
                mResultFileName = ALName( mWildPathOnly + mFindFileData.cFileName );
                return mResultFileName;
//
// The procedure to get the first subdirectory is an awful lot like that
// we use to get the first file.  If we find a valid subdirectory, we create
// a new expander to deal with its wildcards.  If we find a file, but
// it isn't a subdirectory, we keep on searching.  If we don't find
// anything, we are going to go back and check out the next file spec
// from the input line.
//
            case GET_FIRST_DIRECTORY :
                mFindFileHandle = FindFirstFile( mWildPathOnly + "*.*", &mFindFileData );
                if ( mFindFileHandle == INVALID_HANDLE_VALUE ) {
                    mState = GET_NEXT_WILD_NAME;
                    break;
                }
                mState = GET_NEXT_DIRECTORY;
                if ( mFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
                    if ( strcmp( mFindFileData.cFileName, ".." ) == 0 )
                        break;
                    if ( strcmp( mFindFileData.cFileName, "." ) == 0 )
                        break;
                    mpNextExpander = new ALWildCardExpander( mWildPathOnly + mFindFileData.cFileName + "\\" + (char *) mWildNameOnly, 1, mCase );
                }
                break;
//
// This works the same as the state where I get the first directory.
// The only difference here is that if I run out of file names in the
// directory, I have to call FindClose() to clean up after myself.
//
            case GET_NEXT_DIRECTORY :
                if ( !FindNextFile( mFindFileHandle, &mFindFileData ) ) {
                    FindClose( mFindFileHandle );
                    mFindFileHandle = INVALID_HANDLE_VALUE;
                    mState = GET_NEXT_WILD_NAME;
                    break;
                }
                if ( mFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) {
                    if ( strcmp( mFindFileData.cFileName, ".." ) == 0 )
                        break;
                    if ( strcmp( mFindFileData.cFileName, "." ) == 0 )
                        break;
                    mpNextExpander = new ALWildCardExpander( mWildPathOnly + mFindFileData.cFileName + "\\" + (char *) mWildNameOnly, 1 );
                }
                break;
            default :
                return 0;
        }
    }
}

#else

//
// This is the MS-DOS version of the file expander.  In structure,
// it is almost identical to the NT version.
//

char AL_DLL_FAR * AL_PROTO ALWildCardExpander::GetNextFile()
{
//
// mpFfblk is the pointer to my structure used by _dos_findfirst()
// and _dos_findnext().  If for some reason this is a null pointer,
// I need to quit.  The only reason this should be null is a memory
// allocation failure.
//
    if ( mpFfblk == 0 )
        return 0;
    for ( ; ; ) {
//
// If the pointer to the next expander is non-zero, it means I am in
// the middle of a subdirectory search.  If that is the case, I call
// the next expander to see if it can come up with a file name.  if
// it does, we return it.  If it doesn't, it means it is done, and
// I can delete it and try my luck with the next subdirectory.
//
        if ( mpNextExpander ) {
            char *p = mpNextExpander->GetNextFile();
            if ( p )
                return p;
            delete mpNextExpander;
            mpNextExpander = 0;
        }
        switch ( mState ) {
//
// This is where I start, and this is where I end up after completely
// processing one of the input wild specs.  I get the next name from
// the input line here.  If there aren't any more names, I can return
// 0, meaning the whole thing is done.
//
            case GET_NEXT_WILD_NAME :
                if ( GetNextWildName() == 0 )
                    return 0;
                mWildPathOnly = mFullWildName;
                mWildPathOnly.StripFileName();
                mWildNameOnly = mFullWildName;
                mWildNameOnly.StripPath();
                mState = GET_FIRST_FILE_NAME;
                break;
//
// Once I have a file name, I start parsing using _dos_findfirst().
// If that doesn't return a name, I have struck out on my first swing.
// if that is the case, I either move on to start searching subdirectories,
// or go back and look for another name from the input line.  On the
// other hand, if I get a name, I return it to the caller.
//
            case GET_FIRST_FILE_NAME :
                if ( _dos_findfirst( mFullWildName, 0, mpFfblk ) ) {
                    if ( miTraverseFlag )
                        mState = GET_FIRST_DIRECTORY;
                    else
                        mState = GET_NEXT_WILD_NAME;
                    break;
                }
                mState = GET_NEXT_FILE_NAME;
                mResultFileName = ALName( mWildPathOnly + mpFfblk->name );
                return mResultFileName;
//
// This state is identical to GET_FIRST_FILE_NAME, except it has to
// use _dos_findnext() instead of _dos_findfirst()
//
            case GET_NEXT_FILE_NAME :
                if ( _dos_findnext( mpFfblk ) ) {
                    if ( miTraverseFlag )
                        mState = GET_FIRST_DIRECTORY;
                    else
                        mState = GET_NEXT_WILD_NAME;
                    break;
                }
                mResultFileName = mWildPathOnly + mpFfblk->name;
                return mResultFileName;
//
// After getting all of the file names that a wildspec expands into,
// we can start searching subdirectories, if needed.  Unlike with NT,
// we can set our search up to look for directories only.  that means
// we don't have to check the status of the file returned from _dos_findxxxx().
// However, we always do have to check to make sure it isn't one of the
// two bogus directory entries, "." or "..".
//
// If we score here, we create a new ALWildCardExpander, and put him to
// work.  If we strike out, time to go back and get our next input
// file name.
//
            case GET_FIRST_DIRECTORY :
                if ( _dos_findfirst( mWildPathOnly + "*.*", _A_SUBDIR, mpFfblk ) ) {
                    mState = GET_NEXT_WILD_NAME;
                    break;
                }
                mState = GET_NEXT_DIRECTORY;
                if ( mpFfblk->attrib & _A_SUBDIR ) {
                    if ( strcmp( mpFfblk->name, ".." ) == 0 )
                        break;
                    if ( strcmp( mpFfblk->name, "." ) == 0 )
                        break;
                    mpNextExpander = new ALWildCardExpander( mWildPathOnly + mpFfblk->name + "\\" + (char *) mWildNameOnly, 1, mCase );
                }
                break;
//
// This is just like GET_FIRST_DIRECTORY, except it gets to call
// _dos_findnext() instead of _dos_findfirst().
//
            case GET_NEXT_DIRECTORY :
                if ( _dos_findnext( mpFfblk ) ) {
                    mState = GET_NEXT_WILD_NAME;
                    break;
                }
                if ( mpFfblk->attrib & _A_SUBDIR ) {
                    if ( strcmp( mpFfblk->name, ".." ) == 0 )
                        break;
                    if ( strcmp( mpFfblk->name, "." ) == 0 )
                        break;
                    mpNextExpander = new ALWildCardExpander( mWildPathOnly + mpFfblk->name + "\\" + (char *) mWildNameOnly, 1 );
                }
                break;
            default :
                return 0;
        }
    }
#if defined( AL_MICROSOFT ) && ( AL_MICROSOFT < 800 )
    return 0; //MSC 7.0 thinks I need this return path.  No way to get here!
#endif
}
#endif // #if defined( AL_WIN32S )... #else
