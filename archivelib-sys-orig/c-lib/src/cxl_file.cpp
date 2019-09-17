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

#include "al.h"
#include "alcxl.h"

//
// extern "C" hALStorage newALFile( char *file_name )
//
// ARGUMENTS:
//
//  file_name  :  The name of the file to construct.
//
// RETURNS
//
//  A handle for (pointer to) a newly constructed ALFile object.  It is
//  possible to get a 0 back if the file could not be constructed for
//  some reason.
//
// DESCRIPTION
//
//  This is the C and VB translation function that provides access to
//  ALFile::ALFile().  It calls the constructor, then casts the result
//  to a type C will be happy with and returns it.
//
// REVISION HISTORY
//
//   May 24, 1994  1.0A  : First release
//

extern "C" hALStorage AL_FUNCTION newALFile( char AL_DLL_FAR *file_name )
{
    return (hALStorage) new ALFile( file_name );
}


