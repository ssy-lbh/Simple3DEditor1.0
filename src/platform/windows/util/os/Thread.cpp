#include <util/os/Thread.h>

#include <windows.h>

#include <util/os/Log.h>

namespace simple3deditor {

void ThreadUtils::ExitThread(int code){
    ::ExitThread(code);
}

void ThreadUtils::ExitProcess(int code){
    ::ExitProcess(code);
}

ulong ThreadLocal::tlsIndex[THREAD_LOCAL_SIZE] = {TLS_OUT_OF_INDEXES};

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

}