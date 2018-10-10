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

#ifndef _60
#define _60
#include <limits.h>
#include "arclib.h"
typedef unsigned short ushort; typedef unsigned char uchar; typedef
unsigned int uint;
#define _132 (CHAR_BIT*sizeof(ushort))
#define _133 16
#define _134 '\0'
#define _135 3
#define _136 16384
#define _137 14
#define _138 10
#define _139 8
#define _140 256
#define _141 (UCHAR_MAX+1+_140-_135+1+1)
#define _142 (_137+1)
#define _143 9
#define _144 (_140+1)
#define _145 (_133+3)
#define _540 5
#define _147 5
#define _148 4096
#define _149 256
#if (1U<<_540)<=_142
#error pbit too small
#endif
#if (1U<<_147)<=_145
#error tbit too small
#endif
#if _145>_142
#define _152 _145
#else
#define _152 _142
#endif
#define _153 4096
#define _154 4
#define _155 8192
#define _156 512
#define _157 (-1)
#define _158 128
#define _159 512
class RCompress {
private : ALStorage *_161;ALStorage *_162;
#if defined(AL_LARGE_DATA)||defined(AL_FLAT_MODEL)||defined(AL_UNIX)
short *_163;short *_164;uchar *_165;
#else
short _far *_163;short _far *_164;uchar _far *_165;
#endif
uchar *_166;unsigned long _533;unsigned long _534;ushort _167[17]; short
_168;short _169;short _170;short _171;short _172;short _173; short
_174;short _175;short _176;short *_177;uchar *_178;uchar *_179;uchar
*_180;uchar *_181;ushort _182;ushort _183;ushort _184;ushort _185;
ushort _186;ushort *_187;ushort *_188;ushort *_189;ushort *_190; ushort
*_191;ushort *_192;ushort *_193;ushort *_194;int _531; private : void
_196();void _197();void _198();void _199(short _200,short _201); void
_202(ushort _203,ushort _204);void _205();void _206();void _207();void
_208(int _209,ushort _203);void _210();int _211(int _212, ushort *_213,
uchar *_214, ushort *_215);void _216(ushort *_217);void _218(short
_219,short _220,short _221);void _222();void _223(short _203);void
_224(ushort _204);void _225(int _226,ushort *_187,short *_177,short
_227);void _228(int _229);void _230(int _219,uchar *_209,ushort
*_231);void _232(int _226); public:RCompress(ALStorage& _233, ALStorage&
_202, int _234, int _235);~RCompress();int Compress();ALStatus
mStatus;protected : RCompress(RCompress&);RCompress&
operator=(RCompress&);};class RExpand {private : ALStorage
*_161;ALStorage *_162;short _175; short _176;uchar *_166;ushort
*_240;ushort *_241;uchar *_242;ushort *_189;ushort *_190;uchar
*_180;uchar *_181;short _243;ushort _244; ushort _182;short _172;uchar
_245;short _246;uchar *_247;long _248;ushort _249();ushort _250();void
_251(); ushort _252(int _219);void _253(short _254,short _220,short
_221);void _255();void _256(int _219);void _257();void _258(int _259,
uchar *_260, int _261, ushort *_262, ushort _263);public :
RExpand(ALStorage& _233, ALStorage& _202, long _264, int
_234);~RExpand();int Expand(); ALStatus
mStatus;protected:RExpand(RExpand &);RExpand& operator=(RExpand&);};
#endif
