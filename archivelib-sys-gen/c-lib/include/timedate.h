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


#ifndef _TIMEDATE_H
#define _TIMEDATE_H

#include <time.h>

/*
 * class ALOpenInputFile 
 *
 * DESCRIPTION
 *
 *  class ALTimeDate is used strictly to keep track of the time/date
 *  stamp of an ALStorage object.  The only place this class appears is
 *  as the mTimeDate member of ALStorage.  It has a number of conversion
 *  utilities for loading and exporting its values in various formats.
 *  Since most of the ways we have to set time stamps for objects
 *  are not ANSI standard, we end up with quite a few conversion utilities.
 *
 * DATA MEMBERS
 *
 *  miYear   : The year just like you would expect, e.g 1994.  A value of
 *             0 in this field indicates an invalid time.
 *
 *  miMonth  : The month, 1-12.
 *
 *  miDate   : The date, 1-31.
 *
 *  miHour   : In 2400 format, 0 - 23.
 *
 *  miMinute : 0 - 59
 *
 *  miSecond : 0 -59,
 *
 * MEMBER FUNCTIONS
 *
 *  ALTimeDate()        : The constructor, sets all members to 0.
 *  ~ALTimeDate()       : The destructor, has no work to do.
 *  operator new()      : The memory allocation operator, only used
 *                        when the library is inside a DLL.
 *  ToJulian()          : Convert the internal m/d/y members to
 *                        a julian day number.
 *  FromJulian()        : Convert a julian day number to internal
 *                        data members m/d/y.
 *  GetUnixTime()       : Convert all members to a long in unix format,
 *                        total seconds since 1/1/1970.
 *  GetDosTime()        : Convert h:m:s data members to the unsigned int
 *                        used in certain DOS commands.
 *  GetDosDate()        : Convert m/d/y data members to the unsigned int
 *                        used in certain DOS commands.
 *  SetTimeDate(long)   : Set internal data members from a UNIX long.
 *  SetTimeDate(struct tm*) : Set internal data members from a DOS
 *                            struct tm *.
 *  GetTimeDate()       : Convert internal data members to a DOS struct tm *.
 *  Valid()             : Indicate if a valid time has been set.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */


class AL_CLASS_TYPE ALTimeDate {
/*
 * Constructors, destructors, declarations, assignment operator
 */
    public :
        AL_PROTO ALTimeDate();
        AL_PROTO ~ALTimeDate();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 * I usually hide the copy constructor and assignment operators,
 * but in this case they are OK
 *
 *
 * Member functions
 */
    public :
        long AL_PROTO ToJulian();
        void AL_PROTO FromJulian( long jdn );
        long AL_PROTO GetUnixTime();
#if !defined( AL_WIN32S ) && !defined( AL_UNIX )
        unsigned short int AL_PROTO GetDosTime();
        unsigned short int AL_PROTO GetDosDate();
#endif /* #if !defined( AL_WIN32S ) ...*/
        void AL_PROTO SetTimeDate( long unix_time );
        void AL_PROTO SetTimeDate( struct tm AL_DLL_FAR *tblock );
        void AL_PROTO GetTimeDate( struct tm AL_DLL_FAR *tblock );
        int AL_PROTO Valid(){ return miYear != 0; }
/*
 * Data members
 */
    protected :
        short int miYear;       /* What you expect, e.g. 1995  */
        short int miMonth;      /* 1-12                        */
        short int miDate;       /* 1-31                        */
        short int miHour;       /* 0-23                        */
        short int miMinute;     /* 0-59                        */
        short int miSecond;     /* 0-59                        */
};

#endif
