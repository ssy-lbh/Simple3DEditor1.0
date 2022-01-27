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
    for (; *text != '\0'; text++){
        glCallList(font + *text);
    }
}

//TODO 暂时不可用，需要修改
void glDrawCNString(const char* text){
    size_t wlen = 0;
    wchar_t* wstr;

    for (; *text != '\0'; text++){
        if(IsDBCSLeadByte(*text))
            ++text;
        ++wlen;
    }

    wstr = (wchar_t*)malloc((wlen + 1) * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, text, -1, wstr, wlen);
    wstr[wlen] = L'\0';

    glDrawCNString(wstr);

    free(wstr);
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