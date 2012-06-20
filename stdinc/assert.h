// C11 7.2
#pragma once

#if defined(NDEBUG)
#  define assert(ignore) ((void)0)
#elif defined(XV6_USER)
#  define assert(c)                                                     \
  ((c) ? ((void)0)                                                      \
   : die("Assertion failed: %s, function %s, file %s, line %d",         \
         #c, __PRETTY_FUNCTION__, __FILE__, __LINE__))
#elif defined(XV6_KERNEL) || defined(LWIP)
#  define assert(c)                                                     \
  ((c) ? ((void)0)                                                      \
   : panic("Assertion failed: %s, function %s, file %s, line %d",       \
           #c, __PRETTY_FUNCTION__, __FILE__, __LINE__))
#endif

#ifndef __cplusplus
#define static_assert _Static_assert
#endif
