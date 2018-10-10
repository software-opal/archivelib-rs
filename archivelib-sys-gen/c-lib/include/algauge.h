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


#ifndef _ALGAUGE_H
#define _ALGAUGE_H

/*
 * These are all the different messages you can send to
 * an ALGauge object.  Our demo programs don't use any of these
 * except ALGaugeSetPosition, so most of these aren't even tested.
 */
typedef enum {
    ALGaugeSetRange       = WM_USER,
    ALGaugeGetRange,
    ALGaugeSetPosition,
    ALGaugeGetPosition,
    ALGaugeSetOrientation,
    ALGaugeGetOrientation,
    ALGaugeSetForegroundColor,
    ALGaugeGetForegroundColor,
    ALGaugeSetBackgroundColor,
    ALGaugeGetBackgroundColor,
    ALGaugeSetDeltaPosition,
} ALGaugeMessages;

/*
 * Send one of these with ALGaugeSetOrientation, to determined which
 * direction the flood fill progresses.
 */

typedef enum {
    ALGaugeOrientLeftToRight,
    ALGaugeOrientRightToLeft,
    ALGaugeOrientBottomToTop,
    ALGaugeOrientTopToBottom,
} ALGaugeOrientation;


/* 
 * public function prototypes 
 */

#ifdef __cplusplus
extern "C"
#endif
int AL_FUNCTION ALGaugeInit( HINSTANCE hInstance, HINSTANCE hPrevInstance );

#endif /* #ifdef _ALGAUGE_H */

