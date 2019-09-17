#include "arclib.h"

#include <iostream>
#include <ostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

//
// void _ALAssertFailure( const char *condition,
//                        const char *filename,
//                        int line,
//                        const char *message,
//                        ... )
//
// ARGUMENTS:
//
//  condition   :  A character string containing the condition that failed,
//                 leading to the assertion.
//
//  filename    :  The name of the file where the assertion error took place.
//
//  line        :  The line in the file where the assertion error took place.
//
//  message     :  The error message associated with the assertion error.
//                 This message is a sprintf() style format string.
//
//  ...         :  Any additional arguments.
//
// RETURNS
//
//  Nothing.
//
// DESCRIPTION
//
//  The C run time library features an assert() macro, that can be used to
//  abort a program if a given condition isn't true.  It aborts the program
//  by calling a routine that looks something like this.  The AL_ASSERT()
//  macro that we use is even better, because it includes a comment
//  that gets displayed when the abort takes place.  This routine is
//  responsible for displaying that comment, along with the file name and
//  the line number, then aborting the program.  It is called by the
//  AL_ASSERT() macro when the conditional expression argument fails.
//
// REVISION HISTORY
//
//   May 22, 1994  1.0A  : First release
//

void _ALAssertFailure(const char *condition, const char *filename, int line,
                      const char *message, ...) {
  char buf1[256];
  char buf2[128];
  va_list argptr;

  va_start(argptr, message);
  vsprintf(buf2, message, argptr);
  va_end(argptr);

  sprintf(buf1,
          "Assertion error, ArchiveLib is aborting the application.\n"
          "Condition = %s\n"
          "File = %s, line = %d\n"
          "%s",
          condition, filename, line, buf2);

  std::cerr << buf1 << "\n" << std::flush;
  abort();
}
