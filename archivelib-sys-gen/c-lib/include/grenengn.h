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


#ifndef _GRENENGN_H
#define _GRENENGN_H

#include "arclib.h"

#if defined( __cplusplus )


#include "cmpengn.h"

/*
 * class ALGreenleafEngine : public ALCompressionEngine 
 *
 * DESCRIPTION
 *
 * ALGreenleaf Engine is the Greenleaf proprietary compression
 * engine.  Compression engines are simple, so there aren't 
 * too many functions.  This class has two data members that
 * are initialized in the constructor.  One of them, the compression level, 
 * has to be saved with the data in order for decompression to work
 * properly.  It is saved and read with the virtual functions
 * ReadEngineData() and WriteEngineData(), using a single short
 * in the archive directory.
 *
 * DATA MEMBERS
 *
 *  miCompressionLevel    : This is the compression level that the
 *                          compressor will attempt to use, selected
 *                          from one of the five settings found in
 *                          the enumerated type in ALDEFS.H.  If the
 *                          file is incompressible and the engine
 *                          performs a straight binary copy this value
 *                          changes to AL_GREENLEAF_COPY.
 *
 *  miFailUncompressible  : This data member is used to flag the
 *                          action the compressor takes if a file
 *                          turns out to be incompressible.  If this
 *                          member is set, the engine will stop compressing,
 *                          seek back to the start of the file, and
 *                          just copy the data.  If this member is clear,
 *                          we don't ever check to see if the file is
 *                          compressing properly.
 *
 * MEMBER FUNCTIONS
 *
 *  ALGreenleafEngine()     : The constructor.
 *  ~ALGreenleafEngine()    : The virtual destructor.
 *  operator new()          : The memory allocation operator, which is
 *                            only used when the library is in a DLL.
 *  WriteEngineData()       : The virtual function that writes private
 *                            configuration data.  For this class, this
 *                            function writes out a single short int 
 *                            containing the compression level.
 *  ReadEngineData()        : The complement for the above function, used
 *                            during extraction.
 *  Compress()              : The routine that actually performs the
 *                            compression.
 *  Decompress()            : The routine that actually performs the
 *                            decompression.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

class AL_CLASS_TYPE ALGreenleafEngine : public ALCompressionEngine {
/*
 * Declarations, friends, constructors, destructors
 */
    public :
        AL_PROTO ALGreenleafEngine( short int compression_level = AL_GREENLEAF_LEVEL_2,
                                    short int fail_uncompressible = 0 );
        virtual AL_PROTO ~ALGreenleafEngine();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 * The copy constructor and assignment operator do not exist.  I define
 * them here to prevent the compiler from creating default versions.
 */
    protected :
        AL_PROTO ALGreenleafEngine( ALGreenleafEngine AL_DLL_FAR & );
        ALGreenleafEngine AL_DLL_FAR & AL_PROTO operator=( ALGreenleafEngine AL_DLL_FAR & rhs );
/*
 * Member functions
 */
    protected :
        virtual int AL_PROTO WriteEngineData( ALStorage AL_DLL_FAR * archive );
        virtual int AL_PROTO ReadEngineData( ALStorage AL_DLL_FAR * archive );
    
    public :
        virtual int AL_PROTO Compress( ALStorage AL_DLL_FAR &input,
                                       ALStorage AL_DLL_FAR &output );
        virtual int AL_PROTO Decompress( ALStorage AL_DLL_FAR &input,
                                         ALStorage AL_DLL_FAR &output,
                                         long compressed_length = -1 );
        short int AL_PROTO CompressionLevel() { return miCompressionLevel; }
/*
 * Data members
 */
    protected :
        short int miCompressionLevel;
        short int miFailUncompressible;
    public :
        AL_CLASS_TAG( _ALGreenleafEngineTag );
};

#endif /* #ifdef __cplusplus */

#endif /* #ifdef _GRENENGN_H */
