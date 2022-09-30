#include <util/os/Log.h>

#include <stdio.h>

#include <windows.h>

#include <util/os/Font.h>

#include <lib/imgui/imgui.h>

namespace simple3deditor {

int DebugLog(const char* format, ...){
    va_list list;
    va_start(list, format);
    int ret = vprintf(format, list);
    va_end(list);
    putchar('\n');
    return ret;
}

int DebugLog(const wchar_t* format, ...){
    va_list list;
    va_start(list, format);
    int ret = vwprintf(format, list);
    va_end(list);
    putchar('\n');
    return ret;
}

int DebugWarning(const char* format, ...){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN);
    va_list list;
    va_start(list, format);
    int ret = vfprintf(stderr, format, list);
    va_end(list);
    putchar('\n');
    SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    return ret;
}

int DebugWarning(const wchar_t* format, ...){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN);
    va_list list;
    va_start(list, format);
    int ret = vwprintf(format, list);
    va_end(list);
    putchar('\n');
    SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    return ret;
}

int DebugError(const char* format, ...){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, FOREGROUND_RED);
    va_list list;
    va_start(list, format);
    int ret = vfprintf(stderr, format, list);
    va_end(list);
    putchar('\n');
    SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    return ret;
}

int DebugError(const wchar_t* format, ...){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, FOREGROUND_RED);
    va_list list;
    va_start(list, format);
    int ret = vwprintf(format, list);
    va_end(list);
    putchar('\n');
    SetConsoleTextAttribute(hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    return ret;
}

}