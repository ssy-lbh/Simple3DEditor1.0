#include <io/File.h>

#include <windows.h>

#include <util/String.h>
#include <util/os/Log.h>

namespace simple3deditor {

File::File(){}

File::File(File &&f){
    this->path = f.path;
    if (f.IsOpened()){
        this->hFile = f.hFile;
        ref = true;
    }
}

File::File(const File &f){
    this->path = f.path;
    if (f.IsOpened()){
        this->hFile = f.hFile;
        ref = true;
    }
}

File &File::operator=(File &&f){
    Close();
    this->path = f.path;
    if (f.IsOpened()){
        this->hFile = f.hFile;
        ref = true;
    }
    return *this;
}

File &File::operator=(const File &f){
    Close();
    this->path = f.path;
    if (f.IsOpened()){
        this->hFile = f.hFile;
        ref = true;
    }
    return *this;
}

File::File(const char* path){
    this->path = path;
}

File::File(const wchar_t* path){
    this->path = path;
}

File::File(const String& path){
    this->path = path;
}

File::File(const WString& path){
    this->path = path;
}

File::~File(){
    Close();
}

bool File::Open(){
    if (IsOpened())
        return true;
    hFile = CreateFileA(
        path.GetString(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (!IsOpened()){
        DebugError("File::Open %s Failed", path.GetString());
        return false;
    }
    return true;
}

void File::Close(){
    if (IsOpened()){
        if (!ref)
            CloseHandle(hFile);
        ref = false;
        hFile = INVALID_HANDLE_VALUE;
        return;
    }
    return;
}

bool File::IsOpened() const{
    return hFile != INVALID_HANDLE_VALUE;
}

bool File::Exist(){
    if (IsOpened())
        return true;
    Open();
    if (IsOpened()){
        Close();
        return true;
    }
    return false;
}

bool File::Create(){
    if (IsOpened())
        return false;
    hFile = CreateFileA(
        path.GetString(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        // 文件不存在时创建
        CREATE_NEW,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (!IsOpened()){
        DebugError("File::Create %s Failed", path.GetString());
        return false;
    }
    return true;
}

bool File::CreateNew(){
    if (IsOpened())
        return false;
    hFile = CreateFileA(
        path.GetString(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        // 总是创建新文件
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (!IsOpened()){
        DebugError("File::CreateNew %s Failed", path.GetString());
        return false;
    }
    return true;
}

bool File::Delete(){
    Close();
    return DeleteFileA(path.GetString()) == TRUE;
}

bool File::Delete(const char* path){
    return DeleteFileA(path) == TRUE;
}

bool File::Delete(const wchar_t* path){
    return DeleteFileW(path) == TRUE;
}

bool File::Delete(const String& path){
    return DeleteFileA(path.GetString()) == TRUE;
}

bool File::Delete(const WString& path){
    return DeleteFileW(path.GetString()) == TRUE;
}

size_t File::Read(void* buf, size_t len){
    DWORD readLen;

    Open();
    if (!IsOpened())
        return 0;
    ReadFile(hFile, buf, len, &readLen, NULL);
    return readLen;
}

size_t File::Write(const void* buf, size_t len){
    DWORD writeLen;

    Open();
    if (!IsOpened())
        return 0;
    WriteFile(hFile, buf, len, &writeLen, NULL);
    return writeLen;
}

size_t File::Available(){
    LARGE_INTEGER dis;
    LARGE_INTEGER pos;
    LARGE_INTEGER end;

    if (!IsOpened())
        return 0;
    dis.QuadPart = 0;
    SetFilePointerEx(hFile, dis, &pos, FILE_CURRENT);
    SetFilePointerEx(hFile, dis, &end, FILE_END);
    SetFilePointerEx(hFile, pos, NULL, FILE_BEGIN);
    return end.QuadPart - pos.QuadPart;
}

size_t File::GetPointer() const{
    LARGE_INTEGER dis;
    LARGE_INTEGER pos;

    if (!IsOpened())
        return 0;
    dis.QuadPart = 0;
    SetFilePointerEx(hFile, dis, &pos, FILE_CURRENT);
    return pos.QuadPart;
}

void File::SetPointer(size_t ptr){
    LARGE_INTEGER pos;

    Open();
    if (!IsOpened())
        return;
    pos.QuadPart = ptr;
    SetFilePointerEx(hFile, pos, NULL, FILE_BEGIN);
}

size_t File::GetSize(){
    BY_HANDLE_FILE_INFORMATION Info;

    Open();
    if (!IsOpened())
        return 0;
    GetFileInformationByHandle(hFile, &Info);
    return (((size_t)Info.nFileSizeHigh << 32) | (size_t)Info.nFileSizeLow);
}

String File::GetPath() const{
    return path;
}

bool File::IsSpecial(String s){
    if (s.HasChars("\\/:*?\"<>|."))
        return true;
    if (s.StartsWith("\\\\"))
        return true;
    if (s.IgnoreCaseEqual("CON") || s.IgnoreCaseEqual("NUL"))
        return true;
    return false;
}

bool File::IsSpecial(WString s){
    if (s.HasChars(L"\\/:*?\"<>|."))
        return true;
    if (s.StartsWith(L"\\\\"))
        return true;
    if (s.IgnoreCaseEqual(L"CON") || s.IgnoreCaseEqual(L"NUL"))
        return true;
    return false;
}

}