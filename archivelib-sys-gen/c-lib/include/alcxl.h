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


#ifndef _ALCXL_H
#define _ALCXL_H

#include <time.h>
#include "aldefs.h"

/*
 * The deal with these bogus structures is simply a convoluted way
 * to provide some type checking when using these handles.  As far
 * as the C compiler can be concerned, they might as well all be
 * void*, but then we wouldn't have as much type safety.  This
 * method is a lot like that used with STRICT in <windows.h>
 * All we are doing is creating a purely arbitrary pointer type
 * that will be used to communicate with C++ functions in ArchiveLib.
 * As soon as we get inside ArchiveLib, we cast it to a more
 * useful type.
 */

/*
 * If using the portable C compiler, we have to basically skip this
 * entire file, and use the K&R replacement.  The replacement has
 * the K&R style of week function declaration.
 *
 */

#if defined( AL_KR )
#  include "alcxlkr.h"
#else
#  define DECLARE_AL_HANDLE( x )                           \
           struct x##_bogus_struct {                       \
               int x##_bogus_unused_member;                \
           };                                              \
           typedef struct x##_bogus_struct AL_DLL_FAR * x  

DECLARE_AL_HANDLE( hALArchive );
DECLARE_AL_HANDLE( hALMonitor );
DECLARE_AL_HANDLE( hALEntryList );
DECLARE_AL_HANDLE( hALEntry );
DECLARE_AL_HANDLE( hALStorage );
DECLARE_AL_HANDLE( hALExpander );
DECLARE_AL_HANDLE( hALEngine );
DECLARE_AL_HANDLE( hALCompressed );

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ALEngine functions
 */
void AL_FUNCTION deleteALEngine( hALEngine this_object );
int AL_FUNCTION ALEngineCompress( hALEngine this_object,
                                  hALStorage input_object,
                                  hALStorage output_object );
int AL_FUNCTION ALEngineDecompress( hALEngine this_object,
                                    hALStorage input_object,
                                    hALStorage output_object,
                                    long compressed_length );
int AL_FUNCTION ALEngineGetTypeCode( hALEngine this_object );
char AL_DLL_FAR * AL_FUNCTION ALEngineGetTypeString( hALEngine this_object );
int AL_FUNCTION ALEngineGetStatusCode( hALEngine this_object );
int AL_FUNCTION ALEngineSetError( hALEngine this_object, 
                                  int error_code, 
                                  char AL_DLL_FAR *text );
char AL_DLL_FAR * AL_FUNCTION ALEngineGetStatusString( hALEngine this_object );
char AL_DLL_FAR * AL_FUNCTION ALEngineGetStatusDetail( hALEngine this_object );

/*
 * ALEngine functions for Visual Basic only.
 */
#if defined( AL_VB )
long AL_FUNCTION ALEngineGetTypeStringVB( hALEngine this_object );
long AL_FUNCTION ALEngineGetStatusStringVB( hALEngine this_object );
long AL_FUNCTION ALEngineGetStatusDetailVB( hALEngine this_object );
#endif

/*
 * ALCopyEngine
 */
hALEngine AL_FUNCTION newALCopyEngine( void );

/*
 * ALGreenleafEngine
 */
hALEngine AL_FUNCTION newALGreenleafEngine( int level );
short int AL_FUNCTION ALGreenleafEngineGetLevel( hALEngine this_object );

/*
 * ALStorage functions
 */
void AL_FUNCTION deleteALStorage( hALStorage this_object );
int AL_FUNCTION ALStorageCompare( hALStorage this_object, 
                                  hALStorage test_object );
int AL_FUNCTION ALStorageIsOpen( hALStorage this_object );
int AL_FUNCTION ALStorageReadChar( hALStorage this_object );
int AL_FUNCTION ALStorageReadShort( hALStorage this_object, 
                                    short int AL_DLL_FAR *short_data );
int AL_FUNCTION ALStorageReadLong( hALStorage this_object, 
                                   long AL_DLL_FAR *long_data );
int AL_FUNCTION ALStorageWriteLong( hALStorage this_object, long long_data );
int AL_FUNCTION ALStorageWriteShort( hALStorage this_object, 
                                     short int short_data );
int AL_FUNCTION ALStorageWriteString( hALStorage this_object, 
                                      char AL_DLL_FAR *string_data );
long AL_FUNCTION ALStorageGetCrc32( hALStorage this_object );
long AL_FUNCTION ALStorageGetSize( hALStorage this_object );
long AL_FUNCTION ALStorageTell( hALStorage this_object );
size_t AL_FUNCTION ALStorageReadBuffer( hALStorage this_object,
                                        unsigned char AL_DLL_FAR *buffer,
                                        size_t length );
size_t AL_FUNCTION ALStorageWriteBuffer( hALStorage this_object, 
                                         unsigned char AL_DLL_FAR *buffer,
                                         size_t length );
void AL_FUNCTION ALStorageInitCrc32( hALStorage this_object, long seed );
int AL_FUNCTION ALStorageClose( hALStorage this_object );
int AL_FUNCTION ALStorageCreate( hALStorage this_object );
int AL_FUNCTION ALStorageDelete( hALStorage this_object );
int AL_FUNCTION ALStorageFlushBuffer( hALStorage this_object );
int AL_FUNCTION ALStorageLoadBuffer( hALStorage this_object, long address );
int AL_FUNCTION ALStorageOpen( hALStorage this_object );
int AL_FUNCTION ALStorageRename( hALStorage this_object, 
                                 char *new_name, 
                                 int delete_on_clash );
int AL_FUNCTION ALStorageRenameToBackup( hALStorage this_object, 
                                         int delete_on_clash );
int AL_FUNCTION ALStorageSeek( hALStorage this_object, long address );
int AL_FUNCTION ALStorageUnRename( hALStorage this_object, 
                                   int delete_on_clash );
int AL_FUNCTION ALStorageWriteChar( hALStorage this_object, int c );
int AL_FUNCTION ALStorageGetType( hALStorage this_object );
void AL_FUNCTION ALStorageSetMonitor( hALStorage this_object, 
                                      hALMonitor monitor );
long AL_FUNCTION ALStorageGetUnixTime( hALStorage this_object );
long AL_FUNCTION ALStorageToJulian( hALStorage this_object );
void AL_FUNCTION ALStorageFromJulian( hALStorage this_object, long jdn );
void AL_FUNCTION 
ALStorageSetTimeDateFromStruc( hALStorage this_object, 
                               struct tm AL_DLL_FAR * time_struct );
void AL_FUNCTION ALStorageSetTimeDateFromUnix( hALStorage this_object, 
                                               long unix_time );
void AL_FUNCTION 
ALStorageGetStrucFromTimeDate( hALStorage this_object, 
                               struct tm AL_DLL_FAR * time_struct );
unsigned short int AL_FUNCTION 
ALStoragePackedAttributes( hALStorage this_object );

#if defined( AL_UNIX )
void AL_FUNCTION
ALStorageSetFromUnixAttributes( hALStorage this_object, mode_t attributes );
mode_t AL_FUNCTION ALStorageGetUnixAttributes( hALStorage this_object );
#endif

#if defined( AL_WIN32 )
void AL_FUNCTION
ALStorageSetFromWin32Attributes( hALStorage this_object, DWORD attributes );
DWORD AL_FUNCTION ALStorageGetWin32Attributes( hALStorage this_object );
#endif

#if !defined( AL_UNIX ) && !defined( AL_WIN32S )
void AL_FUNCTION
ALStorageSetFromDosAttributes( hALStorage this_object,
                               unsigned short int dos_attributes );
unsigned short int AL_FUNCTION ALStorageGetDosAttributes( hALStorage this_object );
#endif

void AL_FUNCTION 
ALStorageSetFromPackedAtts( hALStorage this_object, 
                            unsigned short int packed_attributes );
char AL_DLL_FAR * AL_FUNCTION ALStorageGetName( hALStorage this_object );
void AL_FUNCTION ALStorageSetName( hALStorage this_object,
                                   char AL_DLL_FAR *object_name );
int AL_FUNCTION ALStorageWildCardMatch( hALStorage this_object, 
                                        char AL_DLL_FAR *pattern );
char AL_DLL_FAR * AL_FUNCTION 
ALStorageChangeExtension( hALStorage this_object,
                          char AL_DLL_FAR *new_extension );
char AL_DLL_FAR * AL_FUNCTION
ALStorageChangeTrailingChar( hALStorage this_object,
                             char new_char );
char AL_DLL_FAR * AL_FUNCTION ALStorageGetOldName( hALStorage this_object );
int AL_FUNCTION ALStorageGetStatusCode( hALStorage this_object );
int AL_FUNCTION ALStorageSetError( hALStorage this_object,
                                   int error_code, 
                                   char AL_DLL_FAR * text );
char AL_DLL_FAR * AL_FUNCTION 
ALStorageGetStatusString( hALStorage this_object );
char AL_DLL_FAR * AL_FUNCTION 
ALStorageGetStatusDetail( hALStorage this_object );

#if !defined( AL_UNIX )
unsigned short int AL_FUNCTION ALStorageGetDosTime( hALStorage this_object );
unsigned short int AL_FUNCTION ALStorageGetDosDate( hALStorage this_object );
#endif

int AL_FUNCTION ALStorageValidTimeDate( hALStorage this_object );

/*
 * ALStorage functions for Visual Basic only.
 */
#if defined( AL_VB )
long AL_FUNCTION
ALStorageChangeExtensionVB( hALStorage this_object, 
                            char AL_DLL_FAR *new_extension );
long AL_FUNCTION
ALStorageChangeTrailingCharVB( hALStorage this_object, 
                               char new_char );
long AL_FUNCTION ALStorageGetNameVB( hALStorage this_object );
long AL_FUNCTION ALStorageGetOldNameVB( hALStorage this_object );
long AL_FUNCTION ALStorageGetStatusStringVB( hALStorage this_object );
long AL_FUNCTION ALStorageGetStatusDetailVB( hALStorage this_object );
#endif

/*
 * ALFile functions
 */
hALStorage AL_FUNCTION newALFile( char AL_DLL_FAR *file_name );

/*
 * ALMemory Functions
 */
#ifdef AL_WINDOWS_MEMORY
hALStorage AL_FUNCTION newALMemory( char AL_DLL_FAR *buffer_name,
                                    char AL_HUGE *user_buffer,
                                    DWORD user_buffer_size );
UINT AL_FUNCTION ALMemoryGetHandle( hALStorage this_object );
long AL_FUNCTION ALMemoryGetBufferSize( hALStorage this_object );
char AL_HUGE *AL_FUNCTION ALMemoryGetBuffer( hALStorage this_object );
#else
hALStorage AL_FUNCTION newALMemory( char AL_DLL_FAR *buffer_name,
                                    char AL_DLL_FAR *user_buffer,
                                    int user_buffer_size );
size_t AL_FUNCTION ALMemoryGetBufferSize( hALStorage this_object );
char AL_DLL_FAR *AL_FUNCTION ALMemoryGetBuffer( hALStorage this_object );
#endif
int AL_FUNCTION ALMemoryGetBufferOwner( hALStorage this_object );
void AL_FUNCTION ALMemorySetBufferOwner( hALStorage this_object,
                                         int user_owns_buffer );
#if defined( AL_VB )
long AL_FUNCTION ALMemoryCopyBufferVB( hALStorage this_object );
#endif
/*
 * ALMonitor functions
 */
void AL_FUNCTION deleteALMonitor( hALMonitor this_object );
long AL_FUNCTION ALMonitorSetObjectSize( hALMonitor this_object,
                                         long object_size );
long AL_FUNCTION ALMonitorSetObjectStart( hALMonitor this_object,
                                          long object_start );
long AL_FUNCTION ALMonitorSetJobSize( hALMonitor this_object, long job_size );
long AL_FUNCTION ALMonitorSetJobSoFar( hALMonitor this_object,
                                       long job_so_far );

#if defined( AL_WINDOWS_GUI )
hALMonitor AL_FUNCTION 
newALWindowsMessage( enum ALMonitorType monitor_type,
                     HWND progress_text_window,
                     enum ALWindowsMessageType message_type,
                     HWND progress_number_window,
                     UINT windows_message );
#else
hALMonitor AL_FUNCTION newALBarGraph( enum ALMonitorType monitor_type );
hALMonitor AL_FUNCTION newALSpinner( enum ALMonitorType );
#endif

/*
 * ALEntry functions
 */

hALEntry AL_FUNCTION newALEntry( hALEntryList list,
                                 hALStorage storage, 
                                 hALEngine engine );
void AL_FUNCTION deleteALEntry( hALEntry this_object );
int AL_FUNCTION ALEntryDuplicate( hALEntry this_object, hALEntryList list );
int AL_FUNCTION ALEntryCompressionRatio( hALEntry this_object );
int AL_FUNCTION ALEntryGetMark( hALEntry this_object );
int AL_FUNCTION ALEntrySetComment( hALEntry this_object, 
                                   char AL_DLL_FAR *comment );
long AL_FUNCTION ALEntryGetCompressedSize( hALEntry this_object );
long AL_FUNCTION ALEntryGetCrc32( hALEntry this_object );
hALEntry AL_FUNCTION ALEntryGetNextEntry( hALEntry this_object );
char AL_DLL_FAR *AL_FUNCTION ALEntryGetComment( hALEntry this_object );
void AL_FUNCTION ALEntryClearMark( hALEntry this_object );
void AL_FUNCTION ALEntrySetMark( hALEntry this_object );
void AL_FUNCTION ALEntrySetMarkState( hALEntry this_object, 
                                      short int new_state );
hALStorage AL_FUNCTION ALEntryGetStorage( hALEntry this_object );
void AL_FUNCTION ALEntrySetStorage( hALEntry this_object, 
                                    hALStorage storage_object );
hALEngine AL_FUNCTION ALEntryGetEngine( hALEntry this_object );
void AL_FUNCTION ALEntrySetEngine( hALEntry this_object, hALEngine engine );

/*
 * ALEntry functions for Visual Basic only.
 */
#if defined( AL_VB )
long AL_FUNCTION ALEntryGetCommentVB( hALEntry this_object );
#endif

/*
 * ALEntryList functions
 */

hALEntryList AL_FUNCTION newALEntryList( hALMonitor monitor );
void AL_FUNCTION deleteALEntryList( hALEntryList this_object );
int AL_FUNCTION ALEntryListClearMarks( hALEntryList this_object, 
                                        char AL_DLL_FAR *pattern );
int AL_FUNCTION ALEntryListDeleteUnmarked( hALEntryList this_object );
int AL_FUNCTION ALEntryListSetMarks( hALEntryList this_object, 
                                     char AL_DLL_FAR *pattern );
int AL_FUNCTION ALEntryListToggleMarks( hALEntryList this_object );
void AL_FUNCTION ALEntryListUnmarkDuplicates( hALEntryList this_object, 
                                              hALEntryList list, char 
                                              AL_DLL_FAR *error_message );
hALEntry AL_FUNCTION ALEntryListGetFirstEntry( hALEntryList this_object );
int AL_FUNCTION ALEntryListGetStatusCode( hALEntryList this_object );
char AL_DLL_FAR * AL_FUNCTION 
ALEntryListGetStatusString( hALEntryList this_object );
char AL_DLL_FAR * AL_FUNCTION 
ALEntryListGetStatusDetail( hALEntryList this_object );
int AL_FUNCTION ALEntryListAddWildCardFiles( hALEntryList this_object, 
                                             char AL_DLL_FAR *pattern, 
                                             int traverse );
/*
 * ALEntry functions for Visual Basic only.
 */
#if defined( AL_VB )
long AL_FUNCTION ALEntryLisGetStatusStringVB( hALEntryList this_object );
long AL_FUNCTION ALEntryListGetStatusDetailVB( hALEntryList this_object );
#endif

#if defined( AL_WINDOWS_GUI )
int AL_FUNCTION ALEntryListAddFromDialog( hALEntryList this_object, 
                                          HWND hWnd, 
                                          int list_box_id );
int AL_FUNCTION ALEntryListAddFromWindow( hALEntryList this_object, 
                                          HWND hWnd );
int AL_FUNCTION ALEntryListSetMarksFromDialog( hALEntryList this_object, 
                                               HWND hWnd, 
                                               int id );
int AL_FUNCTION ALEntryListSetMarksFromWindow( hALEntryList this_object, 
                                                  HWND hWnd );
int AL_FUNCTION ALEntryListFillListBoxWindow( hALEntryList this_object, 
                                              HWND hWnd );
int AL_FUNCTION ALEntryListFillListBoxDialog( hALEntryList this_object, 
                                              HWND hDlg, 
                                              int list_box_id );
#endif

/*
 * ALArchiveBase Functions
 */
void AL_FUNCTION deleteALArchive( hALArchive this_object );
int AL_FUNCTION ALArchiveCreate( hALArchive this_object, hALEntryList list );
int AL_FUNCTION ALArchiveCreateFromArchive( hALArchive this_object,
                                            hALArchive source_archive,
                                            hALEntryList source_list );
int AL_FUNCTION ALArchiveAppend( hALArchive this_object, hALEntryList list );
int AL_FUNCTION ALArchiveAppendFromArchive( hALArchive this_object,
                                            hALArchive source_archive,
                                            hALEntryList source_list );
int AL_FUNCTION ALArchiveExtract( hALArchive this_object, hALEntryList list );
int AL_FUNCTION ALArchiveDelete( hALArchive this_object,
                                 hALEntryList list,
                                 hALArchive destination_archive );
char AL_DLL_FAR * AL_FUNCTION ALArchiveGetComment( hALArchive this_object );
int AL_FUNCTION ALArchiveSetComment( hALArchive this_object, 
                                     char AL_DLL_FAR *comment );
int AL_FUNCTION ALArchiveReadDirectory( hALArchive this_object, 
                                        hALEntryList list );
int AL_FUNCTION ALArchiveWriteDirectory( hALArchive this_object, 
                                         hALEntryList list );
int AL_FUNCTION ALArchiveGetVersion( hALArchive this_object );
hALStorage AL_FUNCTION ALArchiveGetStorage( hALArchive this_object );
int AL_FUNCTION ALArchiveGetStatusCode( hALArchive this_object );
int AL_FUNCTION ALArchiveSetError( hALArchive this_object, 
                                   int error_code, 
                                   char AL_DLL_FAR *text );
char AL_DLL_FAR * AL_FUNCTION 
ALArchiveGetStatusString( hALArchive this_object );
char AL_DLL_FAR * AL_FUNCTION 
ALArchiveGetStatusDetail( hALArchive this_object );

/*
 * ALArchiveBase functions for Visual Basic only.
 */

#if defined( AL_VB )
long AL_FUNCTION ALArchiveGetCommentVB( hALArchive this_object );
long AL_FUNCTION ALArchiveGetStatusStringVB( hALArchive this_object );
long AL_FUNCTION ALArchiveGetStatusDetailVB( hALArchive this_object );
#endif

#if defined( AL_WINDOWS_GUI )
int AL_FUNCTION ALArchiveFillListBoxDialog( hALArchive this_object, 
                                            HWND hDlg, 
                                            int list_box);
int AL_FUNCTION ALArchiveFillListBoxWindow( hALArchive this_object, 
                                            HWND hWnd );
#endif

/*
 * ALArchive functions
 */

hALArchive AL_FUNCTION newALArchive( char AL_DLL_FAR *file_name );
hALArchive AL_FUNCTION newALArchiveFromStorage( hALStorage object );
/*
 * Note: this function has been replaced by the more properly
 * named ALEntryListAddWildCardFiles.  Calls the same C++ member fn.
 */
int AL_FUNCTION ALArchiveAddFilesToList( hALArchive this_object, 
                                         hALEntryList list, 
                                         char AL_DLL_FAR * pattern, 
                                         int traverse_flag );

/*
 * ALCompressed functions
 */
 
hALCompressed AL_FUNCTION newALCompressed( hALStorage storage, 
                                           hALEngine engine );
void AL_FUNCTION deleteALCompressed( hALCompressed this_object );
int AL_FUNCTION ALCompressedExtract( hALCompressed this_object, 
                                     hALStorage output_object );
int AL_FUNCTION ALCompressedInsert( hALCompressed this_object, 
                                    hALStorage input_object );
int AL_FUNCTION ALCompressedGetStatusCode( hALCompressed this_object );
int AL_FUNCTION ALCompressedSetError( hALCompressed this_object, 
                                      int error_code, 
                                      char AL_DLL_FAR *text );
char AL_DLL_FAR * AL_FUNCTION 
ALCompressedGetStatusString( hALCompressed this_object );
char AL_DLL_FAR * AL_FUNCTION 
ALCompressedGetStatusDetail( hALCompressed this_object );

/*
 * ALCompressed functions for Visual Basic only.
 */
#if defined( AL_VB )
long AL_FUNCTION ALCompressedGetStatusStringVB( hALCompressed this_object );
long AL_FUNCTION ALCompressedGetStatusDetailVB( hALCompressed this_object );
#endif

/*
 * ALWildCardExpander functions
 */

hALExpander AL_FUNCTION newALExpander( char AL_DLL_FAR *file_list,
                                       int traverse_flag,
                                       enum ALCase name_case );
void AL_FUNCTION deleteALExpander( hALExpander this_object );
char AL_DLL_FAR * AL_FUNCTION ALExpanderGetNextFile( hALExpander this_object );

/*
 * ALWildCardExpander functions for Visual Basic only.
 */

#if defined( AL_VB )
long AL_FUNCTION ALExpanderGetNextFileVB( hALExpander this_object );
#endif

/*
 * Utility
 */

char AL_DLL_FAR * AL_FUNCTION StripFileName( char AL_DLL_FAR *file_name );
char AL_DLL_FAR * AL_FUNCTION StripPath( char AL_DLL_FAR *file_name );

/*
 * Utility functions for Visual Basic only
 */
#if defined( AL_VB )
long AL_FUNCTION StripFileNameVB( char AL_DLL_FAR *file_name );
long AL_FUNCTION StripPathVB( char AL_DLL_FAR *file_name );
#endif

#if defined( AL_WINDOWS_GUI )
void AL_CFUNCTION EditDisplay( HWND hDlg, int id, char AL_DLL_FAR *fmt, ... );
#endif

/*
 * Internal use for VB only
 */
#if defined( AL_VB )
extern "C" long _far _pascal ALCreateVBString( const char _far *string, unsigned short int length );
#endif

#if defined( AL_SUN4 ) && defined( AL_GCC )

int printf( char *fmt, ... );

#endif


#ifdef __cplusplus
}
#endif

#endif 
#endif /* #ifndef _ALCXL_H */
