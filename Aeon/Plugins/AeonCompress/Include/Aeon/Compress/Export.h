// GENERATED BY CMAKE

#ifndef AEON_COMPRESS_EXPORT_H
#define AEON_COMPRESS_EXPORT_H

#ifdef AeonCompress_BUILT_AS_STATIC
#  define AEON_COMPRESS_EXPORT
#  define AEON_COMPRESS_NO_EXPORT
#else
#  ifndef AEON_COMPRESS_EXPORT
#    ifdef AeonCompress_EXPORTS
// We are building this library
#      define AEON_COMPRESS_EXPORT __declspec(dllexport)
#    else
// We are using this library
#      define AEON_COMPRESS_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef AEON_COMPRESS_NO_EXPORT
#    define AEON_COMPRESS_NO_EXPORT 
#  endif
#endif

#ifndef AEON_COMPRESS_DEPRECATED
#  define AEON_COMPRESS_DEPRECATED __declspec(deprecated)
#  define AAEON_COMPRESS_DEPRECATED_EXPORT AEON_COMPRESS_EXPORT __declspec(deprecated)
#  define AEON_COMPRESS_DEPRECATED_NO_EXPORT AEON_COMPRESS_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define AEON_COMPRESS_NO_DEPRECATED
#endif

#endif
