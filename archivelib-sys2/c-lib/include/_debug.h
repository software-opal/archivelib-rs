#ifndef __DEBUG_H
#define __DEBUG_H
/*
 * _ALAssertFailure is the function called by AL_ASSERT() and
 * AL_ASSERT_OBJECT() when their assertion fails.
 */
void _ALAssertFailure(const char *condition, const char *filename, int line,
                      const char *message, ...);
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
  ((condition) ? (void)0                                                       \
               : _ALAssertFailure(#condition, __FILE__, __LINE__, message))

#endif

/*
 * The AL_CLASS_TAG() macro assigns a new debug class and data
 * member to each of the classes in ArchiveLib.  Each of these
 * debug classes uses a special integer tag (stored in the data member)
 * to uniquely identify itself.  These are the integer values of
 * these integers.
 */

enum _ALClassTags {
  _ALDeletedObjectTag = 0,
  _ALStorageTag,
  _ALFileTag,
  _ALMemoryTag,
  _ALEntryTag,
  _ALEntryListTag,
  _ALArchiveBaseTag,
  _ALArchiveTag,
  _ALMonitorTag,
  _ALBarGraphTag,
  _ALSpinnerTag,
  _ALWindowsMessageTag,
  _ALCompressionEngineTag,
  _ALCopyEngineTag,
  _ALGreenleafEngineTag,
  _ALCompressedObjectTag,
  _ALNameTag,
  _ALWildCardExpanderTag,
};

/*
 * AL_CLASS_TAG( x ) is a macro that is used to help debug
 * ArchiveLib.  The insertion of this macro in a class definition
 * adds a new data member and member function to the class.  The
 * data member is an object of a class uniquely created by the
 * macro.  The reason the data member is a class object instead
 * of a simple integer or character tag is this: By making it a
 * class object, we can automatically assign it a valid value
 * when constructed, and an invalid value when destroyed.
 *
 * The member function added to the class is called GoodTag().
 * Once you have added AL_CLASS_TAG( x ) to your class definition,
 * you can call object.GoodTag() anytime you want.  It will return
 * a true value only if the data member has the correct value,
 *
 * We make use of this function in AL_ASSERT_OBJECT().  It
 * checks the value of this object frequently in member functions
 * and destructors, generating an assertion failure if the object
 * doesn't look like the correct type.
 *
 * Note that the ASSERT_OBJECT() statements generate no code when the
 * library is compiled with NDEBUG, so this class will not be
 * generating much low overhead.  However, the data member will
 * still be taking up a single byte in each instance.
 *
 * If you want to eliminate class tags, this line in will do it
 * You will save one byte per instance.  The best way to accomplish this
 * is to define the macro in ALCUSTOM.H, then rebuild the library with
 * macro AL_CUSTOM defined in your project.  After you build this new
 * version of the library, you must absolutely, positively, be sure
 * that you continue to use AL_CUSTOM and ALCUSTOM.H when working
 * with the library.  If you don't, your library and your application
 * will think that most classes in ArchiveLib are different sizes, and
 * *nothing* will work.
 *
 *#define AL_CLASS_TAG( x ) int GoodTag(){ return 1; }
 */
#define AL_CLASS_TAG(x)                                                        \
  int GoodTag() { return 1; }

#endif
