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


#ifndef _FILEATTR_H
#define _FILEATTR_H

/*
 * class ALFileAttributes
 *
 * DESCRIPTION
 *
 * This class is used to carry around file attributes.  Its only
 * real job at this time is to sit in the ALStorage class
 * and then produce packed attributes for inclusion in an archive,
 * and vice versa.  We will need to add some additional members
 * here for searching archives based on certain attribute criteria.
 *
 * Note that most of the classes in Archive library deliberately
 * withhold the copy constructor and assignment operator.  In this case,
 * however, the compiler is able to generate an adequate version of
 * these functions, so they aren't disabled.
 *
 * DATA MEMBERS
 *
 *  miReadOnly  : The read only file attribute bit, set when the file
 *                is opened or when the directory is read from the archive.
 *
 *  miSystem    : The system file attribute bit.
 *
 *  miHidden    : The hidden file attribute bit.
 *
 *  miArchive   : The archive (backup) file attribute bit.
 *
 * MEMBER FUNCTIONS
 *
 *  ALFileAttributes()          : The constructor, doesn't have to do much.
 *  ~ALFileAttributes()         : The destructor, doesn't have to do anything.
 *  operator new( size_t size ) : Memory allocation operator, only used if
 *                                the library is inside a DLL.
 *  PackedAttributes()          : Returns the bits packed into an integer
 *                                in ArchiveLib proprietary format.
 *  SetFromWin32Attributes()    : Sets the four member bits using as input
 *                                the attributes as returned in a Win32 call.
 *  GetWin32Attributes()        : Returns the four member bits packed into
 *                                Win 32 format.
 *  SetFromUNIXAttributes()     : Sets the 10 member bits returned in a
 *                                UNIX stat() call.
 *  GetUNIXAttributes()         : Returns the 9 member bits packed into
 *                                UNIX format.
 *  SetFromDosAttributes()      : Sets the four using as input the settings
 *                                returned from a DOS function call.
 *  GetDosAttributes()          : Returns the member bits formatted into
 *                                the short int used by a Dos function call.
 *  SetFromPackedAttributes()   : Sets the member bits using as input a
 *                                short int in proprietary ArchiveLib format.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

#if defined( AL_UNIX )
#include <sys/stat.h>
#endif

class AL_CLASS_TYPE ALFileAttributes {
/*
 * Constructors, destructors, declarations, and friends
 */
    public :
        AL_PROTO ALFileAttributes();
        AL_PROTO ~ALFileAttributes();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 * Member functions
 */
    public :
        short unsigned int AL_PROTO PackedAttributes();
#if defined( AL_UNIX )
        void AL_PROTO SetFromUnixAttributes( mode_t mode );
        mode_t AL_PROTO GetUnixAttributes();
#elif defined( AL_WIN32S )
        void AL_PROTO SetFromWin32Attributes( DWORD win32_attributes );
        DWORD AL_PROTO GetWin32Attributes();
#else /* MS-DOS */
        void AL_PROTO SetFromDosAttributes( unsigned dos_attributes );
        unsigned short int AL_PROTO GetDosAttributes();
#endif
        void AL_PROTO SetFromPackedAttributes( short int attributes );
/*
 * Data members
 */
    protected :
/*
 * The DOS attributes.
 */
        int miReadOnly        : 1;
        int miSystem          : 1;
        int miHidden          : 1;
        int miArchive         : 1;
/*
 * Additional attributes that might be used for UNIX files.  The
 * DOS bits are applied to the UNIX user settings.
 */
        int miUnixBitsPresent : 1;
        int miUserRead        : 1;
        int miUserWrite       : 1;
        int miUserExecute     : 1;
        int miOtherRead       : 1;
        int miOtherWrite      : 1;
        int miOtherExecute    : 1;
        int miGroupRead       : 1;
        int miGroupWrite      : 1;
        int miGroupExecute    : 1;
};

#endif /* #ifndef _FILEATTR_H */
