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


#ifndef _MEMSTORE_H
#define _MEMSTORE_H

#if defined( __cplusplus )

/*
 * class ALMemory : public ALStorage
 *
 * DESCRIPTION
 *
 *  Class ALMemory is an ALStorage class that stores its data in memory
 *  buffers. Under real mode MS-DOS, buffers are limited to 64Kbytes.
 *  Under Windows, they can get a lot bigger.  There are slight
 *  differences in operations between the the Windows and MS-DOS
 *  versions, so you will see some #ifdefs here and there.
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

class AL_CLASS_TYPE ALMemory : public ALStorage {
/*
 * Constructors, destructors, assignment operator, friends, declarations
 */

    public :
#ifdef AL_WINDOWS_MEMORY
        AL_PROTO ALMemory( const char AL_DLL_FAR *buffer_name = "",
                           char AL_HUGE *user_buffer = 0,
                           DWORD user_buffer_size = 0,
                           ALCase name_case = AL_MIXED );
#else
        AL_PROTO ALMemory( const char AL_DLL_FAR *buffer_name = "",
                           char AL_DLL_FAR *user_buffer = 0,
                           int user_buffer_size = 0,
                           ALCase name_case = AL_MIXED );
#endif
        virtual AL_PROTO ~ALMemory();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 * As usual, I don't want the compiler to generate a default copy constructor,
 * or an assignment operator here.  I force it to back off by declaring them
 * here.  They do not exist!
 */
    protected :
        AL_PROTO ALMemory( ALMemory AL_DLL_FAR & );
        ALMemory AL_DLL_FAR & AL_PROTO operator=( const ALMemory AL_DLL_FAR & );

/*
 * Member functions, grouped by category.
 *
 *
 * Protected member manipulation, used inside library, not for public use.
 */
    protected :
/*
 * The file I/O access public interface
 */
    public :
        virtual int AL_PROTO Open();
        virtual int AL_PROTO Create();
        virtual int AL_PROTO Close();
        virtual int AL_PROTO LoadBuffer( long address );
        virtual int AL_PROTO FlushBuffer();
        virtual int AL_PROTO Seek( long address );

/*
 * File name and underlying object manipulation public interface
 */
    public :
        virtual int AL_PROTO Rename( const char AL_DLL_FAR *new_name = 0,
                                     int delete_on_clash = 1 );
        virtual int AL_PROTO UnRename( int delete_on_clash = 1 );
        virtual int AL_PROTO Delete();
        virtual int AL_PROTO RenameToBackup( int delete_on_clash = 1 );
/*
 * Unique to this class
 */
    protected :
        int AL_PROTO GrowUserBuffer( long minimum_new_size );
/*
 * Data members
 */
    protected :
    public :   /* Should some of these might be better off private */
        int mfUserOwnsBuffer;
#ifdef AL_WINDOWS_MEMORY
        HGLOBAL mhUserMemoryHandle;
        long muUserBufferSize;
        char AL_HUGE *mpcUserBuffer;
#else
        size_t muUserBufferSize;
        char AL_DLL_FAR *mpcUserBuffer;
#endif
};

#endif /* #if defined( __cplusplus ) */

#endif /* #ifndef _MEMSTORE_H        */
