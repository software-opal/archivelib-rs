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


#ifndef _STOR_H
#define _STOR_H

#if defined( __cplusplus )

#include <stddef.h>   /* need for size_t */

#include "timedate.h"
#include "fileattr.h"

/*
 * class ALStorage
 *
 * DESCRIPTION
 *
 * ALStorage is a base class that defines the different types
 * of storage objects used by Archive Library.  The two most
 * commonly used storage object types are file objects and memory
 * objects, defined by the derived classes ALFile and
 * ALMemory.
 *
 * ALStorage objects are used to store and retrieve objects from archives.
 * They are also used to store and retrieve the archives themselves,
 * allowing archives to be stored in files or directly in memory.
 *
 * The ALStorage adds buffering to the storage object, allowing for
 * fast access to data presently cached in memory.  This is very similar to
 * the buffering provided for FILE types in stdio.h.  Note that this
 * buffering is generally only efficient/useful if lots of sequential
 * reads or writes are being done, as opposed to random accessess.
 *
 * ALStorage objects give up a lot of flexibility in order to provide
 * quick and efficient access to data.  The primary way this affects
 * use of the class is that the I/O buffer can only be used for reading
 * or writing, but not both simultaneously.  The class doesn't check
 * for this at run time, so programmers need to enforce it themselves.
 *
 * When a read is initiated for the first time, the buffer is loaded up,
 * and subsequent reads are performed out of the I/O buffer.  To switch
 * to writing mode, a call to FlushBuffer needs to be performed, which
 * will reset the input and output indices.  Likewise, when, done writing,
 * a call to FlushBuffer() can be performed to clear the indices.  A
 * read can be done subsequently.
 *
 * DATA MEMBERS
 *
 *  mpcBuffer            : This is the I/O buffer.  I read big blocks of
 *                         data into this buffer, then I can perform
 *                         character reads from an inline functin that
 *                         doesn't have to access any virtual fns.  Speeds
 *                         things up tremendously.  Likewise, I write
 *                         to this buffer using inline functions until it
 *                         it is full.  Only then do I call a virtual
 *                         to flush it to disk, memory, or whatever.
 *
 *  muBufferValidData    : This keeps track of the end of valid data,
 *                         both when reading and writing.  When re
 *                         read in a block of data, this index is set
 *                         to the end of the data.  When writing, this
 *                         index is continually updated to reflect the
 *                         end of the user written data.
 *                         
 *  muWriteIndex         : The index in the I/O buffer where the next byte
 *                         is going to be written.
 *
 *  muReadIndex          : The index in the I/O buffer where the next read
 *                         will come from.
 *
 *  mlFilePointer        : The current location of the read/write pointer
 *                         in the underlying object, e.g. a file.  This
 *                         is the location where the data will be written
 *                         out of the I/O buffer when a FlushBuffer() call
 *                         is made.  Or, if reading, it is where the next
 *                         LoadBuffer() will read data from.
 *
 *  mlSize               : The size of the file/object.  This will ordinarily
 *                         be set to -1 when we create an object, because
 *                         we don't know the size yet.  When you call Open()
 *                         for an existing object, the value will usually
 *                         be loaded using some sort of system call.  We
 *                         also can figure out what the size is when we do
 *                         a ReadDirectory call on an archive.
 *
 *  mlCrc32              : The CRC-32 for the object.  This value normally
 *                         won't be known until an object has been placed
 *                         in an archive, or when the information has
 *                         been read out using in ReadDirectory().
 *
 *  miUpdateCrcFlag      : This flag is set to indicate that we are in the
 *                         process of calculating the CRC while the file
 *                         is being compressed.
 *
 *  miCreated            : This flag will be set if the file was opened
 *                         using Create(), clear if it was opened using
 *                         Open().  When miCreated is set, we will try
 *                         to set the file time, date and attributes when
 *                         we close the file.  This is so we can set these
 *                         attributes when we are recreating a file that
 *                         was stored in an archive.
 *
 *  miStorageObjectType  : An integer that is assigned when the object was
 *                         constructed.  Usually one of the enumerated
 *                         constants found in ALDEFS.H.  This is the number
 *                         that gets stored in the Archive directory with 
 *                         the object, so we can figure out what type of 
 *                         object to create when extracting.
 *
 *  muBufferSize         : The size of the I/O buffer.
 *
 *  mpMonitor            : A pointer to the monitor attached to this object.
 *                         During the archiving process, this pointer gets
 *                         set by the archive routine for each storage object
 *                         as it is being processes.  A value of 0 just
 *                         means no monitor is watching this object at the
 *                         moment.
 *
 *  mTimeDate            : The time and date stamp for the file, this usually
 *                         gets set when the object is opened using Open(),
 *                         it is also set when we read in a storage object's
 *                         information using ReadDirectory().
 *
 *  mAttributes          : The attributes associated with the file. R/H/S/A.
 *
 *  mName                : The name of the storage object.
 *
 *  mStatus              : The current status of the object.
 *
 * MEMBER FUNCTIONS
 *
 *  ALStorage()          : The constructor, creates the object, but doesn't
 *                         necessarily create the file/whatever.
 *  operator=()          : Assignment operator.
 *  operator new()       : The memory allocation operator.  This is only
 *                         used if the library is in a DLL.
 *  ~ALStorage()         : Virtual destructor.
 *  UpdateCrc()          : Protected function used internally when the
 *                         crc is being calculated
 *  ReadString()         : Read a string in ArchiveLib's proprietary format.
 *  WriteStorageObjectData() : Protected function to write custom data needed
 *                             for a particular derived classe.
 *  ReadStorageObjectData()  : Protected function read that data back in.
 *  ReadChar()           : Superfast inline function to read a bytee
 *  WriteChar()          : Fast inline function to write a byte.
 *  ReadBuffer()         : Function to read blocks of data.
 *  WriteBuffer()        : Function to write blocks of data.
 *  Open()               : Open() used to prepare an existing object for I/O.
 *  Create()             : Create a new underlying object for I/O.
 *  Close()              : Called when I/O is complete.
 *  LoadBuffer()         : Called to reload the I/O buffer, used internally
 *                         when ReadChar() runs out of stuff to read.
 *  FlushBuffer()        : Called to flush the I/O buffer to the underlying
 *                         object.  Called when WriteChar() has gone too far.
 *  Seek()               : Called to reposition the I/O pointer of the 
 *                         underlying object.
 *  YieldTime()          : Called whenever a FlushBuffer() or LoadBuffer()
 *                         takes place.  Used to update the Monitor attached
 *                         to the file, and to yield time to the O/S.
 *  Compare()            : Compare two storage objects.
 *  InitCrc32()          : Called to start calculating the CRC for an object.
 *  WritePortableShort() : Write 16 bit integer in little endian format.
 *  WritePortableLong()  : Write 32 bit integer in little endian format.
 *  ReadPortableShort()  : Read 16 bit integer in little endian format.
 *  ReadPortableLong()   : Read 32 bit integer in little endian format.
 *  WriteString()        : Write string in ArchiveLib format.
 *  Rename()             : Rename the underlying object.
 *  UnRename()           : Undo a rename operation.
 *  RenameToBackup()     : Rename to a special backup name.
 *  Delete()             : Delete an underlying storage object.
 *  GetCrc32()           : Return value of the CRC member.
 *  GetSize()            : Reeturn value of the size member.
 *  IsOpen()             : Indicate if the file is open.
 *  Tell()               : Indicate where the next read or write will
 *                         take place.
 *  ReadCopyright()      : A function to get at the Greenleaf Copyright.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */


/*
 * Forward declaration
 */

class AL_CLASS_TYPE ALMonitor;

class AL_CLASS_TYPE ALStorage {
    public :
/*
 * Classes I trust
 */
    friend class AL_CLASS_TYPE ALArchiveBase;
    friend class AL_CLASS_TYPE ALCompressedObject;
/*
 * Constructors, destructors, assignment operator
 */
    protected :
        AL_PROTO ALStorage( const char AL_DLL_FAR *file_name,
                            size_t buffer_size,
                            const ALStorageType storage_type,
                            ALCase name_case = AL_MIXED );
        ALStorage AL_DLL_FAR & AL_PROTO operator=( const ALStorage AL_DLL_FAR & );
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
    public :
        virtual AL_PROTO ~ALStorage();
/*
 * I don't want to allow the copy constructor to exist.
 */
    protected :
        AL_PROTO ALStorage( const ALStorage AL_DLL_FAR & );

/*
 * Member functions, grouped somewhat
 *
 *
 * Private member manipulation
 */
    protected :
        void AL_PROTO UpdateCrc( size_t count );
/*
 * This is private, because it allocates memory in the DLL, so it
 * must be deleted in the DLL as well.
 */
    private :
        char AL_DLL_FAR * AL_PROTO ReadString();
        virtual int AL_PROTO
        WriteStorageObjectData( ALStorage AL_DLL_FAR * archive );
        virtual int AL_PROTO 
        ReadStorageObjectData( ALStorage AL_DLL_FAR * archive );
/*
 * The file I/O access public interface
 */
    public :
        int AL_PROTO ReadChar();
        int AL_PROTO WriteChar( int c );
        size_t AL_PROTO ReadBuffer( unsigned char AL_DLL_FAR *buffer, 
                                    size_t length );
        /* Please keep this arg const, breaks WriteString o/w */
        size_t AL_PROTO WriteBuffer( const unsigned char AL_DLL_FAR *buffer, 
                                     size_t length );
        virtual int AL_PROTO Open();
        virtual int AL_PROTO Create();
        virtual int AL_PROTO Close();
        virtual int AL_PROTO LoadBuffer( long address ) = 0;
        virtual int AL_PROTO FlushBuffer() = 0;
        virtual int AL_PROTO Seek( long address ) = 0;
        virtual void AL_PROTO YieldTime();
        int AL_PROTO Compare( ALStorage AL_DLL_FAR &test_object );
        void AL_PROTO InitCrc32( unsigned long seed = 0xffffffffl );
        int AL_PROTO WritePortableShort( short int short_data );
        int AL_PROTO WritePortableLong( long long_data );
        int AL_PROTO ReadPortableShort( short int AL_DLL_FAR &short_data );
        int AL_PROTO ReadPortableLong( long AL_DLL_FAR &long_data );
        int AL_PROTO WriteString( const char AL_DLL_FAR *string_data );
/*
 * File manipulation public interface
 */
    public :
        virtual int AL_PROTO Rename( const char AL_DLL_FAR *new_name = 0, 
                                     int delete_on_clash = 1 ) = 0;
        virtual int AL_PROTO UnRename( int delete_on_clash = 1 ) = 0;
        virtual int AL_PROTO RenameToBackup( int delete_on_clash = 1 ) = 0;
        virtual int AL_PROTO Delete() = 0;
/*
 * Access functions
 */
    public :
        long AL_PROTO GetCrc32();
        long AL_PROTO GetSize() const { return mlSize; }
        int AL_PROTO IsOpen(){ return mpcBuffer != 0; }
        long AL_PROTO Tell();
        char AL_DLL_FAR * AL_PROTO ReadCopyright();
/*
 * Data members
 */
    protected :
        unsigned char AL_DLL_FAR *mpcBuffer;
        size_t muBufferValidData;
        size_t muWriteIndex;
        size_t muReadIndex;
        long mlFilePointer;
        long mlSize;
        long mlCrc32;
        short int miUpdateCrcFlag;
        short int miCreated;
/*
 * Public members
 */
    public :
        const ALStorageType miStorageObjectType;
        const size_t muBufferSize;
        ALMonitor AL_DLL_FAR *mpMonitor;
        ALTimeDate mTimeDate;
        ALFileAttributes mAttributes;
        ALName mName;
        ALStatus mStatus;
        AL_CLASS_TAG( _ALStorageTag );
};

/*
 * It is really important to keep these guys inline.
 */


/*
 * inline int ALStorage::ReadChar()
 *
 * ARGUMENTS:
 *
 *  None.
 *
 * RETURNS
 *
 *  Either the next character available from the I/O buffer, or
 *  AL_END_OF_FILE.
 *
 * DESCRIPTION
 *
 *  This is an inline function that is able to quickly do buffered I/O. 
 *  By utilizing an I/O buffer we can make this routine very fast, since
 *  it doesn't have to call a virtual function.  The virtual function
 *  only has to be called when LoadBuffer() gets called.
 *
 *  Different compilers have different abilities to make this code inline,
 *  so sometimes it needs to be tinkered with.  If you see anything in here
 *  that looks funny, that probably explains why.
 *
 * REVISION HISTORY
 *
 *   May 26, 1994  1.0A  : First release
 *
 */

inline int AL_PROTO ALStorage::ReadChar()
{
    int result;

    AL_ASSERT( muWriteIndex == 0, "ReadChar(): Attempt to read while in write mode" ); /*Can't read if I've done a write!*/
    result = muBufferValidData - muReadIndex;
    if ( result <= 0 )
        result = LoadBuffer( mlFilePointer );
    AL_ASSERT( mpcBuffer != 0, "ReadChar(): Attempt to read from closed file" );    /*Potential disaster*/
    if ( result < 0 )
        return result;
    else
        return mpcBuffer[ muReadIndex++ ] & 0xff;
}

/*
 * inline int ALStorage::WriteChar( int c )
 *
 * ARGUMENTS:
 *
 *  c  : The character that is going to be written.
 *
 * RETURNS
 *
 *  Either the character that we just wrote out, or an error < AL_SUCCESS.
 *
 * DESCRIPTION
 *
 *  This is an inline function that is able to quickly do buffered I/O. 
 *  By utilizing an I/O buffer we can make this routine very fast, since
 *  it doesn't have to call a virtual function.  The virtual function
 *  only has to be called when FlushBuffer() gets called.
 *
 *  Different compilers have different abilities to make this code inline,
 *  so sometimes it needs to be tinkered with.  If you see anything in here
 *  that looks funny, that probably explains why.
 *
 * REVISION HISTORY
 *
 *   May 26, 1994  1.0A  : First release
 *
 */

inline int AL_PROTO ALStorage::WriteChar( int c )
{
    int result;

/*    assert( muReadIndex == 0 ); */ /* Can't write if I've done a read */
    AL_ASSERT( mpcBuffer != 0, "WriteChar(): Attempt to write to closed file" );   /* Disaster! */
    result = muBufferSize - muWriteIndex;
    if ( result <= 0 )
        result = FlushBuffer();
    if ( result < 0 )
        return mStatus;
    else
        return mpcBuffer[ muWriteIndex++ ] = (char) c;
}

#endif /* #if defined( __cplusplus ) */

#endif /* #ifndef _STOR_H         */
