#include <utils/os/Font.h>

#include <lib/glew/gl/glew.h>
#include <lib/glew/gl/wglew.h>

#include <lib/freetype/ft2build.h>
#include FT_FREETYPE_H
#include <lib/freetype/freetype/ftglyph.h>

#include <utils/String.h>
#include <utils/os/Log.h>
#include <utils/os/AppFrame.h>
#include <utils/math3d/Math.h>
#include <utils/math3d/LinearAlgebra.h>

void glFontSize(uint size) {
    HFONT hFont;
    HDC hDC = wglGetCurrentDC();

    hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
        GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "微软雅黑");

    HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);
    DeleteObject(hOldFont);
}

void glInitASCIIFont(){
    AppFrame* frame = AppFrame::GetLocalInst();
    HDC hDC;

    frame->fontASCII = glGenLists(MAX_ASCII_CHARS);
    hDC = wglGetCurrentDC();
    wglUseFontBitmapsA(hDC, 0, MAX_ASCII_CHARS, frame->fontASCII);
}

void glDrawString(const char* text){
    AppFrame* frame = AppFrame::GetLocalInst();

    glListBase(frame->fontASCII);
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
    AppFrame* frame = AppFrame::GetLocalInst();
    HDC hDC;
    GLuint font;

    hDC = wglGetCurrentDC();
    font = glGenLists(1);

    for (; *text != L'\0'; text++){
        if ((uint)*text < MAX_ASCII_CHARS){
            glCallList(frame->fontASCII + (uint)*text);
            continue;
        }
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

Vector2 glGetStringSize(const char* text){
    HDC hDC = wglGetCurrentDC();
    size_t len = strlen(text);
    SIZE size;
    
    GetTextExtentPoint32A(hDC, text, len, &size);

    return Vector2(size.cx, size.cy);
}

Vector2 glGetCNStringSize(const wchar_t* text){
    HDC hDC = wglGetCurrentDC();
    size_t len = wcslen(text);
    SIZE size;
    
    GetTextExtentPoint32W(hDC, text, len, &size);

    return Vector2(size.cx, size.cy);
}

FT_Library Font::ftLib = NULL;

void Font::Init(){
    if (!ftLib)
        FT_Init_FreeType(&ftLib);
}

Font::Font(String path){
    FT_New_Face(ftLib, path.GetString(), 0, &face);
    FT_Select_Charmap(face, FT_ENCODING_UNICODE);
}

Font::Font(String path, uint width, uint height){
    FT_New_Face(ftLib, path.GetString(), 0, &face);
    FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    FT_Set_Pixel_Sizes(face, width, height);
}

Font::~Font(){}

void Font::SetSize(Vector2 size){
    SetSize(Round(size.x), Round(size.y));
}

void Font::SetSize(uint x, uint y){
    FT_Set_Pixel_Sizes(face, x, y);
}

void Font::SetTransform(Matrix2x3 m){
    FT_Vector pos;
    FT_Matrix mat;

    pos.x = m._13; pos.y = m._23;

    mat.xx = m._11; mat.xy = m._21;
    mat.yx = m._12; mat.yy = m._22;

    FT_Set_Transform(face, &mat, &pos);
}

uint Font::GetIndex(uint c){
    return FT_Get_Char_Index(face, c);
}

const uchar* Font::LoadIndex(uint idx){
    FT_Load_Glyph(face, idx, FT_LOAD_DEFAULT);
    FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    return face->glyph->bitmap.buffer;

}

uint Font::LoadChar(wchar_t c){
    FT_Glyph glyph;
    FT_BitmapGlyph bitmap;
    uint width;
    uint height;
    uint bmpWidth;
    uint bmpRows;
    uchar* buf;
    uchar* bmpBuf;

    CharTexture& chTex = tex[c];
    if (chTex.tex)
        return chTex.tex;
    FT_Load_Char(face, c, FT_LOAD_FORCE_AUTOHINT | FT_LOAD_TARGET_NORMAL);
    FT_Get_Glyph(face->glyph, &glyph);
    FT_Render_Glyph(face->glyph, FT_RENDER_MODE_LCD);
    FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, NULL, TRUE);
    bitmap = (FT_BitmapGlyph)glyph;

    width = bitmap->bitmap.width;
    height = bitmap->bitmap.rows;

    chTex.width = width;
    chTex.height = height;
    chTex.deltaX = bitmap->left;
    chTex.deltaY = bitmap->top - height;
    chTex.advX = face->glyph->advance.x >> 6;
    chTex.advY = face->size->metrics.y_ppem;

    bmpWidth = bitmap->bitmap.width;
    bmpRows = bitmap->bitmap.rows;
    bmpBuf = bitmap->bitmap.buffer;

    glGenTextures(1, &chTex.tex);
    glBindTexture(GL_TEXTURE_2D, chTex.tex);
    buf = new uchar[(width * height) << 2];
    for(uint i = 0; i < width; i++){
        for(uint j = 0; j < height; j++){
            uchar alpha = ((i >= bmpWidth || j >= bmpRows) ? 0 : bmpBuf[i + bmpWidth * j]);
            uint idx = ((i + (height - j - 1) * width) << 2);
            buf[idx] = 0xFF;
            buf[idx | 1] = 0xFF;
            buf[idx | 2] = 0xFF;
            buf[idx | 3] = alpha;
        }
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    delete[] buf;

    return chTex.tex;
}

void Font::DrawString(const wchar_t* text, uint x, uint y, uint maxX, uint lineHeight, float depth){
    WString s(text);
    DrawString(s, x, y, maxX, lineHeight, depth);
}

void Font::DrawString(const WString& text, uint x, uint y, uint maxX, uint lineHeight, float depth){
    uint sx = x;
    uint sy = y;
    size_t len = text.GetLength();

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    for (uint i = 0; i < len; i++){
        if (text[i] == '\n'){
            sx = x; sy += lineHeight + 12;
            continue;
        }

        LoadChar(text[i]);
        CharTexture& chTex = tex[text[i]];

        glBindTexture(GL_TEXTURE_2D, chTex.tex);

        uint chX = sx + chTex.deltaX;
        uint chY = sy - lineHeight - chTex.deltaY;

        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(chX, chY, depth);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(chX + chTex.width, chY, depth);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(chX + chTex.width, chY + lineHeight, depth);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(chX, chY + lineHeight, depth);
        glEnd();

        sx += chTex.advX;

        if(sx > x + maxX){
            sx = x ; sy += lineHeight + 12;
        }
    }

    glDisable(GL_TEXTURE_2D);
}