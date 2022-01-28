#include "shell.h"

#include <windows.h>

#include "main.h"
#include "log.h"

bool ShellEInputWindow(InputAttribute* attrs, int cnt){
    char buf[MAX_PATH + 1];
    int len;
    buf[MAX_PATH] = '\0';
    STARTUPINFO stInfo;
    PROCESS_INFORMATION psInfo;
    HANDLE hAttr = CreateFile(
        ".\\utility\\main.bin",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hAttr == INVALID_HANDLE_VALUE){
        DebugError("File Not Found");
        return ERROR_FILE_NOT_FOUND;
    }
    len = __builtin_snprintf(buf, MAX_PATH, "%d{", cnt);
    WriteFile(hAttr, buf, len, NULL, NULL);
    for (int i = 0; i < cnt; i++){
        switch (attrs[i].type){
        case InputAttribute::Type::INT:
            len = __builtin_snprintf(buf, MAX_PATH, "[%d|%d]", 0, attrs[i].intValue);
            WriteFile(hAttr, buf, len, NULL, NULL);
            break;
        case InputAttribute::Type::FLOAT:
            len = __builtin_snprintf(buf, MAX_PATH, "[%d|%f]", 1, attrs[i].floatValue);
            WriteFile(hAttr, buf, len, NULL, NULL);
            break;
        case InputAttribute::Type::STRING:
            len = __builtin_snprintf(buf, MAX_PATH, "[%d|%s]", 2, attrs[i].strValue);
            WriteFile(hAttr, buf, len, NULL, NULL);
            break;
        }
    }
    WriteFile(hAttr, "}", 1, NULL, NULL);
    GetCurrentDirectory(MAX_PATH, buf);
    //GetStartupInfo(&stInfo);
    RtlZeroMemory(&stInfo, sizeof(STARTUPINFO));
    WINBOOL suc = CreateProcess(".\\utility\\input.exe", NULL, NULL, NULL, FALSE, 0, NULL, buf, &stInfo, &psInfo);
    if (!suc){
        CloseHandle(hAttr);
        return false;
    }
    WaitForSingleObject(psInfo.hProcess, INFINITE);
    CloseHandle(psInfo.hProcess);
    CloseHandle(psInfo.hThread);
    //TODO 工具程序修好再写输出解析
    CloseHandle(hAttr);
    return true;
}

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
    char* args = strchr(lpStr, ' ');
    if (args){
        *(args++) = '\0';
        return (DWORD_PTR)ShellExecuteA(Main::hWnd, "open", lpStr, args, NULL, SW_HIDE) >= 32;
    }
    return (DWORD_PTR)ShellExecuteA(Main::hWnd, "open", lpStr, NULL, NULL, SW_HIDE) >= 32;
}

bool ShellCommandLineW(const wchar_t* lpStr){
    wchar_t* args = wcschr(lpStr, ' ');
    if (args){
        *(args++) = L'\0';
        return (DWORD_PTR)ShellExecuteW(Main::hWnd, L"open", lpStr, args, NULL, SW_HIDE) >= 32;
    }
    return (DWORD_PTR)ShellExecuteW(Main::hWnd, L"open", lpStr, NULL, NULL, SW_HIDE) >= 32;
}