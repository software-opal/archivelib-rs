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

#include "copyengn.h"
#include "_openf.h"

//
// void * ALCopyEngine::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  :  The amount of storage that needs to be allocated for
//           this object.
//
// RETURNS
//
//  A pointer to the storage.
//
// DESCRIPTION
//
//  When using the DLL version of ArchiveLib, it is a good idea to
//  allocate the storage for objects from inside the DLL, since they
//  will be freed inside the DLL.  If we don't have the new operator
//  for a class, its storage will be allocated from the EXE before
//  the constructor code is called.  Then, when it is time to free
//  the storage, the delete operator will be called inside the DLL.
//  Not good, right?
//
//  By providing our own version of operator new inside this class, we
//  ensure that all memory allocation for the class will be done from
//  inside the DLL, not the EXE calling the DLL.
//
// REVISION HISTORY
//
//   May 21, 1994  1.0A  : First release
//

#if defined( AL_BUILDING_DLL )
void AL_DLL_FAR * AL_PROTO ALCopyEngine::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif

//
// ALCopyEngine::ALCopyEngine()
//
// ARGUMENTS:
//
//  None, constructor.
//
// RETURNS
//
//  Nothing, this is a constructor.
//
// DESCRIPTION
//
//  The copy engine doesn't have to store any data, because it just
//  performs a straight binary copy, without any frills.  Because of 
//  this simplicity, it doesn't have any data members to initialize.  
//  The only thing it does initialize is the base class, with the 
//  appropriate enum value and string identifier.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

AL_PROTO ALCopyEngine::ALCopyEngine()
    : ALCompressionEngine( AL_COMPRESSION_COPY, "Binary copy" )
{
}

//
// ALCopyEngine::~ALCopyEngine()
//
// ARGUMENTS:
//
//  None, destructor.
//
// RETURNS
//
//  Nothing, destructor.
//
// DESCRIPTION
//
//  The destructor has absolutely nothing to do.  In the debug
//  versions of the library, the dtor checks to be sure that it
//  is operating on the right type of object.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

AL_PROTO ALCopyEngine::~ALCopyEngine()
{
    AL_ASSERT( GoodTag(), "~ALCopyEngine: Attempt to delete invalid object" );
}

//
// int ALCopyEngine::Compress( ALStorage &input, ALStorage &output )
//
// ARGUMENTS:
//
//  input  : A reference to the input storage object.
//
//  output : A reference to the output storage object.
//
// RETURNS
//
//  AL_SUCCESS, or < AL_SUCCESS if something bad happens.
//
// DESCRIPTION
//
//  This is ostensibly a compression engine, but really all it does 
//  is copy input directly to the output.  The most exciting thing it
//  does during the entire process is initialize CRC checking.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

int AL_PROTO ALCopyEngine::Compress( ALStorage AL_DLL_FAR &input,
                                     ALStorage AL_DLL_FAR &output )
{
//
// Open the input and output files, and initialize CRC 32 checking.
//
    ALOpenFiles files( input, output );
    input.InitCrc32();
//
// Now read all the data from the input file, and write it to the
// output file.
//
    int c;
    for ( ; ; ) {
        c = input.ReadChar();
        if ( c < 0 )
            break;
        output.WriteChar( c );
    }
//
// Finally, check on the error status codes, then return.
//
    if ( input.mStatus < AL_SUCCESS )
        return mStatus = input.mStatus;
    if ( output.mStatus < AL_SUCCESS )
        return mStatus = output.mStatus;
    return mStatus;
}

//
// int ALCopyEngine::Decompress( ALStorage &input,
//                               ALStorage &output,
//                               long length )
//
// ARGUMENTS:
//
//  input     : A reference to the storage object containing the 
//              compressed data.
//
//  output    : A reference to the storage object that is going to receive
//              the uncompressed data.
//
//  length    : The number of byte in the uncompressed image.
//
// RETURNS
//
//  AL_SUCCESS if things went properly, error code < AL_SUCCESS if
//  a problem occurred.
//
// DESCRIPTION
//
//  This is a decompression routine, but really it just performs a 
//  straight binary copy of input to output.  This is the copy engine you
//  use when you just want to copy/archive files, and aren't worried
//  about saving disk space.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

int AL_PROTO ALCopyEngine::Decompress( ALStorage AL_DLL_FAR & input,
                                       ALStorage AL_DLL_FAR & output,
                                       long length )
{
    ALOpenFiles files( input, output );
    output.InitCrc32();

    int c;
//    if ( length == -1 )
//        return SetError( NEED_LENGTH,
//                           "The copy engine requires a length parameter when "
//                           "decompressing" );
    for ( ; length != 0 ; length-- ) {
        c = input.ReadChar();
        if ( c < 0 )
            break;
        output.WriteChar( c );
    }
    if ( input.mStatus < AL_SUCCESS )
        return mStatus = input.mStatus;
    if ( output.mStatus < AL_SUCCESS )
        return mStatus = output.mStatus;
    return mStatus;
}


