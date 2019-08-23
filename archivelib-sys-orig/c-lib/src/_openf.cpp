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
// void * ALOpenInputFile::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  : The size of the object being created.
//
// RETURNS
//
//  A pointer to the storage allocated for the object.
//
// DESCRIPTION
//
//  When we construct an object when using the DLL, trouble can arise.
//  The main bad thing is that when we call the constructor from an
//  EXE, we allocate the storage for the new object from inside our EXE.
//  When we destroy the same object, the destructor frees up the memory
//  inside the DLL.  This is bad, because you can mangle the heap inside
//  the DLL by trying to free an object that doesn't belong to it.
//
//  The fix to this conundrum is to allocate the object inside the DLL,
//  and we can make this happen by overloading the new operator.  We don't
//  bother unless it's a DLL deal.
//
//  This lecture will be repeated at times throughout the source in this
//  project.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

#if defined( AL_BUILDING_DLL )
void AL_DLL_FAR * AL_PROTO ALOpenInputFile::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif

//
// void * ALOpenOutputFile::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  : The size of the object being created.
//
// RETURNS
//
//  A pointer to the storage allocated for the object.
//
// DESCRIPTION
//
//  See the function directly above.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

#if defined( AL_BUILDING_DLL )
void AL_DLL_FAR * AL_PROTO ALOpenOutputFile::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif

//
// void * ALOpenFiles::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  : The size of the object being created.
//
// RETURNS
//
//  A pointer to the storage allocated for the object.
//
// DESCRIPTION
//
//  See the function directly above.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

#if defined( AL_BUILDING_DLL )
void AL_DLL_FAR * AL_PROTO ALOpenFiles::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif


//
// ALOpenInputFile::ALOpenInputFile( ALStorage &file )
//
// ARGUMENTS:
//
//  file  : The storage object that has to opened.
//
// RETURNS
//
//  Nothing, this is a constructor.
//
// DESCRIPTION
//
//  You can stick this constructor at the start of a function, and it
//  opens up an ALStorage object for you.  You can then take it for
//  granted that it is open.  You can also take it for granted that
//  the storage object will be closed by the destructor when the
//  function exits.  All of this saves a lot of repetitive code.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

AL_PROTO ALOpenInputFile::ALOpenInputFile( ALStorage AL_DLL_FAR &file )
{
    mpFile = &file;
    miFileWasOpen = file.IsOpen();
    if ( !miFileWasOpen )
        file.Open();
}

//
// ALOpenInputFile::~ALOpenInputFile()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  None, it is a destructor.
//
// DESCRIPTION
//
//  At the end of the function, it is time to close the storage object.
//  But only if it wasn't open when the constructor was called.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

AL_PROTO ALOpenInputFile::~ALOpenInputFile()
{
    if ( !miFileWasOpen )
        mpFile->Close();
}

//
// ALOpenOutputFile::ALOpenOutputFile( ALStorage &file )
//
// ARGUMENTS:
//
//  file :  The ALStorage object that needs to be created.
//
// RETURNS
//
//  Nothing, it is a constructor.
//
// DESCRIPTION
//
//  This is just like ALOpenInputFile, except instead of calling
//  ALStorage::Open(), it calls ALStorage::Create().  Note that if
//  the file is already open, we keep track of the fact and leave it
//  alone.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

AL_PROTO ALOpenOutputFile::ALOpenOutputFile( ALStorage AL_DLL_FAR &file )
{
    mpFile = &file;
    miFileWasOpen = file.IsOpen();
    if ( !miFileWasOpen )
        file.Create();
}

//
// ALOpenOutputFile::~ALOpenOutputFile()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  If the file was closed when the constructor was called, we close
//  it in the constructor.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

AL_PROTO ALOpenOutputFile::~ALOpenOutputFile()
{
    if ( !miFileWasOpen )
        mpFile->Close();
}

//
// ALOpenFiles::ALOpenFiles( ALStorage &input,
//                           ALStorage &output )
//
// ARGUMENTS:
//
//  input  :  The storage object that needs to be opened, maybe.
//
//  output :  The storage object that needs to be created, maybe.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This is just a combination of the ALOpenOutputFile() and
//  ALOpenInputFile() guys rolled into one.  To combine them, we
//  just create this object that contains one of both objects.
//
//  So this guy takes care of opening an input file and an output
//  file right there at the same time.  The most exciting part of it
//  is that they both get closed up in the destructor.
//
//  So all the constructor has to do here is call the other two
//  constructors in an initializer list.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

AL_PROTO ALOpenFiles::ALOpenFiles( ALStorage AL_DLL_FAR &input,
                                   ALStorage AL_DLL_FAR &output )
    : mInputFile( input ), mOutputFile( output )
{
}

//
// ALOpenFiles::~ALOpenFiles()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This guy closes the two files, if they were closed when the constructor
//  was called.  We don't have to do anything explicitly, because the
//  two data members of this object do so in their destructors.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

AL_PROTO ALOpenFiles::~ALOpenFiles()
{
}

