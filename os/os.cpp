#include "os.h"

#if defined(LINUX) || defined(__LINUX__) || defined(__linux__)
#include "linux/dlfunc.cpp"
#include "linux/threads.cpp"
#include "linux/network.cpp"
#include "linux/libpath.cpp"
#else
#if defined(__FreeBSD__)
#include "linux/dlfunc.cpp"
#include "freebsd/threads.cpp"
#include "linux/network.cpp"
#include "linux/libpath.cpp"
#else
#if defined(__WIN32__)
#include "windows/dlfunc.cpp"
#include "windows/threads.cpp"
#include "windows/network.cpp"
#include "windows/libpath.cpp"
#else
#error This operating system is not defined
#endif
#endif
#endif
