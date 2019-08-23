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

#include "al.h"
#include "alcxl.h"

//
// extern "C" hALCompressed newALCompressed( hALStorage storage,
//                                           hALEngine engine )
//
// ARGUMENTS:
//
//  storage  :  A handle for (pointer to) an ALStorage object that is
//              going to hold the compressed object.
//
//  engine   :  A handle for (pointer to) an ALCompressionEngine that will
//              be used to insert and extract objects from the the storage
//              object.
//
// RETURNS
//
//  A handle for (pointer to) a newly created ALCompressedObject, or 0 if
//  the constructor failed.
//
// DESCRIPTION
//
//  This is the C/VB translation function for the C++ constructor
//  ALCompressedObject::ALCompressedObject().  See CMPOBJ.CPP for
//  details on what happens in the member function.
//
//  In the translation function, we just check the arguments for
//  correct type (in the debug version) and call the function, returning
//  the result to the calling function.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" hALCompressed AL_FUNCTION
newALCompressed( hALStorage storage,
                 hALEngine engine )
{
    AL_ASSERT_OBJECT( storage, ALStorage, "newALCompressed" );
    AL_ASSERT_OBJECT( engine, ALCompressionEngine, "newALCompressed" );
    return (hALCompressed) new ALCompressedObject( *(ALStorage *) storage, *(ALCompressionEngine *) engine );
}

//
// extern "C" void deleteALCompressed( hALCompressed this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALCompressedObject
//                 that is going to be destroyed.
//
// RETURNS
//
//  Nothing, this is a destructor.
//
// DESCRIPTION
//
//  This is the C/VB translation function for the C++ destructor
//  ALCompressedObject::~ALCompressedObject().  See CMPOBJ.CPP for
//  details on what happens in the member function.
//
//  In the translation function, we just check the arguments for
//  correct type (in the debug version) and call the function.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION deleteALCompressed( hALCompressed this_object )
{
    AL_ASSERT_OBJECT( this_object, ALCompressedObject, "deleteALCompressed" );
    delete (ALCompressedObject *) this_object;
}

//
// extern "C" int ALCompressedInsert( hALCompressed this_object,
//                                    hALStorage input_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALCompressedObject
//                 that is going to have a storage object inserted
//                 int it.
//
//  input_object : A handle for (pointer to) a storage object that is
//                 going to get compressed into this_object.
//
// RETURNS
//
//  AL_SUCCESS for success, or < AL_SUCCESS for one of many failures
//
// DESCRIPTION
//
//  This is the C/VB translation function for the C++ function
//  ALCompressedObject::Insert().  See CMPOBJ.CPP for more details
//  on what happens in the member function.
//
//  In the translation function, we just check the arguments for
//  correct type (in the debug version) and call the function.  We
//  return the integer result of the function directly to the calling
//  function.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALCompressedInsert( hALCompressed this_object,
                                                hALStorage input_object )
{
    AL_ASSERT_OBJECT( this_object, ALCompressedObject, "ALCompressedInsert" );
    AL_ASSERT_OBJECT( input_object, ALStorage, "ALCompressedInsert" );
    return ( (ALCompressedObject *) this_object )->Insert( * (ALStorage *) input_object );
}

//
// extern "C" int ALCompressedExtract( hALCompressed this_object,
//                                     hALStorage output_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an ALCompressedObject
//                 that is going to have a storage object extracted
//                 from it.
//
//  output_object: A handle for (pointer to) a storage object that is
//                 going to receive the uncompressed object.
//
// RETURNS
//
//  AL_SUCCESS for success, or < AL_SUCCESS for one of many failures
//
// DESCRIPTION
//
//  This is the C/VB translation function for the C++ function
//  ALCompressedObject::Extract().  See CMPOBJ.CPP for more details
//  on what happens in the member function.
//
//  In the translation function, we just check the arguments for
//  correct type (in the debug version) and call the function.  We
//  return the integer result of the function directly to the calling
//  function.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALCompressedExtract( hALCompressed this_object,
                                                 hALStorage output_object )
{
    AL_ASSERT_OBJECT( this_object, ALCompressedObject, "ALCompressedExtract" );
    AL_ASSERT_OBJECT( output_object, ALStorage, "ALCompressedExtract" );
    return ( (ALCompressedObject *) this_object )->Extract( * (ALStorage *) output_object );
}

//
// extern "C" int ALCompressedGetStatusCode( hALCompressed this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALCompressedObject.
//
// RETURNS
//
//  An integer that contains the current status code for the object.
//  Note that values of < 0 always indicate an error conditions.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ function
//  ALName::GetStatusCode() as implemented for objects of type
//  ALCompressedObject.  For details on how the member
//  function actually works, take a look at OBJNAME.CPP.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION
ALCompressedGetStatusCode( hALCompressed this_object )
{
    AL_ASSERT_OBJECT( this_object, ALCompressedObject, "ALCompressedGetStatusCode" );
    return ( (ALCompressedObject *) this_object )->mStatus.GetStatusCode();
}

//
// extern "C" int ALCompressedSetError( hALCompressed this_object,
//                                      int error,
//                                      char *text )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALCompressedObject.
//                  We are going to set the object's status member
//                  so that it is in an error state.
//
//  error         : The error code to apply to the object.  Values from
//                  ALDEFS.H are good, but it really doesn't matter as
//                  long as you use a negative number.
//
//  text          : The text of the error message you want to associate with
//                  this error.
//
// RETURNS
//
//  Returns the error code that you passed it.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ member function
//  ALName::SetError(), as applied to an ALCompressedObject.  For more
//  details on how the function actually works, check out OBJNAME.CPP.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALCompressedSetError( hALCompressed this_object,
                                                  int error_code,
                                                  char AL_DLL_FAR *text )
{
    AL_ASSERT_OBJECT( this_object, ALCompressedObject, "ALCompressedSetError" );
    ( (ALCompressedObject *) this_object )->mStatus.SetError( error_code, text );
    return error_code;
}

//
// extern "C" char *ALCompressedGetStatusString( hALCompressed this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALCompressedObject.
//                  We want to get the string translation of the error
//                  code for this object.
//
// RETURNS
//
//  Always returns a pointer to a short string translation of the
//  current error code.
//
// DESCRIPTION
//
//  This is the C wrapper function for the C++ function
//  ALName::GetStatusString(), as implemented for class ALCompressedObject.
//  Note that we need a completely different function in order to
//  return strings to VB programmers, this function is only good for C!
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" char AL_DLL_FAR * AL_FUNCTION
ALCompressedGetStatusString( hALCompressed this_object )
{
    AL_ASSERT_OBJECT( this_object, ALCompressedObject, "ALCompressedGetStatusString" );
    const char *status = ( (ALCompressedObject *) this_object )->mStatus.GetStatusString();
    if ( status == 0 )
        status = "";
    return (char AL_DLL_FAR *) status;
}

//
// extern "C" long ALCompressedGetStatusStringVB( hALCompressed this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALCompressedObject.
//                  We want to get the string translation of the error
//                  code for this object.
//
// RETURNS
//
//  Always returns a VB handle (pointer?) to a short string translation of
//  the current error code for the ALCompressedObject.
//
// DESCRIPTION
//
//  This is the VB wrapper function for the C++ function
//  ALName::GetStatusString(), as implemented for class ALCompressedObject.
//  Note that we need a completely different function to return strings
//  to C programmers.  In this case, we use a special VB translation routine
//  to convert a C string to one that is nice and easy for VB to use.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Combined a bunch of #ifdefs into a single test
//                          against AL_VB

#if defined( AL_VB )
extern "C" long AL_FUNCTION ALCompressedGetStatusStringVB( hALCompressed this_object )
{
    AL_ASSERT_OBJECT( this_object, ALCompressedObject, "ALCompressedGetStatusStringVB" );
    const char _far *status = ( (ALCompressedObject *) this_object )->mStatus.GetStatusString();
    if ( status == 0 )
        status = "";
    return ALCreateVBString( status, (unsigned short int) _fstrlen( status ) );
}
#endif

//
// extern "C" char *ALCompressedGetStatusDetail( hALCompressed this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALCompressedObject.
//                  We want to get the detailed string describing this
//                  object's current status.
//
// RETURNS
//
//  Always returns a pointer to a status detail message.
//
// DESCRIPTION
//
//  This is the C wrapper function for the C++ function
//  ALName::GetStatusDetail(), as implemented for class ALCompressedObject.
//  Note that we need a completely different function in order to
//  return strings to VB programmers.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" char AL_DLL_FAR * AL_FUNCTION
ALCompressedGetStatusDetail( hALCompressed this_object )
{
    AL_ASSERT_OBJECT( this_object, ALCompressedObject, "ALCompressedGetStatusDetail" );
    const char *status = ( (ALCompressedObject *) this_object )->mStatus.GetStatusDetail();
    if ( status == 0 )
        status = "";
    return (char AL_DLL_FAR *) status;
}

//
// extern "C" long ALCompressedGetStatusDetailVB( hALCompressed this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALCompressedobject.
//                  We want to get the detailed status message for
//                  this object.
//
// RETURNS
//
//  Always returns a VB handle (pointer?) to a translation of
//  the current status detail for the ALCompressedObject.
//
// DESCRIPTION
//
//  This is the VB wrapper function for the C++ function
//  ALName::GetStatusDetail(), as implemented for class ALCompressedObject.
//  Note that we need a completely different function to return strings
//  to C programmers.  In this case, we use a special VB translation routine
//  to convert a C string to one that is nice and easy for VB to use.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Combined a bunch of #ifdefs into a single test
//                          against AL_VB

#if defined( AL_VB )
extern "C" long AL_FUNCTION
ALCompressedGetStatusDetailVB( hALCompressed this_object )
{
    AL_ASSERT_OBJECT( this_object, ALCompressedObject, "ALCompressedGetStatusDetailVB" );
    const char _far *status = ( (ALCompressedObject *) this_object )->mStatus.GetStatusDetail();
    if ( status == 0 )
        status = "";
    return ALCreateVBString( status, (unsigned short int) _fstrlen( status ) );
}
#endif

