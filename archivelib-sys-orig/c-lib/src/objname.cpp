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

#include <string.h>
#include <ctype.h>

#include "_match.h"

//
//  void strupr( char *s )
//
// ARGUMENTS:
//
//  A pointer to a string.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  I was kind of surprised to see that SUN UNIX doesn't have either strlwr()
//  or strupr() in their library.  So I had to whip up this quick and dirty
//  replacement routine.
//
// REVISION HISTORY
//
//   July 7, 1994  1.0B  : First release
//

#ifdef AL_UNIX

static void strupr( char *s )
{
    while ( *s ){
        *s = toupper( *s );
        s++;
    }

}

#endif

//
//  void strlwr( char *s )
//
// ARGUMENTS:
//
//  A pointer to a string.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  I was kind of surprised to see that SUN UNIX doesn't have either strlwr()
//  or strupr() in their library.  So I had to whip up this quick and dirty
//  replacement routine.
//
// REVISION HISTORY
//
//   July 7, 1994  1.0B  : First release
//


#ifdef AL_UNIX

static void strlwr( char *s )
{
    while ( *s ) {
        *s = tolower( *s );
        s++;
    }
}

#endif

//
// void * ALName::operator new( size_t size )
//
// ARGUMENTS:
//
//  size  :  The number of bytes needed to create a new ALName object.
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
void AL_DLL_FAR * AL_PROTO ALName::operator new( size_t size )
{
    return ::new char[ size ];
}
#endif

//
// ALName ALName::operator+( const char *rhs )
//
// ARGUMENTS:
//
//  rhs  : The character pointer that is going to be added to the
//         ALName object.  This will often be the string component
//         of another ALName object, cast to type const char *.
//
// RETURNS
//
//  A newly created ALName object.  This disappears quickly, but can
//  be copied into a result object using either the assignment operator
//  or the copy constructor.
//
// DESCRIPTION
//
//  This is one of those C++ functions that makes converts out of C
//  programmers.  It allows me to add two strings together to create
//  a third.  I really like that.
//
//  The implementation is pretty easy.  I allocate a new character buffer of
//  the correct length, the copy the two strings into it.  I use this
//  result as the initializer for a new ALName object, and return that.
//
//  Note that the new string will have the same case sensitivity as this.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

ALName ALName::operator+( const char AL_DLL_FAR *rhs )
{
    int l1 = ( rhs ) ? strlen( rhs ) : 0;
    int l2 = ( mszName ) ? strlen( mszName ) : 0;
    char *p = new char[ l1 + l2 + 1 ];
    if ( p ) {
        strcpy( p, mszName );
        if ( rhs )
            strcat( p, rhs );
    }
    ALName result( p, mCase );
    if ( p )
        delete[] p;
    return result;
}


//
// ALName::ALName( const char *s = "", ALCase name_case = AL_MIXED )
//
// ARGUMENTS:
//
//  s        : The initial string value of the new object.  Note that
//             you can pass a null pointer here and all will still be okay.
//
//  name_case: The case sensitivity of the new ALName object.  Will its
//             contents be mixed, or will it always be forced to upper
//             or lower.
//
// RETURNS
//
//  Nothing, a constructor.
//
// DESCRIPTION
//
//  This constructor first initializes the mCase member in an initializer.
//  mCase is a const member, which is nice, because you can make it public.
//  But, it means you can't initialize it *in* the constructor, you have
//  to do it before the body.
//
//  Things are pretty easy after that.  We allocate enough space to hold
//  the initializer string and copy it in.  The old name gets set to 0, since
//  this name hasn't been around long enough to have been renamed.
//  And that's it.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

AL_PROTO ALName::ALName( const char AL_DLL_FAR *s /* = "" */,
                         ALCase name_case /* = AL_MIXED */ )
     : mCase( name_case )
{
    mszName = new AL_DLL_FAR char[ strlen( s ) + 1 ];
    if ( mszName )
        Strcpy( s );
     mszOldName = 0;
}

//
// ALName::ALName( const ALName &rhs )
//
// ARGUMENTS:
//
//  rhs   :  A reference to another ALName object.  This is the ALName
//           we are going to copy.
//
// RETURNS
//
//  Nothing, a constructor.
//
// DESCRIPTION
//
//  This is the copy constructor.  It is very nearly the same as the
//  other constructor.
//
//  This constructor first initializes the mCase member in an initializer.
//  mCase is a const member, which is nice, because you can make it public.
//  But, it means you can't initialize it *in* the constructor, you have
//  to do it before the body.
//
//  Things are pretty easy after that.  We allocate enough space to hold
//  a copy of the string in the rhs, and then copy it.  The old name gets
//  set to 0, since this name hasn't been around long enough to have been
//  renamed.  We could have copied the old name from the rhs, but I think
//  this way makes more sense.
//  And that's it.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

AL_PROTO ALName::ALName( const ALName AL_DLL_FAR &rhs )
     : mCase( rhs.mCase )
{
     const char *s = rhs.GetSafeName();
     mszName = new AL_DLL_FAR char[ strlen( s ) + 1 ];
     if ( mszName )
          Strcpy( s );
     mszOldName = 0;
}

//
// ALName & ALName::operator = ( const char * rhs )
//
// ARGUMENTS:
//
//  rhs  :  This is the character string that we are going to assign to this.
//
// RETURNS
//
//  A reference to this.  We need to do it that way so we can do:
//
//    a = b = c = "test";
//
// DESCRIPTION
//
//  This function performs roughly the same function as the first
//  constructor, but there is a twist.  When we assign a new name to
//  an ALName object, we also make a copy of fit and place it in
//  the old name data member.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

ALName AL_DLL_FAR & AL_PROTO ALName::
operator = ( const char AL_DLL_FAR * rhs )
{
    if ( rhs == 0 )
        rhs = "";
    if ( rhs == mszName ) // Pathological?
        return *this;

    char AL_DLL_FAR *new_name = new AL_DLL_FAR char[ strlen( rhs ) + 1 ];
    if ( new_name ) {
        if ( mszOldName )
            delete[] mszOldName;
        mszOldName = mszName;
        mszName = new_name;
        Strcpy( rhs );
    } else {
        if ( mszOldName )
            delete[] mszOldName;
        mszOldName = mszName;
        mszName = 0;
    }
    return *this;
}

//
// ALName & ALName::operator = ( const ALName & rhs )
//
// ARGUMENTS:
//
//  rhs   :  The right hand side of the assignment operator.
//
// RETURNS
//
//  A reference to this.  Like in the above function, we do this so
//  that you can stack assignments:
//
//      a = b = c = "Dummy";
//
// DESCRIPTION
//
//
//  This function is very similar to the copy constructor, but it has
//  one additional twist.  When we copy the rhs string value into our
//  string, we move our old name into the backup copy member,
//  mszOldName.  That way, if we change our mind, we can easily
//  switch back to the old name.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
ALName AL_DLL_FAR & AL_PROTO ALName::
operator = ( const ALName AL_DLL_FAR & rhs )
{
    return *this = rhs.GetName();
}

//
// ALName::~ALName()
//
// ARGUMENTS:
//
//  None, this is a destructor.
//
// RETURNS
//
//  None, destructor.
//
// DESCRIPTION
//
//  All this function has to do is free up the two pieces of
//  dynamic memory.  I just can't get out of that old C habit of
//  checking a pointer for NULL before deleting it.
//
//  In debug mode, I check the state of this before and after freeing
//  the dynamically allocated memory, in hopes of catching any
//  heap errors near their source.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

AL_PROTO ALName::~ALName()
{
    AL_ASSERT( GoodTag(), "~ALName: attempt to delete invalid object" );
    if ( mszName )
        delete[] mszName;
    if ( mszOldName )
        delete[] mszOldName;
    AL_ASSERT( GoodTag(), "~ALName: attempt to delete invalid object" );
}

//
// ALName & ALName::ChangeExtension( const char *new_extension = ".bak" )
//
// ARGUMENTS:
//
//  new_extension  :  The new extension you want to apply to the name.
//
// RETURNS
//
//  A reference to this.  This is nice, because it lets you do things
//  like this:
//
//     fopen( name.ChangeExtension( ".OBJ" ), "rb" );
//
// DESCRIPTION
//
//  This function is used to change the extension of a filename stored
//  in an ALName object.  A lot of times you will want to do this
//  in order to create a backup.  For example, you could change
//  TEMP.DAT to TEMP.BAK.
//
//  This function makes a copy of the current name in mszOldName, so we
//  can keep track of it later.  It then searches for the '.' character
//  in the new file name, and sticks the new extension there.
//
//  If you apply this function to a filename, you can then rename the file
//  by calling ALStorage::Rename() with no argument.  When you do this,
//  the rename function uses the saved OldName and current name as its
//  arguments for the rename() function.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : Had to add a default to the switch() statement,
//                         gcc -Wall was giving me a warning.

//
// I need to fix this code up to handle extensions with and without a
// leading '.' character.  Maybe you can use a leading character
// other than '.' to indicate a different type of extension?
//

ALName AL_DLL_FAR & AL_PROTO ALName::
ChangeExtension( const char AL_DLL_FAR *new_extension /* = ".bak" */ )
{
    AL_ASSERT( new_extension != 0, "ChangeExtension: new extension is null" );
    AL_ASSERT( mszName, "ChangeExtension: current name is null" );

    char *file_name = new char[ strlen( mszName ) + strlen( new_extension ) + 1 ];
    if ( mszOldName )
        delete[] mszOldName;
    mszOldName = mszName;
    mszName = file_name;

    if ( !file_name )
        return *this;
    strcpy( mszName, mszOldName );
    char *p = strrchr( file_name, '.' );
    if ( p )
        strcpy( p, new_extension );
    else
        strcat( mszName, new_extension );
    switch ( mCase ) {
        case AL_UPPER : strupr( mszName ); break;
        case AL_LOWER : strlwr( mszName ); break;
        default       : break;
    }
    return *this;
}

//
// ALName & ALName::ChangeTrailingChar( char new_char = '@' )
//
// ARGUMENTS:
//
//  new_char  :  The new character to use as the last name of the file
//               name.
//
// RETURNS
//
//  A reference to this.  This is nice, because it lets you do things
//  like this:
//
//     fopen( name.ChangeTrailingChar(), "rb" );
//
// DESCRIPTION
//
//  This function is used to change the the last character of the extension
//  in filename stored in an ALName object.  A lot of times you will want to
//  do this in order to create a backup.  For example, you could change
//  TEMP.DAT to TEMP.DA@.
//
//  This function makes a copy of the current name in mszOldName, so we
//  can keep track of it later.  It then searches for the end of the
//  current file name, and changes it.
//
//  Note that if the filename doesn't have an extension, we do something
//  funny.  Instead of just changing the last character, we create a
//  new extension, and append that instead.  So if the filename is
//  "TEST", the new name will be "TEST.@".
//
//  If you apply this function to a filename, you can then rename the file
//  by calling ALStorage::Rename() with no argument.  When you do this,
//  the rename function uses the saved OldName and current name as its
//  arguments for the rename() function.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : Had to add a default to the switch() statement,
//                         gcc -Wall was giving me a warning.

ALName AL_DLL_FAR & AL_PROTO ALName::
ChangeTrailingChar( char new_char /* = '@' */ )
{
    AL_ASSERT( mszName != 0, "ChangeTrailingChar: current name is null" );

    char *file_name = new char[ strlen( mszName ) + 2 ];
    if ( mszOldName )
        delete[] mszOldName;
    mszOldName = mszName;
    mszName = file_name;
    switch ( mCase ) {
        case AL_UPPER : new_char = (char) toupper( new_char ); break;
        case AL_LOWER : new_char = (char) tolower( new_char ); break;
        default       : break;
    }
    if ( !file_name )
        return *this;
    strcpy( mszName, mszOldName );
    char *p;
    if ( ( p = strrchr( mszName, '.' ) ) != 0 ) {
        if ( p[ 1 ] == '0' ) {
            p[ 1 ] = new_char;
            p[ 2 ] = '\0';
        } else
            mszName[ strlen( mszName ) - 1 ] = new_char;
    } else {
        char new_extension[ 3 ];
        new_extension[ 0 ] ='.';
        new_extension[ 1 ] = new_char;
        new_extension[ 2 ] = 0;
        strcat( mszName, new_extension );
    }
    return *this;
}

//
// const char * ALName::GetSafeName() const
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  This function normally returns the value of mszName.  However, if
//  mszName is currently a null pointer, we return a pointer to an
//  empty string instead.  This means you can use the return value from
//  this function anywhere you want without checking for its NULLity.
//
// DESCRIPTION
//
//  See above.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

const char AL_DLL_FAR * AL_PROTO ALName::GetSafeName() const
{
    if ( mszName )
        return mszName;
    else
        return "";
}

//
// const char * ALName::GetSafeOldName() const
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  This function normally returns the value of mszOldName.  However, if
//  mszOldName is currently a null pointer, we return a pointer to an
//  empty string instead.  This means you can use the return value from
//  this function anywhere you want without checking for its NULLity.
//
// DESCRIPTION
//
//  See above.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

const char AL_DLL_FAR * AL_PROTO ALName::GetSafeOldName() const
{
    if ( mszOldName )
        return mszOldName;
    else
        return "";
}

//
// ALName::operator const char *() const
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  A pointer to the string inside the object.  If the string is
//  presently a null pointer, we return a pointer to an empty string
//  instead.  See ALName::GetSafeName() for an explanation of why
//  this is.
//
// DESCRIPTION
//
//  This is the casting operator that pretty much lets me use
//  an ALName object anywhere I use a char *.  There are some strange
//  Microsoft compiler problems that make me use this goofy STRINGF
//  typedef instead of char *, but it all adds up the same in
//  the wash.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//

#if defined( AL_MICROSOFT ) && ( AL_MICROSOFT < 800 ) && defined( AL_BUILDING_DLL ) //??????
AL_PROTO ALName::operator char *()  const
#else
AL_PROTO ALName::operator const STRINGF() const
#endif
{
    if ( mszName )
        return mszName;
    else
        return (char*)"";
}

//
// ALName & ALName::StripFileName()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  A reference to this.  This is nice, because you can strip the
//  file name from an object and use it in the same operation.
//
// DESCRIPTION
//
//  In the wildcard parsing code, sometimes I need to get the path
//  of a file, which means stripping off the filename and extension.
//  This is pretty easy to do, I just find the right spot and stick
//  a string termination character in that position.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : Needed to use a different path separator character
//                         for UNIX.

ALName AL_DLL_FAR & AL_PROTO ALName::StripFileName()
{
    if ( mszName ) {
#ifdef AL_UNIX
        char *p = strrchr( mszName, '/' );
#else
        char *p = strrchr( mszName, '\\' );
#endif
        if ( p == 0 )
            p = strrchr( mszName, ':' );
        if ( p )
            p[ 1 ] = '\0';
        else
            mszName[ 0 ] = '\0';
    }
    return *this;
}

//
// ALName & ALName::StripPath()
//
// ARGUMENTS:
//
//  None.
//
// RETURNS
//
//  A reference to this.  This is nice, because you can strip the
//  file name from an object and use it in the same operation.
//
// DESCRIPTION
//
//  In the wildcard parsing code, sometimes I need to get just the
//  filename and extension of a file, which means stripping off the
//  drive and path information.  This is pretty easy to do, I just find
//  the start of the filename, and move it up to the start of the string.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : Needed to use a different path separator character
//                         for UNIX.


ALName AL_DLL_FAR & AL_PROTO ALName::StripPath()
{
    if ( mszName ) {
#ifdef AL_UNIX
        char *p = strrchr( mszName, '/' );
#else
        char *p = strrchr( mszName, '\\' );
#endif
        if ( p == 0 )
            p = strrchr( mszName, ':' );
        if ( p ) {
            p++;
            char *s = mszName;
            while( ( *s++ = *p++ ) != 0 )
                ;
            *s = '\0';
        } //If not p, path is already stripped
    }
    return *this;
}

//
// int ALName::WildCardMatch( const char *pattern )
//
// ARGUMENTS:
//
//  pattern : A pointer to a regular expression, including wildcards
//            and sets.  It can get pretty complicated.
//
// RETURNS
//
//  0 if the pattern doesn't match the object name, 1 if it does.
//
// DESCRIPTION
//
//  This is a super-duper powerful function.  It is used to compare
//  a regular expression to the contents of an ALName.  The real meat
//  here is in the public domain string matching code found in _MATCH.CPP.
//
//  The tricky bit here is that we have to make a comparison based
//  on the case sensitivity of this.  Rather than trying to modify
//  the code in _MATCH.CPP, I just make a new copy of the pattern, and
//  mangle the case according to what this expects.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : Had to add a default case to the switch()
//                         statement to avoid a warning from gcc -Wall.

int AL_PROTO ALName::WildCardMatch( const char AL_DLL_FAR *pattern )
{
    int error;
    int result;
    char *p = new char[ strlen( pattern ) + 1 ];

    if ( !p )
        return 0;
    strcpy( p, pattern );
    switch ( mCase ) {
        case AL_UPPER : strupr( p ); break;
        case AL_LOWER : strlwr( p ); break;
        default       : break;
    }
    if ( !is_valid_pattern( p, &error ) )
        result = 0;
    else if ( matche( p, mszName ) == MATCH_VALID )
        result = 1;
    else
        result = 0;
    delete[] p;
    return result;
}

// PROTECTED MEMBER FUNCTION.
//
// void ALName::Strcpy( const char *s )
//
// ARGUMENTS:
//
//  s  :  A character string to be copied into mszName.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  Whenever I am going to copy a string into mszName, I need to convert
//  it to the case that this object expects.  If it is AL_UPPER or
//  AL_LOWER, that means copying and then converting to either all upper
//  case or all lower case.  I do that here.
//
// REVISION HISTORY
//
//   May 26, 1994  1.0A  : First release
//
//   July 7, 1994  1.0B  : Had to add a default case to the switch()
//                         statement to avoid a warning from gcc -Wall.


void AL_PROTO ALName::Strcpy( const char *s )
{
    strcpy( mszName, s );
     switch ( mCase ) {
        case AL_UPPER : strupr( mszName ); break;
        case AL_LOWER : strlwr( mszName ); break;
        default       : break;
     }
}
