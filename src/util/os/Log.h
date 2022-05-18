#ifndef __UTIL_OS_LOG__
#define __UTIL_OS_LOG__

namespace simple3deditor {

int DebugLog(const char* format, ...);
int DebugLog(const wchar_t* format, ...);
int DebugWarning(const char* format, ...);
int DebugWarning(const wchar_t* format, ...);
int DebugError(const char* format, ...);
int DebugError(const wchar_t* format, ...);

}

#endif