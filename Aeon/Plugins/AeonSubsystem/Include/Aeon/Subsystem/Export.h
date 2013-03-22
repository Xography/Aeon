// GENERATED BY CMAKE

#ifndef AEON_SUBSYSTEM_EXPORT_H
#define AEON_SUBSYSTEM_EXPORT_H

#ifdef AeonSubsystem_BUILT_AS_STATIC
#  define AEON_SUBSYSTEM_EXPORT
#  define AEON_SUBSYSTEM_NO_EXPORT
#else
#  ifndef AEON_SUBSYSTEM_EXPORT
#    ifdef AeonSubsystem_EXPORTS
// We are building this library
#      define AEON_SUBSYSTEM_EXPORT __declspec(dllexport)
#    else
// We are using this library
#      define AEON_SUBSYSTEM_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef AEON_SUBSYSTEM_NO_EXPORT
#    define AEON_SUBSYSTEM_NO_EXPORT 
#  endif
#endif

#ifndef AEON_SUBSYSTEM_DEPRECATED
#  define AEON_SUBSYSTEM_DEPRECATED __declspec(deprecated)
#  define AEON_SUBSYSTEM_DEPRECATED_EXPORT AEON_SUBSYSTEM_EXPORT __declspec(deprecated)
#  define AEON_SUBSYSTEM_DEPRECATED_NO_EXPORT AEON_SUBSYSTEM_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define AEON_SUBSYSTEM_NO_DEPRECATED
#endif

#endif
