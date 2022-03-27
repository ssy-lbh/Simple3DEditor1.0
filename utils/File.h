#ifndef __UTILS_FILE__
#define __UTILS_FILE__

#include <define.h>

#include <utils/String.h>

// 设计中
class File final : public Object {
protected:
    String path;

#ifdef PLATFORM_WINDOWS
    handle hFile = (handle)(-1);
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
    File(String path);
    File(WString path);
    ~File();

    bool Open();
    bool Close();
    bool IsOpened() const;
    bool Exist();
    bool Create();
    bool CreateNew();
    bool Delete();
    static bool Delete(const char* path);
    static bool Delete(const wchar_t* path);
    static bool Delete(String path);
    static bool Delete(WString path);
    size_t Read(void* buffer, size_t size);
    size_t Write(const void* buffer, size_t size);
    size_t GetPointer() const;
    void SetPointer(size_t ptr);
    size_t GetSize();
    String GetPath() const;

    // 检测注入使用
    static bool IsSpecial(String s);
    static bool IsSpecial(WString s);
};

class ISerializable {
public:
    void Serialize(File& f);
};

class IDeserializable {
public:
    void Deserialize(File& f);
};

// 多继承中只含有接口，期望数据不会有不确定性
class IMemorable : public ISerializable, public IDeserializable {};

#endif