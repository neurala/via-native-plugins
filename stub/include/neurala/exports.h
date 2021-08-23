
#ifndef NEURALA_PUBLIC_H
#define NEURALA_PUBLIC_H

#if defined(NEURALA_STATIC_LIB) || defined(__GNUC__)
#  define NEURALA_PUBLIC
#  define NEURALA_PRIVATE
#else
#  ifndef NEURALA_PUBLIC
#    ifdef NeuralaB4B_EXPORTS
        /* We are building this library */
#      define NEURALA_PUBLIC __declspec(dllexport)
#    else
        /* We are using this library */
#      define NEURALA_PUBLIC __declspec(dllimport)
#    endif
#  endif

#  ifndef NEURALA_PRIVATE
#    define NEURALA_PRIVATE 
#  endif
#endif

#ifndef NEURALA_DEPRECATED
#  define NEURALA_DEPRECATED __declspec(deprecated)
#endif

#ifndef NEURALA_DEPRECATED_EXPORT
#  define NEURALA_DEPRECATED_EXPORT NEURALA_PUBLIC NEURALA_DEPRECATED
#endif

#ifndef NEURALA_DEPRECATED_NO_EXPORT
#  define NEURALA_DEPRECATED_NO_EXPORT NEURALA_PRIVATE NEURALA_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef NEURALA_NOT_DEPRECATED
#    define NEURALA_NOT_DEPRECATED
#  endif
#endif

#endif /* NEURALA_PUBLIC_H */
