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


#define FILE_MATCH

/* match defines */
#define MATCH_PATTERN  6        /* bad pattern */
#define MATCH_LITERAL  5        /* match failure on literal match */
#define MATCH_RANGE    4        /* match failure on [..] construct */
#define MATCH_ABORT    3        /* premature end of text string */
#define MATCH_END      2        /* premature end of pattern string */
#define MATCH_VALID    1        /* valid match */

/* pattern defines */
#define PATTERN_VALID  0        /* valid pattern */
#define PATTERN_ESC   -1        /* literal escape at end of pattern */
#define PATTERN_RANGE -2        /* malformed range in [..] construct */
#define PATTERN_CLOSE -3        /* no end bracket in [..] construct */
#define PATTERN_EMPTY -4        /* [..] contstruct is empty */

/*----------------------------------------------------------------------------
 *
 * Match the pattern PATTERN against the string TEXT;
 *
 *      match() returns TRUE if pattern matches, FALSE otherwise.
 *      matche() returns MATCH_VALID if pattern matches, or an errorcode
 *          as follows otherwise:
 *
 *           MATCH_PATTERN  - bad pattern

#ifndef FILE_MATCH
 *           MATCH_LITERAL  - match failure on literal mismatch
#endif

 *           MATCH_RANGE    - match failure on [..] construct
 *           MATCH_ABORT    - premature end of text string
 *           MATCH_END      - premature end of pattern string
 *           MATCH_VALID    - valid match
 *
 *
 * A match means the entire string TEXT is used up in matching.
 *
 * In the pattern string:
 *      `*' matches any sequence of characters (zero or more)
 *      `?' matches any character
 *      [SET] matches any character in the specified set,
 *      [!SET] or [^SET] matches any character not in the specified set.
 *
 * A set is composed of characters or ranges; a range looks like character
 * hyphen character (as in 0-9 or A-Z).  [0-9a-zA-Z_] is the minimal set of
 * characters allowed in the [..] pattern construct.  Other characters are
 * allowed (ie. 8 bit characters) if your system will support them.
 *
 * To suppress the special syntactic significance of any of `[]*?!^-\', and
 * match the character exactly, precede it with a `\'.
 *
 ---------------------------------------------------------------------------*/

int AL_FUNCTION match(char *pattern, char *text);

int AL_FUNCTION matche( const char *pattern, char *text );

/*----------------------------------------------------------------------------
*
* Return TRUE if PATTERN has any special wildcard characters
*
----------------------------------------------------------------------------*/

int AL_FUNCTION is_pattern( const char *pattern);

/*----------------------------------------------------------------------------
 *
 * Return TRUE if PATTERN has is a well formed regular expression according
 * to the above syntax
 *
 * error_type is a return code based on the type of pattern error.  Zero is
 * returned in error_type if the pattern is a valid one.  error_type return
 * values are as follows:
 *
 *   PATTERN_VALID - pattern is well formed

#ifndef FILE_MATCH
 *   PATTERN_ESC   - pattern has invalid escape ('\' at end of pattern)
#endif

 *   PATTERN_RANGE - [..] construct has a no end range in a '-' pair (ie [a-])
 *   PATTERN_CLOSE - [..] construct has no end bracket (ie [abc-g )
 *   PATTERN_EMPTY - [..] construct is empty (ie [])
 *
 ---------------------------------------------------------------------------*/

int AL_FUNCTION is_valid_pattern( const char *pattern, int *error_type );
