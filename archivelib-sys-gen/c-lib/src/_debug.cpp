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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

//
// void _ALAssertFailure( const char *condition,
//                        const char *filename,
//                        int line,
//                        const char *message,
//                        ... )
//
// ARGUMENTS:
//
//  condition   :  A character string containing the condition that failed,
//                 leading to the assertion.
//
//  filename    :  The name of the file where the assertion error took place.
//
//  line        :  The line in the file where the assertion error took place.
//
//  message     :  The error message associated with the assertion error.
//                 This message is a sprintf() style format string.
//
//  ...         :  Any additional arguments.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  The C run time library features an assert() macro, that can be used to
//  abort a program if a given condition isn't true.  It aborts the program
//  by calling a routine that looks something like this.  The AL_ASSERT()
//  macro that we use is even better, because it includes a comment
//  that gets displayed when the abort takes place.  This routine is
//  responsible for displaying that comment, along with the file name and
//  the line number, then aborting the program.  It is called by the
//  AL_ASSERT() macro when the conditional expression argument fails.
//
//  This routine is full of #ifdefs, and looks like a real mess.  This
//  is too bad, because it is really quite simple.  Basically it has to
//  quit with an abort() under MS-DOS, and a FatalAppExit() under
//  windows.  The error message is displayed on the console under MS-DOS,
//  (hope you're not in graphics mode!) and in a MessageBox under
//  Windows.  Man, it would be great to have just a little bit of control
//  of the formatting in the message box!
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

void AL_CFUNCTION _ALAssertFailure( const char AL_DLL_FAR *condition,
                                    const char AL_DLL_FAR *filename,
                                    int line,
                                    const char AL_DLL_FAR *message,
                                    ... )
{
    char buf1[ 256 ];
    char buf2[ 128 ];
    va_list argptr;

    va_start( argptr, message );
#if defined( AL_BUILDING_DLL ) && defined( AL_WINDOWS_GUI )
//
// Watcom is kind of annoying in that they format their variable arguments
// just a little differently than everyone else.
//
  #if defined( AL_WATCOM )
     wvsprintf( buf2, message, *argptr );
  #else
     wvsprintf( buf2, message, argptr );
  #endif
#else
     vsprintf( buf2, message, argptr );
#endif
     va_end( argptr );

#if defined( AL_BUILDING_DLL ) && defined( AL_WINDOWS_GUI )
     wsprintf
#else
     sprintf
#endif
     ( buf1,
       "Assertion error, ArchiveLib is aborting the application.\n"
       "Condition = %s\n"
       "File = %s, line = %d\n"
       "%s",
       condition,
       filename,
       line,
       buf2 );
#if defined( AL_WINDOWS_GUI )
#ifdef AL_BUILDING_DLL
     MessageBox( 0, buf1, "                      ArchiveLib (DLL) assertion error                      ", MB_ICONSTOP );
#else
     MessageBox( 0,
                 buf1,
                 "                      "
                 "ArchiveLib (static) assertion error"
                 "                      ",
                 MB_ICONSTOP );
#endif
     FatalAppExit( 0, "Application terminated" );
#else
     std::cerr << buf1 << "\n" << std::flush;
     abort();
#endif
}
