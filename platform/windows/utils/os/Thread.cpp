#include <utils/os/Thread.h>

#include <windows.h>

#include <utils/os/Log.h>

void ThreadUtils::ExitThread(int code){
    ::ExitThread(code);
}

void ThreadUtils::ExitProcess(int code){
    ::ExitProcess(code);
}

Thread::Thread(int(*func)(void*), void* user) : func(func), user(user) {}
Thread::~Thread(){}

bool Thread::Start(){
    if (isStarted())
        return false;
    ThreadData* data = new ThreadData;
    data->func = func;
    data->user = user;
    hThread = CreateThread(NULL, 0, Thread::RunThread, data, 0, NULL);
    CloseHandle(hThread);
    return isStarted();
}

bool Thread::isStarted() const{
    return hThread != INVALID_HANDLE_VALUE;
}

bool Thread::Join(){
    return WaitForSingleObject(hThread, INFINITE) == WAIT_OBJECT_0;
}

DWORD CALLBACK Thread::RunThread(LPVOID data){
    ThreadData* th = (ThreadData*)data;
    DWORD ret = th->func(th->user);
    delete th;
    return ret;
}

DWORD ThreadLocal::tlsIndex[THREAD_LOCAL_SIZE] = {TLS_OUT_OF_INDEXES};

bool ThreadLocal::Alloc(int idx){
    if (idx >= THREAD_LOCAL_SIZE){
        DebugError("ThreadLocal::Alloc %d When Max Size Is %d", idx, THREAD_LOCAL_SIZE);
        return false;
    }
    tlsIndex[idx] = TlsAlloc();
    return tlsIndex[idx] != TLS_OUT_OF_INDEXES;
}

bool ThreadLocal::Free(int idx){
    if (idx >= THREAD_LOCAL_SIZE){
        DebugError("ThreadLocal::Free %d When Max Size Is %d", idx, THREAD_LOCAL_SIZE);
        return false;
    }
    return TlsFree(tlsIndex[idx]);
}

void ThreadLocal::Set(int idx, void* val){
    TlsSetValue(tlsIndex[idx], val);
}

void* ThreadLocal::Get(int idx){
    return TlsGetValue(tlsIndex[idx]);
}