#include <util/os/Log.h>

#include <stdio.h>

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
    printf("\033[0;33m");
    va_list list;
    va_start(list, format);
    int ret = vfprintf(stderr, format, list);
    va_end(list);
    putchar('\n');
    printf("\033[0m");
    return ret;
}

int DebugWarning(const wchar_t* format, ...){
    printf("\033[0;33m");
    va_list list;
    va_start(list, format);
    int ret = vwprintf(format, list);
    va_end(list);
    putchar('\n');
    printf("\033[0m");
    return ret;
}

int DebugError(const char* format, ...){
    printf("\033[0;31m");
    va_list list;
    va_start(list, format);
    int ret = vfprintf(stderr, format, list);
    va_end(list);
    putchar('\n');
    printf("\033[0m");
    return ret;
}

int DebugError(const wchar_t* format, ...){
    printf("\033[0;31m");
    va_list list;
    va_start(list, format);
    int ret = vwprintf(format, list);
    va_end(list);
    putchar('\n');
    printf("\033[0m");
    return ret;
}

}