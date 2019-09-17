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


#ifndef _BARGRAPH_H
#define _BARGRAPH_H

#include "arclib.h"

#if defined( __cplusplus )

/*
 * class ALBarGraph : public ALMonitor
 *
 * DESCRIPTION
 *
 *  This is a utility class.  The constructor opens a file for input,
 *  and keeps track of whether it was already open or not.  The destructor
 *  will automatically close the file if it was closed when the
 *  ctor was invoked.
 *
 * DATA MEMBERS
 *
 *  miCurrentOffset   : The current offset of the bargraph, in screen
 *                      units.  Usually the bar itself is 20 characters
 *                      long, in which case this value will be somewhere
 *                      between 0 and 19.
 *
 *  miBarLength       : The length of the bar, defined when the
 *                      constructor is called.  This is a const member,
 *                      which means we can leave it public.
 *
 *  mrStream          : Reference to an output stream.  This is the
 *                      stream where the bargraph gets drawn.
 *
 * MEMBER FUNCTIONS
 *
 *  ALBarGraph()        : The constructor.
 *  ~ALBarGraph()       : Virtual destructor.
 *  Progress()          : The progress routine, where the bargraph
 *                        gets updated.
 *  ArchiveOperation()  : The routine that gets called when files
 *                        are opened, closed, etc.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

class AL_CLASS_TYPE ALBarGraph : public ALMonitor {
/*
 * Constructors, destructors, and friend classes
 */
     public :
          AL_PROTO ALBarGraph( ALMonitorType monitor_type,
                               std::ostream AL_DLL_FAR& stream = std::cout,
                               int bar_length = 25 );
          virtual AL_PROTO ~ALBarGraph();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 * The copy constructor and assignment operator do not exist.
 */
     protected :
          AL_PROTO ALBarGraph( const ALBarGraph AL_DLL_FAR & );
          ALBarGraph AL_DLL_FAR & AL_PROTO operator=( const ALBarGraph AL_DLL_FAR & );
/*
 * Member functions
 */
     protected :
          virtual void AL_PROTO Progress( long object_so_far,
                                          ALStorage AL_DLL_FAR & object );
          virtual void AL_PROTO
          ArchiveOperation( ALArchiveOperation operation,
                            ALArchiveBase AL_DLL_FAR *archive,
                            ALEntry AL_DLL_FAR *job );
/*
 * Data Members
 */
     protected :
          int miCurrentOffset;
          const int miBarLength;
          std::ostream AL_DLL_FAR & mrStream;
     public :
          AL_CLASS_TAG( _ALBarGraphTag );
};

#endif /* #if defined( __cplusplus ) */

#endif /* #ifdef _BARGRAP_H          */
