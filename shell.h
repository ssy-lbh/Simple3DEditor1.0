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
bool ShellFileSelectWindow(HWND hWnd, wchar_t* buffer, size_t len, const wchar_t* lpstrFilter, DWORD flags);

#endif