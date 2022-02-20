#ifndef __FONT__
#define __FONT__

void glSelectFont(int size, int charset, const char* face);
void glInitASCIIFont();
void glDrawString(const char* text);
void glDrawCNString(const char* text);
void glDrawCNString(const wchar_t* text);
float glGetStringWidth(const char* text);
float glGetCNStringWidth(const wchar_t* text);
float glGetStringHeight(const char* text);
float glGetCNStringHeight(const wchar_t* text);

#endif