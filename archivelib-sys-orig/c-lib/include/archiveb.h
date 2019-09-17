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


#ifndef _ARCHIVEB_H
#define _ARCHIVEB_H

#if defined( __cplusplus )

#include "arcentry.h"

/*
 * class ALArchiveBase
 *
 * DESCRIPTION
 *
 * This is the most important class definition in the library.
 * ALArchiveBase defines the interface to archive objects, on disk,
 * memory, or whatever.  This class is a pure base class, so you
 * won't ever create an instance of an ALArchiveBase object.
 *
 * The derived class for a given archive type has several very important
 * virtual functions it has to provide.  The most important two are
 * CreateCompressionEngine() and CreateStorageObject().  These
 * two functions are called when the directory is being read from
 * a given archive.  In the directory, the type of a storage object
 * and the compression engine used to create it are defined as just
 * plain integers.  Somewhere, somebody has to look at these integers
 * and create real objects based on their contents, and then maybe
 * perform some initialization as well.
 *
 * At first glance it might seem like there is no reason the base class
 * shouldn't just do all this work itself.  However, there is a good reason
 * not to do it this way.  If the base class is designed to be a completely
 * general purpose class, it would need to know how to create a Greenleaf
 * compression engine, a Microsoft compression engine, and so on.  It
 * would also need to know how to create all different kinds of storage
 * objects, probably including our demo storage objects.  This would mean
 * that the base ALArchiveBase class would basically have to link in our
 * entire library.  What is worse, if someone created a lean and mean
 * derived class that only supported one storage type and one compression
 * engine, they would still have to link in all that other code because
 * it was referenced in the base class.
 *
 * So the compromise is the current hierarchy.  We have a base class that
 * has no links to any engines or storage objects.  A user can derive
 * a lean and mean class from the base class and not have to worry as much
 * about code bloat.  On the other hand, we have a relatively full
 * featured derived class one level down, called ALArchive.  It
 * has the built in links to the "ordinary" object types.  This way, you
 * have a ready-built class that will do lots of ordinary jobs without a
 * lot of fuss.
 *
 * DATA MEMBERS
 *
 *  mpArchiveStorageObject   : Pointer to the Archive's storage object.
 *                             This is the ALFile, or whatever, that
 *                             actually contains (or will contain) the
 *                             archive.
 *
 *  mszComment               : The comment associated with this archive.
 *                             This comment is stored on disk along with
 *                             the archive directory.
 *
 *  mlDirectoryOffset        : The offset in the storage object to the
 *                             archive's directory.  All of the compressed
 *                             objects in the archive are stored sequentially
 *                             at the start of the file.  The directory
 *                             and other information starts after all of
 *                             the objects.  This data member tells us
 *                             how to get there.  Not filled in until
 *                             an archive has actually been created.
 *
 *  miVersion                : The archive version.  This version field
 *                             refers to the format version of the archive,
 *                             which won't necessarily be the save as
 *                             the format of ArchiveLib.
 *
 *  miDeleteStorageObject    : If this flag is set, the storage object
 *                             associated with the archive object will
 *                             have its object destroyed in the
 *                             archive destructor.  This saves you
 *                             from having to do the job explicitly
 *                             in your code.
 *
 *  mStatus                  : The only public data member, this is a
 *                             standard ArchiveLib status object.
 *
 *
 * MEMBER FUNCTIONS
 *
 *  ALArchiveBase()              : The only constructor.
 *  ~ALArchiveBase()             : The virtual destructor.
 *  operator new()               : Memory allocation operator used when
 *                                 the library is in a DLL
 *  AddJobs()                    : Protected function used to create
 *  CopyJobs()                   : Protected function used to create
 *  AddDirectoryEntries()        : Protected function used to create, append
 *  CalculateJobSize()           : Protected function to help monitor
 *  CalculateCompressedJobSize() : Protected function to help monitor
 *  ScanStatus()                 : Protected function to update status
 *  CreateCompressionEngine()    : Virtual fn used when extracting
 *  CreateStorageObject()        : Virtual fn used when extracting
 *  WriteArchiveData()           : Protected fn to write part of directory
 *  ReadArchiveData()            : Protected fn to read part of directory
 *  Create(ALEntryList&)         : Create a new archive
 *  Create(ALArchiveBase&,       : Create a new archive from another
 *         ALEntryList&)
 *  Append(ALEntryList&)         : Add objects to archive
 *  Append(ALArchiveBase&,       : Add objects to archive from another one
 *         ALEntryList&)
 *  Extract()                    : Extract objects from an archive
 *  Delete()                     : Delete objects from an archive
 *  GetComment()                 : Get the comment stored in the archive
 *  SetComment()                 : Set the comment that will be stored
 *  ReadDirectory()              : Read the directory into an ALEntryList
 *  WriteDirectory()             : Write the directory back out.
 *  GetVersion()                 : Get the archive's version
 *  GetStorageObject()           : Get pointer to the archive's storage object
 *  FillListBox()                : Fill a list box with the archive's entries
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

class AL_CLASS_TYPE ALArchiveBase {
/*
 * Constructors, destructors, declarations, and friends
 */
    protected :
    public :
        AL_PROTO ALArchiveBase( ALStorage AL_DLL_FAR *, short int delete_in_dtor );
        virtual AL_PROTO ~ALArchiveBase();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif

/* The copy constructor and assignment operator do not exist. */
    protected :
        AL_PROTO ALArchiveBase( const ALArchiveBase AL_DLL_FAR & );
        ALArchiveBase AL_DLL_FAR & AL_PROTO operator=( ALArchiveBase AL_DLL_FAR &rhs );

/*
 * Member functions
 *
 *
 * Used internally, not for public access
 */
    private :
        int AL_PROTO AddJobs( ALEntryList AL_DLL_FAR & list );
        int AL_PROTO CopyJobs( ALArchiveBase AL_DLL_FAR & source_archive,
                                 ALEntryList AL_DLL_FAR & list );
        int AL_PROTO AddDirectoryEntries( ALEntryList AL_DLL_FAR &list );
        long AL_PROTO CalculateJobSize( ALEntryList AL_DLL_FAR &list );
        long AL_PROTO CalculateCompressedJobSize( ALEntryList AL_DLL_FAR &list );
        void AL_PROTO ScanStatus( ALEntryList AL_DLL_FAR &list );
/*
 * This batch of functions are the ones that have to be created by a derived
 * class.  The first four are simple U/I functions that are normally used
 * to print stuff on the screen when starting or finishing a compression
 * or extraction.  The final two provide the very important function of
 * binding compression engines and object types to the archive.
 */
    protected :
        virtual ALCompressionEngine AL_DLL_FAR * AL_PROTO CreateCompressionEngine( int engine_type ) = 0;
        virtual ALStorage AL_DLL_FAR * AL_PROTO CreateStorageObject( const char AL_DLL_FAR *name, int object_type ) = 0;
        virtual int AL_PROTO WriteArchiveData();
        virtual int AL_PROTO ReadArchiveData();
/*
 * These functions provide the public API to an archive
 */
    public :
        int AL_PROTO Create( ALEntryList AL_DLL_FAR &list );
        int AL_PROTO Create( ALArchiveBase AL_DLL_FAR &source_archive,
                               ALEntryList AL_DLL_FAR &source_list );
        int AL_PROTO Append( ALEntryList AL_DLL_FAR &list );
        int AL_PROTO Append( ALArchiveBase AL_DLL_FAR &source_archive,
                                ALEntryList AL_DLL_FAR &source_list );
        int AL_PROTO Extract( ALEntryList AL_DLL_FAR &list );
        int AL_PROTO Delete( ALEntryList AL_DLL_FAR &list,
                               ALArchiveBase AL_DLL_FAR & destination_archive );
        const char AL_DLL_FAR * AL_PROTO GetComment(){ return mszComment; }
        int AL_PROTO SetComment( char AL_DLL_FAR *comment );
        int AL_PROTO ReadDirectory( ALEntryList AL_DLL_FAR &list );
        int AL_PROTO WriteDirectory( ALEntryList AL_DLL_FAR &list );
        short AL_PROTO GetVersion(){ return miVersion; }
        ALStorage AL_DLL_FAR *GetStorageObject(){ return mpArchiveStorageObject; }
#if defined( AL_WINDOWS_GUI )
        int AL_PROTO FillListBox( HWND hWnd, int list_box_id = -1 );
#endif
/*
 * Data members
 */
    protected :
        ALStorage AL_DLL_FAR *mpArchiveStorageObject;
        char AL_DLL_FAR *mszComment;
        long mlDirectoryOffset;
        short miVersion;
        const short int miDeleteStorageObject;

    public :
        ALStatus mStatus;
        AL_CLASS_TAG( _ALArchiveBaseTag );
};

#endif /* #if defined( __cplusplus ) */

#endif /* #ifndef _ARCHIVEB_H         */
