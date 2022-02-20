#include "font.h"

#include "opengl/gl/gl.h"

#define MAX_CHARS 128

static GLuint font = 0;

void glSelectFont(int size, int charset, const char* face) {
    HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
        charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
    HDC hDC = wglGetCurrentDC();
    HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);
    DeleteObject(hOldFont);
}

void glInitASCIIFont(){
    HDC hDC;

    if (font == 0){
        font = glGenLists(MAX_CHARS);
        hDC = wglGetCurrentDC();
        wglUseFontBitmaps(hDC, 0, MAX_CHARS, font);
    }
}

void glDrawString(const char* text){
    glListBase(font);
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
}

void glDrawCNString(const char* text){
    size_t wlen = 0;
    wchar_t* wstr;

    wlen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text, -1, NULL, 0);

    wstr = new wchar_t[wlen + 1];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text, -1, wstr, wlen);
    wstr[wlen] = L'\0';

    glDrawCNString(wstr);

    delete[] wstr;
}

void glDrawCNString(const wchar_t* text){
    HDC hDC;
    GLuint font;

    hDC = wglGetCurrentDC();
    font = glGenLists(1);

    for (; *text != L'\0'; text++){
        wglUseFontBitmapsW(hDC, *text, 1, font);
        glCallList(font);
    }

    glDeleteLists(font, 1);
}

float glGetStringWidth(const char* text){
    HDC hDC = wglGetCurrentDC();
    size_t len = strlen(text);
    SIZE size;
    
    GetTextExtentPoint32A(hDC, text, len, &size);

    return (float)size.cx;
}

float glGetCNStringWidth(const wchar_t* text){
    HDC hDC = wglGetCurrentDC();
    size_t len = wcslen(text);
    SIZE size;
    
    GetTextExtentPoint32W(hDC, text, len, &size);

    return (float)size.cx;
}

float glGetStringHeight(const char* text){
    HDC hDC = wglGetCurrentDC();
    size_t len = strlen(text);
    SIZE size;
    
    GetTextExtentPoint32A(hDC, text, len, &size);

    return (float)size.cy;
}

float glGetCNStringHeight(const wchar_t* text){
    HDC hDC = wglGetCurrentDC();
    size_t len = wcslen(text);
    SIZE size;
    
    GetTextExtentPoint32W(hDC, text, len, &size);

    return (float)size.cy;
}