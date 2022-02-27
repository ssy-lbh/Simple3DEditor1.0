#ifndef __UTILS_STRING__
#define __UTILS_STRING__

#include <define.h>

#define DEFAULT_STRING_LENGTH 512
class String final : public Object {
protected:
    char* str;
    size_t len;

    String(bool alloc);

public:
    String();
    String(int resid);
    String(char c);
    String(const char*);
    String(const char*, size_t);
    String(const wchar_t*);
    String(const wchar_t*, size_t);
    String(String &&);
    String(WString &&);
    String(const String &);
    String(const WString &);
    String &operator=(const char*);
    String &operator=(const wchar_t*);
    String &operator=(String &&);
    String &operator=(WString &&);
    String &operator=(const String &);
    String &operator=(const WString &);
    ~String();
    char operator[](size_t index) const;
    String operator+(const String) const;
    bool operator==(const String) const;
    bool operator!=(const String) const;
    bool operator<(const String) const;
    bool operator>(const String) const;
    bool operator<=(const String) const;
    bool operator>=(const String) const;

    const char* GetString() const;
    size_t GetLength() const;
    char CharAt(size_t index) const;
    String Reverse() const;
    String SubString(size_t start) const;
    String SubString(size_t start, size_t end) const;
    // 未找到返回-1
    size_t FindChar(char c) const;
    size_t FindChar(char c, size_t beg) const;
    size_t FindRevChar(char c) const;
    size_t FindRevChar(char c, size_t beg) const;
    size_t FindString(const char* s) const;
    size_t FindString(const char* s, size_t beg) const;
    size_t FindString(const String s) const;
    size_t FindString(const String s, size_t beg) const;
    bool StartsWith(const String) const;
    bool EndsWith(const String) const;
    bool IgnoreCaseEqual(const String) const;
    bool IgnoreCaseNotEqual(const String) const;
    bool IgnoreCaseLess(const String) const;
    bool IgnoreCaseGreater(const String) const;
    bool IgnoreCaseLessEqual(const String) const;
    bool IgnoreCaseGreaterEqual(const String) const;
    String ToLowerCase() const;
    String ToUpperCase() const;
    size_t Count(char c) const;
    size_t Count(const char* s) const;
    size_t Count(const String s) const;
    size_t Spilt(char c, String* arr, size_t len) const;
    size_t Split(const char* s, String* arr, size_t len) const;
    size_t Split(const String s, String* arr, size_t len) const;
};

class WString final : public Object {
protected:
    wchar_t* str;
    size_t len;

    WString(bool alloc);

public:
    WString();
    WString(int resid);
    WString(wchar_t c);
    WString(const char*);
    WString(const char*, size_t);
    WString(const wchar_t*);
    WString(const wchar_t*, size_t);
    WString(String &&);
    WString(WString &&);
    WString(const String &);
    WString(const WString &);
    WString &operator=(const char*);
    WString &operator=(const wchar_t*);
    WString &operator=(String &&);
    WString &operator=(WString &&);
    WString &operator=(const String &);
    WString &operator=(const WString &);
    ~WString();
    wchar_t operator[](size_t index) const;
    WString operator+(const WString) const;
    bool operator==(const WString) const;
    bool operator!=(const WString) const;
    bool operator<(const WString) const;
    bool operator>(const WString) const;
    bool operator<=(const WString) const;
    bool operator>=(const WString) const;

    const wchar_t* GetString() const;
    size_t GetLength() const;
    wchar_t CharAt(size_t index) const;
    WString Reverse() const;
    WString SubString(size_t start) const;
    WString SubString(size_t start, size_t end) const;
    // 未找到返回-1
    size_t FindChar(wchar_t c) const;
    size_t FindChar(wchar_t c, size_t beg) const;
    size_t FindRevChar(wchar_t c) const;
    size_t FindRevChar(wchar_t c, size_t beg) const;
    size_t FindString(const wchar_t* s) const;
    size_t FindString(const wchar_t* s, size_t beg) const;
    size_t FindString(const WString s) const;
    size_t FindString(const WString s, size_t beg) const;
    bool StartsWith(const WString) const;
    bool EndsWith(const WString) const;
    bool IgnoreCaseEqual(const WString) const;
    bool IgnoreCaseNotEqual(const WString) const;
    bool IgnoreCaseLess(const WString) const;
    bool IgnoreCaseGreater(const WString) const;
    bool IgnoreCaseLessEqual(const WString) const;
    bool IgnoreCaseGreaterEqual(const WString) const;
    WString ToLowerCase() const;
    WString ToUpperCase() const;
    size_t Count(wchar_t c) const;
    size_t Count(const wchar_t* s) const;
    size_t Count(const WString s) const;
    size_t Spilt(wchar_t c, WString* arr, size_t len) const;
    size_t Split(const wchar_t* s, WString* arr, size_t len) const;
    size_t Split(const WString s, WString* arr, size_t len) const;
};

#endif