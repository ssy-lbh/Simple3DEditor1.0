#ifndef __UTILS_OS_SYSTEM__
#define __UTILS_OS_SYSTEM__

//! 注意，此文件夹仅供平台相关代码使用

#ifdef PLATFORM_WINDOWS
#include <windows.h>

HMODULE GetModule();
#endif

#endif