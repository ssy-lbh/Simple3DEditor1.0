#include <utils/os/Log.h>

#include <stdio.h>

#include <windows.h>

int DebugLog(const char* format, ...){
    __builtin_va_list list;
    __builtin_va_start(list, format);
    int ret = __builtin_vprintf(format, list);
    __builtin_va_end(list);
    __builtin_putchar('\n');
    return ret;
}

int DebugLog(const wchar_t* format, ...){
    __builtin_va_list list;
    __builtin_va_start(list, format);
    int ret = __mingw_wprintf(format, list);
    __builtin_va_end(list);
    __builtin_putchar('\n');
    return ret;
}

int DebugError(const char* format, ...){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, FOREGROUND_RED);
    __builtin_va_list list;
    __builtin_va_start(list, format);
    int ret = __builtin_vfprintf(stderr, format, list);
    __builtin_va_end(list);
    __builtin_putchar('\n');
    SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    return ret;
}

int DebugError(const wchar_t* format, ...){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, FOREGROUND_RED);
    __builtin_va_list list;
    __builtin_va_start(list, format);
    int ret = __mingw_wprintf(format, list);
    __builtin_va_end(list);
    __builtin_putchar('\n');
    SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    return ret;
}