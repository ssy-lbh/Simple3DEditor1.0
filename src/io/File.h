#ifndef __IO_FILE__
#define __IO_FILE__

#include <define.h>

#include <io/IOStream.h>
#include <util/String.h>

namespace simple3deditor {

// 设计中
class File : public Object, public IInputStream, public IOutputStream {
protected:
    String path;

#ifdef PLATFORM_WINDOWS
    typedef void* HANDLE;
    HANDLE hFile = (HANDLE)(-1);
    bool ref = false;
#endif

public:
    File();
    File(File &&);
    File(const File &);
    File &operator=(File &&);
    File &operator=(const File &);
    File(const char* path);
    File(const wchar_t* path);
    File(const String& path);
    File(const WString& path);
    ~File();

    bool Open();
    virtual void Close() override;
    bool IsOpened() const;
    bool Exist();
    bool Create();
    bool CreateNew();
    bool Delete();
    static bool Delete(const char* path);
    static bool Delete(const wchar_t* path);
    static bool Delete(const String& path);
    static bool Delete(const WString& path);
    virtual size_t Read(void* buf, size_t len) override;
    virtual size_t Write(const void* buf, size_t len) override;
    virtual size_t Available() override;
    size_t GetPointer() const;
    void SetPointer(size_t ptr);
    size_t GetSize();
    String GetPath() const;

    // 检测注入使用
    static bool IsSpecial(String s);
    static bool IsSpecial(WString s);
};

}

#endif