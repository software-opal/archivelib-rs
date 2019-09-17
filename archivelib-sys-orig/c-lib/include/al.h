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


#ifndef _AL_H
#define _AL_H

#include "arclib.h"

/*
 * Derived classes
 */

#include "filestor.h"
#include "memstore.h"
#include "copyengn.h"
#include "grenengn.h"
#include "archive.h"

/*
 * Demo classes and functions
 */

#if defined( AL_WINDOWS_GUI )
#  include "winmon.h"
#  include "algauge.h"
#  include "alstream.h"
#else
#  include "bargraph.h"
#  include "spinner.h"
#endif

/*
 * Additional classes
 */
#include "wildcard.h"

#endif /* #ifdef _AL_H */
