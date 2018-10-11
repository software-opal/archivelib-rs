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

//
// void * ALEntry::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  :  The number of bytes needed to create a new ALEntry object.
//
// RETURNS
//
//  A pointer to the newly allocated storage area, or 0 if no storage
//  was available.
//
// DESCRIPTION
//
//  When using a DLL, it is easy to create a dangerous situation when
//  creating objects whose ctor and dtor are both in the DLL.  The problem
//  arises because when you create an object using new, the memory for
//  the object will be allocated from the EXE.  However, when you destroy
//  the object using delete, the memory is freed inside the DLL.  Since
//  the DLL doesn't really own that memory, bad things can happen.
//
//  But, you say, won't the space just go back to the Windows heap regardless
//  of who tries to free it?  Maybe, but maybe not.  If the DLL is using
//  a subsegment allocation schemed, it might do some sort of local free
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
void AL_DLL_FAR * AL_PROTO ALEntry::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif

//
// void * ALEntryList::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  :  The number of bytes that are going to need to be allocated
//           to create a new ALEntryList object.
//
// RETURNS
//
//  A pointer to the newly allocated storage area.  A 0 is returned if no
//  storage could be found.
//
// DESCRIPTION
//
//  Look at the explanation for ALEntry::operator new(), directly above
//  this guy.  The description is identical.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

#if defined( AL_BUILDING_DLL )
void AL_DLL_FAR * AL_PROTO ALEntryList::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif

//
// ALEntry::ALEntry( ALEntryList &list,
//                   ALStorage *object,
//                   ALCompressionEngine *engine )
//
// ARGUMENTS:
//
//  list        :  A reference to the list the ALEntry object is going to
//                 be linked into. ALEntry objects aren't allowed to exist
//                 without being in a list.
//
//  object      :  A pointer to the storage object that is attached to this
//                 entry.  Remember, this is an unopened storage object,
//                 so it is not consuming very much space.  It is okay
//                 to have a zillion or so of these just lying around.
//                 Don't forget that the ALEntry dtor is going to destroy
//                 this guy for you, don't you dare try it!.
//
//  engine      :  A pointer to the compression engine that is going to
//                 be used to create/insert/extract the storage object
//                 to/from the archive.  Just like with the compression
//                 engine, it is a low cost object, and you can keep lots
//                 of them on hand.  This engine will be destroyed in the
//                 ALEntry dtor, so be sure to give up any claim you might
//                 have on this guy.
//
// RETURNS
//
//  Nothing, this is a ctor.
//
// DESCRIPTION
//
//  This ctor creates a new ALEntry object.  You can do this by hand, but
//  frequently you will ask ArchiveLib to create ALEntry objects for you,
//  maybe by pulling them out of a list box, or reading them in from and
//  archive.  Note that ALEntry objects aren't allowed to ever exist
//  outside a list, each entry absolutely has to appear in a list.
//
//  dtor issues relating to the ALEntry object are very important.  Since
//  ALEntry objects always are part of a list, it made sense for the
//  ALEntryList destructor to clean up all the entries in its list.  So
//  even though you might have created this ALEntry object, you don't get to
//  delete it, that will be done for you.
//
//  Also, the storage object and compression engine in the ALEntry object
//  are going to be automatically destroyed by the ALEntry dtor.  Don't
//  even think about trying it yourself!
//
//  You can think of an ALEntryList as a directory of an archive, and each
//  ALEntry object in the list is a single entry in that directory.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

AL_PROTO ALEntry::ALEntry( ALEntryList &list,
                           ALStorage *object,
                           ALCompressionEngine *engine )
    : mrList( list ) // Initialize our own pointer to the list we will
                     // be a member of.
{
    mpNextItem = this;
    mpPreviousItem = this;
    mpStorageObject = object;
    mpCompressionEngine = engine;
    mlCompressedSize = -1;
    mlCompressedObjectPosition = -1;
    miMark = 1; //Always construct with the mark turned on
    mszComment = 0;
//
// I check for the object member to be non-zero because of a clunky design
// choice I made a while back.  Each ALEntryList has an ALEntry member that
// points to the first and last members of the list.  I could have (and
// probably should have) made the root of the list just be a pair of pointers,
// instead of a dummy ALEntry.  Anyway, I can tell that dummy entry apart
// from the valid entries by virtue of the fact that it has a null
// pointer in its object pointer.
//
// So anyway, when I create this dummy object, I don't want to try to add
// it to the list, because by definition it is already in the list.  So
// I do a check before adding any ALEntry to the list.
//
    if ( object )
        InsertBefore( *list.mpListHead );
}

//
// ALEntry::~ALEntry()
//
// ARGUMENTS:
//
//  Nothing.
//
// RETURNS
//
//  Nothing, this is a destructor.
//
// DESCRIPTION
//
//  This destructor should normally be called by the ALEntryList dtor.  The
//  list that owns an entry will always try to delete it when the list
//  is deleted.
//
//  The ALEntry object tries to delete three dynamically allocated objects
//  that it has control over:  the storage object, the compression engine,
//  and the comment.  In each case it won't do it if the object pointer
//  is 0.  This provides a convenient mechanism for you to steal a storage
//  object from an ALEntry.  All you have to do is take the pointer, and
//  then sent ALEntry::mpStorageObject to 0.  This is an especially useful
//  thing to do for ALMemory objects.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

AL_PROTO ALEntry::~ALEntry()
{
    AL_ASSERT( GoodTag(), "~ALEntry: Attempting to delete invalid object" );
    if ( mszComment )
        delete[] mszComment;
    if ( mpStorageObject != 0 )
        delete mpStorageObject;
    if ( mpCompressionEngine != 0 )
        delete mpCompressionEngine;
    AL_ASSERT( mpNextItem != 0 ,"~ALEntry: next item is null" );
    AL_ASSERT( mpPreviousItem != 0, "~ALEntry: previous item is null" );

    ALEntry *next_job = mpNextItem;
    ALEntry *previous_job = mpPreviousItem;

    if ( next_job != this ) {
        next_job->mpPreviousItem = previous_job;
        previous_job->mpNextItem = next_job;
    }
//
// Note that I check the object twice, one at the start of the dtor, and
// once again at the end.  With all the linked list and dynamic deletion
// being done here, it seems like it would be really easy to hose things
// up if any mistakes were made.
//
    AL_ASSERT( GoodTag(), "~ALEntry: Attempting to delete invalid object" );
}

//
// int ALEntry::Duplicate( ALEntryList &list )
//
// ARGUMENTS:
//
//  list  : A list of ALEntry objects to scan.
//
// RETURNS
//
//  0 if the entry is not duplicated, 1 if it is.
//
// DESCRIPTION
//
//  This function is used to scan a list of ALEntry objects to see if
//  any of them have the same name as this.  Unmarked objects are ignored.
//  All the function does is zip through the ALEntryList, checking each
//  marked member for an ASCII match with the name of the storage object
//  pointed to by this.  You can see that the case sensitivity of this
//  is observed when making the comparison.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : The Sun version of UNIX uses strcasecmp() instead
//                         of the familiar ANSI version stricmp().  Had to
//                         #ifdef around it.
//
int AL_PROTO ALEntry::Duplicate( ALEntryList &list )
{
    char *name = mpStorageObject->mName;
    int case_sensitive = mpStorageObject->mName.mCase == AL_MIXED;
    ALEntry *job = list.GetFirstEntry();
    while ( job ) {
        if ( job->GetMark() && job != this ) {
            if ( case_sensitive ) {
                if ( strcmp( name, job->mpStorageObject->mName ) == 0 )
                    return 1;
            } else {
#if defined( AL_SUN4 )
                if ( strcasecmp( name, job->mpStorageObject->mName ) == 0 )
#else
                if ( stricmp( name, job->mpStorageObject->mName ) == 0 )
#endif
                    return 1;
            }
        }
        job = job->GetNextEntry();
    }
    return 0;
}

//
// PROTECTED FUNCTION
//
// void ALEntry::InsertBefore( ALEntry &job )
//
// ARGUMENTS:
//
//  job : A reference to another job in the target list.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This function is used inside the ALEntryList class to add a new ALEntry
//  object to an ALEntryList.  Since the list is a doubly linked list, the
//  code to do the job is pretty simple.  It would have been a little more
//  complicated if I used a pair of pointers in the ALEntryList to start
//  the list, instead of a dummy ALEntry object.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

void AL_PROTO ALEntry::InsertBefore( ALEntry &job )
{
    mpNextItem = &job;
    mpPreviousItem = job.mpPreviousItem;
    (job.mpPreviousItem)->mpNextItem = this;
    job.mpPreviousItem = this;
}

//
// int ALEntry::SetComment( const char *comment )
//
// ARGUMENTS:
//
//  comment : The new comment that is going to be associated with the
//            ALEntry object.
//
// RETURNS
//
//  AL_SUCCESS if the new comment was set, < 0 if an error occurred.
//
// DESCRIPTION
//
//  Before adding an object to an archive, you may want to change or set
//  its comment.  You do so by calling this function before performing any
//  operation that will write the directory, such as Create() or
//  WriteDirectory().  It has to dynamically allocate the space in the
//  ALEntry object in order to store the new comment.  This is good for
//  you, because it means you don't have to worry about who owns the comment
//  you just passed in.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALEntry::SetComment( const char AL_DLL_FAR *comment )
{
    if ( mszComment )
        delete[] mszComment;
    if ( comment ) {
        mszComment = new char[ strlen( comment ) + 1 ];
        if ( mszComment )
            strcpy( mszComment, comment );
        else
            return mrList.mStatus.SetError( AL_CANT_ALLOCATE_MEMORY,
                                            "Failed to allocate memory when "
                                            "adding comment to storage object %s",
                                            (char *) mpStorageObject->mName );
    } else
        mszComment = 0;
    return AL_SUCCESS;
}

//
// int ALEntry::CompressionRatio()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  The integer representing the compression ratio.  The ration is a number
//  from 0 to 100 (or maybe more) with 0 being perfect compression.
//
//  It is possible to get a -1 back from this routine if the compression
//  ratio is not presently known.  This will be the case if you have
//  not created the archive yet, or have a new object that hasn't been
//  inserted yet.
//
// DESCRIPTION
//
//  This calculates and returns the compression ratio.  We don't store the
//  ratio in ALEntry, because it is so darned easy to calculate when
//  we need it.  However, there are going to be times when we don't have
//  it.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALEntry::CompressionRatio()
{
    long uncompressed_size = mpStorageObject->GetSize();

    if ( uncompressed_size  <= 0 )
        return -1;
    if ( mlCompressedSize <= 0 )
       return -1;
    return (int) ( 100 * mlCompressedSize / uncompressed_size );
}


//
// ALEntryList::ALEntryList( ALMonitor * monitor = 0 )
//
// ARGUMENTS:
//
//  monitor : A pointer to a monitor that will be used whenever we are
//            processing objects in the list.  If no argument is supplied,
//            the default argument value of 0 is used.  When the ctor sees
//            that the value of the monitor pointer is 0, it assigns the
//            default monitor instead.
//
// RETURNS
//
//  No returns from constructors.
//
// DESCRIPTION
//
//  Constructing an ALEntryList object doesn't take much work.  I have to
//  initialize two data members.  The first is the pointer to the monitor
//  that will be used when processing objects in the list.  The second is
//  the root of the linked list, which is a dummy ALEntry object.  Note
//  that the root is created as a dummy by setting the storage object pointer
//  to 0.
//
//  The default monitor is defined below.  If you don't specify a real
//  monitor, you get the default, which is a do nothing function.  Everyone
//  can share one instance of the default monitor, because it doesn't have
//  any data members to be concerned about.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

ALMonitor ALDefaultMonitor( AL_MONITOR_OBJECTS );

AL_PROTO ALEntryList::ALEntryList( ALMonitor AL_DLL_FAR * monitor /* = 0 */ )
    : mrMonitor( monitor ? *monitor : ALDefaultMonitor )
{
    mpListHead = new ALEntry( *this, 0, 0 );
}

//
// ALEntryList::~ALEntryList()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  None, destructors don't get to return anything.
//
// DESCRIPTION
//
//  The destructor for ALEntryList goes through the list and deletes every
//  ALEntry object it finds.  Note that this also causes the ALEntry
//  object to destroy its storage object and compression engine.  Once
//  the whole list is obliterated, the list head ALEntry object can be
//  safely deleted.  Then the whole thing is done.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

AL_PROTO ALEntryList::~ALEntryList()
{
    AL_ASSERT( GoodTag(), "~ALEntryList: attempting to delete invalid object" );
    ALEntry *job = GetFirstEntry();
    while ( job ) {
        ALEntry *next_job = job->GetNextEntry();
        delete job;
        job = next_job;
    }
    if ( mpListHead )
        delete mpListHead;
    AL_ASSERT( GoodTag(), "~ALEntryList: attempting to delete invalid object" );
}

// PROTECTED FUNCTION
//
//  int ALEntryList::SetMarkState( const char *name,
//                                 short int new_state )
//
// ARGUMENTS:
//
//  name       :  The object name, specifying which storage objects are
//                to have their state set.  This name can include
//                wild card characters.  Note that passing a null
//                pointer here will cause a match to *every* object name.
//
//  new_state  :  The new state that the ALEntry mark should be set to.
//
// RETURNS
//
//  A count of the number of ALEntry objects whose state was changed.
//
// DESCRIPTION
//
//  This protected function is used internally to help out a couple of the
//  public functions.  It rips through every entry of the list, checks to
//  see if storage object associate with the entry has a name that matches
//  the wildcard specification, and sets the mark if it does.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALEntryList::SetMarkState( const char AL_DLL_FAR *name,
                                        short int new_state )
{
    int count = 0;

    ALEntry *job = GetFirstEntry();
    while ( job ) {
        if ( name ) {
            if ( job->mpStorageObject->mName.WildCardMatch( name ) ) {
                job->SetMarkState( new_state );
                count++;
            }
        } else {
            job->SetMarkState( new_state );
            count++;
        }
        job = job->GetNextEntry();
    }
    return count;
}

//
// int ALEntryList::ToggleMarks()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  A count of the number of entries whose mark was changed.
//  (Just the total number of entries.)
//
// DESCRIPTION
//
//  This simple member function just goes through the entire list,
//  toggling the mark state of every entry.  In other words, if the mark
//  was previously set, it will now be cleared, and vice versa.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALEntryList::ToggleMarks()
{
    int count = 0;
    ALEntry *job = GetFirstEntry();
    while ( job ) {
        job->SetMarkState( (short int) !job->GetMark() );
        job = job->GetNextEntry();
        count++;
    }
    return count;
}

//
// ALEntry * ALEntry::GetNextEntry()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  A pointer to the next entry in the list.  If the next entry is the
//  list head, it means we have reached the end of the list, and a value
//  of 0 is returned.
//
// DESCRIPTION
//
//  This function is used to iterate through the list.  Each entry has
//  a pointer to the next and previous entries, so this function is really
//  simple.  The only complications comes from trying to detect the end of
//  the list, which is denoted by the list head instance of ALEntry.  We
//  can tell it apart from all the legitimate entries by the fact that
//  its storage object is 0.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

ALEntry AL_DLL_FAR * AL_PROTO ALEntry::GetNextEntry()
{
    ALEntry *next_entry = this->mpNextItem;
//
// The list head has the special case where both the compression engine
// and storage object pointers are 0, and that makes the end of the list.
//
    if ( mpNextItem->mpStorageObject == 0 )
        return 0;
    else
        return next_entry;
}

//
// ALEntry * ALEntryList::GetFirstEntry()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  A pointer to the first valid ALEntry object in the list, or 0 if there
//  are no entries.
//
// DESCRIPTION
//
//  If you are going to iterate through the entire list, this function is
//  used to start you off.  It gets the first entry in the list by call
//  GetNextEntry() for the list head.  Don't worry about what happens if
//  the list is empty, the GetNextEntry() code figures that out with no
//  problem, and returns a 0.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

ALEntry AL_DLL_FAR * AL_PROTO ALEntryList::GetFirstEntry()
{
    return mpListHead->GetNextEntry();
}

//
// void ALEntryList::UnmarkDuplicates( ALEntryList &list,
//                                     const char *error_message = 0 )
//
// ARGUMENTS:
//
//  list           :  The list that is going to be compared to this.
//
//  error_message  : Each entry in this that turns out to have a duplicate
//                   entry in the list argument will not only be unmarked,
//                   it will also have its error status set, if an error
//                   message is provide.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  I think this function is a little confusing.  At first blush, you would
//  probably expect this function to scan all the items in a single list,
//  and unmark any object that turn out to have duplicates elsewhere
//  in the list.  Unfortunately, it doesn't work that way.
//
//  Instead, this function goes through the list specified by this, and
//  checks to see if each entry in this appears in the list specified by
//  the list parameter.  This means that we are working with two different
//  lists, which certainly offers plenty of chances to get confused.
//
//  Anyway, each entry in this that turns out to have a duplicate gets its
//  mark cleared.  If the calling program specifies and error message,
//  the entry also gets its mStatus error member set to flag this as an
//  error.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

void AL_PROTO ALEntryList::UnmarkDuplicates( ALEntryList &list,
                                             const char *error_message /* = 0 */ )
{
    ALEntry *job = GetFirstEntry();
    while ( job ) {
        if ( job->GetMark() ) {
            if ( job->Duplicate( list ) ) {
                job->ClearMark();
                if ( error_message && error_message[ 0 ] != '\0' )
                    job->mpStorageObject->mStatus.SetError(
                            AL_DUPLICATE_ENTRY,
                            error_message );
            }
        }
        job = job->GetNextEntry();
    }
}

//
// int ALEntryList::DeleteUnmarked()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  The number of entries that are deleted.
//
// DESCRIPTION
//
//  Sometimes you may have a list with a whole bunch of unmarked entries.
//  Those unmarked entries are just sitting there taking up space, so it
//  would be handle to be able to just delete them.  That is what this
//  function does.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALEntryList::DeleteUnmarked()
{
    ALEntry *job;
    int count = 0;

    job = GetFirstEntry();
    while ( job ) {
        ALEntry *next_job = job->GetNextEntry();
        if ( job->GetMark() == 0 ) {
            count++;
            delete job;
        }
        job = next_job;
    }
    return count;
}

//
// in ALEntryList::FillListBox( HWND hDlg, int list_box = -1 )
//
// ARGUMENTS:
//
//  hDlg       : The handle of the dialog box that contains the list box
//               control.  If the list box is not a control in a dialog,
//               set the next parameter to -1, and just pass the handle
//               of the list box in this argument.
//
//  list_box   : The ID of the list box, if and only if the list box is
//               in a dialog box specified by by the hDlg argument.
//
// RETURNS
//
//  The number of entries that were stuffed into the list box.
//
// DESCRIPTION
//
//  This function is a handy helper when using the Windows GUI.  It goes
//  through an ALEntryList, and finds all the marked entries.  For every
//  marked entry, it stuffs the name of the storage object into the list box.
//  This means that if you are planning on letting the user select a list
//  of storage objects, you can initialize the list with just one
//  function call.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

#if defined( AL_WINDOWS_GUI )
int AL_PROTO ALEntryList::FillListBox( HWND hDlg, int list_box /* = -1 */ )
{
    HWND window;

    if ( list_box != -1 )
        window = GetDlgItem( hDlg, (short int) list_box );
    else
        window = hDlg;
    SendMessage( window, LB_RESETCONTENT, 0, 0 );
    ALEntry *job = GetFirstEntry();
    int count = 0;
    while ( job ) {
        if ( job->GetMark() ) {
            count++;
            SendMessage( window,
                         LB_ADDSTRING,
                         0,
                         (LPARAM)( (LPSTR) job->mpStorageObject->mName ) );
        }
        job = job->GetNextEntry();
    }
    if ( count == 0 )
        SendMessage( window,
                     LB_ADDSTRING,
                     0,
                     (LPARAM)( (LPSTR) "<none>" ) );
    return count;
}


//
// int ALEntryList::SetMarksFromListBox( HWND hDlg, int list_box = - 1 )
//
// ARGUMENTS:
//
//  hDlg        : The handle of the dialog box that contains the list box.
//                If the list box control is standalone window, this parameter
//                can be its handle, if the list_box argument is set to -1.
//
//  list_box    : The ID of the list box, if and only if it is contained in
//                a dialog box whose handle is specified in the hDlg param.
//
// RETURNS
//
//  A count of the number of items whose marks were set.
//
// DESCRIPTION
//
//  This function is called after you have given a user the opportunity
//  to set and clear items in a multiselection list box.  Once the user
//  has done so, you can call this function, which will go through the
//  list and set all the marks that have been set in the list box by the
//  user.  Note that it will not clear the marks on any of the ALEntry
//  objects in the list, you might want to do that first.
//
// REVISION HISTORY
//
//   May 23, 1994  1.0A  : First release
//

int AL_PROTO ALEntryList::SetMarksFromListBox( HWND hDlg, int list_box /* = -1 */ )
{
    HWND window;

    if ( list_box != -1 )
        window = GetDlgItem( hDlg, (short int) list_box );
    else
        window = hDlg;

    WORD count = (WORD) SendMessage( window, LB_GETSELCOUNT, 0, 0L );
    int *items = new int[ count ];
    if ( items == 0 )
        return mStatus.SetError( AL_CANT_ALLOCATE_MEMORY,
                                   "Memory allocation failure in SetMarksFromListBox()" );
#ifdef AL_FLAT_MODEL
    if ( count != (WORD) SendMessage( window, LB_GETSELITEMS, count, (LPARAM) ( items ) ) ) {
#else
    if ( count != (WORD) SendMessage( window, LB_GETSELITEMS, count, (LPARAM) ( (int _far * ) items ) ) ) {
#endif
        mStatus.SetError( AL_LOGIC_ERROR,
                            "Logic error in SetMarksFromListBox()."
                            "Mismatch in select count from list box." );
        delete[] items;
        return AL_LOGIC_ERROR;
    }
    for ( WORD i = 0 ; i < count ; i++ ) {
        WORD length = (WORD) SendMessage( window, LB_GETTEXTLEN, (short int) items[ i ], 0L );
        AL_ASSERT( length != (WORD) LB_ERR, "SetMarksFromListBox: LB_ERR returned from list box" );
        if ( length > 0 ) {
            char *name = new char[ length + 1 ];
            if ( name ) {
                if ( SendMessage( window, LB_GETTEXT, (short int) items[ i ], (LPARAM)( (LPSTR) name ) ) >= 0 )
                    SetMarks( name );
                delete[] name;
            }
        }
    }
    delete[] items;
    return count;
}

#endif
