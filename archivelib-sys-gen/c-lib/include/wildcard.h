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


#ifndef _WILDCARD_H
#define _WILDCARD_H

#if defined( AL_UNIX )
#include <dirent.h>
#elif defined( AL_WIN32S )
#else
#include <dos.h>
#endif
#include "arclib.h"

#if defined( __cplusplus )


/*
 * class ALWildCardExpander
 *
 * DESCRIPTION
 *
 * This class is used to expand wild card specifications on a DOS
 * or NT file system. Note that you can do exciting things with this, like
 * traversing through subdirectories, and separate various specs using 
 * commas or white space.
 *
 * DATA MEMBERS
 *
 *  mState          : The current state of the expander.  This is a value
 *                    from ALExpander state that lets use keep track
 *                    of where we are between calls to the expander.
 *
 *  mpNextExpander  : If we are traversing subdirectories, we will
 *                    open a new expander for each subdirectory.  This
 *                    is a pointer to any subdirectory we might already
 *                    have open for a search in progress.
 *
 *  mInputLine      : The list of wildcard filespecs passed in as an
 *                    argument.  This gets whittled away, and will be
 *                    smaller and smaller as all the names are parsed out.
 *
 *  mFullWildName   : The current wild card file spec that is being
 *                    expanded.
 *
 *  mWildNameOnly   : The current wild name that is being expanded,
 *                    stripped of its drive and path information.
 *
 *  mWildPathOnly   : The current drive and path being expanded, stripped
 *                    of its filename and extension.
 *
 *  mResultFileName : The file name that is returned to the calling
 *                    program.
 *
 * mpDirectory      : Under UNIX, directories are scanned using the
 *                    the opendir()/readdir() commands.  The value
 *                    returned by opendir() gets stored here so it can
 *                    be reused in subsequent calls.
 *
 *  mFindFileData   : Under Win32s, this holds data about the file we
 *                    found.
 *
 *  mFindFileHandle : Under Win32s, this is a handle used during the
 *                    wildcard expansion.
 *
 *  mpFfblk         : Under MS-DOS, this structure holds the state of
 *                    the wildcard expansion in progress.
 *
 *  miTraverseFlag  : This flag indicates whether the search should traverse
 *                    traverse through subdirectories or just search
 *                    in the current directory.
 *
 *  mCase           : Indicates whether file names should always be forced
 *                    to upper case, forced to lower case, or left mixed.
 *
 * MEMBER FUNCTIONS
 *
 *  ALWildCardExpander()    : Constructor, everything we need to know to
 *                            perform the search is defined here.
 *  ~ALWildCardExpander()   : Destructor.
 *  operator new()          : Memory allocation function, used when the
 *                            library is in a DLL.
 *  GetNextWildName()       : Protected routine to get the next wild name
 *                            from the input line.
 *  GetNextFile()           : The function to get the next expanded file
 *                            name.  It keeps chunking out names until
 *                            the search is complete.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

class AL_CLASS_TYPE ALWildCardExpander {
/*
 * Constructors, destructors, assignment operators, declarations
 */
    protected :
        enum ALExpanderState {
            GET_NEXT_WILD_NAME,
            GET_FIRST_FILE_NAME,
            GET_NEXT_FILE_NAME,
            GET_FIRST_DIRECTORY,
            GET_NEXT_DIRECTORY,
        };
    public :
        AL_PROTO ALWildCardExpander( const char AL_DLL_FAR *file_list,
                                     int traverse_flag = 0,
#ifdef AL_UNIX
                                     ALCase name_case = AL_MIXED );
#else
                                     ALCase name_case = AL_LOWER );
#endif
        AL_PROTO ~ALWildCardExpander();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 * Disable copy constructor and assignment operator
 */
    protected :
        ALWildCardExpander AL_DLL_FAR & AL_PROTO operator=( ALWildCardExpander AL_DLL_FAR & );
        AL_PROTO ALWildCardExpander( ALWildCardExpander AL_DLL_FAR & );
/*
 * Member functions
 */
    protected :
        int AL_PROTO GetNextWildName();

    public :
        char AL_DLL_FAR * AL_PROTO GetNextFile();
/*
 * Data members
 */
    protected :
        ALExpanderState mState;
        ALWildCardExpander AL_DLL_FAR *mpNextExpander;
        ALName mInputLine;
        ALName mFullWildName;
        ALName mWildNameOnly;
        ALName mWildPathOnly;
        ALName mResultFileName;
#if defined( AL_UNIX )
        DIR *mpDirectory;
#elif defined( AL_WIN32S )
        WIN32_FIND_DATA mFindFileData;
        HANDLE mFindFileHandle;
#else
        struct find_t AL_DLL_FAR *mpFfblk;
#endif
        int miTraverseFlag;
    public :
        const ALCase mCase;
        AL_CLASS_TAG( _ALWildCardExpanderTag );
};

#endif /* #if defined( __cplusplus ) */

#endif /* #ifdef _WINMON_H           */
