#include <utils/os/Shell.h>

#include <windows.h>

#include <utils/os/AppFrame.h>
#include <utils/os/Log.h>
#include <utils/String.h>

String ShellFileSelectWindow(String filter, int flags){
    OPENFILENAMEA ofn;
    char buffer[DEFAULT_STRING_LENGTH + 1];
    AppFrame* frame = AppFrame::GetLocalInst();
    
    buffer[DEFAULT_STRING_LENGTH] = '\0';

    RtlZeroMemory(&ofn, sizeof(OPENFILENAMEA));
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = (frame == NULL ? NULL : frame->hWnd);
    // 结尾为两个'\0'，每两个字符串构成描述、过滤对，增加过滤模板可用';'分隔多个模板
    ofn.lpstrFilter = filter.GetString();
    ofn.lpstrInitialDir = "./";
    ofn.lpstrFile = buffer;
    ofn.nMaxFile = DEFAULT_STRING_LENGTH;
    ofn.nFilterIndex = 0;
    //标志如果是多选要加上OFN_ALLOWMULTISELECT 
    ofn.Flags = flags;

    if (!GetOpenFileNameA(&ofn)){
        return String();
    }

    if (*buffer == '\0'){
        return String();
    }

    return String(buffer);
}

WString ShellFileSelectWindow(WString filter, int flags){
    OPENFILENAMEW ofn;
    wchar_t buffer[DEFAULT_STRING_LENGTH + 1];
    AppFrame* frame = AppFrame::GetLocalInst();
    
    buffer[DEFAULT_STRING_LENGTH] = L'\0';

    RtlZeroMemory(&ofn, sizeof(OPENFILENAMEW));
    ofn.lStructSize = sizeof(OPENFILENAMEW);
    ofn.hwndOwner = (frame == NULL ? NULL : frame->hWnd);
    // 结尾为两个'\0'，每两个字符串构成描述、过滤对，增加过滤模板可用';'分隔多个模板
    ofn.lpstrFilter = filter.GetString();
    ofn.lpstrInitialDir = L"./";
    ofn.lpstrFile = buffer;
    ofn.nMaxFile = DEFAULT_STRING_LENGTH;
    ofn.nFilterIndex = 0;
    //标志如果是多选要加上OFN_ALLOWMULTISELECT 
    ofn.Flags = flags;

    if (!GetOpenFileNameW(&ofn)){
        return WString();
    }

    if (*buffer == L'\0'){
        return WString();
    }

    return WString(buffer);
}

bool ShellCommandLine(String s){
    DWORD_PTR res;
    size_t argsIdx = s.FindChar(' ');
    String args;

    if (argsIdx != -1){
        args = s.SubString(argsIdx + 1);
        s = s.SubString(0, argsIdx);

        DebugLog("[Shell] %s", s.GetString());
        DebugLog("[Shell] Arguments %s", args.GetString());
    }else{
        DebugLog("[Shell] %s", s.GetString());
    }

    res = (DWORD_PTR)ShellExecuteA(NULL, "open", s.GetString(), (args.GetLength() == 0 ? NULL : args.GetString()), NULL, SW_HIDE);
    
    if (res >= 32){
        return true;
    }else{
        DebugError("[Shell] Failed %d", res);
        return false;
    }
}

bool ShellCommandLine(WString s){
    DWORD_PTR res;
    size_t argsIdx = s.FindChar(L' ');
    WString args;

    if (argsIdx != -1){
        args = s.SubString(argsIdx + 1);
        s = s.SubString(0, argsIdx);

        DebugLog("[Shell] %S", s.GetString());
        DebugLog("[Shell] Arguments %S", args.GetString());
    }else{
        DebugLog("[Shell] %S", s.GetString());
    }

    res = (DWORD_PTR)ShellExecuteW(NULL, L"open", s.GetString(), (args.GetLength() == 0 ? NULL : args.GetString()), NULL, SW_HIDE);
    
    if (res >= 32){
        return true;
    }else{
        DebugError("[Shell] Failed %d", res);
        return false;
    }
}

bool ShellFFmpeg(String src, String dst){
    String cmd = String(".\\lib\\ffmpeg\\ffmpeg.exe -i \"") + src + "\" -y " + dst;

    if (!ShellCommandLine(cmd)){
        return false;
    }

    for (int i = 0; i < 50; i++){
        HANDLE hFile = CreateFileA(
            dst.GetString(),
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

bool ShellFFmpeg(WString src, WString dst){
    WString cmd = WString(L".\\lib\\ffmpeg\\ffmpeg.exe -i \"") + src + L"\" -y " + dst;

    if (!ShellCommandLine(cmd)){
        return false;
    }

    for (int i = 0; i < 50; i++){
        HANDLE hFile = CreateFileW(
            dst.GetString(),
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

int ShellMsgBox(String caption, String text){
    AppFrame* frame = AppFrame::GetLocalInst();
    return MessageBoxA((frame == NULL ? NULL : frame->hWnd), text.GetString(), caption.GetString(), MB_YESNOCANCEL | MB_ICONINFORMATION);
}

int ShellMsgBox(WString caption, WString text){
    AppFrame* frame = AppFrame::GetLocalInst();
    return MessageBoxW((frame == NULL ? NULL : frame->hWnd), text.GetString(), caption.GetString(), MB_YESNOCANCEL | MB_ICONINFORMATION);
}