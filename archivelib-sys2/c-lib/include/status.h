#ifndef _STATUS_H
#define _STATUS_H

#if defined(__cplusplus)

#include "simple_status.h"

class  ALStatus {
  /*
   * Constructors, destructors, assignment operators, and declarations
   */
public:
   ALStatus();
   ~ALStatus();
#if defined(AL_USING_DLL) || defined(AL_BUILDING_DLL)
  void  * operator new(size_t size);
#endif

protected:
   ALStatus(ALStatus  &);
  /*
   * Member functions
   */
public:
  int  SetError(int error, const char  *fmt, ...);
  int  GetStatusCode() { return miStatus; }
  const char  * GetStatusString();
  const char  * GetStatusDetail() const;
  SimpleStatus copyToSimple();
   operator int() { return miStatus; }
  ALStatus  & operator=(ALStatus  &);
  /*
   * Data members
   */
protected:
  int miStatus;
  const int miStatusDetailLength;
  char  *mszStatusDetail;
};

#ifndef BINDGEN
inline std::ostream &operator<<(std::ostream &stream,
                                const ALStatus  &status) {
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
