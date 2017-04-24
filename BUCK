def merge_dicts(x, y):
  z = x.copy()
  z.update(y)
  return z

CONFIG_H = """
#ifndef CMARK_CONFIG_H
#define CMARK_CONFIG_H

#ifdef __cplusplus
extern \\"C\\" {
#endif

#define HAVE_STDBOOL_H

#ifdef HAVE_STDBOOL_H
  #include <stdbool.h>
#elif !defined(__cplusplus)
  typedef char bool;
#endif

#define HAVE___BUILTIN_EXPECT

#define HAVE___ATTRIBUTE__

#ifdef HAVE___ATTRIBUTE__
  #define CMARK_ATTRIBUTE(list) __attribute__ (list)
#else
  #define CMARK_ATTRIBUTE(list)
#endif

#ifndef CMARK_INLINE
  #if defined(_MSC_VER) && !defined(__cplusplus)
    #define CMARK_INLINE __inline
  #else
    #define CMARK_INLINE inline
  #endif
#endif

/* snprintf and vsnprintf fallbacks for MSVC before 2015,
   due to Valentin Milea http://stackoverflow.com/questions/2915672/
*/

#if defined(_MSC_VER) && _MSC_VER < 1900

#include <stdio.h>
#include <stdarg.h>

#define snprintf c99_snprintf
#define vsnprintf c99_vsnprintf

CMARK_INLINE int c99_vsnprintf(char *outBuf, size_t size, const char *format, va_list ap)
{
    int count = -1;

    if (size != 0)
        count = _vsnprintf_s(outBuf, size, _TRUNCATE, format, ap);
    if (count == -1)
        count = _vscprintf(format, ap);

    return count;
}

CMARK_INLINE int c99_snprintf(char *outBuf, size_t size, const char *format, ...)
{
    int count;
    va_list ap;

    va_start(ap, format);
    count = c99_vsnprintf(outBuf, size, format, ap);
    va_end(ap);

    return count;
}

#endif

#ifdef __cplusplus
}
#endif

#endif
"""

CMARK_EXPORT_H = """
#ifndef CMARK_EXPORT_H
#define CMARK_EXPORT_H

#ifdef CMARK_STATIC_DEFINE
#  define CMARK_EXPORT
#  define CMARK_NO_EXPORT
#else
#  ifndef CMARK_EXPORT
#    ifdef libcmark_EXPORTS
        /* We are building this library */
#      define CMARK_EXPORT __attribute__((visibility(\\"default\\")))
#    else
        /* We are using this library */
#      define CMARK_EXPORT __attribute__((visibility(\\"default\\")))
#    endif
#  endif

#  ifndef CMARK_NO_EXPORT
#    define CMARK_NO_EXPORT __attribute__((visibility(\\"hidden\\")))
#  endif
#endif

#ifndef CMARK_DEPRECATED
#  define CMARK_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef CMARK_DEPRECATED_EXPORT
#  define CMARK_DEPRECATED_EXPORT CMARK_EXPORT CMARK_DEPRECATED
#endif

#ifndef CMARK_DEPRECATED_NO_EXPORT
#  define CMARK_DEPRECATED_NO_EXPORT CMARK_NO_EXPORT CMARK_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef CMARK_NO_DEPRECATED
#    define CMARK_NO_DEPRECATED
#  endif
#endif

#endif

"""

CMARK_VERSION_H = """
#ifndef CMARK_VERSION_H
#define CMARK_VERSION_H

#define CMARK_VERSION ((0 << 16) | (27 << 8)  | 1)
#define CMARK_VERSION_STRING \\"0.27.1\\"

#endif
"""

CMARKEXTENSIONS_EXPORT_H = """
#ifndef CMARKEXTENSIONS_EXPORT_H
#define CMARKEXTENSIONS_EXPORT_H

#ifdef CMARKEXTENSIONS_STATIC_DEFINE
#  define CMARKEXTENSIONS_EXPORT
#  define CMARKEXTENSIONS_NO_EXPORT
#else
#  ifndef CMARKEXTENSIONS_EXPORT
#    ifdef libcmarkextensions_EXPORTS
        /* We are building this library */
#      define CMARKEXTENSIONS_EXPORT __attribute__((visibility(\\"default\\")))
#    else
        /* We are using this library */
#      define CMARKEXTENSIONS_EXPORT __attribute__((visibility(\\"default\\")))
#    endif
#  endif

#  ifndef CMARKEXTENSIONS_NO_EXPORT
#    define CMARKEXTENSIONS_NO_EXPORT __attribute__((visibility(\\"hidden\\")))
#  endif
#endif

#ifndef CMARKEXTENSIONS_DEPRECATED
#  define CMARKEXTENSIONS_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef CMARKEXTENSIONS_DEPRECATED_EXPORT
#  define CMARKEXTENSIONS_DEPRECATED_EXPORT CMARKEXTENSIONS_EXPORT CMARKEXTENSIONS_DEPRECATED
#endif

#ifndef CMARKEXTENSIONS_DEPRECATED_NO_EXPORT
#  define CMARKEXTENSIONS_DEPRECATED_NO_EXPORT CMARKEXTENSIONS_NO_EXPORT CMARKEXTENSIONS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef CMARKEXTENSIONS_NO_DEPRECATED
#    define CMARKEXTENSIONS_NO_DEPRECATED
#  endif
#endif

#endif
"""

genrule(
  name = 'config.h',
  out = 'config.h',
  cmd = 'echo "' + CONFIG_H + '" > $OUT',
)

genrule(
  name = 'cmark_export.h',
  out = 'cmark_export.h',
  cmd = 'echo "' + CMARK_EXPORT_H + '" > $OUT',
)

genrule(
  name = 'cmark_version.h',
  out = 'cmark_version.h',
  cmd = 'echo "' + CMARK_VERSION_H + '" > $OUT',
)

genrule(
  name = 'cmarkextensions_export.h',
  out = 'cmarkextensions_export.h',
  cmd = 'echo "' + CMARKEXTENSIONS_EXPORT_H + '" > $OUT',
)

cxx_library(
  name = 'cmark',
  header_namespace = '',
  exported_headers = merge_dicts(subdir_glob([
    ('src', '*.h'),
  ]), {
    'config.h': ':config.h',
    'cmark_export.h': ':cmark_export.h',
    'cmark_version.h': ':cmark_version.h',
    'cmarkextensions_export.h': ':cmarkextensions_export.h',
  }),
  srcs = glob([
    'src/*.c',
  ]),
  visibility = [
    'PUBLIC',
  ],
)
