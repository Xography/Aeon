// GENERATED BY CMAKE

#ifndef AEON_XML_EXPORT_H
#define AEON_XML_EXPORT_H

#ifdef AeonXML_BUILT_AS_STATIC
#  define AEON_XML_EXPORT
#  define AEON_XML_NO_EXPORT
#else
#  ifndef AEON_XML_EXPORT
#    ifdef AeonXML_EXPORTS
// We are building this library
#      define AEON_XML_EXPORT __declspec(dllexport)
#    else
// We are using this library
#      define AEON_XML_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef AEON_XML_NO_EXPORT
#    define AEON_XML_NO_EXPORT 
#  endif
#endif

#ifndef AEON_XML_DEPRECATED
#  define AEON_XML_DEPRECATED __declspec(deprecated)
#  define AEON_XML_DEPRECATED_EXPORT AEON_XML_EXPORT __declspec(deprecated)
#  define AEON_XML_DEPRECATED_NO_EXPORT AEON_XML_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define AEON_XML_NO_DEPRECATED
#endif

#endif