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

//
// void * ALCompressionEngine::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  :  The number of bytes needed to create a new ALCompressionEngine
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
//   May 23, 1994  1.0A  : First release
//

#if defined( AL_BUILDING_DLL )
void AL_DLL_FAR * AL_PROTO ALCompressionEngine::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif

//
// ALCompressionEngine::
// ALCompressionEngine( ALCompressionType compression_type_int,
//                      const char *compression_type_string )
//
// ARGUMENTS:
//
//  compression_type_int    : The enumerated constant for the compression
//                            type supported by this compression engine.
//
//  compression_type_string : The string describing the compression engine.
//
// RETURNS
//
//  None.
//
// DESCRIPTION
//
//  This constructor can only be called by derived classes, and all they 
//  do with it is call it to set up the compression type and integer
//  members.  This class has a couple of pure virtual functions, so
//  you can't instantiate a freestanding object.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

AL_PROTO 
ALCompressionEngine::ALCompressionEngine( ALCompressionType compression_type_int,
                                          const char *compression_type_string )
    : miCompressionType( compression_type_int ),
      mszCompressionType( compression_type_string )
{
}

//
// int ALCompressionEngine::WriteEngineData( ALStorage *archive )
//
// ARGUMENTS:
//
//  archive  :  The storage object where the engine specific data is
//              going to be written.
//
// RETURNS
//
//  AL_SUCCESS, or < AL_SUCCESS if something bad happens.
//
// DESCRIPTION
//
//  Compression engines can write private data out to the archive
//  directory to provide customization information. For example, the
//  Greenleaf compression engine writes its compression level 
//  using this function.  By default, there is no data, which is what
//  this function writes out, a 0 length string.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO 
ALCompressionEngine::WriteEngineData( ALStorage AL_DLL_FAR * archive )
{
    return archive->WritePortableShort( 0 );
}

//
// int ALCompressionEngine::ReadEngineData( ALStorage * archive )
//
// ARGUMENTS:
//
//  archive  :  The storage object where the engine specific data is
//              going to be read in from.
//
// RETURNS
//
//  AL_SUCCESS, or < AL_SUCCESS if something bad happened.
//
// DESCRIPTION
//
//  Compression engines can write private data out to the archive
//  directory to provide customization information. For example, the
//  Greenleaf compression engine writes its compression level 
//  using this function.  By default, no data is written out.  This
//  function expects to find a zero length string, and complains
//  with a fatal error if it doesn't.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO 
ALCompressionEngine::ReadEngineData( ALStorage AL_DLL_FAR * archive )
{
    short temp;
    int status = archive->ReadPortableShort( temp );
    AL_ASSERT( temp == 0, "ReadEngineData: engine data not 0 length" );
    return status;
}

//
// ALCompressionEngine::~ALCompressionEngine()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  None, destructor.
//
// DESCRIPTION
//
//  This guy doesn't have anything to do.  In the debug version of
//  the library, it at least checks to be sure the object type is correct.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

AL_PROTO ALCompressionEngine::~ALCompressionEngine()
{
    AL_ASSERT( GoodTag(), "~ALCompressionEngine: attempt to delete invalid object" );
}

