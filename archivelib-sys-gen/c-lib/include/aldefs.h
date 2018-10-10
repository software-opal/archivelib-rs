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

#ifndef _ALDEFS_H
#define _ALDEFS_H
/*
 *  The next long set of definitions and tests are all here simply to
 *  determine which compiler we are using, and what sort of target
 *  configuration we are trying to build/use.
 */
#if defined(__BORLANDC__) || defined(__TURBOC__)
#  if defined( __WIN32__ )
#    define AL_FLAT_MODEL
#    define AL_WINDOWS_MEMORY
#  endif
#  if !defined( __BORLANDC__ )
#      define AL_BORLAND __TURBOC__
#  else
#      define AL_BORLAND __BORLANDC__
#  endif
#  if sizeof( void * ) == 4
#    define AL_LARGE_DATA
#  endif
#  if defined( _Windows )
#    define AL_WINDOWS_MEMORY
#    ifdef __DLL__
#      define AL_BUILDING_DLL
#    endif
#    if !defined( __DPMI16__ ) && !defined( __CONSOLE__ )
#      define AL_WINDOWS_GUI
#    endif
#  endif
#elif defined( __SC__ )
#  define AL_SYMANTEC _MSC_VER
#  if defined( __NT__ )
#    define AL_FLAT_MODEL
#    define AL_WINDOWS_MEMORY
#    define AL_WINDOWS_GUI
#  endif
#  if defined( _M_I86HM ) || defined( _M_I86CM ) || defined( _M_I86LM )
#    define AL_LARGE_DATA
#  endif
/*
 * The _WINDOWS and _WINDLL macros are only documented in LIBRARY.TXT
 */
#  ifdef _WINDOWS
#    define AL_WINDOWS_MEMORY
#    define AL_WINDOWS_GUI
#    ifdef _WINDLL
#      define AL_BUILDING_DLL
#      ifndef M_I86LM
         #error All DLLs must be built using Large Model!
#      endif
#    endif
#  endif
#elif defined( _MSC_VER ) && defined( _MSDOS )
#  define AL_MICROSOFT _MSC_VER
#  if defined( _M_I86HM ) || defined( _M_I86CM ) || defined( _M_I86LM )
#    define AL_LARGE_DATA
#  endif
#  if ( AL_MICROSOFT >= 800 )
/*
 * I really don't want MSC to tell me when it is using a precompiled
 * header file.  What is really dumb is that I probably do want it
 * to tell me when it is creating one, but they generate the same
 * warning!
 */
#    pragma warning( disable : 4699 )
/*
 * This error occurs if you have inline functions in a header file and
 * they don't get used in a particular file.  Bogus.
 */
#    pragma warning( disable : 4505 )
/*
 * This warning occurs if you are using assert() macros with NDEBUG and /Ox
 * it is bogus
 */
#    pragma warning( disable : 4705 )
/*
 * This gives a warning for cout << setw( x )
 */
#    pragma warning( disable : 4270 )
/*
 * This is informational, it tells me when a function has been
 * chosen for inlining.
 */
#    pragma warning( disable : 4711 )
/*
 * This is informational, it tells me when a function has been
 * rejected for inlining.  The funny part is that it gives
 * me this message even if I don't select inlining for that
 * particular function???
 */
#    pragma warning( disable : 4710 )
#  else  /*#if ( AL_MICROSOFT >= 800 ) */
/*
 * Microsoft C 7.0 has a major linker problems if a symbol exceeds
 * 64 characters.  Unfortunately, with full decoration, we have
 * a couple of functions that hit that wall.  So I have to redefine
 * a couple of innocuous class names. I am trying to use similar
 * names so that if you hit them in the debugger you will be able
 * to understand what they mean.
 *
 *  #if defined( _WINDLL )
 *     #define ALWindowsMessage ALWinMsg_
 *      #define ALCompressionEngine ALEngine_
 *  #endif  NOTE: Fixing problem with /H64 in BUILD.INI! */
#  endif /*#if ( AL_MICROSOFT >= 800 ) ... #else */
#  ifdef _WINDOWS
#    define AL_WINDOWS_MEMORY
#    define AL_WINDOWS_GUI
#    ifdef _WINDLL
#      define AL_BUILDING_DLL
#      ifndef M_I86LM
         #error All DLLs must be built using Large Model!
#      endif
#    endif
#  endif
#elif defined( _MSC_VER ) && !defined( _MSDOS ) /* Looks like NT */
#  define AL_MICROSOFT _MSC_VER
#  define AL_FLAT_MODEL
#  define AL_LARGE_DATA
#  define AL_WIN32S
#  define AL_WINDOWS_MEMORY
/*
 * See the previous MSC standard section for the meanings of these pragmas.
 */
#  pragma warning( disable : 4699 )
#  pragma warning( disable : 4505 )
#  pragma warning( disable : 4705 )
#  pragma warning( disable : 4270 )
#  pragma warning( disable : 4711 )
#  pragma warning( disable : 4710 )
/*
 * We get this error in Microsoft's header files!
 */
# pragma warning( disable : 4201 )
#  ifdef _WINDOWS
#    define AL_WINDOWS_MEMORY
#    define AL_WINDOWS_GUI
#    ifdef _WINDLL
#      define AL_BUILDING_DLL
#    endif
#  endif
#elif defined( __WATCOMC__ ) && defined( __386__ )
#  define AL_FLAT_MODEL
#  ifdef __WINDOWS__
#    define AL_WINDOWS_MEMORY
#    define AL_WINDOWS_GUI
#  endif
#  if __WATCOMC__ > 950
#    pragma warning 690 9 /*  Warning for AL_ASSERT() at /w3 */
#    pragma warning 549 9 /*  Warning for sizeof() on class */
#  endif
#elif defined( __WATCOMC__ ) && !defined( __386__ )
#  define AL_WATCOM
#  ifdef __WINDOWS__
#    define AL_WINDOWS_MEMORY
#    define AL_WINDOWS_GUI
#    ifdef __SW_ZU
#      define AL_BUILDING_DLL
#      ifndef M_I86LM
         #error All DLLs must be built using Large Model!
#      endif
#    endif
#  endif
#  if defined( _M_I86HM ) || defined( _M_I86CM ) || defined( _M_I86LM )
#    define AL_LARGE_DATA
#  endif
#  pragma warning 549 9 /*  Warning for sizeof() on class */
#elif defined( __IBMC__ ) || defined( __IBMCPP__ )
#  define AL_IBM
#  define AL_OS2
#  define AL_FLAT_MODEL
#elif defined( AL_SUN4 )
#  define AL_UNIX
#  if !defined( AL_GCC ) && !defined( __cplusplus )
#    define AL_KR
#  endif
#else
  #error "Unknown compiler!"
#endif

#if defined( AL_WINDOWS_MEMORY ) || defined( AL_WINDOWS_GUI ) || defined( AL_WIN32S )
#  define STRICT
#  include <windows.h>
#  ifdef AL_FLAT_MODEL
#    define AL_HUGE
#  else
#    define AL_HUGE _huge
#  endif
#else
#  define WORD unsigned int
#  define DWORD unsigned long
#endif

/*
 * I modified some assumptions here.  I now assume that all 32 bit DOS
 * Extenders are going to use the Win32 memory API.
 */
#if ( defined( AL_WINDOWS_GUI ) || defined( AL_WINDOWS_MEMORY ) ) && defined( AL_FLAT_MODEL )
#  define AL_WIN32S
#endif

#if defined( AL_WINDOWS_GUI ) && defined( AL_WINDOWS_MEMORY ) && \
    !defined( AL_FLAT_MODEL ) && defined( AL_BUILDING_DLL )
#  define AL_VB
#endif

#if defined( AL_BUILDING_DLL )
#if defined( AL_FLAT_MODEL ) && defined( AL_MICROSOFT )
/*
 * This doesn't work in Release 1.01A.  Sorry, no 32 bit DLLs using
 * Microsoft.  The problem is that functions need to be declare with
 * this _declspec() guy, but the declaration goes in a different
 * place in the function signature than the old _export routine
 * went.  I can either modify *every* function, or I can create a
 * DEF file.  I will modify the functions in ArcLib 2.0.
 */
#    define AL_CLASS_TYPE     __declspec( dllexport )
#    define AL_PROTO
#    define AL_DLL_FAR
#    define AL_FUNCTION
#    define AL_CFUNCTION
#  elif defined( AL_FLAT_MODEL )
#    define AL_CLASS_TYPE     _export
#    define AL_PROTO          _export
#    define AL_DLL_FAR
#    define AL_FUNCTION       _export pascal
#    define AL_CFUNCTION      _export cdecl
#  elif defined( AL_WATCOM )
#    define AL_CLASS_TYPE
#    define AL_PROTO          _export
#    define AL_DLL_FAR
#    define AL_FUNCTION       _export _far pascal
#    define AL_CFUNCTION      _export _far cdecl
#  else
#    define AL_CLASS_TYPE     _export
#    define AL_PROTO          _far
#    define AL_DLL_FAR        _far
#    define AL_FUNCTION       _export _far pascal
#    define AL_CFUNCTION      _export _far cdecl
#  endif
#elif defined( AL_USING_DLL )
#  if defined( AL_FLAT_MODEL ) && defined( AL_BORLAND )
#    define AL_CLASS_TYPE     _import
#    define AL_PROTO          _import
#    define AL_DLL_FAR
#    define AL_FUNCTION       _import pascal
#    define AL_CFUNCTION      _import cdecl
#  elif defined( AL_FLAT_MODEL ) && defined( AL_MICROSOFT )
#    define AL_CLASS_TYPE
#    define AL_PROTO
#    define AL_DLL_FAR
#    define AL_FUNCTION
#    define AL_CFUNCTION
#  elif defined( AL_FLAT_MODEL ) && defined( AL_SYMANTEC )
#    define AL_CLASS_TYPE     _export
#    define AL_PROTO          _export
#    define AL_DLL_FAR
#    define AL_FUNCTION       _export pascal
#    define AL_CFUNCTION      _export cdecl
#  elif defined( AL_WATCOM )
#    define AL_PROTO
#    define AL_DLL_FAR
#    define AL_CLASS_TYPE
#    define AL_FUNCTION       _far _pascal
#    define AL_CFUNCTION      _far cdecl
#  else
#    define AL_PROTO          _far
#    define AL_DLL_FAR        _far
#    ifdef AL_BORLAND
#      if ( AL_BORLAND >= 0x450 )
#        define AL_CLASS_TYPE _import
#      else
#        define AL_CLASS_TYPE   _huge
#      endif
#    else
#      define AL_CLASS_TYPE   _export
#    endif
#    define AL_FUNCTION       _export _far _pascal
#    define AL_CFUNCTION      _export _far cdecl
#  endif
#else
#  define AL_PROTO
#  define AL_CLASS_TYPE
#  define AL_DLL_FAR
#  define AL_FUNCTION
#  define AL_CFUNCTION
#endif

#if defined( AL_WIN32S )
#define AL_EXPORT
#else
#define AL_EXPORT _export
#endif

#if defined( AL_MICROSOFT )
# define AL_UNUSED_PARAMETER( a ) ( a = a )
#else
# define AL_UNUSED_PARAMETER( a ) (void) a
#endif

/*
 * All objects of type ALMonitor are set up to monitor jobs or objects.
 * This enum is passed to the constructor to select which one is desired.
 */
enum ALMonitorType {
    AL_MONITOR_OBJECTS,
    AL_MONITOR_JOB
};

/*
 * An ALMonitor object can either send out byte counts or percentage
 * complete ratios.  When constructing the object, this enum indicates
 * which strategy is going to be used.
 */
enum ALWindowsMessageType {
    AL_SEND_BYTE_COUNT,
    AL_SEND_RATIO,
};

/*
 * Global enumerated error codes
 */
enum ALErrors {
        AL_CANT_OPEN_BUFFER = -1200,
        AL_CANT_ALLOCATE_MEMORY,
        AL_CANT_CREATE_ENGINE,
        AL_CANT_CREATE_STORAGE_OBJECT,
        AL_RENAME_ERROR,
        AL_CANT_OPEN_FILE,
        AL_SEEK_ERROR,
        AL_READ_ERROR,
        AL_WRITE_ERROR,
        AL_DELETE_ERROR,
        AL_ILLEGAL_PARAMETER,
        AL_INTERNAL_ERROR,
        AL_USER_ABORT,
        AL_SERVER_NOT_PRESENT,
        AL_COMPRESSION_TYPE_MISMATCH,
        AL_NEED_LENGTH,
        AL_CRC_ERROR,
        AL_COMPARE_ERROR,
        AL_UNKNOWN_COMPRESSION_TYPE,
        AL_UNKNOWN_STORAGE_OBJECT,
        AL_INVALID_ARCHIVE,
        AL_LOGIC_ERROR,
        AL_BACKUP_FAILURE,
        AL_GETSEL_ERROR,
        AL_DUPLICATE_ENTRY,
        AL_END_OF_FILE = -1,
        AL_SUCCESS = 0
};

/*
 * Enum used by ALName guys.  Names can either be forced to upper
 * or lower case, or support normal mixed case representations.  Objects
 * like MS-DOS file names should be forced to upper or lower, since the
 * O/S keeps the names as case insensitive.
 */

enum ALCase {
    AL_UPPER,
    AL_LOWER,
    AL_MIXED
};

/*
 * Compressing levels used by the Greenleaf Engine.  In the archive, this
 * ratio is stored in the engine private data.
 */
enum ALGreenleafCompressionLevels {
    AL_GREENLEAF_COPY    = -1,
    AL_GREENLEAF_LEVEL_0 = 0,
    AL_GREENLEAF_LEVEL_1,
    AL_GREENLEAF_LEVEL_2,
    AL_GREENLEAF_LEVEL_3,
    AL_GREENLEAF_LEVEL_4,
};

/*
 * Used when constructing ALWildCardExpander objects.  It decides whether
 * the expander will traverse the entire subdirectory tree, or just stay
 * on the current level.
 */

enum ALTraverseSetting {
    AL_TRAVERSE = 1,
    AL_DONT_TRAVERSE = 0,
};

/*
 * The type of storage object.  This is stored in the base class, ALStorage,
 * and is initialized in the constructor.
 */
enum ALStorageType {
    AL_UNDEFINED     = -1,
    AL_MEMORY_OBJECT = 0,
    AL_FILE_OBJECT   = 1 };

/*
 * The type of compression engine.  This is stored in the base class,
 * ALEngine, and is initialized in the constructor.
 */
enum ALCompressionType {
    AL_COMPRESSION_COPY = 0,
    AL_COMPRESSION_GREENLEAF = 1
};

#endif
