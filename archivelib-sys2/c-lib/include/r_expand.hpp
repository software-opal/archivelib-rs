#ifndef R_EXPAND_HPP
#define R_EXPAND_HPP

// #include "new/expand.hpp"

#include "aldefs.h"
#include <climits>
#include "stor.h"

typedef uint16_t ushort;
typedef uint8_t uchar;

#ifdef __cplusplus

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

  ushort fn249();
  ushort fn250();
  void fn251();
  ushort fn252(int _219);
  void fn253(short _254, short _220, short _221);
  void fn255();
  void fn256(int _219);
  void fn257();
  void fn258(int _259, uchar *_260, int _261, ushort *_262, ushort _263);

public:
  RExpand(ALStorage &_233, ALStorage &_202, long _264, int _234);
  ~RExpand();
  int Expand();
  ALStatus mStatus;

protected:
  RExpand(RExpand &);
  RExpand &operator=(RExpand &);
};

#endif
#endif
