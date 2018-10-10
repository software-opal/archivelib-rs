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


#ifndef _ARCENTRY_H
#define _ARCENTRY_H

#if defined( __cplusplus )

/*
 * Forward declarations.
 */

class AL_CLASS_TYPE ALEntryList;
class AL_CLASS_TYPE ALArchiveBase;
class AL_CLASS_TYPE ALMonitor;

/*
 * class ALEntry 
 *
 * DESCRIPTION
 *
 *  ALEntry objects describe an entry in an archive.  When you read in
 *  the directory from an archive, it consists of a list of ALEntry
 *  objects.  The description can also refer to objects that you
 *  want to put in an archive.
 *
 *  An ALEntry object has pointers to both a compression engine and
 *  a storage object.  It also has the position of an object in an archive,
 *  its CRC-32, and more.  Some of this data will not be filled in when
 *  you pass a list of these objects as an argument to an Archive command
 *  like Create().
 *
 *
 * DATA MEMBERS
 *
 *  mszComment             : The comment stored with the archive.
 *
 *  mpNextItem             : A pointer to the next ALEntry object in 
 *                           the list.  (Note that ALEntry items are
 *                           always in a list.)
 *
 *  mpPreviousItem         : A pointer to the previous item in the list.
 *
 *  mrList                 : A reference to the list that the ALEntry
 *                           guy is a member of.
 *
 *  mlCompressedSize       : How big the object is after it is compressed.
 *                           If you are inserting an object for the first
 *                           time, you will have to wait for the ALArchiveBase
 *                           member function to fill this guy in after
 *                           the insertion takes place.
 *
 *  mlCompressedObjectPosition : Where the object is found in the archive.
 *                               Another field that gets filled in during
 *                               insertion.
 *
 *  mlCrc32                 : The CRC-32 of the uncompressed object.  This
 *                            gets filled in during compression.
 *
 *  miMark                  : The object's mark.  If the mark is not set,
 *                            most of the archive commands will ignore
 *                            this entry.
 *
 *
 * mpStorageObject          : A pointer to the storage object associated
 *                            with this entry.  This is a public member,
 *                            so you can dink with it.  It will be destroyed
 *                            by the ALEntry destructor!
 *
 * mpCompressionEngine      : A pointer to the compression engine associated
 *                            with this archive entry.
 *
 * MEMBER FUNCTIONS
 *
 * ALEntry()             : The constructor.
 * ~ALEntry()            : The destructor.
 * operator new()        : Memory allocation operator, only used when the
 *                         constructor is inside the DLL.
 * InsertBefore()        : A private function used when updating an
 *                         ALEntrylist
 * GetNextEntry()        : A routine used when interating an ALEntrylist.
 * GetCompressedSize()   : An access routine to get a protected member.
 * GetCrc32()            : An access routine to get a protected member.
 * GetComment()          : An access routine to get a protected member.
 * SetMark()             : Set the mark for an ALEntry, the default state is set.
 * ClearMark()           : Clear the mark for an ALEntry.
 * SetMarkState()        : Private function to set or clear the mark.
 * SetComment()          : Set the comment for an entry.
 * GetMark()             : Get the current state of the mark.
 * CompressionRatio()    : Calculate the compression ratio for an object.
 * Duplicate()           : Test to see if an entry is found in a list.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

class AL_CLASS_TYPE ALEntry {
/*
 * Constructors, destructors, friends
 */
    friend class AL_CLASS_TYPE ALArchiveBase;

    public :
        AL_PROTO ALEntry( ALEntryList AL_DLL_FAR &,
                          ALStorage AL_DLL_FAR *,
                          ALCompressionEngine AL_DLL_FAR * );
        AL_PROTO ~ALEntry();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 * The copy constructor and assignment operator are not supported.  I
 * declare them here because I don't want the compiler to generate
 * default versions for me.
 */
    protected :
        AL_PROTO operator=( ALEntry AL_DLL_FAR & );
        AL_PROTO ALEntry( ALEntry AL_DLL_FAR & );
/*
 * Member functions
 */
    protected :
        void AL_PROTO InsertBefore( ALEntry AL_DLL_FAR & );

    public :
        ALEntry AL_DLL_FAR * AL_PROTO GetNextEntry();
        long AL_PROTO GetCompressedSize() const { return mlCompressedSize; }
        long AL_PROTO GetCrc32() const { return mlCrc32; }
        const char AL_DLL_FAR * AL_PROTO GetComment(){ return mszComment; }
        void AL_PROTO SetMark(){ miMark = 1; }
        void AL_PROTO ClearMark(){ miMark = 0; }
        void AL_PROTO SetMarkState( short int new_state ){ miMark = new_state; }
        int AL_PROTO SetComment( const char AL_DLL_FAR *comment );
        int AL_PROTO GetMark(){ return miMark; }
        int AL_PROTO CompressionRatio();
        int AL_PROTO Duplicate( ALEntryList AL_DLL_FAR &list );
/*
 * Data members
 */
    protected :
        char AL_DLL_FAR *mszComment;
        ALEntry AL_DLL_FAR *mpNextItem;
        ALEntry AL_DLL_FAR *mpPreviousItem;
        ALEntryList AL_DLL_FAR &mrList;
        long mlCompressedSize;
        long mlCompressedObjectPosition;
        long mlCrc32;
        short int miMark;

    public :
        ALStorage AL_DLL_FAR *mpStorageObject;
        ALCompressionEngine AL_DLL_FAR *mpCompressionEngine;
        AL_CLASS_TAG( _ALEntryTag );
};

/*
 * class ALEntryList
 *
 * DESCRIPTION
 *
 *  This class is simply a list of ALEntry objects.  There are
 *  quite a few member functions that operate on this list.  
 *  ALEntryList objects are passed as arguments to many of the
 *  archive functions, such as ReadDirectory(), Create(), Extract(),
 *  and more.
 *
 * DATA MEMBERS
 *
 *  mpListHead   : The head of the list is a dummy entry that is a 
 *                 placeholder.  
 *
 *  &mrMonitor   : A reference to the monitor associated with this list.
 *                 The monitor will take care of generating all the
 *                 user interface activity to go with this list.
 *
 *  mStatus      : A standard status member, the status of the whole list.
 *
 * MEMBER FUNCTIONS
 *
 *  ALEntryList()         : The constructor
 *  ~ALEntryList()        : The destructor.
 *  operator new()        : Memory allocation for the class, only used
 *                          when the ctor is in a DLL.
 *  SetMarkState()        : Set the marks of items in the list to 1 or 0
 *  GetFirstEntry()       : A list iterator function, starts the iteration
 *  SetMarks()            : Set some of the marks in the list
 *  ClearMarks(           : Clear some of the marks in the list
 *  DeleteUnmarked()      : Delete list entries that aren't marked
 *  ToggleMarks()         : Toggle every mark in the list
 *  UnmarkDuplicates()    : Use this to avoid processing duplicates
 *  FillListBox()         : Fill a list box up with a list
 *  SetMarksFromListBox() : Use list feedback to set marks
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

class AL_CLASS_TYPE ALEntryList {
/*
 * Constructors, destructors, friends
 */
    friend class AL_CLASS_TYPE ALEntry;

    public :
        AL_PROTO ALEntryList( ALMonitor AL_DLL_FAR * = 0 );
        AL_PROTO ~ALEntryList();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 * The copy constructor and assignment operator are not supported.  I
 * declare them here because I don't want the compiler to generate
 * default versions for me.
 */
    protected :
        AL_PROTO operator=( ALEntryList AL_DLL_FAR & );
        AL_PROTO ALEntryList( ALEntryList AL_DLL_FAR & );
/*
 * Member Functions
 */
    protected :
        int AL_PROTO SetMarkState( const char AL_DLL_FAR *name, short int state );

    public :
        ALEntry AL_DLL_FAR * AL_PROTO GetFirstEntry();
        int AL_PROTO SetMarks( const char AL_DLL_FAR *pattern = 0 );
        int AL_PROTO ClearMarks( const char AL_DLL_FAR *pattern = 0 );
        int AL_PROTO DeleteUnmarked();
        int AL_PROTO ToggleMarks();
        void AL_PROTO UnmarkDuplicates( ALEntryList AL_DLL_FAR &list,
                                           const char AL_DLL_FAR *error_message = 0 );
/*
 * A windows utility function
 */
#if defined( AL_WINDOWS_GUI )
        int AL_PROTO FillListBox( HWND hDlg, int list_box = -1 );
        int AL_PROTO SetMarksFromListBox( HWND hDlg, int list_box = -1 );
#endif

/*
 * Data members
 */
    protected :
        ALEntry *mpListHead; /* The head is never used */

    public :
        ALMonitor AL_DLL_FAR &mrMonitor;
        ALStatus mStatus;
        AL_CLASS_TAG( _ALEntryListTag );
};

/*
 * inline int ALEntryList::SetMarks( const char *name )
 *
 * ARGUMENTS:
 *
 *  name  : A wild care file spec.
 *
 * RETURNS
 *
 *  The number of entries that matched the wild card.
 *
 * DESCRIPTION
 *
 *  This function sets the mark for every entry in the list object
 *  that matches the wild card specification.
 *
 * REVISION HISTORY
 *
 *   May 26, 1994  1.0A  : First release
 */

inline int AL_PROTO ALEntryList::SetMarks( const char AL_DLL_FAR *name )
{
    return SetMarkState( name, 1 );
}

/*
 * inline int ALEntryList::ClearMarks( const char *name )
 *
 * ARGUMENTS:
 *
 *  name  : A wild care file spec.
 *
 * RETURNS
 *
 *  The number of entries that matched the wild card.
 *
 * DESCRIPTION
 *
 *  This function clears the mark for every entry in the list object
 *  that matches the wild card specification.
 *
 * REVISION HISTORY
 *
 *   May 26, 1994  1.0A  : First release
 */

inline int AL_PROTO ALEntryList::ClearMarks( const char AL_DLL_FAR *name )
{
    return SetMarkState( name, 0 );
}

#endif /* #if defined( __cplusplus ) */
#endif /* #ifdef _ARCENTRY_H         */

