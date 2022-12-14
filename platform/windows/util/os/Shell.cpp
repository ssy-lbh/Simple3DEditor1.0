#include <util/os/Shell.h>

#include <util/os/AppFrame.h>
#include <util/os/Log.h>
#include <util/String.h>
#include <util/StringBuilder.h>

#include <windows.h>

#include <glfw/GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw/GLFW/glfw3native.h>

namespace simple3deditor {

String ShellFileSelectWindow(const String filter, int flags, bool save){
    OPENFILENAMEA ofn;
    char buffer[DEFAULT_STRING_LENGTH];
    AppFrame* frame = AppFrame::GetLocalInst();
    
    *buffer = '\0';

    RtlZeroMemory(&ofn, sizeof(OPENFILENAMEA));
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.hwndOwner = (frame == NULL ? NULL : glfwGetWin32Window(frame->GetWindow()));
    // 结尾为两个'\0'，每两个字符串构成描述、过滤对，增加过滤模板可用';'分隔多个模板
    ofn.lpstrFilter = filter.GetString();
    ofn.lpstrInitialDir = "./";
    ofn.lpstrFile = buffer;
    ofn.nMaxFile = DEFAULT_STRING_LENGTH;
    ofn.nFilterIndex = 0;
    //标志如果是多选要加上OFN_ALLOWMULTISELECT 
    ofn.Flags = flags | OFN_EXPLORER;

    if (save){
        if (!GetSaveFileNameA(&ofn) || *buffer == '\0'){
            SetCurrentDirectoryA(GetAppDirectoryA().GetString());
            return String();
        }
        if (ofn.nFileExtension == 0 && ofn.nFilterIndex > 0){
            DWORD cnt = ofn.nFilterIndex << 1;
            const char* p = ofn.lpstrFilter;
            const char* end;

            while (--cnt)
                p += (strlen(p) + 1);
            p++;
            SetCurrentDirectoryA(GetAppDirectoryA().GetString());
            if (*p != '.' || !strcmp(p, ".*"))
                return String(buffer);
            if (end = strchr(p, ';'))
                return String(buffer) + String(p, end - p);
            return String(buffer) + p;
        }
    }else{
        if (!GetOpenFileNameA(&ofn) || *buffer == '\0'){
            SetCurrentDirectoryA(GetAppDirectoryA().GetString());
            return String();
        }
    }

    // 防止文件选择框改变当前目录
    SetCurrentDirectoryA(GetAppDirectoryA().GetString());

    return String(buffer);
}

WString ShellFileSelectWindow(const WString filter, int flags, bool save){
    OPENFILENAMEW ofn;
    wchar_t buffer[DEFAULT_STRING_LENGTH];
    AppFrame* frame = AppFrame::GetLocalInst();
    
    *buffer = L'\0';

    RtlZeroMemory(&ofn, sizeof(OPENFILENAMEW));
    ofn.lStructSize = sizeof(OPENFILENAMEW);
    ofn.hwndOwner = (frame == NULL ? NULL : glfwGetWin32Window(frame->GetWindow()));
    // 结尾为两个'\0'，每两个字符串构成描述、过滤对，增加过滤模板可用';'分隔多个模板
    ofn.lpstrFilter = filter.GetString();
    ofn.lpstrInitialDir = L"./";
    ofn.lpstrFile = buffer;
    ofn.nMaxFile = DEFAULT_STRING_LENGTH;
    ofn.nFilterIndex = 0;
    //标志如果是多选要加上OFN_ALLOWMULTISELECT 
    ofn.Flags = flags | OFN_EXPLORER;

    if (save){
        if (!GetSaveFileNameW(&ofn) || *buffer == L'\0'){
            SetCurrentDirectoryW(GetAppDirectoryW().GetString());
            return WString();
        }
        if (ofn.nFileExtension == 0 && ofn.nFilterIndex > 0){
            DWORD cnt = ofn.nFilterIndex << 1;
            const wchar_t* p = ofn.lpstrFilter;
            const wchar_t* end;

            while (--cnt)
                p += (wcslen(p) + 1);
            p++;
            SetCurrentDirectoryW(GetAppDirectoryW().GetString());
            if (*p != L'.' || !wcscmp(p, L".*"))
                return WString(buffer);
            if (end = wcschr(p, L';'))
                return WString(buffer) + WString(p, end - p);
            return WString(buffer) + p;
        }
    }else{
        if (!GetOpenFileNameW(&ofn) || *buffer == L'\0'){
            SetCurrentDirectoryW(GetAppDirectoryW().GetString());
            return WString();
        }
    }

    // 防止文件选择框改变当前目录
    SetCurrentDirectoryW(GetAppDirectoryW().GetString());

    return WString(buffer);
}

bool ShellCommandLineArgs(const String s, const String args){
    DWORD_PTR res;

    DebugLog("[Shell] %s", s.GetString());
    DebugLog("[Shell] Arguments %s", args.GetString());

    res = (DWORD_PTR)ShellExecuteA(NULL, "open", s.GetString(), (args.GetLength() == 0 ? NULL : args.GetString()), NULL, SW_HIDE);
    
    if (res >= 32){
        return true;
    }else{
        DebugError("[Shell] Failed %d", res);
        return false;
    }
}

bool ShellCommandLineArgs(const WString s, const WString args){
    DWORD_PTR res;

    DebugLog("[Shell] %S", s.GetString());
    DebugLog("[Shell] Arguments %S", args.GetString());

    res = (DWORD_PTR)ShellExecuteW(NULL, L"open", s.GetString(), (args.GetLength() == 0 ? NULL : args.GetString()), NULL, SW_HIDE);
    
    if (res >= 32){
        return true;
    }else{
        DebugError("[Shell] Failed %d", res);
        return false;
    }
}

bool ShellCommandLine(const String s){
    DWORD_PTR res;
    size_t argsIdx = s.FindChar(' ');
    String cmd;
    String args;

    if (argsIdx != -1){
        args = s.SubString(argsIdx + 1);
        cmd = s.SubString(0, argsIdx);

        DebugLog("[Shell] %s", cmd.GetString());
        DebugLog("[Shell] Arguments %s", args.GetString());
    }else{
        DebugLog("[Shell] %s", cmd.GetString());
    }

    res = (DWORD_PTR)ShellExecuteA(NULL, "open", cmd.GetString(), (args.GetLength() == 0 ? NULL : args.GetString()), NULL, SW_HIDE);
    
    if (res >= 32){
        return true;
    }else{
        DebugError("[Shell] Failed %d", res);
        return false;
    }
}

bool ShellCommandLine(const WString s){
    DWORD_PTR res;
    size_t argsIdx = s.FindChar(L' ');
    WString cmd;
    WString args;

    if (argsIdx != -1){
        args = s.SubString(argsIdx + 1);
        cmd = s.SubString(0, argsIdx);

        DebugLog("[Shell] %S", cmd.GetString());
        DebugLog("[Shell] Arguments %S", args.GetString());
    }else{
        DebugLog("[Shell] %S", cmd.GetString());
    }

    res = (DWORD_PTR)ShellExecuteW(NULL, L"open", cmd.GetString(), (args.GetLength() == 0 ? NULL : args.GetString()), NULL, SW_HIDE);
    
    if (res >= 32){
        return true;
    }else{
        DebugError("[Shell] Failed %d", res);
        return false;
    }
}

bool ShellCheckFileExistence(const String file, size_t times, size_t interval){
    for (size_t i = 0; i < times; i++){
        HANDLE hFile = CreateFileA(
            file.GetString(),
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
        Sleep(interval);
    }
    return false;
}

bool ShellCheckFileExistence(const WString file, size_t times, size_t interval){
    for (size_t i = 0; i < times; i++){
        HANDLE hFile = CreateFileW(
            file.GetString(),
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
        Sleep(interval);
    }
    return false;
}

bool ShellFFmpeg(const String src, const String dst){
    StringBuilderA args;

    args += "-i \"";
    args += src;
    args += "\" -y ";
    args += dst;

    if (!ShellCommandLineArgs(String(EXECUTABLE_FFMPEG), args.ToString())){
        return false;
    }
    return ShellCheckFileExistence(dst, 100, 100);
}

bool ShellFFmpeg(const WString src, const WString dst){
    StringBuilderW args;

    args += L"-i \"";
    args += src;
    args += L"\" -y ";
    args += dst;

    if (!ShellCommandLineArgs(WString(EXECUTABLE_FFMPEG), args.ToString())){
        return false;
    }
    return ShellCheckFileExistence(dst, 100, 100);
}

bool ShellShaderToBinary(const String src, const String dst, ShaderType type){
    DebugError("ShellShaderToBinary [Unimplemented]");
    return false;
}

bool ShellShaderToBinary(const WString src, const WString dst, ShaderType type){
    DebugError("ShellShaderToBinary [Unimplemented]");
    return false;
}

bool ShellBinaryToShader(const String src, const String dst, ShaderType type){
    DebugError("ShellBinaryToShader [Unimplemented]");
    return false;
}

bool ShellBinaryToShader(const WString src, const WString dst, ShaderType type){
    DebugError("ShellBinaryToShader [Unimplemented]");
    return false;
}

int ShellMsgBox(const String caption, const String text){
    AppFrame* frame = AppFrame::GetLocalInst();
    return MessageBoxA((frame == NULL ? NULL : glfwGetWin32Window(frame->GetWindow())), text.GetString(), caption.GetString(), MB_YESNOCANCEL | MB_ICONINFORMATION);
}

int ShellMsgBox(const WString caption, const WString text){
    AppFrame* frame = AppFrame::GetLocalInst();
    return MessageBoxW((frame == NULL ? NULL : glfwGetWin32Window(frame->GetWindow())), text.GetString(), caption.GetString(), MB_YESNOCANCEL | MB_ICONINFORMATION);
}

bool ShellPrint(const String file){
    DWORD_PTR res;

    res = (DWORD_PTR)ShellExecuteA(NULL, "print", file.GetString(), NULL, NULL, SW_HIDE);
    
    if (res >= 32){
        return true;
    }else{
        DebugError("[Print] Failed %d", res);
        return false;
    }
}

bool ShellPrint(const WString file){
    DWORD_PTR res;

    res = (DWORD_PTR)ShellExecuteW(NULL, L"print", file.GetString(), NULL, NULL, SW_SHOWDEFAULT);
    
    if (res >= 32){
        return true;
    }else{
        DebugError("[Print] Failed %d", res);
        return false;
    }
}

String GetAppDirectoryA(){
    size_t len = DEFAULT_STRING_LENGTH;
    char* str;
    size_t div1, div2;

    str = new char[len];
    while (GetModuleFileNameA(NULL, str, len) >= len){
        delete[] str;
        len <<= 1;
        str = new char[len];
    }

    String res(str, len);
    delete[] str;

    div1 = res.FindRevChar('\\');
    div2 = res.FindRevChar('/');

    if ((div2 != -1 && div2 > div1) || div1 == -1)
        div1 = div2;

    return res.SubString(0, div1);
}

WString GetAppDirectoryW(){
    size_t len = DEFAULT_STRING_LENGTH;
    wchar_t* str;
    size_t div1, div2;

    str = new wchar_t[len];
    while (GetModuleFileNameW(NULL, str, len) >= len){
        delete[] str;
        len <<= 1;
        str = new wchar_t[len];
    }

    WString res(str, len);
    delete[] str;

    div1 = res.FindRevChar(L'\\');
    div2 = res.FindRevChar(L'/');

    if ((div2 != -1 && div2 > div1) || div1 == -1)
        div1 = div2;

    return res.SubString(0, div1);
}

}