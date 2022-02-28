#ifndef __UTILS_OS_FONT__
#define __UTILS_OS_FONT__

#include <define.h>

void glFontSize(uint size);
void glInitASCIIFont();
void glDrawString(const char* text);
void glDrawCNString(const char* text);
void glDrawCNString(const wchar_t* text);
float glGetStringWidth(const char* text);
float glGetCNStringWidth(const wchar_t* text);
float glGetStringHeight(const char* text);
float glGetCNStringHeight(const wchar_t* text);
Vector2 glGetStringSize(const char* text);
Vector2 glGetCNStringSize(const wchar_t* text);

#endif