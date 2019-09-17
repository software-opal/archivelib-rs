#include "_openf.h"
#include "arclib.h"

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

ALOpenInputFile::ALOpenInputFile(ALStorage &file) {
  mpFile = &file;
  miFileWasOpen = file.IsOpen();
  if (!miFileWasOpen) {
    file.Open();
  }
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

ALOpenInputFile::~ALOpenInputFile() {
  if (!miFileWasOpen) {
    mpFile->Close();
  }
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

ALOpenOutputFile::ALOpenOutputFile(ALStorage &file) {
  mpFile = &file;
  miFileWasOpen = file.IsOpen();
  if (!miFileWasOpen) {
    file.Create();
  }
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

ALOpenOutputFile::~ALOpenOutputFile() {
  if (!miFileWasOpen) {
    mpFile->Close();
  }
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

ALOpenFiles::ALOpenFiles(ALStorage &input, ALStorage &output)
    : mInputFile(input), mOutputFile(output) {}

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

ALOpenFiles::~ALOpenFiles() {}
