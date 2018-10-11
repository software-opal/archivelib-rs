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


#ifndef __OPENF_H
#define __OPENF_H

#include "arclib.h"

/*
 * class ALOpenInputFile
 *
 * DESCRIPTION
 *
 *  This is a utility class.  The constructor opens a file for input,
 *  and keeps track of whether it was already open or not.  The destructor
 *  will automatically close the file if it was closed when the
 *  ctor was invoked.
 *
 * DATA MEMBERS
 *
 * miFileWasOpen   : The flag that keeps track of the file's state
 *                   at the start of the routine.
 *
 * mpFile          : A pointer to the file, so we can close it in the dtor.
 *
 * MEMBER FUNCTIONS
 *
 *  ALOpenInputFile   : The constructor, opens the file.
 *  ~ALOpenInputFile  : The destructor, can close the file.
 *  operator new      : This operator is used in the Win16
 *                      DLL version of ArchiveLib.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

class AL_CLASS_TYPE ALOpenInputFile {
    public :
        AL_PROTO ALOpenInputFile( ALStorage AL_DLL_FAR &file );
        AL_PROTO ~ALOpenInputFile();
#if defined( AL_BUILDING_DLL ) || defined( AL_USING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 * Prevent the compiler from generating these members.
 */
    protected :
        AL_PROTO ALOpenInputFile( ALOpenInputFile AL_DLL_FAR &);
        ALOpenInputFile AL_DLL_FAR & operator=( ALOpenInputFile AL_DLL_FAR & );
    protected :
        int miFileWasOpen;
        ALStorage AL_DLL_FAR *mpFile;
};

/*
 * class ALOpenOutputFile
 *
 * DESCRIPTION
 *
 *  This is a utility class.  The constructor opens a file for output,
 *  and keeps track of whether it was already open or not.  The destructor
 *  will automatically close the file if it was closed when the
 *  ctor was invoked.
 *
 * DATA MEMBERS
 *
 * miFileWasOpen   : The flag that keeps track of the file's state
 *                   at the start of the routine.
 *
 * mpFile          : A pointer to the file, so we can close it in the dtor.
 *
 * MEMBER FUNCTIONS
 *
 *  ALOpenOutputFile   : The constructor, opens the file.
 *  ~ALOpenOutputFile  : The destructor, can close the file.
 *  operator new       : This operator is used in the Win16
 *                       DLL version of ArchiveLib.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

class AL_CLASS_TYPE ALOpenOutputFile {
    public :
        AL_PROTO ALOpenOutputFile( ALStorage AL_DLL_FAR &file );
        AL_PROTO ~ALOpenOutputFile();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 * Prevent the compiler from generating these members.
 */
    protected :
        AL_PROTO ALOpenOutputFile( ALOpenOutputFile AL_DLL_FAR &);
        ALOpenOutputFile AL_DLL_FAR & operator=( ALOpenOutputFile AL_DLL_FAR & );
    protected :
        int miFileWasOpen;
        ALStorage AL_DLL_FAR *mpFile;
};

/*
 * class ALOpenFiles
 *
 * DESCRIPTION
 *
 *  This is a utility class.  The constructor opens the first file for
 *  input, and the second for output.  It does so using the previous
 *  two classes, so it doesn't have to keep track of anything.
 *
 * DATA MEMBERS
 *
 *  mInputFile     : The input file open object.  It does all the work
 *                   related to the input file.
 *
 *  mOutputFile    : The output file open object.  It does all the work
 *                   related to the output file.
 *
 * MEMBER FUNCTIONS
 *
 *  ALOpenFiles   : The constructor, opens both files.
 *
 *  ~ALOpenFiles  : The destructor, can close one or both files.
 *
 *  operator new  : This operator is used in the Win16
 *                  DLL version of ArchiveLib.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

class AL_CLASS_TYPE ALOpenFiles {
    public :
        AL_PROTO ALOpenFiles( ALStorage AL_DLL_FAR &input,
                              ALStorage AL_DLL_FAR &output );
        AL_PROTO ~ALOpenFiles();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 * Prevent the compiler from generating these members.
 */
    protected :
        AL_PROTO ALOpenFiles( ALOpenFiles AL_DLL_FAR & );
        ALOpenFiles AL_DLL_FAR & operator=( ALOpenFiles AL_DLL_FAR & );
    protected :
        ALOpenInputFile mInputFile;
        ALOpenOutputFile mOutputFile;
};

#endif /* #ifndef __OPENF_H */
