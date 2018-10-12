#ifndef _ARCLIB_H
#define _ARCLIB_H

/*
 * Want to customize the workings of our library?  Just define AL_CUSTOM,
 * and then create your own personal version of ALCUSTOM.H. (No, we don't
 * ship a copy of this header file with the library, it is for you to
 * define.  This is a really good way to use products like MEMCHECK or
 * SmartHeap that want to insert an included file in every one of
 * our source files.
 */

#if defined(AL_CUSTOM)
#include "alcustom.h"
#endif

#if defined(__cplusplus)

/* All these includes needed for various library features */

#ifndef BINDGEN
#include <iostream>
#include "string.h"
#endif

#include "aldefs.h"
#include "_debug.h"

/* Base classes */

#include "status.h"
#include "stor.h"
#include "cmpengn.h"

#if defined(AL_SUN4) && defined(AL_GCC)

extern "C" int strcasecmp(const char *s1, const char *s2);

#endif

#endif /* #if defined( __cplusplus ) */

#endif /* ARCLIB_H */
