#ifndef __FONT__
#define __FONT__

void glSelectFont(int size, int charset, const char* face);
void glInitASCIIFont();
void glDrawString(const char* text);
void glDrawCNString(const char* text);
void glDrawCNString(const wchar_t* text);
int glGetStringWidth(const char* text);
int glGetCNStringWidth(const wchar_t* text);

#endif