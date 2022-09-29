#ifndef __UTIL_OS_FONT__
#define __UTIL_OS_FONT__

#include <define.h>

namespace simple3deditor {

#define MAX_ASCII_CHARS 0x80
#define MAX_CACHE_CHARS 0x10000

void glFontSize(uint size);
void glDrawString(const char* text);
void glDrawCNString(const char* text);
void glDrawCNString(const wchar_t* text);
float glGetStringWidth(const char* text);
float glGetCNStringWidth(const wchar_t* text);
float glGetStringHeight(const char* text);
float glGetCNStringHeight(const wchar_t* text);
Vector2 glGetStringSize(const char* text);
Vector2 glGetCNStringSize(const wchar_t* text);

// outText 指针由new[]得到，需要delete[]回收内存，返回字符串由'\0'结尾
bool WideCharToBytes(const wchar_t* text, size_t len, char*& outText, size_t& outLen);
bool BytesToWideChar(const char* text, size_t len, wchar_t*& outText, size_t& outLen);
bool WideCharToBytesUTF8(const wchar_t* text, size_t len, char*& outText, size_t& outLen);
bool BytesToWideCharUTF8(const char* text, size_t len, wchar_t*& outText, size_t& outLen);

}

#endif