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

#include "_openf.h"

//
// void * ALCompressedObject::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  :  The number of bytes needed to create a new ALCompressedObject
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
void AL_DLL_FAR * AL_PROTO ALCompressedObject::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif

//
// ALCompressedObject::
// ALCompressedObject( ALStorage AL_DLL_FAR & storage_object,
//                     ALCompressionEngine AL_DLL_FAR & compression_engine )
//
// ARGUMENTS:
//
//  storage_object     : A reference to the storage object that is going
//                       to get the compressed data.
//
//  compression_engine : A reference to the compression engine that will
//                       be used to insert an object or extract an object.
//
// RETURNS
//
//  A constructor, you don't get a return.
//
// DESCRIPTION
//
//  A compressed object is a storage object that gets a single compressed
//  object packed into it.  You get to call Insert() or Extract(), to
//  put the object in or take it out.  Compressed objects don't get all
//  the fancy options that Archives do.  For example, you have to know in
//  advance what sort of compression engine and storage object you are
//  going to use to put things in and take things out.  You don't get to
//  store comments or time date stamps, or anything like that.
//
//  The one piece of flexibility you do get the ALCompressedObject is
//  the ability to derive a new class from this base, then use the
//  new class to write some custom data out to the object.
//
//  This constructor stores references to the object being used to hold the
//  compressed data, and the engine being used to pack and unpack it.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

AL_PROTO ALCompressedObject::
ALCompressedObject( ALStorage AL_DLL_FAR & storage_object,
                    ALCompressionEngine AL_DLL_FAR & compression_engine )
{
    mpCompressionEngine = &compression_engine;
    mpStorageObject = &storage_object;
}

//
// ALCompressedObject::~ALCompressedObject()
//
// ARGUMENTS:
//
//  None, destructor.
//
// RETURNS
//
//  None, destructor.
//
// DESCRIPTION
//
//  This destructor has nothing important to do.  The debug version
//  checks the object type for validity, but that's it.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

AL_PROTO ALCompressedObject::~ALCompressedObject()
{
    AL_ASSERT_OBJECT( this, ALCompressedObject, "~ALCompressedObject" );
}

//
// int ALCompressedObject::Insert( ALStorage &input_object )
//
// ARGUMENTS:
//
//  input_object  : A storage object that is going to be inserted into
//                  the compressed object.
//
// RETURNS
//
//  AL_SUCCESS if everything worked properly, or < AL_SUCCESS if an
//  error was encountered.
//
// DESCRIPTION
//
//  The compressed object has this format:
//
//    long uncompressed_size
//    long compressed_size
//    DWORD crc_32
//    Any data from derived classes
//    unsigned char data[]
//
//  Writing all this out is pretty straightforward, although you might
//  note that it is going to require at least one seek() back to the
//  start of the compressed object after the compression is done.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Modified the call to WriteHeaderData so that
//                          passes a pointer to the input storage object.
//                          This would allow me, for example, to figure
//                          out that that input file was DATA.TXT, and
//                          store that file name so it can be recovered
//                          upon extraction.

int AL_PROTO ALCompressedObject::Insert( ALStorage AL_DLL_FAR &input_object )
{
    AL_ASSERT_OBJECT( this, ALCompressedObject, "Insert" );
    AL_ASSERT_OBJECT( &input_object, ALStorage, "Insert" );
    if ( mStatus < AL_SUCCESS )
        return mStatus;
//
// Here is where we open the input and the output.
//
    ALOpenFiles files( input_object, *mpStorageObject );
//
// We first write out the uncompressed size, which we already know.  We
// then save the current position, and write placeholder longs out for
// what will become the compressed size and the CRC-32.
//
    mpStorageObject->WritePortableLong( input_object.GetSize() );
    long saved_pos = mpStorageObject->Tell();
    mpStorageObject->WritePortableLong( 0xfedcba98L ); //Temporary
    mpStorageObject->WritePortableLong( 0x01234567L );  //Temporary
//
// If a derived class has any header data to write out, this is where it
// will be performed.  The base class writes 0 bytes here.
//
    WriteHeaderData( &input_object );
    long start = mpStorageObject->Tell();
//
// Next, perform the compression.  Once that is done we can calculate
// the compressed size.  The CRC-32 will have been calculated on the fly
// as the compression was performed.
//
    mpCompressionEngine->Compress( input_object, *mpStorageObject );
    long compressed_size = mpStorageObject->Tell() - start;
    if ( mpCompressionEngine->mStatus < 0 )
        return mStatus = mpCompressionEngine->mStatus;
//
// Go back to the spot we remembered, and write out the compressed
// size and the CRC. At that point, the compressed object is complete.
//
    mpStorageObject->Seek( saved_pos );
    mpStorageObject->WritePortableLong( compressed_size );
    mpStorageObject->WritePortableLong( ~input_object.GetCrc32() );
    if ( mpStorageObject->mStatus < 0 )
        return mStatus = mpStorageObject->mStatus;
    return AL_SUCCESS;
}

//
// int ALCompressedObject::Extract( ALStorage &output_object )
//
// ARGUMENTS:
//
//  output_object  :  The storage object that is going to receive the
//                    extracted data from the compressed object.
//
// RETURNS
//
//  AL_SUCCESS, or < AL_SUCCESS if an error occurs.
//
// DESCRIPTION
//
//  Extracting the data to a new storage object is easy.  We read in
//  all the data so that we can do a little error checking along the
//  way, but that's all.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//
//   July 13, 1994 1.0B  : Rearranged things so that the output file didn't
//                         get opened until *after* ReadHeaderData was called.
//                         Also added a pointer to the output storage object
//                         in the call to ReadHeaderData.  That way, if I
//                         stored the original file name in the header data,
//                         I could use it to restore the original name in
//                         the output object.
//

int AL_PROTO ALCompressedObject::Extract( ALStorage AL_DLL_FAR &output_object )
{
    long compressed_length;
    long crc32;

    AL_ASSERT_OBJECT( this, ALCompressedObject, "Extract" );
    AL_ASSERT_OBJECT( &output_object, ALStorage, "Extract" );
    if ( mStatus < AL_SUCCESS )
        return mStatus;
//
// Open the input file.
//
    ALOpenInputFile input_file( *mpStorageObject );
//
// Now read in all the data stored at the start of the object,
// including any header data created by derived classes.  If we are
// using the base class, there won't be any additional data bytes there.
//
    mpStorageObject->ReadPortableLong( output_object.mlSize );
    mpStorageObject->ReadPortableLong( compressed_length );
    mpStorageObject->ReadPortableLong( crc32 );
    ReadHeaderData( &output_object );
    if ( mpStorageObject->mStatus < 0 )
        return mStatus = mpStorageObject->mStatus;
//
// Open the output file.
//
    ALOpenOutputFile output_file( output_object );
//
// Extract the data and store it in the storage object specified
// as an argument.
//
    if ( mpCompressionEngine->Decompress( *mpStorageObject,
                                          output_object,
                                          compressed_length ) < 0 )
        return mStatus = mpCompressionEngine->mStatus;
//
// A little error checking leads to an error return if things didn't
// go well, or AL_SUCCESS if things did.
//
    if ( mpStorageObject->mStatus < 0 )
        return mStatus = mpStorageObject->mStatus;
    if ( crc32 != ~output_object.GetCrc32() )
        return mStatus.SetError( AL_CRC_ERROR,
                                 "CRC32 differs between %s and %s",
                                 mpStorageObject->mName.GetName(),
                                 output_object.mName.GetName() );
    return AL_SUCCESS;
}

//
// int ALCompressedObject::WriteHeaderData( ALStorage * storage = 0 )
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  AL_SUCCESS, always.
//
// DESCRIPTION
//
//  Derived classes can override this function and use it to add
//  additional data bytes to the header of a compressed object.  Note
//  that this data does not have to be written out in any particular
//  format, we have no portability concerns here.  It is up to the
//  derived class to insure that the data is written in an internally
//  consistent format so that ReadHeaderData() can always position the
//  file pointer to the correct start of data.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//
//   July 13, 1994 1.0B  : Added the pointer to the storage object that
//                         is in the process of being compressed.  This
//                         helps if you want to store information about the
//                         file being compressed.
//
int AL_PROTO ALCompressedObject::
WriteHeaderData( ALStorage AL_DLL_FAR * /* = 0 */  )
{
    return AL_SUCCESS;
}

//
// int ALCompressedObject::ReadHeaderData( ALStorage AL_DLL_FAR * = 0 )
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  AL_SUCCESS, always.
//
// DESCRIPTION
//
//  Derived classes can override this function and use it to read
//  additional data bytes from the header of a compressed object.  Note
//  that this data does not have to be written out in any particular
//  format, we have no portability concerns here.  It is up to the
//  derived class to insure that the data is written in an internally
//  consistent format so that ReadHeaderData() can always position the
//  file pointer to the correct start of data.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//
//   July 13, 1994 1.0B  : Added the pointer to the storage object that
//                         is in the process of being decompressed.  This
//                         helps if you want to read information about the
//                         file before starting the expansion.
//

int AL_PROTO ALCompressedObject::
ReadHeaderData( ALStorage AL_DLL_FAR * /* = 0 */ )
{
    return AL_SUCCESS;
}
