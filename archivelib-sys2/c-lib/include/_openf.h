#ifndef __OPENF_H
#define __OPENF_H

#include "arclib.h"

#if defined(__cplusplus)

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

class ALOpenInputFile {
public:
  ALOpenInputFile(ALStorage &file);
  ~ALOpenInputFile();
  /*
   * Prevent the compiler from generating these members.
   */
protected:
  ALOpenInputFile(ALOpenInputFile &);
  ALOpenInputFile &operator=(ALOpenInputFile &);

protected:
  int miFileWasOpen;
  ALStorage *mpFile;
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

class ALOpenOutputFile {
public:
  ALOpenOutputFile(ALStorage &file);
  ~ALOpenOutputFile();
  /*
   * Prevent the compiler from generating these members.
   */
protected:
  ALOpenOutputFile(ALOpenOutputFile &);
  ALOpenOutputFile &operator=(ALOpenOutputFile &);

protected:
  int miFileWasOpen;
  ALStorage *mpFile;
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

class ALOpenFiles {
public:
  ALOpenFiles(ALStorage &input, ALStorage &output);
  ~ALOpenFiles();
  /*
   * Prevent the compiler from generating these members.
   */
protected:
  ALOpenFiles(ALOpenFiles &);
  ALOpenFiles &operator=(ALOpenFiles &);

protected:
  ALOpenInputFile mInputFile;
  ALOpenOutputFile mOutputFile;
};

#endif
#endif
