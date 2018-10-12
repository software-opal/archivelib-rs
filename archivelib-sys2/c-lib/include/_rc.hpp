
#include "_r.h"

class RCompress {
private:
  ALStorage *_161;
  ALStorage *_162;
#if defined(AL_LARGE_DATA) || defined(AL_FLAT_MODEL) || defined(AL_UNIX)
  short *_163;
  short *_164;
  uchar *_165;
#else
  short _far *_163;
  short _far *_164;
  uchar _far *_165;
#endif
  uchar *_166;
  unsigned long _533;
  unsigned long _534;
  ushort _167[17];
  short _168;
  short _169;
  short _170;
  short _171;
  short _172;
  short _173;
  short _174;
  short _175;
  short _176;
  short *_177;
  uchar *_178;
  uchar *_179;
  uchar *_180;
  uchar *_181;
  ushort _182;
  ushort _183;
  ushort _184;
  ushort _185;
  ushort _186;
  ushort *_187;
  ushort *_188;
  ushort *_189;
  ushort *_190;
  ushort *_191;
  ushort *_192;
  ushort *_193;
  ushort *_194;
  int _531;

private:
  void _196();
  void _197();
  void _198();
  void _199(short _200, short _201);
  void _202(ushort _203, ushort _204);
  void _205();
  void _206();
  void _207();
  void _208(int _209, ushort _203);
  void _210();
  int _211(int _212, ushort *_213, uchar *_214, ushort *_215);
  void _216(ushort *_217);
  void _218(short _219, short _220, short _221);
  void _222();
  void _223(short _203);
  void _224(ushort _204);
  void _225(int _226, ushort *_187, short *_177, short _227);
  void _228(int _229);
  void _230(int _219, uchar *_209, ushort *_231);
  void _232(int _226);

public:
  RCompress(ALStorage &_233, ALStorage &_202, int _234, int _235);
  ~RCompress();
  int Compress();
  ALStatus mStatus;

protected:
  RCompress(RCompress &);
  RCompress &operator=(RCompress &);
};
