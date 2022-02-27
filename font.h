#ifndef __FONT__
#define __FONT__

#include <define.h>

void glFontSize(int size);
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