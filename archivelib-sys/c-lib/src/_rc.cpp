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

#include "_r.h"

#ifdef AL_SYMANTEC
#include <stdlib.h>
#else

#if defined( AL_BORLAND )
#if defined( __BCOPT__ )
#undef __BCOPT__
#endif
#endif

#include <malloc.h>
#endif
#define _519 "Incorrect compression level parameter passed to compressor.  Compression level = %d"
#define _520 "Memory allocation failure in compression startup"
#define _445(_200,_446)((short )((_446<<_154)^(_278[_200+2]))&(_153-1))
#define _447(_200,_201){short _204;if((_204=_163[_201])!=_157)_164[_204]=\
_200;_164[_200]=_201;_163[_200]=_204;_163[_201]=_200;}
#define _448(s){short _204;if((_204=_164[s])!=_157){_164[s]=_157;_163[_204]\
=_157;}}
RCompress::RCompress(ALStorage&_266, ALStorage&_267, int _269, int _235)
{ _161=&_266; _162=&_267; _531=_235; if(_269>_137||_269<_138){
mStatus.SetError(AL_ILLEGAL_PARAMETER,_519,_269-10); _175=2; }else
_175=(short )(1<<_269); _176=(short )(_175-1); if((_166=new uchar
[_175+_140+2])!=0) memset(_166,0,(_175+_140+2)*sizeof(uchar ));
#if defined(AL_LARGE_DATA)||defined(AL_FLAT_MODEL)||defined(AL_UNIX)
if((_163=new short [_175+_153])!=0)
memset(_163,0,(_175+_153)*sizeof(short )); if((_164=new short
[_175])!=0) memset(_164,0,_175*sizeof(short )); _165=new uchar [_155];
#else
_163=(short _far*)_fcalloc(_175+_153,sizeof(short )); _164=(short
_far*)_fcalloc(_175,sizeof(short )); _165=(uchar
_far*)_fcalloc(_155,sizeof(uchar ));
#endif
_179=new uchar [_156];
if((_189=new ushort [2*_141-1])!=0)
memset(_189,0,(2*_141-1)*sizeof(ushort )); if((_190=new ushort
[2*_141-1])!=0) memset(_190,0,(2*_141-1)*sizeof(ushort )); if((_177=new
short [_141+1])!=0) memset(_177,0,(_141+1)*sizeof(short )); _180=new
uchar [_141]; if((_191=new ushort [2*_141-1])!=0)
memset(_191,0,(2*_141-1)*sizeof(ushort )); if((_192=new ushort
[_141])!=0) memset(_192,0,_141*sizeof(ushort )); _181=new uchar [_152];
if((_193=new ushort [2*_142-1])!=0)
memset(_193,0,(2*_142-1)*sizeof(ushort )); if((_194=new ushort
[_152])!=0) memset(_194,0,_152*sizeof(ushort )); if(!_166|| !_163||
!_164|| !_165|| !_179|| !_189|| !_190|| !_177|| !_180|| !_191|| !_192||
!_181|| !_193|| !_194){ mStatus.SetError(AL_CANT_ALLOCATE_MEMORY,_520);
} _533=0; _534=_266.GetSize(); } RCompress::~RCompress() { if(_166)
delete[]_166;
#if defined(AL_LARGE_DATA)||defined(AL_FLAT_MODEL)||defined(AL_UNIX)
if(_163) delete[]_163; if(_164) delete[]_164; if(_165) delete[]_165;
#else
_ffree(_163); _ffree(_164); _ffree(_165);
#endif
if(_179)
delete[]_179; if(_189) delete[]_189; if(_190) delete[]_190; if(_177)
delete[]_177; if(_180) delete[]_180; if(_191) delete[]_191; if(_192)
delete[]_192; if(_181) delete[]_181; if(_193) delete[]_193; if(_194)
delete[]_194; }inline void RCompress::_223(short _203) {
_208(_180[_203],_192[_203]); }int RCompress::Compress() { short _209;
short _201; short _200; short s; int _231; uchar *_278; short _280;
short _279; _278=_166; _280=_176; _279=_175; _231=0; _196();
_198();_200=0; _209=(short )_161->ReadBuffer(_278,_279); s=(short
)(_209&_280); _169=0; _168=0; _201=(short
)(((_278[_200]<<_154)^(_278[_200+1]))&(_153-1)); _201=(short
)(_445(_200,_201)+_279); while(_209>_140+4&&!_170){ _199(_200,_201);
if(_168<_135){ _202(_278[_200],0); _447(_200,_201); _200++; _201=(short
)(_445(_200,_201)+_279); _209--; }else { _209-=_168; _202((ushort
)(_168+(UCHAR_MAX+1-_135)),_169); while(--_168>=0){ _447(_200,_201);
_200++; _201=(short )(_445(_200,_201)+_279); } } }
for(;_209<_140;_209++){ int _203=_161->ReadChar (); if(_203<0) break;
_278[s]=(unsigned char )_203; if(s<_140-1) _278[s+_279]=_278[s];
_448(s); s=(short )((s+1)&(_280)); } while(_209>0&&!_170){
_199(_200,_201); if(_168>_209) _168=_209; if(_168<_135){ _168=1;
_202(_278[_200],0); }else _202((ushort )(_168+(UCHAR_MAX+1-_135)),
_169); while(--_168>=0){ int _203=_161->ReadChar (); if(_203<0) break;
else _278[s]=(unsigned char )_203; if(s<_140-1) _278[s+_279]=_278[s];
_448(s); s=(short )((s+1)&(_280)); _447(_200,_201); _200=(short
)((_200+1)&(_280)); _201=(short )(_445(_200,_201)+_279); }
while(_168-->=0){ _447(_200,_201); _200=(short )((_200+1)&_280);
_201=(short )(_445(_200,_201)+_279); _209--; } if(_162->mStatus<0)
return 1; } if(!_170) _202(_144+(UCHAR_MAX+1-_135),0); _197(); if(_170)
_231=1; return _231; } void RCompress::_196() { int _226;
for(_226=0;_226<_141;_226++) _191[_226]=0; for(_226=0;_226<_142;_226++)
_193[_226]=0; _173=0; _205(); _170=0; _185=1; _184=0; _186=0; _165[0]=0;
_183=_155; _183-=(ushort )((3*CHAR_BIT)+6); } void RCompress::_197() {
if(!_170) _207(); _206(); _183=0; _184=0; } void RCompress::_198() {
#if defined(AL_FLAT_MODEL)||defined(AL_UNIX)
short *_450;
#else
short _far*_450;
#endif
short _226; _450=&_163[_175];
for(_226=_153;_226>0;_226--) *_450++=_157; _450=_164;
for(_226=_175;_226>0;_226--) *_450++=_157; } void RCompress::_199(short
_200,short _201) { uchar *_451; uchar *_278; short
_226,_452,_204,_453; _452=_158; _168=0; _451=&_166[_200]; _204=_201;
while((_204=_163[_204])!=_157){ if(--_452<0) break; _278=&_166[_204];
if(_451[_168]!=_278[_168]) continue; if(_451[0]!=_278[0]) continue;
if(_451[1]!=_278[1]) continue; if(_451[2]!=_278[2]) continue;
for(_226=3;_226<_140;_226++) if(_451[_226]!=_278[_226]) break;
if(_226>_168){ _453=(short )(_200-_204-1); if(_453<0) _453+=_175;
if(_453>=_175){ break; } _169=_453; if((_168=_226)>=_140) break; } } }
void RCompress::_202(ushort _203,ushort _204) { if((_185>>=1)==0){
_185=1U<<(CHAR_BIT-1); if(_184>=_183){ _207(); if(_170) return ;
_184=0; } _186=_184++; _165[_186]=0; } _165[_184++]=(uchar )_203;
_191[_203]++; if(_203>=(1U<<CHAR_BIT)){ _165[_186]|=(uchar )_185;
_165[_184++]=(uchar )_204; _165[_184++]=(uchar )(_204>>CHAR_BIT);
_203=0; while(_204){ _203++; _204>>=1; } _193[_203]++; } } void
RCompress::_205() { _172=0; _182=0; _171=0; } void RCompress::_206() {
if(!_170){ _208(CHAR_BIT-1,0); if(_171) _210(); } _171=0; } void
RCompress::_207() { uint _226,_289,_229,_454,_455; uint _456=0; ushort
_217[2*_145-1]; _229=_211(_141,_191,_180,_192); _455=_191[_229];
_208(16,(ushort )_455); if(_229>=_141){ _216(_217);
_229=_211(_145,_217,_181,_194); if(_229>=_145){ _218(_145,_147,3); }else
{ _208(_147,0); _208(_147,(ushort )_229); } _222(); }else {
_208(_147,0); _208(_147,0); _208(_143,0); _208(_143,(ushort )_229); }
_229=_211(_142,_193,_181,_194); if(_229>=_142){ _218(_142,_540,-1);
}else { _208(_540,0); _208(_540,(ushort )_229); } _454=0;
for(_226=0;_226<_455;_226++){ if(_226%CHAR_BIT==0) _456=_165[_454++];
else _456<<=1; if(_456&(1U<<(CHAR_BIT-1))){ _223((short
)(_165[_454++]+(1U<<CHAR_BIT))); _289=_165[_454++];
_289+=_165[_454++]<<CHAR_BIT; _224((short )_289); }else
_223(_165[_454++]); if(_170) return ; } for(_226=0;_226<_141;_226++)
_191[_226]=0; for(_226=0;_226<_142;_226++) _193[_226]=0; } void
RCompress::_208(int _209,ushort _203) { _203<<=_133-_209; _182|=(ushort
)(_203>>_172); if((_172+=(short )_209)>=8){ if(_171>=_156) _210();
_179[_171++]=(uchar )(_182>>CHAR_BIT); if((_172=(ushort )(_172-
CHAR_BIT))<CHAR_BIT) _182<<=CHAR_BIT; else { if(_171>=_156) _210();
_179[_171++]=(uchar )_182; _172=(ushort )(_172-CHAR_BIT); _182=(ushort
)(_203<<(_209-_172)); } } } void RCompress::_210() { if(_171<=0) return
; if(_531&&(_533+=_171)>=_534) _170=1; else
_162->WriteBuffer(_179,_171); _171=0; } int RCompress::_211(int _212,
ushort *_213, uchar *_214, ushort *_215) { int _226,_276,_289,_292;
short _227; _174=(short )_212; _187=_213; _178=_214; _292=_174; _227=0;
_177[1]=0; for(_226=0;_226<_174;_226++){ _178[_226]=0; if(_187[_226])
_177[++_227]=(short )_226; } if(_227<2){ _215[_177[1]]=0; return
_177[1]; } for(_226=_227/2;_226>=1;_226--) _225(_226,_187,_177,_227);
_188=_215; do{ _226=_177[1]; if(_226<_174) *_188++=(ushort )_226;
_177[1]=_177[_227--]; _225(1,_187,_177,_227); _276=_177[1];
if(_276<_174) *_188++=(ushort )_276; _289=_292++; _187[_289]=(ushort
)(_187[_226]+_187[_276]); _177[1]=(short )_289; _225(1,_187,_177,_227);
_189[_289]=(ushort )_226; _190[_289]=(ushort )_276; }while(_227>1);
_188=_215; _228(_289); _230(_212,_214,_215); return _289; } void
RCompress::_216(ushort *_217) { short _226,_289,_219,_277;
for(_226=0;_226<_145;_226++) _217[_226]=0; _219=_141;
while(_219>0&&_180[_219-1]==0) _219--; _226=0; while(_226<_219){
_289=_180[_226++]; if(_289==0){ _277=1; while(_226<_219&&_180[_226]==0){
_226++; _277++; } if(_277<=2) _217[0]+=_277; else if(_277<=18)
_217[1]++; else if(_277==19){ _217[0]++; _217[1]++; }else _217[2]++;
}else _217[_289+2]++; } } void RCompress::_218(short _219,short
_220,short _221) { short _226,_289; while(_219>0&&_181[_219-1]==0)
_219--; _208(_220,_219); _226=0; while(_226<_219){ _289=_181[_226++];
if(_289<=6){ _208(3,_289); }else _208(_289-3,(ushort )(USHRT_MAX<<1));
if(_226==_221){ while(_226<6&&_181[_226]==0) _226++; _208(2,(ushort
)(_226-3)); } } } void RCompress::_222() { short _226,_289,_219,_277;
_219=_141; while(_219>0&&_180[_219-1]==0) _219--; _208(_143,_219);
_226=0; while(_226<_219){ _289=_180[_226++]; if(_289==0){ _277=1;
while(_226<_219&&_180[_226]==0){ _226++; _277++; } if(_277<=2){
for(_289=0;_289<_277;_289++) _208(_181[0],_194[0]); }else if(_277<=18){
_208(_181[1],_194[1]); _208(4,(ushort )(_277-3)); }else if(_277==19){
_208(_181[0],_194[0]); _208(_181[1],_194[1]); _208(4,15); }else {
_208(_181[2],_194[2]); _208(_143,(ushort )(_277-20)); } }else
_208(_181[_289+2],_194[_289+2]); } } void RCompress::_224(ushort _204) {
ushort _203,_457; _203=0; _457=_204; while(_457){ _203++; _457>>=1; }
_208(_181[_203],_194[_203]); if(_203>1) _208(_203-1,_204); } void
RCompress::_225(int _226,ushort *_187,short *_177,short _227) { int
_276,_289; _289=_177[_226]; while((_276=2*_226)<=_227){
if(_276<_227&&_187[_177[_276]]>_187[_177[_276+1]]) _276++;
if(_187[_289]<=_187[_177[_276]]) break; _177[_226]=_177[_276];
_226=_276; } _177[_226]=(ushort )_289; } void RCompress::_228(int _229)
{ int _226,_289; uint _458; for(_226=0;_226<=16;_226++) _167[_226]=0;
_232(_229); _458=0; for(_226=16;_226>0;_226--)
_458+=_167[_226]<<(16-_226); while(_458!=(1U<<16)){ _167[16]--;
for(_226=15;_226>0;_226--){ if(_167[_226]!=0){ _167[_226]--;
_167[_226+1]=(ushort )(_167[_226+1]+2); break; } } _458--; }
for(_226=16;_226>0;_226--){ _289=_167[_226]; while(--_289>=0)
_178[*_188++]=(uchar )_226; } } void RCompress::_230(int _219,uchar
*_209,ushort *_231) { int _226; ushort _288[18]; _288[1]=0;
for(_226=1;_226<=16;_226++) _288[_226+1]=(ushort
)((_288[_226]+_167[_226])<<1); for(_226=0;_226<_219;_226++)
_231[_226]=_288[_209[_226]]++; } void RCompress::_232(int _226) {
if(_226<_174) _167[(_173<16)?_173:16]++; else { _173++;
_232(_189[_226]); _232(_190[_226]); _173--; } }
