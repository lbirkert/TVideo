#ifndef OS

#if __SIZEOF_POINTER__ == 8
    #define ARCH "x86_64"
#else
    #define ARCH "x86"
#endif

#if defined(_WIN32)
    #define OS "windows"
#elif defined(__linux__)
    #define OS "linux"
#elif defined(__APPLE__)
    #define OS "macos"
#elif defined(__ANDROID__)
    #define OS "android"
#elif defined(__FreeBSD__)
    #define OS "freebsd"
#else
    #define OS "unknown"
#endif

#endif