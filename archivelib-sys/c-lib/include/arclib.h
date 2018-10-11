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


#ifndef _ARCLIB_H
#define _ARCLIB_H

/*
 * Want to customize the workings of our library?  Just define AL_CUSTOM,
 * and then create your own personal version of ALCUSTOM.H. (No, we don't
 * ship a copy of this header file with the library, it is for you to
 * define.  This is a really good way to use products like MEMCHECK or
 * SmartHeap that want to insert an included file in every one of
 * our source files.
 */

#if defined( AL_CUSTOM )
#include "alcustom.h"
#endif

#if defined( __cplusplus )

/* All these includes needed for various library features */

#ifndef BINDGEN
#include <iostream>
#include "string.h"
#endif

#include "aldefs.h"
#include "_debug.h"

/* Base classes */

#include "status.h"
#include "objname.h"
#include "stor.h"
#include "cmpengn.h"
#include "monitor.h"
#include "arcentry.h"
#include "cmpobj.h"
#include "archiveb.h"

#if defined( AL_SUN4 ) && defined( AL_GCC )

extern "C" int strcasecmp( const char *s1, const char *s2 );

#endif

#else  /* #if defined( __cplusplus )                 */

#include "alcxl.h"  /* Included only if we are compiling C code, not C++ */

#endif /* #if defined( __cplusplus ) ... #else       */

#endif /* ARCLIB_H */
