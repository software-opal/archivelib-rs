#ifndef __DEBUG_H
#define __DEBUG_H
/*
 * _ALAssertFailure is the function called by AL_ASSERT() and
 * AL_ASSERT_OBJECT() when their assertion fails.
 */
void _ALAssertFailure(const char *condition, const char *filename, int line,
                      const char *message);
#ifdef NDEBUG
/*
 * In the non-debug versions, both of these macros basically go away.
 * The only difficulty is trying to avoid having the compilers generate
 * error messages when they see this code.  Maybe in NDEBUG mode I could
 * change these to inline functions that do nothing?
 */
#define AL_ASSERT(condition, message) ((void)0)
#else
/*
 * In debug mode, AL_ASSERT() tests the condition, and generates
 * an abort with an error message when the condition fails.
 */
#define AL_ASSERT(condition, message)                                          \
  /* ((condition) ? (void)0                                                       \
                : _ALAssertFailure(#condition, __FILE__, __LINE__, message)) */

#endif

#endif
