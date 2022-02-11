#ifndef __SHELL__
#define __SHELL__

#include "define.h"

#include <windef.h>

bool ShellFileSelectWindowA(HWND hWnd, char* buffer, size_t len, const char* lpstrFilter, DWORD flags);
bool ShellFileSelectWindowW(HWND hWnd, wchar_t* buffer, size_t len, const wchar_t* lpstrFilter, DWORD flags);
bool ShellCommandLineA(const char* lpStr);
bool ShellCommandLineW(const wchar_t* lpStr);
bool ShellFFmpegA(const char* source, const char* target);
bool ShellFFmpegW(const wchar_t* source, const wchar_t* target);

#endif