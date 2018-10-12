#ifndef _STATUS_H
#define _STATUS_H

#if defined(__cplusplus)

#include "simple_status.h"

class AL_CLASS_TYPE ALStatus {
  /*
   * Constructors, destructors, assignment operators, and declarations
   */
public:
  AL_PROTO ALStatus();
  AL_PROTO ~ALStatus();
#if defined(AL_USING_DLL) || defined(AL_BUILDING_DLL)
  void AL_DLL_FAR *AL_PROTO operator new(size_t size);
#endif

protected:
  AL_PROTO ALStatus(ALStatus AL_DLL_FAR &);
  /*
   * Member functions
   */
public:
  int AL_PROTO SetError(int error, const char AL_DLL_FAR *fmt, ...);
  int AL_PROTO GetStatusCode() { return miStatus; }
  const char AL_DLL_FAR *AL_PROTO GetStatusString();
  const char AL_DLL_FAR *AL_PROTO GetStatusDetail() const;
  SimpleStatus copyToSimple();
  AL_PROTO operator int() { return miStatus; }
  ALStatus AL_DLL_FAR &AL_PROTO operator=(ALStatus AL_DLL_FAR &);
  /*
   * Data members
   */
protected:
  int miStatus;
  const int miStatusDetailLength;
  char AL_DLL_FAR *mszStatusDetail;
};

#ifndef BINDGEN
inline std::ostream &operator<<(std::ostream &stream,
                                const ALStatus AL_DLL_FAR &status) {
#if defined(AL_USING_DLL) && !defined(AL_LARGE_MODEL) && !defined(AL_FLAT_MODEL)
  const char _far *p = status.GetStatusDetail();
  char *near_string = new char[_fstrlen(p) + 1];
  if (near_string) {
    _fstrcpy(near_string, p);
    stream << near_string;
    delete near_string;
  } else
    stream << "Memory allocation failure!";
  return stream;
#else
  return stream << status.GetStatusDetail();
#endif
}
#endif

#endif /* #if defined( __cplusplus ) */

#endif /* #ifdef _STATUS_H           */
