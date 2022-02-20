#ifndef __UTILS__
#define __UTILS__

#include "define.h"

#include <handleapi.h>

class String : public Object {
protected:
    char* str;
    size_t len;

public:
    String();
    String(char c);
    String(const char*);
    String(String &&);
    String(const String &);
    String &operator=(const char*);
    String &operator=(String &&);
    String &operator=(const String &);
    ~String();
    char &operator[](size_t index);
    String operator+(const String) const;
    String &operator+=(const String);
    bool operator==(const String) const;
    bool operator!=(const String) const;
    bool operator<(const String) const;
    bool operator>(const String) const;
    bool operator<=(const String) const;
    bool operator>=(const String) const;

    const char* GetString() const;
    size_t GetLength() const;
    char CharAt(size_t index) const;
    void Reverse();
    // 未找到返回-1
    size_t FindChar(char c) const;
    size_t FindChar(char c, size_t beg) const;
    size_t FindRevChar(char c) const;
    size_t FindRevChar(char c, size_t beg) const;
    size_t FindString(const char* s) const;
    size_t FindString(const char* s, size_t beg) const;
    size_t FindString(const String s) const;
    size_t FindString(const String s, size_t beg) const;
    bool IgnoreCaseEqual(String) const;
    bool IgnoreCaseNotEqual(String) const;
    bool IgnoreCaseLess(String) const;
    bool IgnoreCaseGreater(String) const;
    bool IgnoreCaseLessEqual(String) const;
    bool IgnoreCaseGreaterEqual(String) const;
    void ToLowerCase();
    void ToUpperCase();
    size_t Count(char c) const;
    size_t Count(const char* s) const;
    size_t Count(const String s) const;
    // 传入new[]的数组
    size_t Spilt(char c, String* arr, size_t len) const;
    size_t Split(const char* s, String* arr, size_t len) const;
    size_t Split(const String s, String* arr, size_t len) const;
};

class WString : public Object {
protected:
    wchar_t* str;
    size_t len;

public:
    WString();
    WString(wchar_t c);
    WString(const wchar_t*);
    WString(WString &&);
    WString(const WString &);
    WString &operator=(const wchar_t*);
    WString &operator=(WString &&);
    WString &operator=(const WString &);
    ~WString();
    wchar_t &operator[](size_t index);
    WString operator+(const WString) const;
    WString &operator+=(const WString);
    bool operator==(const WString) const;
    bool operator!=(const WString) const;
    bool operator<(const WString) const;
    bool operator>(const WString) const;
    bool operator<=(const WString) const;
    bool operator>=(const WString) const;

    const wchar_t* GetString() const;
    size_t GetLength() const;
    wchar_t CharAt(size_t index) const;
    void Reverse();
    // 未找到返回-1
    size_t FindChar(wchar_t c) const;
    size_t FindChar(wchar_t c, size_t beg) const;
    size_t FindRevChar(wchar_t c) const;
    size_t FindRevChar(wchar_t c, size_t beg) const;
    size_t FindString(const wchar_t* s) const;
    size_t FindString(const wchar_t* s, size_t beg) const;
    size_t FindString(const WString s) const;
    size_t FindString(const WString s, size_t beg) const;
    bool IgnoreCaseEqual(WString) const;
    bool IgnoreCaseNotEqual(WString) const;
    bool IgnoreCaseLess(WString) const;
    bool IgnoreCaseGreater(WString) const;
    bool IgnoreCaseLessEqual(WString) const;
    bool IgnoreCaseGreaterEqual(WString) const;
    void ToLowerCase();
    void ToUpperCase();
    size_t Count(wchar_t c) const;
    size_t Count(const wchar_t* s) const;
    size_t Count(const WString s) const;
    // 传入new[]的数组
    size_t Spilt(wchar_t c, WString* arr, size_t len) const;
    size_t Split(const wchar_t* s, WString* arr, size_t len) const;
    size_t Split(const WString s, WString* arr, size_t len) const;
};

// 设计中
class File : public Object {
private:
    HANDLE hFile = INVALID_HANDLE_VALUE;

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

    bool Exist();
    bool Create();
    bool Delete();
    size_t Read(void* buffer, size_t size);
    size_t Write(const void* buffer, size_t size);
    size_t GetPointer();
    void SetPointer();
    size_t GetSize();
};

#endif