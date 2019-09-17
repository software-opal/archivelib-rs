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

#if defined( AL_BUILDING_DLL )
#define STRICT
#include <windows.h>

//
// extern "C" int CALLBACK LibMain( HINSTANCE,
//                                  WORD,
//                                  WORD,
//                                  LPSTR )
//
//
// ARGUMENTS:
//
//  None of the default arguments are used.
//
// RETURNS
//
//  Always returns 1;
//
// DESCRIPTION
//
//  The only time we need to do anything in the startup of a DLL is under
//  Borland C++.  By default, Borland DLLs allocate memory in non-shared
//  mode, which means multiple processes can delete one-anothers memory
//  when it has been served up out of the subsegment allocator.  This
//  fixes that problem.
//
// REVISION HISTORY
//
//   May 20, 1994  1.0A  : First release
//


extern "C" int CALLBACK LibMain( HINSTANCE,
                                 WORD,
                                 WORD,
                                 LPSTR )
{
//
// I believe that both Microsoft and Symantec take care of this
// detail for me.  However, Borland 3.1 did not.  Not sure about
// Borland 4.0
//
#if defined( AL_BORLAND ) && !defined( AL_FLAT_MODEL )
extern unsigned _WinAllocFlag;
    _WinAllocFlag |= GMEM_SHARE;
#endif
    return 1;
}

//
// extern "C" int FAR PASCAL WEP(int)
//
//
// ARGUMENTS:
//
//  None of the arguments are used.
//
// RETURNS
//
//  Always returns 1
//
// DESCRIPTION
//
//  We have no use for a WEP, and I'm not sure if it is even being called.
//  WEP names are tricky.
//
// REVISION HISTORY
//
//   May 19, 1994  1.0A  : First release
//

#if defined( AL_BORLAND ) && ( AL_BORLAND >= 0x410 )
extern "C" int FAR PASCAL WEP(int)
#else
extern "C" int FAR PASCAL _WEP(int)
#endif
{
      /* Your WEP functionality goes here */
    return 1;
}

#endif //#if defined( AL_BUILDING_DLL )

