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


#ifndef _ARCHIVE_H
#define _ARCHIVE_H

#include "arclib.h"

#if defined( __cplusplus )

#include "filestor.h"
#include "grenengn.h"

/*
 * class ALArchive : public ALArchiveBase
 *
 * DESCRIPTION
 *
 *  ALArchive is a class derived from ALArchiveBase.  It is the
 *  class we use most often for standard archiving use.
 *
 * DATA MEMBERS
 *
 *  None.
 *
 * MEMBER FUNCTIONS
 *
 *  ALArchive(char *)          : A constructor that creates the storage
 *                               object for you.
 *  ALArchive(ALStorage&)      : Constructor using a storage object that
 *                               you have created.
 *  ~ALArchive()               : Virtual destructor.
 *  operator new()             : Memory allocation when inside a DLL.
 *  CreateCompressionEngine()  : Virtual function that knows how to make
 *                               ALCopyEngine and ALGreenleafEngine engines.
 *  CreateStorageObject()      : Virtual function that knows how to make
 *                               ALFile and ALMemory objects.
 *  AddWildCardFiles()         : A static function used to create
 *                               new ALEntry objects, and add them to an
 *                               ALEntryList object.
 *  MakeEntriesFromListBox()   : A second static function used to create
 *                               new ALEntry objects and add them to an
 *                               ALEntryList object.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

class AL_CLASS_TYPE ALArchive : public ALArchiveBase {
/*
 * Constructors, destructors, declarations
 */
    public :
        AL_PROTO ALArchive( const char AL_DLL_FAR *file_name );
        AL_PROTO ALArchive( ALStorage AL_DLL_FAR &object );
        virtual AL_PROTO ~ALArchive();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 * As usual, I prohibit the copy constructor and assignment operator.
 * They are declared here but they don't exist.
 */
    protected :
        AL_PROTO ALArchive( ALArchive AL_DLL_FAR & );
        ALArchive AL_DLL_FAR & AL_PROTO operator=( ALArchive AL_DLL_FAR & );
/*
 * Member Functions
 *
 * Note, these guys are used inside the Archive class for extraction.
 * The may be better off protected.
 */
    public :
        virtual ALCompressionEngine AL_DLL_FAR * AL_PROTO CreateCompressionEngine( int engine_type );
        virtual ALStorage AL_DLL_FAR * AL_PROTO CreateStorageObject( const char AL_DLL_FAR *name, int object_type );

    public :
/*
 * These two member functions are both static.  They are in this class
 * to take advantage of this classes knowledge of how to create
 * ALFile and ALGreenleafEngine objects.  They are used to create
 * new ALEntry objects.  It would be good in a future release to
 * change their names to be more uniform.
 */
        static int AL_PROTO
        AddWildCardFiles( ALEntryList AL_DLL_FAR & list,
                          const char AL_DLL_FAR *pattern = "*.*",
                          int traverse_flag = 0,
                          short int compression_level = AL_GREENLEAF_LEVEL_2,
                          short int fail_uncompressible = 0 );
#if defined( AL_WINDOWS_GUI )
        static int AL_PROTO MakeEntriesFromListBox( ALEntryList AL_DLL_FAR &list, HWND hDlg, int list_box = -1 );
#endif
/*
 * Data members, hey there aren't any!
 */
    public :
        AL_CLASS_TAG( _ALArchiveTag );
};

#endif /* #if defined( __cplusplus ) */

#endif /* #ifndef _ARCHIVE_H        */
