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


#ifndef _STATUS_H
#define _STATUS_H

#if defined( __cplusplus )

/*
 * class ALStatus
 *
 * DESCRIPTION
 *
 *  ALStatus is a utility class that is used as a data member of several
 *  other classes.  It provides an integer member that keeps track
 *  of the current status of the object, with AL_SUCCESS meaning
 *  everything is okay, and a code < 0 meaning the object has
 *  experienced an error.
 *
 *  The object also contains a character pointer to a detailed error
 *  message.  Usually when an error occurs in ArchiveLib, the
 *  routine that detects the error generates a detailed message that
 *  can be stored to provide additional information.
 *
 *  Error states in ArchiveLib are "sticky".  Once an object is flagged
 *  as being in error, it will stay that way until the programmer
 *  resets it, which means it will fail most ordinary operations.
 *  When you want to clear the error statue of an object, call
 *  SetError() with AL_SUCCESS as the error code.
 *
 * DATA MEMBERS
 *
 *  miStatus             : The current status of the object, with AL_SUCCESS
 *                         being a good value.  You can get at this easily
 *                         by casting ALStatus to type int.
 *
 *  miStatusDetailLength : This member keeps track of the length of the
 *                         status detail.  The status detail buffer
 *                         is dynamically allocated when it is needed.
 *                         Keeping this member is really kind of dumb, I
 *                         need to just allocate as much space as necessary
 *                         when the error detail is created.  Look for this
 *                         data member to go away in an upcoming release.
 *
 *  mszStatusDetail      : The detailed error message.  It will be set to
 *                         0 until a message is generated, when it
 *                         is dynamically allocated.  Cleaned up if necessary
 *                         in the destructor.

 *
 * MEMBER FUNCTIONS
 *
 *  ALStatus()          : The default and only constructor.
 *  ~ALStatus()         : The destructor, has to clean up detail string.
 *  operator new()      : Memory allocation operator, allocates space
 *                        for the class object when the library is in
 *                        the DLL.
 *  SetError()          : Sets the error code to a value, and writes
 *                        new data into the detail string.
 *  GetStatusCode()     : Returns the current integer status code,
 *                        just like operator int().
 *  GetStatusString()   : Returns the short string translation.
 *  GetStatusDetail()   : Returns the detailed status message, created
 *                        at the point the error took place.
 *  operator int()      : The casting operator, used all over ArchiveLib
 *                        when testing a status for a value < AL_SUCCESS.
 *  operator=()         : Assignment operator, easy here.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */


class AL_CLASS_TYPE ALStatus {
/*
 * Constructors, destructors, assignment operators, and declarations
 */
    public :
        AL_PROTO ALStatus();
        AL_PROTO ~ALStatus();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 * I don't want to allow the copy constructor, although I probably could
 * support it without too much trouble.
 */
    protected :
        AL_PROTO ALStatus( ALStatus AL_DLL_FAR & );
/*
 * Member functions
 */
    public :
        int AL_PROTO SetError( int error, const char AL_DLL_FAR *fmt, ... );
        int AL_PROTO GetStatusCode(){ return miStatus; }
        const char AL_DLL_FAR * AL_PROTO GetStatusString();
        const char AL_DLL_FAR * AL_PROTO GetStatusDetail() const;
        AL_PROTO operator int(){ return miStatus; }
        ALStatus AL_DLL_FAR & AL_PROTO operator = ( ALStatus AL_DLL_FAR & );
/*
 * Data members
 */
    protected :
        int miStatus;
        const int miStatusDetailLength;
        char AL_DLL_FAR *mszStatusDetail;

};

inline std::ostream& operator << ( std::ostream& stream, const ALStatus AL_DLL_FAR &status )
{
#if defined( AL_USING_DLL ) && !defined( AL_LARGE_MODEL ) && !defined( AL_FLAT_MODEL )
    const char _far *p = status.GetStatusDetail();
    char *near_string = new char[ _fstrlen( p ) + 1 ];
    if ( near_string ) {
        _fstrcpy( near_string, p );
        stream << near_string;
        delete near_string;
    } else
         stream << "Memory allocation failure!";
    return stream;
#else
    return stream << status.GetStatusDetail();
#endif
}

#endif /* #if defined( __cplusplus ) */

#endif /* #ifdef _STATUS_H           */
