#ifndef _MEMSTORE_H
#define _MEMSTORE_H

#include "arclib.h"

#if defined(__cplusplus)

/*
 * class ALMemory : public ALStorage
 *
 * DESCRIPTION
 *
 *  Class ALMemory is an ALStorage class that stores its data in memory
 *  buffers.
 *
 *  You can use ALMemory to work with a buffer of your own, or you
 *  can ask the class to allocate the memory for you.  You can
 *  also change the ownership of the buffer in midstream, allowing
 *  you to take control of a buffer that the class has generated.
 *
 * DATA MEMBERS
 *
 *  mfUserOwnsBuffer    : If this flag is set, it indicates that the user
 *                        owns the buffer, not the class.  This means
 *                        the class can't grow the buffer if it runs out
 *                        of space, and it can't delete it in the
 *                        ALMemory destructor.
 *
 *  mhUserMemoryHandle  : Under Windows, this member contains the handle
 *                        of the Windows memory block that has been
 *                        allocated
 *
 *  muUserBufferSize    : The actual size of the buffer, whether it is
 *                        owned by the user or not.  This is a size_t
 *                        member under real mode DOS, and a long under
 *                        Windows.
 *
 *  mpcUserBuffer       : A pointer to the buffer the class is presently
 *                        using.  The name User Buffer was probably a bad
 *                        choice, because this is the pointer we use
 *                        regardless of whether or not the user owns the
 *                        buffer.
 *
 * MEMBER FUNCTIONS
 *
 *  ALMemory()        : The constructor, slightly different between DOS
 *                      and Windows.
 *  ~ALMemory()       : The virtual destructor.
 *  operator new()    : Memory allocation operator, only used when the
 *                      library is in a DLL.  Note that this isn't the
 *                      operator used to allocate the buffer, just the
 *                      one to allocate a class object.
 *  Open()            : Open the storage object for reading and writing.
 *  Create()          : Create a new buffer to write to.
 *  Close()           : Close the existing memory object.
 *  LoadBuffer()      : Load a new block from the memory object into
 *                      the I/O buffer.
 *  FlushBuffer()     : Flush the contents of the I/O buffer, sending
 *                      the contents into the memory object.
 *  Seek()            : Seek to a new location in the memory object.
 *  Rename()          : Give the object a new name.  Names are pretty
 *                      irrelevant for memory objects, feel free to use
 *                      whatever you want here.
 *  UnRename()        : Restore the old name.
 *  Delete()          : Delete the memory object.  It is gone forever.
 *  RenameToBackup()  : Give the memory object an arbitrary new name.
 *  GrowUserBuffer()  : A private function used to give us more space
 *                      when the memory object is owner of the buffer.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

class ALMemory : public ALStorage {
  /*
   * Constructors, destructors, assignment operator, friends, declarations
   */

public:
  ALMemory(uint8_t *user_buffer = 0, int user_buffer_size = 0);
  virtual ~ALMemory();
  /*
   * As usual, I don't want the compiler to generate a default copy constructor,
   * or an assignment operator here.  I force it to back off by declaring them
   * here.  They do not exist!
   */
protected:
  ALMemory(ALMemory &);
  ALMemory &operator=(const ALMemory &);

  /*
   * Member functions, grouped by category.
   *
   *
   * Protected member manipulation, used inside library, not for public use.
   */
protected:
  /*
   * The file I/O access public interface
   */
public:
  virtual int Open();
  virtual int Create();
  virtual int Close();
  virtual int LoadBuffer(long address);
  virtual int FlushBuffer();
  virtual int Seek(long address);

  /*
   * File name and underlying object manipulation public interface
   */
public:
  virtual int Delete();

  /*
   * Unique to this class
   */
protected:
  int GrowUserBuffer(long minimum_new_size);
  /*
   * Data members
   */
protected:
public: /* Should some of these might be better off private */
  int mfUserOwnsBuffer;
  size_t muUserBufferSize;
  uint8_t *mpcUserBuffer;
};

#endif
#endif
