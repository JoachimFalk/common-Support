// vim: set sw=2 ts=8:

#ifndef _INCLUDED_COSUPPORT_PORTABLE_SNPRINTF_H
#define _INCLUDED_COSUPPORT_PORTABLE_SNPRINTF_H

#define PORTABLE_SNPRINTF_VERSION_MAJOR 2
#define PORTABLE_SNPRINTF_VERSION_MINOR 1

#ifdef HAVE_SNPRINTF
#include <stdio.h>
#else /* !HAVE_SNPRINTF */
/* Allow the use in C++ code.  */
# ifdef __cplusplus
extern "C" {
# endif
extern int snprintf(char *, size_t, const char *, /*args*/ ...);
extern int vsnprintf(char *, size_t, const char *, va_list);
# ifdef __cplusplus
}
# endif
#endif /* HAVE_SNPRINTF */

/* Allow the use in C++ code.  */
#ifdef __cplusplus
extern "C" {
#endif

#if defined(HAVE_SNPRINTF) && defined(PREFER_PORTABLE_SNPRINTF)
extern int portable_snprintf(char *str, size_t str_m, const char *fmt, /*args*/ ...);
extern int portable_vsnprintf(char *str, size_t str_m, const char *fmt, va_list ap);
#define snprintf  portable_snprintf
#define vsnprintf portable_vsnprintf
#endif

extern int asprintf  (char **ptr, const char *fmt, /*args*/ ...);
extern int vasprintf (char **ptr, const char *fmt, va_list ap);
extern int asnprintf (char **ptr, size_t str_m, const char *fmt, /*args*/ ...);
extern int vasnprintf(char **ptr, size_t str_m, const char *fmt, va_list ap);

#ifdef __cplusplus
}
#endif

#endif // _INCLUDED_COSUPPORT_PORTABLE_SNPRINTF_H