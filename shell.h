#ifndef __SHELL__
#define __SHELL__

#include "define.h"

#include <windef.h>

struct InputAttribute {
    enum Type {
        INT,
        FLOAT,
        STRING
    } type;
    union {
        int intValue;
        float floatValue;
        const char* strValue;// Flush After Use
    };
};

// 现在此代码暂时弃用，等待bug修复或删除
bool ShellEInputWindow(InputAttribute* attrs, int cnt);
bool ShellFileSelectWindowA(HWND hWnd, char* buffer, size_t len, const char* lpstrFilter, DWORD flags);
bool ShellFileSelectWindowW(HWND hWnd, wchar_t* buffer, size_t len, const wchar_t* lpstrFilter, DWORD flags);
bool ShellCommandLineA(const char* lpStr);
bool ShellCommandLineW(const wchar_t* lpStr);

#endif