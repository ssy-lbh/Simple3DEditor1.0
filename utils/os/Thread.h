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

class Thread final : public Object {
private:
#ifdef PLATFORM_WINDOWS
    handle hThread = (handle)(-1);

    struct ThreadData {
        int(*func)(void*);
        void* user;
    };

    static ulong __stdcall RunThread(void* data);
#endif

    int(*func)(void*);
    void* user;

public:
    Thread(int(*func)(void*), void* user);
    ~Thread();

    bool Start();
    bool isStarted() const;
    bool Join();
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

class ThreadPool final : public Object {
    
};

#endif