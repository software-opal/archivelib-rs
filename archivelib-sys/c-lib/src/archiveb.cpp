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
// void * ALArchiveBase::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  :  The number of bytes needed to create a new ALArchiveBase object.
//
// RETURNS
//
//  A pointer to the newly allocated storage area, or 0 if no storage
//  was available.
//
// DESCRIPTION
//
//  When using a DLL, it is easy to enter a dangerous situation when
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
void AL_DLL_FAR * AL_PROTO ALArchiveBase::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif

//
// ALArchiveBase::ALArchiveBase( ALStorage *storage_object,
//                               short int delete_in_dtor )
//
// ARGUMENTS:
//
//  storage_object   : A pointer to the storage object that will/does
//                     hold the archive.
//
//  delete_in_dtor   : This flags whether the ALArchiveBase object should call
//                     the destructor for the storage object when the
//                     ALArchiveBase object is created.
//
// RETURNS
//
//  Nothing, it is a constructor.
//
// DESCRIPTION
//
//  This is the ALArchiveBase constructor.  It is a public member function,
//  but in practice it should only be called by the constructors for
//  class derived from ALArchiveBase.  Since there are pure functions
//  in this class, you can't construct an object of this type anyway,
//  no matter how hard you try.
//
//  Despite the complexity of this class, and the vast array of member
//  functions it contains, the constructor doesn't have much to do.  It
//  just sets up the contents of a few data members, and that's that.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

AL_PROTO ALArchiveBase::ALArchiveBase( ALStorage AL_DLL_FAR *storage_object,
                                       short int delete_in_dtor )
    : miDeleteStorageObject( delete_in_dtor )
{
    mpArchiveStorageObject = storage_object;
    mszComment = 0;
    mlDirectoryOffset = -1L;
    miVersion = -1;
}

//
// ALArchiveBase::~ALArchiveBase()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  None, destructors don't get any.
//
// DESCRIPTION
//
//  The destructor for ALArchiveBase has a few pieces of busy work to
//  complete.  First, it might have a comment to delete.  Second, it
//  might have to delete its storage object, but only if it was told
//  to in the constructor.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

AL_PROTO ALArchiveBase::~ALArchiveBase()
{
    AL_ASSERT( GoodTag(), "~Archive(): Attempting to delete invalid ALArchiveBase" );
    if ( mszComment )
        delete[] mszComment;
    if ( mpArchiveStorageObject && miDeleteStorageObject )
        delete mpArchiveStorageObject;
    AL_ASSERT( GoodTag(), "~Archive::Attempting to delete invalid ALArchiveBase" );
}

//
// int ALArchiveBase::SetComment( char * comment )
//
// ARGUMENTS:
//
//  comment :  The new comment that is going to be attached to the archive.
//
// RETURNS
//
//  AL_SUCCESS, if things went well, AL_CANT_ALLOCATE_MEMORY if allocation
//  of the character array failed.
//
// DESCRIPTION
//
//  The archive object has a comment member, that is blank when first
//  constructed.  It can be set to something interesting either by
//  reading in a new comment along with the archive directory, or by
//  setting it using this function.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALArchiveBase::SetComment( char AL_DLL_FAR * comment )
{
    if ( mszComment )
        delete[] mszComment;
    if ( comment == 0 )
        mszComment = 0;
    else {
        mszComment = new char[ strlen( comment ) + 1 ];
        if ( mszComment )
            strcpy( mszComment, comment );
        else
            return mStatus.SetError( AL_CANT_ALLOCATE_MEMORY,
                                     "Failed to allocate memory for "
                                     "comment in archive %s",
                                     mpArchiveStorageObject->mName.GetName() );
    }
    return mStatus;
}

//
// int ALArchiveBase::WriteDirectory( ALEntryList &list )
//
// ARGUMENTS:
//
//  list  :  The ALEntryList object that contains the Archive's
//           up to date directory.
//
// RETURNS
//
//  The integer stored in mStatus, which ought to be AL_SUCCESS if everything
//  went okay, or some int < AL_SUCCESS on error.
//
// DESCRIPTION
//
//  This is the public function the user can call to rewrite the directory
//  for an archive. It is also called internally be several of the functions
//  the update archive contents.  Probably the main reason to call this under
//  normal circumstances would be after modifying the comment field of an
//  archive.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

//
// Don't call ArchiveOperation() here, because this might
// just be a component of a directory write (during an append, eg.)
//
int AL_PROTO ALArchiveBase::WriteDirectory( ALEntryList AL_DLL_FAR &list )
{
    ALOpenInputFile archive( *mpArchiveStorageObject );

    mpArchiveStorageObject->Seek( mlDirectoryOffset );
    mpArchiveStorageObject->WritePortableShort( miVersion );
    WriteArchiveData();
    mpArchiveStorageObject->WriteString( mszComment );

    AddDirectoryEntries( list );
    return mStatus;
}

// PRIVATE MEMBER FUNCTION
//
// void ALArchiveBase::ScanStatus( ALEntryList &list )
//
// ARGUMENTS:
//
//  list  :  The list of entries that have just been processed.
//
// RETURNS
//
//  None.  This function sort of has a return, it will update
//  the member mStatus with an error code if one is found.
//
// DESCRIPTION
//
//
// After an archive operation, I use this function to update the
// status member of the archive.  If the archive doesn't already
// have an error, I check through all the storage objects and
// compression engines to see if any of them hosed up.  Any error
// of any sort by any of them is copied into the archive status.
// The whole point of this is to ensure that if
// ALArchiveBase.mStatus == AL_SUCCESS, it means everything worked.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

void AL_PROTO ALArchiveBase::ScanStatus( ALEntryList AL_DLL_FAR &list )
{
    if ( mStatus < AL_SUCCESS )
        return;
    ALEntry *job = list.GetFirstEntry();
    while ( job ) {
        if ( job->mpStorageObject->mStatus < AL_SUCCESS ) {
            mStatus.SetError( job->mpStorageObject->mStatus,
                              "%s: %s",
                              job->mpStorageObject->mName.GetSafeName(),
                              job->mpStorageObject->mStatus.GetStatusDetail() );
            return;
        }
        job = job->GetNextEntry();
    }
}

//
// int ALArchiveBase::Extract( ALEntryList &list )
//
// ARGUMENTS:
//
//  list   :  A list of storage objects to be extracted (if marked.)
//
// RETURNS
//
//  AL_SUCCESS if all went well, or < AL_SUCCESS if the process
//  went sour at any point.
//
// DESCRIPTION
//
//  This function is one of the high level functions that can be called
//  from a user program.  It has several important things it needs to do
//  in order to extract the appropriate objects from an archive:
//
//  o  Flag any duplicates.  We don't extract anything twice, that will
//     be considered an error.
//
//  o  Open the Archive storage object.
//
//  o  Calculate the total number of bytes to be processed in the entire
//     job, and give this information to the monitor, who might care if
//     he is in AL_MONITOR_JOB mode.
//
//  o  Iterate through the list, performing the following actions for
//     each object marked for extraction:
//
//     o  Update the monitor with information about the object destined
//        for immediate extraction.
//
//     o  Locate the compressed object in the archive.
//
//     o  Decompress the object, and check for CRC errors.
//
//     o  Update the monitor.
//
//  o  After all objects have been extracted, update the monitor again.
//
//  o  Scan for extraction errors, then return the result.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALArchiveBase::Extract( ALEntryList AL_DLL_FAR &list )
{
//
// Open the input storage object, if not already open.  Let the monitor
// know about it.
//
    ALOpenInputFile archive( *mpArchiveStorageObject );
    list.mrMonitor.ArchiveOperation( AL_ARCHIVE_OPEN, this, 0 );
//
// Get rid of any duplicate entries, and set up the monitor sizes.
//
    list.UnmarkDuplicates( list, "Duplicate entry in list passed to Extract()" );
    list.mrMonitor.mlJobSoFar = 0L;
    if ( list.mrMonitor.miMonitorType == AL_MONITOR_JOB )
        list.mrMonitor.mlJobSize = CalculateCompressedJobSize( list );
//
// This loop iterates through the entire ALEntryList.  We only care about
// ALEntry objects that have their mark set.
//
    ALEntry *job = list.GetFirstEntry();
    while ( job ) {
        if ( job->miMark ) {
//
// Go to the correct input position in this, and set up the monitor for
// this particular object.
//
            list.mrMonitor.ArchiveOperation( AL_EXTRACTION_OPEN, this, job );
            mpArchiveStorageObject->Seek( job->mlCompressedObjectPosition );
            list.mrMonitor.mlObjectStart = job->mlCompressedObjectPosition;
            list.mrMonitor.mlObjectSize = job->mlCompressedSize;
            mpArchiveStorageObject->mpMonitor = &list.mrMonitor;
//
// Extract it, then check the CRC.
//
            job->mpCompressionEngine->Decompress( *mpArchiveStorageObject,
                                                    *job->mpStorageObject,
                                                    job->mlCompressedSize );
            if ( job->mpStorageObject->GetCrc32() != job->GetCrc32() )
                job->mpStorageObject->mStatus.SetError(
                        AL_CRC_ERROR,
                        "CRC32 was supposed to be %08lx, was %08lx",
                        job->GetCrc32(),
                        job->mpStorageObject->GetCrc32() );
//
// Update the monitor data, and yield some time. Note that I turn off
// the monitor at this point so it doesn't jump around while I seek to the
// next position in the archive.
//
            list.mrMonitor.mlJobSoFar  += job->mlCompressedSize;
            mpArchiveStorageObject->YieldTime();
            mpArchiveStorageObject->mpMonitor = 0;
            list.mrMonitor.ArchiveOperation( AL_EXTRACTION_CLOSE, this, job );
            job->mpStorageObject->mpMonitor = 0;
        }
        job = job->GetNextEntry();
    }
//
// Update the monitor, then scan the list for status errors.
//
    list.mrMonitor.ArchiveOperation( AL_ARCHIVE_CLOSE, this, 0 );
    ScanStatus( list );
    return mStatus;
}

// PRIVATE MEMBER FUNCTION
//
// int ALArchiveBase::AddJobs( ALEntryList &list )
//
// ARGUMENTS:
//
//  list  :  A list of marked objects to be added to the archive.
//
// RETURNS
//
//  AL_SUCCESS if things are going well, < AL_SUCCESS in case of error.
//
// DESCRIPTION
//
//  This is a helper function that is called by both Create() and
//  Append().  There is enough code here to justify breaking this
//  out into a separate module.
//
//  All this guy does is sit in a loop, look for marked entries in the
//  list, and compress each one into the archive.  Before it adds each object
//  to the archive, it has to set up the monitor so that progress on the
//  selected object will be monitored properly.  It has to dink with the
//  monitor once again when the object has been compressed.  It relies on
//  the calling function to have set up the total job size and other info
//  that the monitor might need.  It also has to set up some of the
//  data in the ALEntry object for each job, as not all of this information
//  is available until *after* the job has been compressed.  For example,
//  the storage object's CRC32 gets calculated as a byproduct of the
//  compression process.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALArchiveBase::AddJobs( ALEntryList AL_DLL_FAR &list )
{
    list.mrMonitor.mlObjectStart = 0L; // This will be true for all input jobs
 //
 // This loop iterates through all of the entries in the list, picking off
 // only the marked entries.
 //
    ALEntry *job = list.GetFirstEntry();
    while ( job ) {
        if ( job->miMark ) {
//
// We need to keep track of the position in the archive where the compressed
// data is going to go.
//
            job->mlCompressedObjectPosition = mpArchiveStorageObject->Tell();
//
// Attach the monitor to the storage object that is going to be inserted
// in the archive.
//
            list.mrMonitor.ArchiveOperation( AL_INSERTION_OPEN, this, job );
            list.mrMonitor.mlObjectSize = -1L; // This means we ask for it in ALMonitor, after the object is opened
            job->mpStorageObject->mpMonitor = &list.mrMonitor;
//
// Compress the object into the archive.  Then store the resulting CRC
// the compressed size in the ALEntry object.
//
            job->mpCompressionEngine->Compress( *job->mpStorageObject,
                                                *mpArchiveStorageObject );
            job->mlCrc32 = job->mpStorageObject->GetCrc32();
            job->mpStorageObject->mpMonitor = 0;
            if ( job->mpCompressionEngine->mStatus < 0 )
                return mStatus = job->mpCompressionEngine->mStatus;
            job->mlCompressedSize = mpArchiveStorageObject->Tell() -
                                    job->mlCompressedObjectPosition;
//
// Update the monitor
//
            list.mrMonitor.mlJobSoFar += job->mpStorageObject->GetSize();
            list.mrMonitor.ArchiveOperation( AL_INSERTION_CLOSE, this, job );
        }
        job = job->GetNextEntry();
        if ( mStatus < 0 )
            break;
    }
    return mStatus;
}

// PRIVATE MEMBER FUNCTION
//
 // int ALArchiveBase::AddDirectoryEntries( ALEntryList &list )
//
// ARGUMENTS:
//
//  list  :  The list of ALEntry objects to be written to the directory.
//
// RETURNS
//
//  AL_SUCCESS if everything goes well, < AL_SUCCESS otherwise.
//
// DESCRIPTION
//
//  This function writes all the entries in the list to the Archive
//  directory.  It doesn't do a seek() to the start of the directory,
//  so the calling routine needs to be absolutely sure that it is in
//  the write spot when it invokes this.
//
//  This routine leaves the output pointer of the storage object pointing
//  at just the right spot to write some more entries.  That means you can
//  call this function repeatedly as new entries are added to the list.
//  The function also terminates the directory properly, so that if you
//  don't add any more directory entries, the archive is still ready
//  for primetime.
//
//  This function is called by WriteDirectory(), and both versions of
//  Append().
//
//  Writing directory entries is a real simple linear task.  The source
//  code here should explain it all.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

//
// No call to ArchiveOperation here, either.  The setup and everything
// else has to be done by the calling routine.
//
int AL_PROTO ALArchiveBase::AddDirectoryEntries( ALEntryList AL_DLL_FAR &list )
{
    ALEntry *job = list.GetFirstEntry();
    while ( job ) {
        if ( job->miMark ) {
            mpArchiveStorageObject->WriteString( job->mpStorageObject->mName.GetSafeName() );
            mpArchiveStorageObject->WriteChar( job->mpCompressionEngine->miCompressionType );
            job->mpCompressionEngine->WriteEngineData( mpArchiveStorageObject );
            mpArchiveStorageObject->WriteChar( job->mpStorageObject->miStorageObjectType );
            job->mpStorageObject->WriteStorageObjectData( mpArchiveStorageObject );

            mpArchiveStorageObject->WritePortableLong( job->mpStorageObject->GetSize() );
            mpArchiveStorageObject->WritePortableLong( job->GetCompressedSize() );
            mpArchiveStorageObject->WritePortableLong( job->GetCrc32() );
            mpArchiveStorageObject->WritePortableLong( job->mlCompressedObjectPosition );
            mpArchiveStorageObject->WriteString( job->GetComment() );
            mpArchiveStorageObject->WritePortableLong( job->mpStorageObject->mTimeDate.GetUnixTime() );
            mpArchiveStorageObject->WritePortableShort( job->mpStorageObject->mAttributes.PackedAttributes() );
            if ( mpArchiveStorageObject->mStatus < 0 )
                return mStatus = mpArchiveStorageObject->mStatus;
        }
        job = job->GetNextEntry();
    }
//
// I write out the end of directory string here.  But then I back up the
// file pointer so new entries can be appended without causing any trouble
// The end of the directory is denoted by an entry with an empty name.
//
    mpArchiveStorageObject->WriteString( "" );
    mpArchiveStorageObject->Seek( mpArchiveStorageObject->Tell() - 2 );

    return AL_SUCCESS;
}

// PRIVATE MEMBER FUNCTION
//
// long ALArchiveBase::CalculateJobSize( ALEntryList &list )
//
// ARGUMENTS:
//
//  list  :  The list of entries in the job.
//
// RETURNS
//
//  This function is used to calculate the total number of bytes that
//  are going to have to be moved when performing a Create() or Append()
//  operation.  We need that info in order to set up a monitor properly
//  when its mode is AL_MONITOR_JOB.  Naturally, we don't really care
//  about the total size when the monitor is in AL_MONITOR_OBJECTS mode.
//  Anyway, it returns the total size of all the objects.
//
// DESCRIPTION
//
//  If a monitor is running in AL_MONITOR_JOB mode, we need to add up
//  the sizes of all the storage objects we are going to process, so
//  that we can accurately track our progress from 0 to 100%.  In many
//  cases, the sizes of all the files will not yet be known, which means
//  this routine will have to open the files up and check the values.
//  That is why we only call this routine when we have to.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

long AL_PROTO ALArchiveBase::CalculateJobSize( ALEntryList AL_DLL_FAR &list )
{
    long total = 0;
    ALEntry *job = list.GetFirstEntry();
    while ( job ) {
        if ( job->miMark ) {
            long obj_size;
            if ( ( obj_size = job->mpStorageObject->GetSize() ) == -1 ) {
                job->mpStorageObject->Open();
                obj_size = job->mpStorageObject->GetSize();
                job->mpStorageObject->Close();
                if ( obj_size == -1 )
                    return -1;
            }
            total += obj_size;
        }
        job = job->GetNextEntry();
    }
    return total;
}

// PRIVATE MEMBER FUNCTION
//
// long ALArchiveBase::CalculateCompressedJobSize( ALEntryList &list )
//
// ARGUMENTS:
//
//  list  :  The list of compressed jobs to be processed.
//
// RETURNS
//
//  The total size of a bunch of compressed objects, not the uncompressed
//  size.
//
// DESCRIPTION
//
//  When we are monitoring an Extract() command, the monitor object
//  gets attached to the Archive, not to the objects that are getting
//  sucked out of it.  This means that progress is being measured
//  against the compressed objects, not the true size objects.  So
//  before I start the extract, I call this function to see just how
//  much compressed space is taken up by the compressed objects in
//  the archive.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

long AL_PROTO ALArchiveBase::CalculateCompressedJobSize( ALEntryList AL_DLL_FAR &list )
{
    long total = 0;
    ALEntry *job = list.GetFirstEntry();
    while ( job ) {
        if ( job->miMark ) {
            if ( job->mlCompressedSize  == -1 )
                return -1;
            else
                total += job->mlCompressedSize;
        }
        job = job->GetNextEntry();
    }
    return total;
}

//
// int ALArchiveBase::Create( ALEntryList &list )
//
// ARGUMENTS:
//
//  list  :  A list of ALEntry objects describing what is going to
//           be stuffed into the archive.
//
// RETURNS
//
//  AL_SUCCESS if things went well, <AL_SUCCESS if things sucked.
//
// DESCRIPTION
//
//  This is one of two public Create() functions.  This is the one you
//  call if you have a bunch of storage objects just sitting around
//  and you want to put them into an archive.  It works by simply
//  walking through the list, and adding each object to the archive
//  storage object by stuffing it through the compression engine.
//
//  This routine has to first create the archive object by opening the
//  associated storage object an reserving a long for a pointer to
//  the directory (which will have to be written later, since we don't
//  have any idea where it is going to be at this time).
//
//  After creating the archive storage object, we go through and remove any
//  duplicated entries in the input list.  If the monitor we will use
//  for this operation is in AL_MONITOR_JOB mode, we then have to
//  calculate the total job size by scanning all the input files
//  (this is really done by CalculateJobSize().)  Finally, we call
//  AddJobs() to do the real work.  Once that is done, we can call
//  WriteDirectory() to finish up.  Note that there are a few calls
//  to ArchiveOperation scattered at key points throughout the process.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALArchiveBase::Create( ALEntryList AL_DLL_FAR &list )
{
//
// Miscellaneous: open the archive, set the archive version, initialize
// the monitor.  If the storage object is broken, quite now!
//
    ALOpenOutputFile archive( *mpArchiveStorageObject );
    miVersion = 0x100;
    list.mrMonitor.ArchiveOperation( AL_ARCHIVE_OPEN, this, 0 );
    if ( mpArchiveStorageObject->mStatus < 0 )
        return mStatus = mpArchiveStorageObject->mStatus;
//
// We don't want to create an archive with duplicate entries, so we check here.
//
    list.UnmarkDuplicates( list,
                           "Duplicate entry in list passed to Create()" );
//
// The first four bytes in the archive are a long that points to the
// first byte of the directory.  I don't know where the directory is
// going to be, so I just reserve space at this time with a dummy value.
//
    mpArchiveStorageObject->WritePortableLong( 0x12345678L );
//
// Set up the monitor.
//
    list.mrMonitor.mlJobSoFar = 0L;
    if ( list.mrMonitor.miMonitorType == AL_MONITOR_JOB )
        list.mrMonitor.mlJobSize = CalculateJobSize( list );
//
// AddJobs() takes care of actually adding the jobs to the archive.
//
    AddJobs( list );
//
// All the jobs are written, now I can figure out where the
// directory is in the storage object.  I copy it, then write
// it out to the storage object at position 0.
//
    mlDirectoryOffset = mpArchiveStorageObject->Tell();
    mpArchiveStorageObject->Seek( 0L );
    mpArchiveStorageObject->WritePortableLong( mlDirectoryOffset );
//
// Return without writing the directory if there is an error in the
// archive storage object.
//
    if ( mpArchiveStorageObject->mStatus < 0 ) {
        list.mrMonitor.ArchiveOperation( AL_ARCHIVE_CLOSE, this, 0 );
        return mStatus = mpArchiveStorageObject->mStatus;
    }
//
// Finally, write out the directory to the storage object.
//
    list.mrMonitor.ArchiveOperation( AL_START_DIRECTORY_WRITE, this, 0 );
    WriteDirectory( list );
//
// Update the monitor, check for errors, and blow.
//
    list.mrMonitor.ArchiveOperation( AL_END_DIRECTORY_WRITE, this, 0 );
    list.mrMonitor.ArchiveOperation( AL_ARCHIVE_CLOSE, this, 0 );
    ScanStatus( list );
    return mStatus;
}

// PRIVATE MEMBER FUNCTIONS
//
// int ALArchiveBase::CopyJobs( ALArchiveBase & source_archive,
//                              ALEntryList & source_list )
//
// ARGUMENTS:
//
//  source_archive  :  The source for all the ALEntry objects that are
//                     going to get copied to this.
//
//  source_list     :  The list of ALEntry objects that are going to be copied.
//
// RETURNS
//
//  An mStatus value, either AL_SUCCESS or < AL_SUCCESS.
//
// DESCRIPTION
//
//  This private member function is used by the Create() and Append() member
//  functions.  Each of these two public functions has two versions, one
//  which compresses freestanding storage objects into an archive, and
//  another which copies jobs out of one archive and into this.  The
//  second versions of the two functions use CopyJobs() to get the
//  compressed objects out of one archive and put it into this.
//
//  The actual operation of this guy is pretty simple.  It would be almost
//  trivial without having to take the monitor into account.  Basically,
//  it just has to work its way through the list of entries.  For each
//  marked entry, we just seek to the correct position in the input file,
//  the copy the correct number of bytes to this.
//
//  One thing kind of funny here is that the ALEntryList starts off with
//  offsets for the objects within the source archive.  But after copying them
//  over, we change the offset field in the ALEntry object to reflect the new
//  position in this.  This means that after this function has completed,
//  you ALEntryList object is no longer associated with source_archive, it
//  is instead associated with this.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALArchiveBase::CopyJobs( ALArchiveBase AL_DLL_FAR &source_archive,
                                      ALEntryList AL_DLL_FAR &source_list )
{
//
// Open the storage object attached to the input archive.  The storage object
// attached to this is already open.
//
    ALOpenInputFile input( *(source_archive.mpArchiveStorageObject) );
//
// Iterate through the list of entries in the list, selecting only the
// marked entries.
//
    ALEntry *job = source_list.GetFirstEntry();
    while ( job ) {
        if ( job->miMark ) {
//
// Seek the compressed object in the source archive, then update the monitor
// to work properly during the copy operation.
//
            source_archive.mpArchiveStorageObject->Seek( job->mlCompressedObjectPosition );
            source_list.mrMonitor.mlObjectStart = job->mlCompressedObjectPosition;
            source_list.mrMonitor.mlObjectSize = job->mlCompressedSize;
            source_list.mrMonitor.ArchiveOperation( AL_COPY_OPEN, this, job );
            source_archive.mpArchiveStorageObject->mpMonitor = &source_list.mrMonitor;
//
// Save the new position in the destination archive, then copy the
// whole thing across.
//
            job->mlCompressedObjectPosition = mpArchiveStorageObject->Tell();
            for ( long i = 0 ; i < job->mlCompressedSize ; i++ ) {
                int c = source_archive.mpArchiveStorageObject->ReadChar();
                mpArchiveStorageObject->WriteChar( c );
            }
//
// Update the monitor now that the copy is complete.
//
            source_list.mrMonitor.ArchiveOperation( AL_COPY_CLOSE, this, job );
            source_archive.mpArchiveStorageObject->YieldTime();
            source_list.mrMonitor.mlJobSoFar += job->mlCompressedSize;
            source_archive.mpArchiveStorageObject->mpMonitor = 0;
            if ( source_archive.mpArchiveStorageObject->mStatus < 0 )
                return mStatus = source_archive.mpArchiveStorageObject->mStatus;
            if ( mpArchiveStorageObject->mStatus < 0 )
                return mStatus = mpArchiveStorageObject->mStatus;
        }
        job = job->GetNextEntry();
        if ( mStatus < 0 )
            break;
    }
    return mStatus;
}

//
// int ALArchiveBase::Create( ALArchiveBase &source_archive,
//                            ALEntryList &source_list )
//
// ARGUMENTS:
//
//  source_archive  : The archive that contains the compressed objects
//                    we are using to create this.
//
//  source_list     : The ALEntryList that contains the marked ALEntry
//                    objects that are going to be inserted in this.
//
// RETURNS
//
//  AL_SUCCESS if things went well, < AL_SUCCESS to flag an error.
//
// DESCRIPTION
//
//  This is the second version of Create().  Instead of creating a new
//  archive by using a bunch of freestanding objects, this guy just
//  sucks existing compressed objects out of one archive and copies
//  them directly into another.  The actual copying gets done in
//  CopyJobs().
//
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALArchiveBase::Create( ALArchiveBase AL_DLL_FAR &source_archive,
                                    ALEntryList AL_DLL_FAR &source_list )
{
//
// Open the source archive, set the version, and blow if for some reason
// the storage object I am writing to isn't working right.
//
    ALOpenOutputFile archive( *mpArchiveStorageObject );
    miVersion = 0x100;
    if ( mpArchiveStorageObject->mStatus < 0 )
        return mStatus = mpArchiveStorageObject->mStatus;
//
// I don't want to create an archive with duplicates, that would be bad.
//
    source_list.UnmarkDuplicates( source_list, "Duplicate entry in list passed to Create()" );
//
// At this point, just for fun, I am going to calculate the total
// compressed size of the jobs I am copying.  Hey, it looks like I
// could substitute a call to CalculateCompressedSize() here!
//
    source_list.mrMonitor.ArchiveOperation( AL_ARCHIVE_OPEN, this, 0 );
    source_list.mrMonitor.mlJobSoFar = 0L;
    source_list.mrMonitor.mlJobSize = 0L;
    for ( ALEntry *job = source_list.GetFirstEntry();
          job != 0;
          job = job->GetNextEntry() ) {
        if ( job->GetMark() )
            source_list.mrMonitor.mlJobSize += job->mlCompressedSize;
    }
//
// Since I am creating a new archive, I write a long out as a place
// holder for the directory pointer.  When I am done copying jobs,
// I'll come back here and write a pointer to the directory.
//
    mpArchiveStorageObject->WritePortableLong( 0x12345678L );
//
// Now copy the data.
//
    CopyJobs( source_archive, source_list );
//
// Write out the directory offset, then the directory itself.
//
    mlDirectoryOffset = mpArchiveStorageObject->Tell();
    mpArchiveStorageObject->Seek( 0L );
    mpArchiveStorageObject->WritePortableLong( mlDirectoryOffset );
    if ( mpArchiveStorageObject->mStatus < 0 ) {
        source_list.mrMonitor.ArchiveOperation( AL_ARCHIVE_CLOSE, this, 0 );
        return mStatus = mpArchiveStorageObject->mStatus;
    }
    source_list.mrMonitor.ArchiveOperation( AL_START_DIRECTORY_WRITE,this, 0 );
    WriteDirectory( source_list );
    source_list.mrMonitor.ArchiveOperation( AL_END_DIRECTORY_WRITE, this, 0 );
    source_list.mrMonitor.ArchiveOperation( AL_ARCHIVE_CLOSE, this, 0 );
//
// Update the error status, and then we are done.
//
    ScanStatus( source_list );
    return mStatus;
}

//
// int ALArchiveBase::Append( ALEntryList &list )
//
// ARGUMENTS:
//
//  list  : A list of objects to append to this.
//
// RETURNS
//
//  AL_SUCCESS if things go okay, < AL_SUCCESS if they didn't.
//
// DESCRIPTION
//
//  This routine is one of the public functions.  It is called to add
//  a list of standalone objects to an existing archive, this.
//  To accomplish this, we have to read in the existing directory, then
//  add the new batch of objects to the archive.  Finally, I write out
//  the old directory, then the directory for the new batch of objects.
//
//  There is another version of Append() that takes as input a list of
//  entries that are in another archive.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALArchiveBase::Append( ALEntryList AL_DLL_FAR &list )
{
    ALEntryList old_list;
//
// Open the storage object for this.
//
    ALOpenInputFile archive( *mpArchiveStorageObject );
//
// I read in the current directory for this archive.  I am going to
// write over the directory with new stuff, so I will have to write it
// back out later.
//
    ReadDirectory( old_list );
    if ( mStatus < 0 )
        return mStatus;
//
// The list of new objects I am going to add needs to be scanned for
// duplicates.  First I clear duplicate entries from the list itself.
// Then I clear any duplicates between the current list and the
// stuff already in the archive.
//
    list.UnmarkDuplicates( list, "Duplicate entry in list passed to Append()" );
    list.UnmarkDuplicates( old_list, "Duplicate entry in list passed to Append()" );
//
// I get the monitor set up, for the batch of entries I am about to do.
//
    list.mrMonitor.ArchiveOperation( AL_ARCHIVE_OPEN, this, 0 );
    list.mrMonitor.mlJobSoFar = 0L;
    if ( list.mrMonitor.miMonitorType == AL_MONITOR_JOB )
        list.mrMonitor.mlJobSize = CalculateJobSize( list );
//
// The new entries start at the position currently occupied by the
// directory.  I seek to that point, then call AddJobs() to do the
// dirty work.
//
    mpArchiveStorageObject->Seek( mlDirectoryOffset );
    AddJobs( list );
    if ( mStatus < 0 ) {
        list.mrMonitor.ArchiveOperation( AL_ARCHIVE_CLOSE, this, 0 );
        return mStatus;
    }
//
// Now that all the new stuff is in the archive, I can figure
// out where the directory belongs, and write it out to position
// 0 in the archive.
//
    mlDirectoryOffset = mpArchiveStorageObject->Tell();
    mpArchiveStorageObject->Seek( 0L );
    mpArchiveStorageObject->WritePortableLong( mlDirectoryOffset );
    if ( mpArchiveStorageObject->mStatus < 0 ) {
        list.mrMonitor.ArchiveOperation( AL_ARCHIVE_CLOSE, this, 0 );
        return mStatus = mpArchiveStorageObject->mStatus;
    }
//
// Now I write the old directory out, and then add in the new
// directory entries.
//
    list.mrMonitor.ArchiveOperation( AL_START_DIRECTORY_WRITE, this, 0 );
    WriteDirectory( old_list );
    AddDirectoryEntries( list );
//
// Update the monitor, check for errors, then leave.
//
    list.mrMonitor.ArchiveOperation( AL_END_DIRECTORY_WRITE, this, 0 );
    list.mrMonitor.ArchiveOperation( AL_ARCHIVE_CLOSE, this, 0 );
    ScanStatus( list );
    return mStatus;
}

//
// int ALArchiveBase::Append( ALArchiveBase &source_archive,
//                            ALEntryList &source_list )
//
// ARGUMENTS:
//
//  source_archive   : The archive where the objects that we are going
//                     use can be found.
//
//  source_list      : The ALEntryList that has a batch of marked entries.
//
// RETURNS
//
//  AL_SUCCESS if things work, < AL_SUCCESS if they don't.
//
// DESCRIPTION
//
//  This append function works just like the previous one, except it
//  is appending jobs that have already been compressed and can be found
//  in a different archive.  It has to go through exactly the same process,
//  which consists of reading the current directory in from this, appending
//  the new compressed objects to this, then writing out the old directory
//  and the new list.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALArchiveBase::Append( ALArchiveBase AL_DLL_FAR &source_archive,
                                    ALEntryList AL_DLL_FAR &source_list )
{
    ALEntryList old_list;
//
// Open the storage object associated with this.
//
    ALOpenInputFile archive( *mpArchiveStorageObject );

    source_list.mrMonitor.ArchiveOperation( AL_ARCHIVE_OPEN, this, 0 );
//
// I have to read the current directory into memory, because as soon as
// I start to write objects out to this, I am going to obliterate
// the directory.
//
    ReadDirectory( old_list );
    if ( mStatus < 0 ) {
        source_list.mrMonitor.ArchiveOperation( AL_ARCHIVE_CLOSE, this, 0 );
        return mStatus;
    }
//
// We don't want to create an archive that has duplicate entries, that would
// be a bad thing.  So I first comb all the duplicate entries out of the list
// of objects to append.  I then compare that list for duplicates against
// the list of objects already in the library, and comb out any matches
// there as well.
//
    source_list.UnmarkDuplicates( source_list, "Duplicate entry in list passed to Append()" );
    source_list.UnmarkDuplicates( old_list, "Duplicate entry in list passed to Append()" );
//
// I am going to start writing new stuff at the location where the
// the directory starts right now.
//
    mpArchiveStorageObject->Seek( mlDirectoryOffset );
//
// Before starting to copy jobs, I have to set up the monitor.
// This includes calculating the total number of compressed bytes
// in all the marked jobs.  I could do this a lot easier by calling
// the CalculateCompressedBytes() function, but it's too late to change now.
//
    source_list.mrMonitor.mlJobSoFar = 0L;
    source_list.mrMonitor.mlJobSize = 0L;
    for ( ALEntry *job = source_list.GetFirstEntry();
          job != 0;
          job = job->GetNextEntry() ) {
        if ( job->GetMark() )
            source_list.mrMonitor.mlJobSize += job->mlCompressedSize;
    }
//
// CopyJobs() does the hard work for me.
//
    CopyJobs( source_archive, source_list );
    if ( mStatus < 0 ) {
        source_list.mrMonitor.ArchiveOperation( AL_ARCHIVE_CLOSE, this, 0 );
        return mStatus;
    }
//
// The jobs are now in, I just have to update the directory with
// the old files and the new files.
//
    mlDirectoryOffset = mpArchiveStorageObject->Tell();
    mpArchiveStorageObject->Seek( 0L );
    mpArchiveStorageObject->WritePortableLong( mlDirectoryOffset );
    if ( mpArchiveStorageObject->mStatus < 0 ) {
        source_list.mrMonitor.ArchiveOperation( AL_ARCHIVE_CLOSE, this, 0 );
        return mStatus = mpArchiveStorageObject->mStatus;
    }
    source_list.mrMonitor.ArchiveOperation( AL_START_DIRECTORY_WRITE, this, 0 );
    WriteDirectory( old_list );
    AddDirectoryEntries( source_list );
//
// Wrap it up.
//
    source_list.mrMonitor.ArchiveOperation( AL_END_DIRECTORY_WRITE, this, 0 );
    source_list.mrMonitor.ArchiveOperation( AL_ARCHIVE_CLOSE, this, 0 );
    ScanStatus( source_list );
    return mStatus;
}

//
// int ALArchiveBase::ReadDirectory( ALEntryList &list )
//
// ARGUMENTS:
//
//  list : The target for the directory listing.
//
// RETURNS
//
//  AL_SUCCESS or < AL_SUCCESS if things don't work.
//
// DESCRIPTION
//
//  This function reads the directory from archive this and places the
//  results in the list parameter.  I have to apologize for the fact
//  that it is so long.  The only thing I can say in my defense is that
//  even though it is really long, it is also really simple, not tricky
//  bits here.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALArchiveBase::ReadDirectory( ALEntryList AL_DLL_FAR &list )
{
    list.mrMonitor.ArchiveOperation( AL_START_DIRECTORY_READ, this, 0 );
    ALOpenInputFile archive( *mpArchiveStorageObject );
    if ( mpArchiveStorageObject->mStatus < 0 )
        return mStatus = mpArchiveStorageObject->mStatus;
//
// First I seek to the start of the directory (offset found at 0), and
// read in the version.  This function only supports the directory
// structure defined in version 0x100.
//
    mpArchiveStorageObject->Seek( 0 );
    mpArchiveStorageObject->ReadPortableLong( mlDirectoryOffset );
    mpArchiveStorageObject->Seek( mlDirectoryOffset );
    mpArchiveStorageObject->ReadPortableShort( miVersion );
    if ( miVersion != 0x100 )
        return mStatus.SetError( AL_INVALID_ARCHIVE,
                                 "%s is not a valid archive file",
                                 mpArchiveStorageObject->mName.GetSafeName() );
//
// Read in any customized archive data defined by a derived class.
//
    ReadArchiveData();
//
// Read in the comment, deleting the old one if necessary.
//
    if ( mszComment )
        delete[] mszComment;
    mszComment = mpArchiveStorageObject->ReadString();
//
// Now, the big loop.  I have to read in each entry, one at a time, and
// add it to the list.  If I broke this out into a separate routine it
// would make the whole thing a lot more manageable.
//
    for ( ; ; ) {
        if ( mpArchiveStorageObject->mStatus < 0 )
            return mStatus = mpArchiveStorageObject->mStatus;
        char *name = mpArchiveStorageObject->ReadString();
        if ( name == 0 )
            break;
//
// The directory ends with a blank name.
//
        if ( strlen( name ) == 0 ) {
            delete[] name;
            break;
        }
//
// Derived classes are responsible for providing a version of
// CreateCompressionEngine() that will convert the engine_type
// integer into a created compression engine.  The derived class is
// then also responsible for reading in the engine data from the archive.
//
        int engine_type = mpArchiveStorageObject->ReadChar();
        ALCompressionEngine *engine = CreateCompressionEngine( engine_type );
        if ( engine )
            engine->ReadEngineData( mpArchiveStorageObject );
        else {
            char *temp = mpArchiveStorageObject->ReadString();
            if ( temp )
                delete[] temp;
            return mStatus.SetError( AL_CANT_CREATE_ENGINE,
                                     "Failure creating compression engine for object %s",
                                     name );
        }
//
// Now we go through a nearly identical process to create the storage object.
// The derived class is responsible for writing a CreateStorageObject()
// function that converts an object_type integer to a created storage
// object.  The derived class also has to read in the storage object
// data.
//
        int object_type = mpArchiveStorageObject->ReadChar();
        ALStorage *storage_object = CreateStorageObject( name, object_type );
        delete[] name; // Don't need it any more
        name = 0;
        if ( storage_object )
            storage_object->ReadStorageObjectData( mpArchiveStorageObject );
        else {
            char *temp = mpArchiveStorageObject->ReadString();
            if ( temp )
                delete[] temp;
            return mStatus.SetError( AL_CANT_CREATE_STORAGE_OBJECT,
                                     "Failure creating storage object for object %s",
                                     name );
        }
//
// The rest of the stuff in the entry is pretty straightforward.
//
        mpArchiveStorageObject->ReadPortableLong( storage_object->mlSize );
        ALEntry *job = new ALEntry( list, storage_object, engine );
        mpArchiveStorageObject->ReadPortableLong( job->mlCompressedSize );
        mpArchiveStorageObject->ReadPortableLong( job->mlCrc32 );
        mpArchiveStorageObject->ReadPortableLong( job->mlCompressedObjectPosition );
        char *comment = mpArchiveStorageObject->ReadString();
        job->SetComment( comment );
        if ( comment )
            delete[] comment;
        long unix_time;
        mpArchiveStorageObject->ReadPortableLong( unix_time );
        storage_object->mTimeDate.SetTimeDate( unix_time );
        short int packed_attributes;
        mpArchiveStorageObject->ReadPortableShort( packed_attributes );
        storage_object->mAttributes.SetFromPackedAttributes( packed_attributes );
    }
    list.mrMonitor.ArchiveOperation( AL_END_DIRECTORY_READ, this, 0 );
    return mStatus;
}

//
// int ALArchiveBase::WriteArchiveData()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  AL_SUCCESS if everything writes out okay, or < AL_SUCCESS for trouble.
//
// DESCRIPTION
//
//  Derived classes can write out customized archive data, for whatever
//  reasons they deem necessary.  Our base class has nothing that it
//  needs to save, so it just writes out a zero length string, which takes
//  two bytes to save.  Instead of using WriteString like I ought to, for
//  some reason I write the 0 out directly.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALArchiveBase::WriteArchiveData()
{
    return mpArchiveStorageObject->WritePortableShort( 0 );
}

//
// int ALArchiveBase::ReadArchiveData()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  AL_SUCCESS if things went well, < AL_SUCCESS it things go sour.
//
// DESCRIPTION
//
//  The base class doesn't store anything in the archive specific
//  data area.  That means that when I am reading the archive specific
//  data in, I should see a zero length string, which is the same thing
//  as a single short of value 0.  I read it in and verify it here.
//
//  Note that derived classes are free to override this function, but
//  nothing we ship with ArchiveLib does so.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALArchiveBase::ReadArchiveData()
{
    short temp;
    mpArchiveStorageObject->ReadPortableShort( temp );
    AL_ASSERT( temp == 0, "ReadArchiveData(): archive data != 0" );
    return mpArchiveStorageObject->mStatus;
}

//
// int ALArchiveBase::Delete( ALEntryList &list,
//                            ALArchiveBase &destination )
//
// ARGUMENTS:
//
//  list        : A list of the objects to delete from the archive.
//
//  destination : The destination archive, which is the result after
//                deleting all the objects from this.
//
// RETURNS
//
//  AL_SUCCESS if things went well, < AL_SUCCESS otherwise.
//
// DESCRIPTION
//
//  Delete is really more like copy.  It doesn't actually delete objects
//  out of an existing archive.  Instead it deletes by excluding the
//  specified objects from a copy command, copying only those objects
//  that aren't in the delete list.  The resulting archive looks as if
//  it is one that has had objects deleted from it.
//
//  After deleting, we do some renaming to make it look like the delete
//  operation did what you really expected.  As a result, the original
//  archive (this) has been renamed to a backup, and the new archive
//  now has the original name of this.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : Had to cast the argument to Rename().  One of
//                         the UNIX compilers was having trouble with the
//                         implicit conversion of ALName to const char *
//
int AL_PROTO ALArchiveBase::Delete( ALEntryList AL_DLL_FAR &list,
                                    ALArchiveBase AL_DLL_FAR &destination )
{
    destination.SetComment( mszComment );
    list.ToggleMarks();
    destination.Create( *this, list );
    list.ToggleMarks();
    ALName temp = mpArchiveStorageObject->mName;
    mpArchiveStorageObject->RenameToBackup();
    destination.mpArchiveStorageObject->Rename( (const char *) temp );
    if ( destination.mStatus < 0 )
        return mStatus = destination.mStatus;
    return mStatus;
}

//
// int ALArchiveBase::FillListBox( HWND hDlg, int list_box = -1 )
//
// ARGUMENTS:
//
//  hDlg       : The handle of the dialog box that has a list box in it.
//               If the value of list_box is set to -1, it means that
//               hDlg doesn't refer to a dialog box, instead it refers
//               to the actual list box itself.
//
//  list_box   : This is set to the id of the list box control found in
//               the hDlg dialog box.  If this value is set to -1, it
//               means the hDlg parameter is the handle of the list box.
//
// RETURNS
//
//  The count of marked items stuffed into the list box.
//
// DESCRIPTION
//
//  This is a quicky useful function to read the names of all the
//  storage objects out of this, then stuffing them all into a list
//  box.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

#if defined( AL_WINDOWS_GUI )
int AL_PROTO ALArchiveBase::FillListBox( HWND hDlg, int list_box /* = -1 */ )
{
    ALEntryList list;
    HWND window;
    ReadDirectory( list );
    if ( list_box != -1 )
        window = GetDlgItem( hDlg, (short int) list_box );
    else
        window = hDlg;
    int count;
    if ( ( count = list.FillListBox( window ) ) == 0 ) {
        if ( mStatus < 0 ) {
            SendMessage( window, LB_RESETCONTENT, 0, 0 );
            SendMessage( window,
                         LB_ADDSTRING,
                         0,
                         (LPARAM)( (LPSTR) "Error!" ) );
        }
    }
    return count;
}
#endif
