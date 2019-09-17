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
#define _519 "Incorrect compression level parameter passed to compressor.  Compression level = %d"
#define _520 "Memory allocation failure in expansion startup"
#define _521 "Internal 1 error in Greenleaf Decompression routine"
#define _522 "Internal 2 error in Greenleaf Decompression routine"

RExpand::RExpand(ALStorage&_266, ALStorage&_267, long _268, int _269) {
_161=&_266; _162=&_267; _248=_268;; if(_269>_137||_269<_138){
mStatus.SetError(AL_ILLEGAL_PARAMETER, _519, _269-10); _175=2; }else
_175=(short )(1<<_269); _176=(short )(_175-1); _166=new uchar [_175+2];
if(_166) memset(_166,0,(_175+2)*sizeof(uchar )); _240=new ushort [_148];
if(_240) memset(_240,0,_148*sizeof(ushort )); _241=new ushort [_149];
if(_241) memset(_241,0,_149*sizeof(ushort )); _242=new uchar [_159];
if(_242) memset(_242,0,_159*sizeof(uchar )); _189=new ushort [2*_141-1];
if(_189) memset(_189,0,(2*_141-1)*sizeof(ushort )); _190=new ushort
[2*_141-1]; if(_190) memset(_190,0,(2*_141-1)*sizeof(ushort )); _180=new
uchar [_141]; _181=new uchar [_152]; if(!_166|| !_240|| !_241|| !_242||
!_189|| !_190|| !_180|| !_181){
mStatus.SetError(AL_CANT_ALLOCATE_MEMORY,_520); } } RExpand::~RExpand()
{ if(_166) delete[]_166; if(_240) delete[]_240; if(_241) delete[]_241;
if(_242) delete[]_242; if(_189) delete[]_189; if(_190) delete[]_190;
if(_180) delete[]_180; if(_181) delete[]_181; } int RExpand::Expand() {
int _231; short _226; short _276; short _203; short _200;
uchar *_278; short _279; short _280; _278=_166; _279=_175; _280=_176;
_231=0; _243=0; _251(); _200=0; while(_243<5){
if((_203=_249())<=UCHAR_MAX){ _278[_200]=(uchar )_203;
if(++_200>=_279){ _200=0; if((short )_162->WriteBuffer(_278,_279)!=_279)
goto _282; } }else { _276=(short )(_203-(UCHAR_MAX+1-_135));
if(_276==_144) break; _226=(short )((_200-_250()-1)&_280);
if(_226<_279-_140-1&&_200<_279-_140-1){ while(--_276>=0)
_278[_200++]=_278[_226++]; }else { while(--_276>=0){
_278[_200]=_278[_226]; if(++_200>=_279){ _200=0; if((short
)_162->WriteBuffer(_278,_279)!=_279) goto _282; } _226=(short
)((_226+1)&_280); } } } } if(_200!=0) _162->WriteBuffer(_278,_200);
_282: return _231; } ushort RExpand::_249() { ushort _276,_283;
if(_244==0){ _244=_252(16); _253(_145,_147,3); _255();
_253(_142,_540,-1); if(mStatus<0) return 0; } _244--;
_276=_240[_182>>4]; if(_276>=_141){ _283=1U<<3; do{ if(_182&_283)
_276=_190[_276]; else _276=_189[_276]; _283>>=1; }while(_276>=_141); }
_256(_180[_276]); return _276; } ushort RExpand::_250() { ushort
_276,_283; _276=_241[_182>>8]; if(_276>=_142){ _283=1U<<7; do{
if(_182&_283) _276=_190[_276]; else _276=_189[_276]; _283>>=1;
}while(_276>=_142); } _256(_181[_276]); if(_276!=0){ _276--; _276=(short
)((1U<<_276)+_252(_276)); } return _276; } void RExpand::_251() {
_244=0; _257(); } ushort RExpand::_252(int _219) { ushort _284;
_284=(ushort )(_182>>(2*CHAR_BIT-_219)); _256(_219); return _284; } void
RExpand::_253(short _254,short _220,short _221) { short _226,_203,_219;
ushort _283; _219=_252(_220); if(_219==0){ _203=_252(_220);
for(_226=0;_226<_254;_226++) _181[_226]=0; for(_226=0;_226<256;_226++)
_241[_226]=_203; }else { _226=0; while(_226<_219){ _203=(short
)(_182>>13); if(_203==7){ _283=1U<<12; while(_283&_182){ _283>>=1;
_203++; } } _256((_203<7)?3:_203-3); _181[_226++]=(uchar )_203;
if(_226==_221){ _203=_252(2); while(--_203>=0) _181[_226++]=0; } }
while(_226<_254) _181[_226++]=0; _258(_254,_181,8,_241,_149); } } void
RExpand::_255() { short _226,_203,_219; ushort _283; _219=_252(_143);
if(_219==0){ _203=_252(_143); for(_226=0;_226<_141;_226++) _180[_226]=0;
for(_226=0;_226<_148;_226++) _240[_226]=_203; }else { _226=0;
while(_226<_219){ _203=_241[_182>>8]; if(_203>=_145){ _283=1U<<7; do{
if(_182&_283) _203=_190[_203]; else _203=_189[_203]; _283>>=1;
}while(_203>=_145); } _256(_181[_203]); if(_203<=2){ if(_203==0) _203=1;
else if(_203==1) _203=(short )(_252(4)+3); else _203=(short
)(_252(_143)+20); while(--_203>=0) _180[_226++]=0; }else
_180[_226++]=(uchar )(_203-2); } while(_226<_141) _180[_226++]=0;
_258(_141,_180,12,_240,_148); } } void RExpand::_256(int _219) {
while(_219>_172){ _219-=_172; _182=(ushort
)((_182<<_172)+(_245>>(CHAR_BIT-_172))); if(_246<=0){ _247=_242;
if(_248>=0&&_248<_159){ _246=(short
)_161->ReadBuffer(_242,(size_t)_248); _248-=_246; }else _246=(short
)_161->ReadBuffer(_242,_159); if(_246<=0) _243++; } _245=*_247++;
_246--; _172=CHAR_BIT; } _172=(short )(_172-_219); _182=(ushort
)((_182<<_219)+(_245>>(CHAR_BIT-_219))); _245<<=_219; } void
RExpand::_257() { _182=0; _245=0; _172=0; _246=0; _256(2*CHAR_BIT); }
#if defined(AL_BORLAND)&&defined(AL_FLAT_MODEL)
#pragma option -Od
#endif
void RExpand::_258(int _259, uchar *_260, int _261, ushort *_262, ushort
_263) { ushort _277[17],_287[17],_288[18],*_204; uint
_226,_289,_209,_290,_291,_292,_293,_283; for(_226=1;_226<=16;_226++)
_277[_226]=0; for(_226=0;(int )_226<_259;_226++) _277[_260[_226]]++;
_288[1]=0; for(_226=1;_226<=16;_226++) _288[_226+1]=(ushort
)(_288[_226]+(_277[_226]<<(16-_226))); if(_288[17]!=(ushort )(1U<<16)){
mStatus.SetError(AL_INTERNAL_ERROR,_521); _243=10; return ; }
_291=16-_261; for(_226=1;(int )_226<=_261;_226++){ _288[_226]>>=_291;
_287[_226]=(ushort )(1U<<(_261-_226)); } while(_226<=16){
_287[_226]=(ushort )(1U<<(16-_226)); _226++; } _226=_288[_261+1]>>_291;
if(_226!=(ushort )(1U<<16)){ _289=1U<<_261; while(_226!=_289)
_262[_226++]=0; } _292=_259; _283=1U<<(15-_261); for(_290=0;(int
)_290<_259;_290++){ if((_209=_260[_290])==0) continue;
_293=_288[_209]+_287[_209]; if((int )_209<=_261){ if(_293>_263){
mStatus.SetError(AL_INTERNAL_ERROR,_522); _243=10; return ; }
for(_226=_288[_209];_226<_293;_226++) _262[_226]=(ushort )_290; }else {
_289=_288[_209]; _204=&_262[_289>>_291]; _226=_209-_261; while(_226!=0){
if(*_204==0){ _190[_292]=_189[_292]=0; *_204=(ushort )_292++; }
if(_289&_283) _204=&_190[*_204]; else _204=&_189[*_204]; _289<<=1;
_226--; } *_204=(ushort )_290; } _288[_209]=(ushort )_293; } }
