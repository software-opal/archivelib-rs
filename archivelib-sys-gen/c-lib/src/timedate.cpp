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

#include <time.h>
#include "timedate.h"

//
// void * ALTimeDate::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  :  The number of bytes needed to create a new ALTimeDate object.
//
// RETURNS
//
//  A pointer to the newly allocated storage area, or 0 if no storage
//  was available.
//
// DESCRIPTION
//
//  When using a DLL, it is easy to get into a dangerous situation when
//  creating objects whose ctor and dtor are both in the DLL.  The problem
//  arises because when you create an object using new, the memory for
//  the object will be allocated from the EXE.  However, when you destroy
//  the object using delete, the memory is freed inside the DLL.  Since
//  the DLL doesn't really own that memory, bad things can happen.
//
//  But, you say, won't the space just go back to the Windows heap regardless
//  of who tries to free it?  Maybe, but maybe not.  If the DLL is using
//  a subsegment allocation scheme, it might do some sort of local free
//  before returning the space to the windows heap.  That is the point where
//  you could conceivably cook your heap.
//
//  By providing our own version of operator new inside this class, we
//  ensure that all memory allocation for the class will be done from
//  inside the DLL, not the EXE calling the DLL.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

#if defined( AL_BUILDING_DLL )
void AL_DLL_FAR * AL_PROTO ALTimeDate::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif

//
// ALTimeDate::ALTimeDate()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  None.
//
// DESCRIPTION
//
//  All the constructor does is initialize the data members.  By
//  setting the year to an invalid value of 0, we can always see
//  that the time date stamp for a file hasn't been initialized.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

AL_PROTO ALTimeDate::ALTimeDate()
{
    miYear = 0;  //This is an illegal year, means it is uninitialized
    miMonth = 0;
    miDate = 0;
    miHour = 0;
    miMinute = 0;
    miSecond = 0;
}

//
// ALTimeDate::~ALTimeDate()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  None.
//
// DESCRIPTION
//
//  The destructor has nothing to do.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

AL_PROTO ALTimeDate::~ALTimeDate()
{
}

//
// long ALTimeDate::ToJulian()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  A Julian day.
//
// DESCRIPTION
//
//   This function is used to make a Julian day number from a normal
//   month/day/year thing.  We need a Julian day in order to make a
//   UNIX style time stamp.  The UNIX time stamp is used to store
//   time stamps in Archive directories.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

long AL_PROTO ALTimeDate::ToJulian()
{
    return (long)( miDate - 32076)
          + 1461L * ( miYear + 4800L + ( miMonth - 14) / 12) / 4
          + 367 * ( miMonth - 2 - ( miMonth - 14) / 12 * 12) / 12
          - 3 * (( miYear + 4900L + ( miMonth - 14) / 12) / 100) / 4
          + 1;
}


//
// void ALTimeDate::FromJulian( long jdn )
//
// ARGUMENTS:
//
//  jdn  :  A julian date number, ideally one produced by ToJulian().
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//   This function is used to convert a julian date to a normal
//   year/month/day.  Time/date stamps are stored in Archives in
//   UNIX format.  This function is needed to convert a UNIX
//   time stamp to a normal mm/dd/yy.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

void AL_PROTO ALTimeDate::FromJulian( long jdn )
{
        long x;
        long z;
        long m;
        long d;
        long y;
        const long daysPer400Years = 146097L;
        const long fudgedDaysPer4000Years = 1460970L + 31;

        x = jdn + 68569L;
        z = 4 * x / daysPer400Years;
        x = x - (daysPer400Years * z + 3) / 4;
        y = 4000 * (x + 1) / fudgedDaysPer4000Years;
        x = x - 1461 * y / 4 + 31;
        m = 80 * x / 2447;
        d = x - 2447 * m / 80;
        x = m / 11;
        m = m + 2 - 12 * x;
        y = 100 * (z - 49) + y + x;
//
// I don't know whether or not we could eliminate these temporary longs
//
        miYear = (short int) y;
        miMonth = (short int) m;
        miDate = (short int) d;
}


//
// long ALTimeDate::GetUnixTime()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  A UNIX time, converted from the internal m/d/y h:m:s data.
//
// DESCRIPTION
//
//  This function is used to convert the m/d/y h:m:s time stamp for a file
//  into a UNIX time stamp.  The UNIX time stamp is a 32 bit long that
//  is used to store time stamps in an Archive.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

long AL_PROTO ALTimeDate::GetUnixTime()
{
    const long UnixFirstDay = 2440588L;
    long result;

    result = ToJulian();
    result -= UnixFirstDay;
    if ( result >= 0L ) {
        result *= 3600L * 24;
        result += 3600L * miHour;
        result += 60L * miMinute;
        result += miSecond;
    } else
        result = 0L;
    return result;
}

//
// void ALTimeDate::SetTimeDate( long unix_time )
//
// ARGUMENTS:
//
//  unix_time  :  A long integer in UNIX timestamp format.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This function is called when we are reading a directory in from
//  an archive.  It is used to set the internal data members of an
//  ALTimeDate object, after converting from unix time.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

void AL_PROTO ALTimeDate::SetTimeDate( long unix_time )
{
    const long UnixFirstDay = 2440588L;

    long jd = unix_time / ( 3600L * 24 );
    long hms = unix_time % ( 3600L * 24 );
    FromJulian( jd + UnixFirstDay );
    miHour = (short int) ( hms / 3600 );
    hms -= 3600L * miHour;
    miMinute = (short int) ( hms / 60 );
    miSecond = (short int) ( hms - ( miMinute * 60 ) );
}

//
// void ALTimeDate::SetTimeDate( struct tm *tblock )
//
// ARGUMENTS:
//
//  tblock :  A time date stamp as used by the C run time library.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  When working with DOS files, time stamps are read in to a structure
//  in the struct tm format.  This function provides an easy way to convert
//  the structure into our internal format.  When a DOS file is opened
//  using Open(), this function is called.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

void AL_PROTO ALTimeDate::SetTimeDate( struct tm AL_DLL_FAR *tblock )
{
    AL_ASSERT( tblock != 0, "SetTimeDate: passing illegal null parameter" );
    miYear = (short int) ( tblock->tm_year + 1900 );
    miMonth = (short int) ( tblock->tm_mon + 1 );
    miDate = (short int) tblock->tm_mday;
    miHour = (short int) tblock->tm_hour;
    miMinute = (short int) tblock->tm_min;
    miSecond = (short int) tblock->tm_sec;
}

//
// void ALTimeDate::GetTimeDate( struct tm *tblock )
//
// ARGUMENTS:
//
//  tblock  : A structure in the format used by the C runtime library for
//            storing time and date stamps.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  This function provides the reverse of SetTimeDate().  You would think
//  that we could just set the appropriate members of struct tm, but
//  there is a problem with that.  struct tm has one element that is
//  supposed to be the day of the week, and another that is supposed
//  to be the number of the day within the year.  We could try to
//  figure those out using the julian day function, but since gmtime()
//  will figure them out for us, we'll use that instead.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

void AL_PROTO ALTimeDate::GetTimeDate( struct tm AL_DLL_FAR *tblock )
{
    AL_ASSERT( tblock != 0, "GetTimeDate: passing illegal null parameter" );
    long unix_time = GetUnixTime();
    struct tm *result = gmtime( (const time_t *) &unix_time );
    if ( result ) {
        *tblock = *result;
        tblock->tm_isdst = 0;
    } else {    //This should never happen!
        tblock->tm_year = miYear - 1900;
        tblock->tm_mon = miMonth - 1;
        tblock->tm_mday = miDate;
        tblock->tm_hour = miHour;
        tblock->tm_min = miMinute;
        tblock->tm_sec = miSecond;
        tblock->tm_wday = 0;
        tblock->tm_yday = 0;
        tblock->tm_isdst = 0;
    }
}

//
// unsigned short int ALTimeDate::GetDosTime()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  This function returns the time stored in this.  The bits of the
//  time are packed into the form that is needed by the _dos_setftime()
//  format.
//
// DESCRIPTION
//
//  When we close a file that needs to have its time and date stamp
//  set, we normally use the _dos_setftime() function to do the
//  work.  It expects to see the time packed into a particular
//  sequence of bits in an unsigned short.  That is what this
//  function does.  It packs the bits just the way you want them.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A   : First release
//
//   August 10, 1994 1.0B : Had to make sure this got turned off when
//                          running under UNIX.  It wouldn't actually
//                          hurt anything, but it isn't particularly
//                          useful either.

#if !defined( AL_WIN32S ) && !defined( AL_UNIX )

unsigned short int AL_PROTO ALTimeDate::GetDosTime()
{
    int result;
    result = miSecond / 2;
    result |= miMinute << 5;
    result |= miHour << 11;
    return (unsigned short int) result;
}

#endif //#if !defined( AL_WIN32S ) ....


//
// unsigned short int ALTimeDate::GetDosDate()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  This function returns the date stored in this object.  The bits of the
//  date are packed into the form that is needed by the _dos_setftime()
//  format.
//
// DESCRIPTION
//
//  When we close a file that needs to have its time and date stamp
//  set, we normally use the _dos_setftime() function to do the
//  work.  It expects to see the date packed into a particular
//  sequence of bits in an unsigned short.  That is what this
//  function does.  It packs the bits just the way you want them.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   August 10, 1994 1.0B : Had to make sure this got turned off when
//                          running under UNIX.  It wouldn't actually
//                          hurt anything, but it isn't particularly
//                          useful either.

#if !defined( AL_WIN32S ) && !defined( AL_UNIX )

unsigned short int AL_PROTO ALTimeDate::GetDosDate()
{
    int result;
    result = miDate;
    result |= miMonth << 5;
    result |= (miYear-1980) << 9;
    return (unsigned short int ) result;
}
#endif //#if !defined( AL_WIN32S )

