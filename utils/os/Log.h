#ifndef __UTILS_OS_LOG__
#define __UTILS_OS_LOG__

int DebugLog(const char* format, ...);
int DebugLog(const wchar_t* format, ...);
int DebugError(const char* format, ...);
int DebugError(const wchar_t* format, ...);

#endif