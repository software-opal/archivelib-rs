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


#ifndef _CMPENGN_H
#define _CMPENGN_H

#if defined( __cplusplus )


/*
 * class ALOpenInputFile
 *
 * DESCRIPTION
 *
 * This header file defines the ALCompressionEngine class.  It is a base
 * class with pure virtual functions, so it cannot be instantiated.
 * The two derived classes supplied with Archive Library at this time are
 * ALGreenleafEngine, which implements a Greenleaf proprietary compression
 * engine, and ALCopyEngine, which just performs straight copying.
 *
 * ALCompressionEngine has two public virtual functions, Compress()
 * and Decompress(), which are pure here.  It also provides two
 * virtual protected functions which are used to store configuration
 * in the archive directory.
 *
 * DATA MEMBERS
 *
 *  miCompressionType   :  The compression type integer is what gets stored
 *                         in the archive directory, and what the archive
 *                         class looks at when extracting so it can figure
 *                         out what type of compression engine to construct
 *                         for a specific type of object.
 *
 *  mszCompressionType  :  This string just describes the compression type
 *                         in ASCII format suitable for printing or display.
 *
 *  mStatus             :  A standard ArchiveLib status object, stored
 *                         with the compression engine.  Check this after
 *                         completing a compression or decompression to
 *                         see how things went.
 *
 * MEMBER FUNCTIONS
 *
 *  ALCompressionEngine()  : The constructor.  Only called by the ctors for
 *                           derived classes.
 *  ~ALCompressionEngine() : The virtual destructor.
 *  operator new()         : Memory allocation operator, only gets used
 *                           when the library is in a DLL.
 *  WriteEngineData()      : Private virtual function used to store engine
 *                           specific data.
 *  ReadEngineData()       : Private virtual function used to read back
 *                           engine specific data.
 *  Compress()             : The compression routine.  Derived classes
 *                           have to provide their own versions of this
 *                           function.  Wouldn't be much good without it.
 *  Decompress()           : The inverse, has to know how to undo the
 *                           Compress() output.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

class AL_CLASS_TYPE ALCompressionEngine {
/*
 * Constructors, destructors, declarations, and friends
 */
    friend class AL_CLASS_TYPE ALArchiveBase;

    public :
        AL_PROTO ALCompressionEngine( ALCompressionType compression_type_int,
                                      const char AL_DLL_FAR *compression_type_string );
        virtual AL_PROTO ~ALCompressionEngine();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 * The copy constructor and assignment operator do not exist.  I define
 * them here to prevent the compiler from creating default versions.
 */
    protected :
        AL_PROTO ALCompressionEngine( ALCompressionEngine AL_DLL_FAR & );
        ALCompressionEngine AL_DLL_FAR & AL_PROTO operator=( ALCompressionEngine AL_DLL_FAR & rhs );
/*
 * Member functions
 */
    protected :
        virtual int AL_PROTO WriteEngineData( ALStorage AL_DLL_FAR * archive );
        virtual int AL_PROTO ReadEngineData( ALStorage AL_DLL_FAR * archive );

    public :
        virtual int AL_PROTO Compress( ALStorage AL_DLL_FAR &input_object,
                                         ALStorage AL_DLL_FAR &output_object ) = 0;
        virtual int AL_PROTO Decompress( ALStorage AL_DLL_FAR &input_object,
                                           ALStorage AL_DLL_FAR &output_object,
                                           long compressed_length = -1 ) = 0;
/*
 * Data members
 */
    public :
        const ALCompressionType miCompressionType;
        const char AL_DLL_FAR *mszCompressionType;
        ALStatus mStatus;
        AL_CLASS_TAG( _ALCompressionEngineTag );
};

#endif /* #if defined( __cplusplus ) */

#endif /* #ifndef _CMPENGN_H         */
