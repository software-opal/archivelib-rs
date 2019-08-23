#ifndef _STATUS_H
#define _STATUS_H

#if defined(__cplusplus)

#include "simple_status.h"

class ALStatus {
  /*
   * Constructors, destructors, assignment operators, and declarations
   */
public:
  ALStatus();
  ~ALStatus();

protected:
  ALStatus(ALStatus &);
  /*
   * Member functions
   */
public:
  int SetError(int error, const char *fmt, ...);
  int GetStatusCode() { return miStatus; }
  const char *GetStatusString();
  const char *GetStatusDetail() const;
  SimpleStatus copyToSimple();
  operator int() { return miStatus; }
  ALStatus &operator=(ALStatus &);
  /*
   * Data members
   */
protected:
  int miStatus;
  const int miStatusDetailLength;
  char *mszStatusDetail;
};

#endif
#endif
