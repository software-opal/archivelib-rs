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


#ifndef _OBJNAME_H
#define _OBJNAME_H

#if defined( __cplusplus )

#include <string.h>
#include <iostream.h>

/*
 * class ALName 
 *
 * DESCRIPTION
 *
 * Object names are mostly used for names of storage objects.
 * There are enough things that I do with these guys to justify
 * a class of their own.  Having all the object name functions in their
 * own class also cuts back on the number of functions in ALStorage,
 * which is already cluttered.
 *
 * Besides serving as the mName member in ALStorage, this class is 
 * also pressed into service in ALWildCardExpander, where it is very
 * handy.
 *
 * DATA MEMBERS
 *
 *  mszName     : A pointer to the name associated with this object.
 *                This pointer can be a null pointer.  The object is
 *                responsible for deleting this guy in the ALName
 *                destructor, along with the next member, mszOldName.
 *
 *  mszOldName  : A pointer to the last name associated with this
 *                object.  When you assign a new name to one of these
 *                objects, the old name gets stored here.  This makes
 *                it easy to revert to the old name in case of trouble.
 *
 *  mCase       : One of AL_UPPER, AL_LOWER, or AL_MIXED.  If the value
 *                is AL_UPPER or AL_LOWER, the name is forced to all
 *                upper or lower case whenever it is assigned to the
 *                object.
 *
 * MEMBER FUNCTIONS
 *
 *  ALName(const ALName &)   : The copy constructor.
 *  ALName(const char *)     : Constructor that initializes with a char *.
 *  operator=(const ALName&) : Assignment operator.
 *  operator=(const char *)  : Assignment operator for char *.
 *  ~ALName()                : Destructor, has to clean up dynamic storage.
 *  operator new()           : Memory allocation operatory, only used
 *                             when the library is inside the DLL.  Be
 *                             aware that this operator allocates space for
 *                             the object itself, not the strings that it
 *                             will contain.
 *  Strcpy()                 : A protected member function, copies and
 *                             converts to the appropriate case if necessary.
 *  GetName()                : Returns a pointer to the name string, might 
 *                             be 0.
 *  GetOldName()             : Returns a pointer to the previous name
 *                             string, might be 0.
 *  GetSafeName()            : Returns a pointer to the name string, but
 *                             is guaranteed not to return 0.
 *  GetSafeOldName()         : Returns a pointer to the old name string, but
 *                             is guaranteed not to return 0.
 *  ChangeExtension()        : Change a filename extension to a new one.
 *  ChangeTrailingChar()     : Change the trailing character in the filename.
 *  StripFileName()          : Remove the filename, leaving the path and drive.
 *  StripPath()              : Remove path and drive, leaving the filename.
 *  WildCardMatch()          : Test for a match against a regular expression.
 *  operator const char *()  : Return a char *.
 *  operator+()              : Append a string to this string.
 *
 * REVISION HISTORY
 *
 *  May 26, 1994  1.0A  : First release
 *
 */

/*
 * Microsoft won't let me create a cast operator for char _far *.
 * But they will let me cast to this typedef.  Ugly, but it works.
 */

typedef char AL_DLL_FAR * STRINGF;

class AL_CLASS_TYPE ALName {
/*
 * Constructors, destructors, assignment operator, and friends
 */
    public :
        AL_PROTO ALName( const ALName AL_DLL_FAR & );
        AL_PROTO ALName( const char AL_DLL_FAR *s = "",
                         ALCase name_case = AL_MIXED );
        ALName AL_DLL_FAR & AL_PROTO operator = ( const ALName AL_DLL_FAR & rhs );
        ALName AL_DLL_FAR & AL_PROTO operator = ( const char AL_DLL_FAR * rhs );
        AL_PROTO ~ALName();
#if defined( AL_USING_DLL ) || defined( AL_BUILDING_DLL )
        void AL_DLL_FAR * AL_PROTO operator new( size_t size );
#endif
/*
 *  Note that I don't have the normal prohibition against a copy constructor
 *  or an assignment operator in this class, because I support them here.
 */

/*
 *  Member Functions
 */
    protected :
        void AL_PROTO Strcpy( const char AL_DLL_FAR *s );

    public :
        const char AL_DLL_FAR * AL_PROTO GetName() const { return mszName; }
        const char AL_DLL_FAR * AL_PROTO GetOldName() const { return mszOldName; }
        const char AL_DLL_FAR * AL_PROTO GetSafeName() const;
        const char AL_DLL_FAR * AL_PROTO GetSafeOldName() const;
        ALName AL_DLL_FAR & AL_PROTO 
        ChangeExtension( const char AL_DLL_FAR *new_extension = ".bak" );
        ALName AL_DLL_FAR & AL_PROTO ChangeTrailingChar( char new_char = '@' );
        ALName AL_DLL_FAR & AL_PROTO StripFileName();
        ALName AL_DLL_FAR & AL_PROTO StripPath();
        int AL_PROTO WildCardMatch( const char AL_DLL_FAR *pattern );
/*
 *  Operators
 */
    public :
#if defined( AL_MICROSOFT ) && ( AL_MICROSOFT < 800 ) && ( defined( AL_BUILDING_DLL ) || defined( AL_USING_DLL ) ) /*??? DON'T ASK ME WHY */
        AL_PROTO operator STRINGF() const;
#else
        AL_PROTO operator const STRINGF() const;
#endif
        ALName AL_PROTO operator + ( const char AL_DLL_FAR *rhs );
/*
 * Data members
 */
    protected :
        char AL_DLL_FAR * mszName;
        char AL_DLL_FAR * mszOldName;
    public :
        const ALCase mCase;
        AL_CLASS_TAG( _ALNameTag );
};

/*
 * ostream& operator << ( ostream& stream, const ALName  &object )
 *
 * ARGUMENTS:
 *
 *  stream  : An I/O stream.
 *
 *  object  : A reference to an ALName object.
 *
 * RETURNS
 *
 *  A reference to the stream provided as an operator.
 *
 * DESCRIPTION
 *
 * This stream operator makes it easy to send ALName objects
 * to an output stream.  I need to define this function as inline,
 * because it is tough to use far references to ostreams from a DLL.  
 * There are other problems associated with using this function
 * in a DLL, and I don't understand them all.  
 *
 * REVISION HISTORY
 *
 *   May 26, 1994  1.0A  : First release
 *
 */

inline ostream& operator << ( ostream& stream, const ALName AL_DLL_FAR &object )
{
#if defined( AL_USING_DLL ) && !defined( AL_LARGE_MODEL ) && !defined( AL_FLAT_MODEL )
    const char _far *p = (STRINGF) object;
    char *near_string = new char[ _fstrlen( p ) + 1 ];
    if ( near_string ) {
        _fstrcpy( near_string, p );
        stream << near_string;
        delete near_string;
    } else
         stream << "Memory allocation failure!";
#else
    stream << (STRINGF) object;
#endif
    return stream;
}

#endif /* #if defined( __cplusplus ) */

#endif /* #ifndef _OBJNAME_H         */

