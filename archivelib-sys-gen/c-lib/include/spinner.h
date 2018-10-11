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


#ifndef _SPINNER_H
#define _SPINNER_H

#include "arclib.h"

#if defined( __cplusplus )

/*
 * class ALSpinner : public ALMonitor
 *
 * DESCRIPTION
 *
 *  ALSpinner is a very simple monitor class, and is only useful under
 *  DOS, not Windows.  All ALSpinner does is spin a little propellor
 *  around while the file is being processed.  This lets you know
 *  that something is happening in an otherwise boring process.
 *
 * DATA MEMBERS
 *
 *  miSpinIndex     : A Static variable that keeps track of which
 *                    position the propellor should be in.  The
 *                    propellor travels through one of four different
 *                    positions.
 *
 *  mrStream        : A reference to the stream that the propellor is
 *                    going to be written on.
 *
 * MEMBER FUNCTIONS
 *
 *  ALSpinner()         : The one and only constructor.
 *  ~ALSpinner()        : The virtual destructor.
 *  Progress()          : The routine that gets called to make the
 *                        propellor twitch.
 *  ArchiveOperation()  : The routine that gets called when archiving
 *                        starts, stops, etc.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

class AL_CLASS_TYPE ALSpinner : public ALMonitor {
/*
 *  Constructors, destructors, and friend classes
 */
    public :
        AL_PROTO ALSpinner( ALMonitorType monitor_type,
                            std::ostream AL_DLL_FAR & stream = std::cout );
        virtual AL_PROTO ~ALSpinner();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 *  The copy constructor and assignment operator do not exist.
 */
    protected :
        AL_PROTO ALSpinner( const ALSpinner AL_DLL_FAR & );
        ALSpinner AL_DLL_FAR & AL_PROTO operator=( const ALSpinner AL_DLL_FAR & );
/*
 *  Member functions
 */
    protected :
        virtual void AL_PROTO Progress( long mlObjectSoFar,
                                        ALStorage AL_DLL_FAR & object );
        virtual void AL_PROTO ArchiveOperation( ALArchiveOperation operation,
                                                ALArchiveBase AL_DLL_FAR *archive,
                                                ALEntry AL_DLL_FAR *job );
/*
 * Data Members
 */
    protected :
        static int miSpinIndex;
        std::ostream AL_DLL_FAR & mrStream;
    public :
        AL_CLASS_TAG( _ALSpinnerTag );
};

#endif /* #if defined( __cplusplus ) */

#endif /* #ifdef SPINNER_H           */
