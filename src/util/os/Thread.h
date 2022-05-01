#ifndef __UTILS_OS_THREAD__
#define __UTILS_OS_THREAD__

#include <define.h>

#define THREAD_LOCAL_SIZE 20

#define THREAD_LOCAL_APPFRAME 0
#define THREAD_LOCAL_LOCALDATA 1
#define THREAD_LOCAL_VIEWMGR 2

class ThreadUtils final {
public:
    static void ExitThread(int code);
    static void ExitProcess(int code);
};

class ThreadLocal final {
private:
#ifdef PLATFORM_WINDOWS
    static ulong tlsIndex[THREAD_LOCAL_SIZE];
#endif

public:
    // 前十索引保留使用
    static bool Alloc(int idx);
    static bool Free(int idx);
    static void Set(int idx, void* val);
    static void* Get(int idx);
};

#endif