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


#ifndef _ALCXLKR_H
#define _ALCXLKR_H

/*
 * These handle declarations are nice under ANSI C, but under K&R
 * they aren't so hot.  Forget any aggressive type checking, it just
 * isn't going to happen.
 *
 * The main problem is that the DECLARE_AL_HANDLE() macro under ANSI C
 * uses a token pasting macro to create a special pointer type for the
 * typedef for each handle.  When using ANSI C protottypes, this means
 * the compiler will check argument types for all ArchiveLib functions.
 * Under K&R, my token pasting macro doesn't work any more, and the trick
 * that people are supposed to be able to use for token pasting doesn't
 * seem to work either.  So all the handle types are the same in the eye
 * of the compiler.  But that's okay too, because K&R function prototypes
 * don't have argument type information anyway.
 */

#define DECLARE_AL_HANDLE( x ) typedef char * x

DECLARE_AL_HANDLE( hALArchive );
DECLARE_AL_HANDLE( hALMonitor );
DECLARE_AL_HANDLE( hALEntryList );
DECLARE_AL_HANDLE( hALEntry );
DECLARE_AL_HANDLE( hALStorage );
DECLARE_AL_HANDLE( hALExpander );
DECLARE_AL_HANDLE( hALEngine );
DECLARE_AL_HANDLE( hALCompressed );

/*
 * ALEngine functions
 */
void AL_FUNCTION deleteALEngine();
int AL_FUNCTION ALEngineCompress();
int AL_FUNCTION ALEngineDecompress();
int AL_FUNCTION ALEngineGetTypeCode();
char AL_DLL_FAR * AL_FUNCTION ALEngineGetTypeString();
int AL_FUNCTION ALEngineGetStatusCode();
int AL_FUNCTION ALEngineSetError();
char AL_DLL_FAR * AL_FUNCTION ALEngineGetStatusString();
char AL_DLL_FAR * AL_FUNCTION ALEngineGetStatusDetail();

/*
 * ALCopyEngine
 */
hALEngine AL_FUNCTION newALCopyEngine();

/*
 * ALGreenleafEngine
 */
hALEngine AL_FUNCTION newALGreenleafEngine();
short int AL_FUNCTION ALGreenleafEngineGetLevel();

/*
 * ALStorage functions
 */
void AL_FUNCTION deleteALStorage();
int AL_FUNCTION ALStorageCompare();
int AL_FUNCTION ALStorageIsOpen();
int AL_FUNCTION ALStorageReadChar();
int AL_FUNCTION ALStorageReadShort();
int AL_FUNCTION ALStorageReadLong();
int AL_FUNCTION ALStorageWriteLong();
int AL_FUNCTION ALStorageWriteShort();
int AL_FUNCTION ALStorageWriteString();
long AL_FUNCTION ALStorageGetCrc32();
long AL_FUNCTION ALStorageGetSize();
long AL_FUNCTION ALStorageTell();
size_t AL_FUNCTION ALStorageReadBuffer();
size_t AL_FUNCTION ALStorageWriteBuffer();
void AL_FUNCTION ALStorageInitCrc32();
int AL_FUNCTION ALStorageClose();
int AL_FUNCTION ALStorageCreate();
int AL_FUNCTION ALStorageDelete();
int AL_FUNCTION ALStorageFlushBuffer();
int AL_FUNCTION ALStorageLoadBuffer();
int AL_FUNCTION ALStorageOpen();
int AL_FUNCTION ALStorageRename();
int AL_FUNCTION ALStorageRenameToBackup();
int AL_FUNCTION ALStorageSeek();
int AL_FUNCTION ALStorageUnRename();
int AL_FUNCTION ALStorageWriteChar();
int AL_FUNCTION ALStorageGetType();
void AL_FUNCTION ALStorageSetMonitor();
long AL_FUNCTION ALStorageGetUnixTime();
long AL_FUNCTION ALStorageToJulian();
void AL_FUNCTION ALStorageFromJulian();
void AL_FUNCTION ALStorageSetTimeDateFromStruc();
void AL_FUNCTION ALStorageSetTimeDateFromUnix();
void AL_FUNCTION ALStorageGetStrucFromTimeDate();
unsigned short int AL_FUNCTION ALStoragePackedAttributes();
void AL_FUNCTION ALStorageSetFromUnixAttributes();
mode_t AL_FUNCTION ALStorageGetUnixAttributes();

void AL_FUNCTION ALStorageSetFromPackedAtts();
char AL_DLL_FAR * AL_FUNCTION ALStorageGetName();
void AL_FUNCTION ALStorageSetName();
int AL_FUNCTION ALStorageWildCardMatch();
char AL_DLL_FAR * AL_FUNCTION ALStorageChangeExtension();
char AL_DLL_FAR * AL_FUNCTION ALStorageChangeTrailingChar();
char AL_DLL_FAR * AL_FUNCTION ALStorageGetOldName();
int AL_FUNCTION ALStorageGetStatusCode();
int AL_FUNCTION ALStorageSetError();
char AL_DLL_FAR * AL_FUNCTION ALStorageGetStatusString();
char AL_DLL_FAR * AL_FUNCTION ALStorageGetStatusDetail();

int AL_FUNCTION ALStorageValidTimeDate();

/*
 * ALFile functions
 */
hALStorage AL_FUNCTION newALFile();

/*
 * ALMemory Functions
 */
hALStorage AL_FUNCTION newALMemory();
size_t AL_FUNCTION ALMemoryGetBufferSize();
char AL_DLL_FAR *AL_FUNCTION ALMemoryGetBuffer();
int AL_FUNCTION ALMemoryGetBufferOwner();
void AL_FUNCTION ALMemorySetBufferOwner();
/*
 * ALMonitor functions
 */
void AL_FUNCTION deleteALMonitor();
long AL_FUNCTION ALMonitorSetObjectSize();
long AL_FUNCTION ALMonitorSetObjectStart();
long AL_FUNCTION ALMonitorSetJobSize();
long AL_FUNCTION ALMonitorSetJobSoFar();

hALMonitor AL_FUNCTION newALBarGraph();
hALMonitor AL_FUNCTION newALSpinner();

/*
 * ALEntry functions
 */

hALEntry AL_FUNCTION newALEntry();
void AL_FUNCTION deleteALEntry();
int AL_FUNCTION ALEntryDuplicate();
int AL_FUNCTION ALEntryCompressionRatio();
int AL_FUNCTION ALEntryGetMark();
int AL_FUNCTION ALEntrySetComment();
long AL_FUNCTION ALEntryGetCompressedSize();
long AL_FUNCTION ALEntryGetCrc32();
hALEntry AL_FUNCTION ALEntryGetNextEntry();
char AL_DLL_FAR *AL_FUNCTION ALEntryGetComment();
void AL_FUNCTION ALEntryClearMark();
void AL_FUNCTION ALEntrySetMark();
void AL_FUNCTION ALEntrySetMarkState();
hALStorage AL_FUNCTION ALEntryGetStorage();
void AL_FUNCTION ALEntrySetStorage();
hALEngine AL_FUNCTION ALEntryGetEngine();
void AL_FUNCTION ALEntrySetEngine();

/*
 * ALEntryList functions
 */

hALEntryList AL_FUNCTION newALEntryList();
void AL_FUNCTION deleteALEntryList();
int AL_FUNCTION ALEntryListClearMarks();
int AL_FUNCTION ALEntryListDeleteUnmarked();
int AL_FUNCTION ALEntryListSetMarks();
int AL_FUNCTION ALEntryListToggleMarks();
void AL_FUNCTION ALEntryListUnmarkDuplicates();
hALEntry AL_FUNCTION ALEntryListGetFirstEntry();
int AL_FUNCTION ALEntryListGetStatusCode();
char AL_DLL_FAR * AL_FUNCTION ALEntryListGetStatusString();
char AL_DLL_FAR * AL_FUNCTION ALEntryListGetStatusDetail();
int AL_FUNCTION ALEntryListAddWildCardFiles();

/*
 * ALArchiveBase Functions
 */
void AL_FUNCTION deleteALArchive();
int AL_FUNCTION ALArchiveCreate();
int AL_FUNCTION ALArchiveCreateFromArchive();
int AL_FUNCTION ALArchiveAppend();
int AL_FUNCTION ALArchiveAppendFromArchive();
int AL_FUNCTION ALArchiveExtract();
int AL_FUNCTION ALArchiveDelete();
char AL_DLL_FAR * AL_FUNCTION ALArchiveGetComment();
int AL_FUNCTION ALArchiveSetComment();
int AL_FUNCTION ALArchiveReadDirectory();
int AL_FUNCTION ALArchiveWriteDirectory();
int AL_FUNCTION ALArchiveGetVersion();
hALStorage AL_FUNCTION ALArchiveGetStorage();
int AL_FUNCTION ALArchiveGetStatusCode();
int AL_FUNCTION ALArchiveSetError();
char AL_DLL_FAR * AL_FUNCTION ALArchiveGetStatusString();
char AL_DLL_FAR * AL_FUNCTION ALArchiveGetStatusDetail();

/*
 * ALArchive functions
 */

hALArchive AL_FUNCTION newALArchive();
hALArchive AL_FUNCTION newALArchiveFromStorage();
/*
 * Note: this function has been replaced by the more properly
 * named ALEntryListAddWildCardFiles.  Calls the same C++ member fn.
 */
int AL_FUNCTION ALArchiveAddFilesToList();

/*
 * ALCompressed functions
 */
 
hALCompressed AL_FUNCTION newALCompressed();
void AL_FUNCTION deleteALCompressed();
int AL_FUNCTION ALCompressedExtract();
int AL_FUNCTION ALCompressedInsert();
int AL_FUNCTION ALCompressedGetStatusCode();
int AL_FUNCTION ALCompressedSetError();
char AL_DLL_FAR * AL_FUNCTION ALCompressedGetStatusString();
char AL_DLL_FAR * AL_FUNCTION ALCompressedGetStatusDetail();

/*
 * ALWildCardExpander functions
 */

hALExpander AL_FUNCTION newALExpander();
void AL_FUNCTION deleteALExpander();
char AL_DLL_FAR * AL_FUNCTION ALExpanderGetNextFile();

/*
 * Utility
 */

char AL_DLL_FAR * AL_FUNCTION StripFileName();
char AL_DLL_FAR * AL_FUNCTION StripPath();

/*
 * Under SUN cc, I don't have a prototype for printf()!.
 *
 * The funny redeclaration of main() is necessary to integrate cc
 * with the C++ compiler, CC.  To use the C++ RTL, you *must* compile
 * main() with the C++ compiler.  So I have a special version of main()
 * in my C++ code, which does nothing except call kr_main().  The
 * macro below takes care of renaming your main() to kr_main().  Probably
 * kind of unuexpected, but I can't think of anyway out of it.  At least
 * you don't have to do anything funny in your source.
 */

#if defined( AL_SUN4 )

int printf();
#define main kr_main

#endif

#endif /* #ifndef _ALCXLKR_H */
