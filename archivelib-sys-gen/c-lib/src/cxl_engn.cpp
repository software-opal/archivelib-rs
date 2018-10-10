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
// extern "C" hALEngine newALCopyEngine( void )
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  A handle for (pointer to) a newly create ALCopyEngine object.
//  If for some reason the constructor failed this function might
//  just return a 0.
//
// DESCRIPTION
//
//  This is the C/VB translation layer function for the C++ constructor
//  ALCopyEngine::ALCopyEngine(). It simply calls the constructor
//  and returns the resulting value.  For more information on what
//  actually happens inside the constructor, see COPYENGN.CPP.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" hALEngine AL_FUNCTION newALCopyEngine( void )
{
    return (hALEngine) new ALCopyEngine();
}

//
// extern "C" hALEngine newALGreenleafEngine( void )
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  A handle for (pointer to) a newly create ALGreenleafEngine object.
//  If for some reason the constructor failed this function might
//  just return a 0.
//
// DESCRIPTION
//
//  This is the C/VB translation layer function for the C++ constructor
//  ALGreenleafEngine::ALGreenleafEngine(). It simply calls the constructor
//  and returns the resulting value.  For more information on what
//  actually happens inside the constructor, see GRENENGN.CPP.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" hALEngine AL_FUNCTION newALGreenleafEngine( int level )
{
    return (hALEngine) new ALGreenleafEngine( (short int) level );
}


//
// extern "C" short int ALGreenleafEngineGetLevel( hALEngine this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALGreenleafEngine object.
//
// RETURNS
//
//  A short integer that contains the current compression level for the
//  Greenleaf Engine.  If a compression pass fails because a file is
//  incompressible, this guy will return AL_GREENLEAF_COPY, otherwise
//  it should normally be one of AL_GREENLEAF_LEVEL_*.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ function
//  ALGreenleafEngine::CompressionLevel() as implemented for objects of
//  type ALGreenleafEngine.  For details on how the member
//  function actually works, take a look at GRENENGN.H.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   August 11, 1994  1.0B  : First release
//

extern "C" short int AL_FUNCTION
ALGreenleafEngineGetLevel( hALEngine this_object )
{
    AL_ASSERT_OBJECT( this_object, ALGreenleafEngine, "ALGreenleafEngineGetLevel" );
    return ( (ALGreenleafEngine *) this_object )->CompressionLevel();
}

//
// extern "C" int ALEngineGetStatusCode( hALEngine this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALCompressionEngine object.
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
//  ALCompressionEngine.  For details on how the member
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

extern "C" int AL_FUNCTION ALEngineGetStatusCode( hALEngine this_object )
{
    AL_ASSERT_OBJECT( this_object, ALCompressionEngine, "ALEngineGetStatusCode" );
    return ( (ALCompressionEngine *) this_object )->mStatus.GetStatusCode();
}

//
// extern "C" int ALEngineSetError( hALEngine this_object,
//                                  int error,
//                                  char *text )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALCompressionEngine object.
//                  We are going to set the engine's status member
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
//  ALName::SetError(), as applied to an ALCompressionEngine object.
//  For more details on how the function actually works, check out
//  OBJNAME.CPP.
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//
extern "C" int AL_FUNCTION ALEngineSetError( hALEngine this_object,
                                             int error,
                                             char AL_DLL_FAR *text )
{
    AL_ASSERT_OBJECT( this_object, ALCompressionEngine, "ALEngineSetError" );
    ( (ALCompressionEngine *) this_object )->mStatus.SetError( error, text );
    return error;
}

//
// extern "C" char *ALEngineGetStatusString( hALEngine this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALCompressionEngine object.
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
//  ALName::GetStatusString(), as implemented for class ALCompressionEngine.
//  Note that we need a completely different function return strings
//  to VB programmers.
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
ALEngineGetStatusString( hALEngine this_object )
{
    AL_ASSERT_OBJECT( this_object, ALCompressionEngine, "ALEngineGetStatusString" );
    const char *status = ( (ALCompressionEngine *) this_object )->mStatus.GetStatusString();
    if ( status == 0 )
        status = "";
    return (char AL_DLL_FAR *) status;
}

//
// extern "C" long ALEngineGetStatusStringVB( hALEngine this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALCompressionEngine object.
//                  We want to get the string translation of the error
//                  code for this object.
//
// RETURNS
//
//  Always returns a VB handle (pointer?) to a short string translation of
//  the current error code for the ALCompressionEngine object.
//
// DESCRIPTION
//
//  This is the VB wrapper function for the C++ function
//  ALName::GetStatusString(), as implemented for class ALCompressionEngine.
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
extern "C" long AL_FUNCTION ALEngineGetStatusStringVB( hALEngine this_object )
{
    AL_ASSERT_OBJECT( this_object , ALCompressionEngine, "ALEngineGetStatusStringVB" );
    const char _far *status = ( (ALCompressionEngine *) this_object )->mStatus.GetStatusString();
    if ( status == 0 )
        status = "";
    return ALCreateVBString( status, (unsigned short int) _fstrlen( status ) );
}
#endif

//
// extern "C" char * ALEngineGetStatusDetail( hALEngine this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALCompressionEngine object.
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
//  ALName::GetStatusDetail(), as implemented for class ALCompressionEngine.
//  Note that we need a completely different function to return strings
//  to VB programmers.
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
ALEngineGetStatusDetail( hALEngine this_object )
{
    AL_ASSERT_OBJECT( this_object, ALCompressionEngine, "ALEngineGetStatusDetail" );
    const char *status = ( (ALCompressionEngine *) this_object )->mStatus.GetStatusDetail();
    if ( status == 0 )
        status = "";
    return (char AL_DLL_FAR *) status;
}

//
// extern "C" long ALEngineGetStatusDetailVB( hALEngine this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALCompressionEngine object.
//                  We want to get the detailed status message for
//                  this object.
//
// RETURNS
//
//  Always returns a VB handle (pointer?) to a translation of
//  the current status detail for the ALCompressionEngine object.
//
// DESCRIPTION
//
//  This is the VB wrapper function for the C++ function
//  ALName::GetStatusDetail(), as implemented for class ALCompressionEngine.
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
extern "C" long AL_FUNCTION ALEngineGetStatusDetailVB( hALEngine this_object )
{
    AL_ASSERT_OBJECT( this_object, ALCompressionEngine, "ALEngineGetStatusDetailVB" );
    const char _far *status = ( (ALCompressionEngine *) this_object )->mStatus.GetStatusDetail();
    if ( status == 0 )
        status = "";
    return ALCreateVBString( status, (unsigned short int) _fstrlen( status ) );
}
#endif

//
// extern "C" void deleteALEngine( hALEngine this_object )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an ALCompressionEngine object.
//                  We destroy it in this function.
//
// RETURNS
//
//  No returns from destructors.
//
// DESCRIPTION
//
//  This is the C/VB wrapper function for the C++ destructor
//  ALCompressionEngine::~ALCompressionEngine()).  For details
//  on what the member function actually works, take a look at
//  CMPENGN.CPP.
//
//  Note that this destructor function is virtual, and should be called
//  to destroy any derived classes (ALCopyEngine and ALGreenleafEngine).
//
//  All that happens here is that the arguments are checked for correct
//  type (when in debug mode), and a call is made to the appropriate
//  member function, with lots of casting.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" void AL_FUNCTION deleteALEngine( hALEngine this_object )
{
    AL_ASSERT_OBJECT( this_object, ALCompressionEngine, "deleteALEngine" );
    delete (ALCompressionEngine *) this_object;
}

//
// extern "C" int ALEngineCompress( hALEngine this_object,
//                                  hALStorage input,
//                                  hALStorage output )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an object of type
//                  ALCompressionEngine.  The engine contains
//                  the code that will perform the compression.
//
//  input         : A handle for (pointer to) an object of type
//                  ALStorage that contains the input data that will
//                  be fed to the compression engine.
//
//  output        : A handle for (pointer to) an object of type
//                  ALStorage that will receive the compressed output
//                  from the engine.
//
// RETURNS
//
//  AL_SUCCESS if everything worked, code < AL_SUCCESS if not.
//
// DESCRIPTION
//
//  This function is the C/VB translation function for the C++ member
//  function ALCompressionEngine::Compress().  For details on what
//  this function does, you will need to look at the source code
//  in the appropriate module, such as COPYENGN.CPP or GRENENGN.CPP.
//
//  All the wrapper function does is check the type of the arguments
//  (if in debug mode), then call the C++ member function, returning
//  the result to the calling routine.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALEngineCompress( hALEngine this_object,
                                             hALStorage input,
                                             hALStorage output )
{
    AL_ASSERT_OBJECT( this_object, ALCompressionEngine, "ALEngineCompress" );
    AL_ASSERT_OBJECT( input, ALStorage, "ALEngineCompress" );
    AL_ASSERT_OBJECT( output, ALStorage, "ALEngineCompress" );
    return ( (ALCompressionEngine *) this_object )->Compress( *(ALStorage *) input, *(ALStorage *) output );
}

//
// extern "C" int ALEngineDecompress( hALEngine this_object,
//                                    hALStorage input,
//                                    hALStorage output )
//
// ARGUMENTS:
//
//  this_object   : A handle for (pointer to) an object of type
//                  ALCompressionEngine.  The engine contains
//                  the code that will perform the expansion.
//
//  input         : A handle for (pointer to) an object of type
//                  ALStorage that contains a stream of compressed
//                  data that was previously created using Compress().
//
//  output        : A handle for (pointer to) an object of type
//                  ALStorage that will receive the plain text output
//                  from the compression engine.
//
// RETURNS
//
//  AL_SUCCESS if everything worked, code < AL_SUCCESS if not.
//
// DESCRIPTION
//
//  This function is the C/VB translation function for the C++ member
//  function ALCompressionEngine::Decompress().  For details on what
//  this function does, you will need to look at the source code
//  in the appropriate module, such as COPYENGN.CPP or GRENENGN.CPP.
//
//  All the wrapper function does is check the type of the arguments
//  (if in debug mode), then call the C++ member function, returning
//  the result to the calling routine.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION
ALEngineDecompress( hALEngine this_object,
                    hALStorage input,
                    hALStorage output,
                    long compressed_length )
{
    AL_ASSERT_OBJECT( this_object, ALCompressionEngine, "ALEngineDecompress" );
    AL_ASSERT_OBJECT( input, ALStorage, "ALEngineDecompress" );
    AL_ASSERT_OBJECT( output, ALStorage, "ALEngineDecompress" );
    return ( (ALCompressionEngine *) this_object )->Decompress( *(ALStorage *) input, *(ALStorage *) output, compressed_length );
}

//
// extern "C" int ALEngineGetTypeCode( hALEngine this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an object of type
//                 ALCompressionEngine.
//
// RETURNS
//
//  The compression engine type, found in enum ALCompressionType in
//  ALDEFS.H.
//
// DESCRIPTION
//
//  This function is the C/VB translation function that provides access
//  to the C++ data member ALCompressionEngine::miCompressionType.  Since
//  C and VB can't access the class object directly, they have to go through
//  this function as an intermediary.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" int AL_FUNCTION ALEngineGetTypeCode( hALEngine this_object )
{
    AL_ASSERT_OBJECT( this_object, ALCompressionEngine, "ALEngineGetTypeCode" );
    return ( (ALCompressionEngine *) this_object )->miCompressionType;
}

//
// extern "C" char *ALEngineGetTypeString( hALEngine this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an object of type
//                 ALCompressionEngine.
//
// RETURNS
//
//  The string describing the engine type.  The description string is always
//  provided by the derived class.
//
// DESCRIPTION
//
//  This function is the C translation function that provides access
//  to the C++ data member ALCompressionEngine::mszCompressionType.  Since
//  C can't access the class object directly, it has to go through
//  this function as an intermediary.  Note that there is a special
//  function to return the compression type string to a VB program.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" char AL_DLL_FAR * AL_FUNCTION ALEngineGetTypeString( hALEngine this_object )
{
    AL_ASSERT_OBJECT( this_object, ALCompressionEngine, "ALEngineGetTypeString" );
    return (char AL_DLL_FAR *) ( (ALCompressionEngine *) this_object )->mszCompressionType;
}

//
// extern "C" char *ALEngineGetTypeStringVB( hALEngine this_object )
//
// ARGUMENTS:
//
//  this_object  : A handle for (pointer to) an object of type
//                 ALCompressionEngine.
//
// RETURNS
//
//  A VB string describing the engine type.  The description string is
//  always provided by the derived class.
//
// DESCRIPTION
//
//  This function is the VB translation function that provides access
//  to the C++ data member ALCompressionEngine::mszCompressionType.  Since
//  VB can't access the class object directly, it has to go through
//  this function as an intermediary.  Note that this is a special
//  function that knows how to convert a standard C string to one that
//  VB likes.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Combined a bunch of #ifdefs into a single test
//                          against AL_VB

#if defined( AL_VB )
extern "C" long AL_FUNCTION ALEngineGetTypeStringVB( hALEngine this_object )
{
    AL_ASSERT_OBJECT( this_object, ALCompressionEngine, "ALEngineGetTypeStringVB" );
    const char _far *status = ( (ALCompressionEngine *) this_object )->mszCompressionType;
    if ( status == 0 )
        status = "";
    return ALCreateVBString( status, (unsigned short int) _fstrlen( status ) );
}
#endif
