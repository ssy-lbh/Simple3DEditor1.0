#include "shell.h"

#include <stdio.h>

#include <windows.h>

#include "main.h"
#include "log.h"

bool ShellFileSelectWindowA(HWND hWnd, char* buffer, size_t len, const char* lpstrFilter, DWORD flags){
    OPENFILENAMEA ofn;

    RtlZeroMemory(&ofn, sizeof(OPENFILENAMEA));
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = hWnd;
    // 结尾为两个'\0'，每两个字符串构成描述、过滤对，增加过滤模板可用';'分隔多个模板
    ofn.lpstrFilter = lpstrFilter;
    ofn.lpstrInitialDir = "./";
    ofn.lpstrFile = buffer;
    ofn.nMaxFile = len;
    ofn.nFilterIndex = 0;
    //标志如果是多选要加上OFN_ALLOWMULTISELECT 
    ofn.Flags = flags;
    if (!GetOpenFileNameA(&ofn)){
        return false;
    }
    if (*buffer == '\0'){
        return false;
    }
    return true;
}

bool ShellFileSelectWindowW(HWND hWnd, wchar_t* buffer, size_t len, const wchar_t* lpstrFilter, DWORD flags){
    OPENFILENAMEW ofn;

    RtlZeroMemory(&ofn, sizeof(OPENFILENAMEW));
    ofn.lStructSize = sizeof(OPENFILENAMEW);
    ofn.hwndOwner = hWnd;
    // 结尾为两个'\0'，每两个字符串构成描述、过滤对，增加过滤模板可用';'分隔多个模板
    ofn.lpstrFilter = lpstrFilter;
    ofn.lpstrInitialDir = L"./";
    ofn.lpstrFile = buffer;
    ofn.nMaxFile = len;
    ofn.nFilterIndex = 0;
    //标志如果是多选要加上OFN_ALLOWMULTISELECT 
    ofn.Flags = flags;
    if (!GetOpenFileNameW(&ofn)){
        return false;
    }
    if (*buffer == '\0'){
        return false;
    }
    return true;
}

bool ShellCommandLineA(const char* lpStr){
    DWORD_PTR res;
    const char* args = strchr(lpStr, ' ');
    if (args){
        char file[MAX_PATH];
        size_t len = (args++) - lpStr;

        strncpy_s(file, lpStr, len);
        DebugLog("[Shell] %s", file);
        DebugLog("[Shell] Arguments %s", args);

        res = (DWORD_PTR)ShellExecuteA(Main::hWnd, "open", file, args, NULL, SW_HIDE);
        if (res >= 32){
            return true;
        }else{
            DebugError("[Shell] Failed %d", res);
            return false;
        }
    }
    DebugLog("[Shell] %s", lpStr);

    res = (DWORD_PTR)ShellExecuteA(Main::hWnd, "open", lpStr, NULL, NULL, SW_HIDE);
    if (res >= 32){
        return true;
    }else{
        DebugError("[Shell] Failed %d", res);
        return false;
    }
}

bool ShellCommandLineW(const wchar_t* lpStr){
    DWORD_PTR res;
    const wchar_t* args = wcschr(lpStr, L' ');
    if (args){
        wchar_t file[MAX_PATH];
        size_t len = (args++) - lpStr;

        wcsncpy_s(file, lpStr, len);
        DebugLog("[Shell] %S", file);
        DebugLog("[Shell] Arguments %S", args);

        res = (DWORD_PTR)ShellExecuteW(Main::hWnd, L"open", file, args, NULL, SW_HIDE);
        if (res >= 32){
            return true;
        }else{
            DebugError("[Shell] Failed %d", res);
            return false;
        }
    }
    DebugLog("[Shell] %S", lpStr);
    
    res = (DWORD_PTR)ShellExecuteW(Main::hWnd, L"open", lpStr, NULL, NULL, SW_HIDE);
    if (res >= 32){
        return true;
    }else{
        DebugError("[Shell] Failed %d", res);
        return false;
    }
}

bool ShellFFmpegA(const char* source, const char* target){
    char cmd[(MAX_PATH << 1) + 10];
    __builtin_snprintf(cmd, (MAX_PATH << 1) + 10, ".\\ffmpeg\\ffmpeg.exe -i \"%s\" -y \"%s\"", source, target);
    if (!ShellCommandLineA(cmd)){
        return false;
    }
    for (int i = 0; i < 50; i++){
        HANDLE hFile = CreateFileA(
            target,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        if (hFile != INVALID_HANDLE_VALUE){
            CloseHandle(hFile);
            return true;
        }
        Sleep(40);
    }
    return false;
}

bool ShellFFmpegW(const wchar_t* source, const wchar_t* target){
    wchar_t cmd[(MAX_PATH << 1) + 10];
    __mingw_snwprintf(cmd, (MAX_PATH << 2) + 10, L".\\ffmpeg\\ffmpeg.exe -i \"%S\" -y \"%S\"", source, target);
    if (!ShellCommandLineW(cmd)){
        return false;
    }
    for (int i = 0; i < 50; i++){
        HANDLE hFile = CreateFileW(
            target,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        if (hFile != INVALID_HANDLE_VALUE){
            CloseHandle(hFile);
            return true;
        }
        Sleep(40);
    }
    return false;
}