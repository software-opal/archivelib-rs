
#include "_r.h"

class RExpand {
private:
  ALStorage *_161;
  ALStorage *_162;
  short _175;
  short _176;
  uchar *_166;
  ushort *_240;
  ushort *_241;
  uchar *_242;
  ushort *_189;
  ushort *_190;
  uchar *_180;
  uchar *_181;
  short _243;
  ushort _244;
  ushort _182;
  short _172;
  uchar _245;
  short _246;
  uchar *_247;
  long _248;
  ushort _249();
  ushort _250();
  void _251();
  ushort _252(int _219);
  void _253(short _254, short _220, short _221);
  void _255();
  void _256(int _219);
  void _257();
  void _258(int _259, uchar *_260, int _261, ushort *_262, ushort _263);

public:
  RExpand(ALStorage &_233, ALStorage &_202, long _264, int _234);
  ~RExpand();
  int Expand();
  ALStatus mStatus;

protected:
  RExpand(RExpand &);
  RExpand &operator=(RExpand &);
};
