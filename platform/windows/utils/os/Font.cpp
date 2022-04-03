#include <utils/os/Font.h>

#include <lib/glew/gl/glew.h>
#include <lib/glew/gl/wglew.h>

#include <lib/ftgl/FTGL/ftgl.h>

#include <utils/String.h>
#include <utils/os/Log.h>
#include <utils/os/AppFrame.h>
#include <utils/os/Thread.h>
#include <utils/math3d/Math.h>
#include <utils/math3d/LinearAlgebra.h>

void glFontSize(uint size) {
    HFONT hFont;
    AppFrame* frame = AppFrame::GetLocalInst();
    HDC hDC = wglGetCurrentDC();

    hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
        GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "微软雅黑");

    HFONT hOldFont = (HFONT)SelectObject(hDC, hFont);
    DeleteObject(hOldFont);

    if (frame->fontASCII)
        glDeleteLists(frame->fontASCII, MAX_ASCII_CHARS);
    frame->fontASCII = glGenLists(MAX_ASCII_CHARS);
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

    if (!frame->fontCache){
        frame->fontCache = new uint[MAX_CACHE_CHARS];
        memset(frame->fontCache, 0, MAX_CACHE_CHARS * sizeof(uint));
    }

    hDC = wglGetCurrentDC();

    for (; *text != L'\0'; text++){
        if ((uint)*text < MAX_ASCII_CHARS){
            glCallList(frame->fontASCII + (uint)*text);
            continue;
        }
        if ((uint)*text < MAX_CACHE_CHARS){
            uint& list = frame->fontCache[(uint)*text];
            if (!list){
                list = glGenLists(1);
                wglUseFontBitmapsW(hDC, *text, 1, list);
            }
            glCallList(list);
            continue;
        }
        font = glGenLists(1);
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

bool WideCharToBytes(const wchar_t* text, size_t len, char*& outText, size_t& outLen){
    outLen = WideCharToMultiByte(CP_ACP, 0, text, len, NULL, 0, NULL, NULL);
    outText = new char[outLen + 1];
    WideCharToMultiByte(CP_ACP, 0, text, len, outText, outLen, NULL, NULL);
    outText[outLen] = '\0';
    return true;
}

bool BytesToWideChar(const char* text, size_t len, wchar_t*& outText, size_t& outLen){
    outLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text, len, NULL, 0);
    outText = new wchar_t[outLen + 1];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text, len, outText, outLen);
    outText[outLen] = L'\0';
    return true;
}

bool WideCharToBytesUTF8(const wchar_t* text, size_t len, char*& outText, size_t& outLen){
    outLen = WideCharToMultiByte(CP_UTF8, 0, text, len, NULL, 0, NULL, NULL);
    outText = new char[outLen + 1];
    WideCharToMultiByte(CP_UTF8, 0, text, len, outText, outLen, NULL, NULL);
    outText[outLen] = '\0';
    return true;
}

bool BytesToWideCharUTF8(const char* text, size_t len, wchar_t*& outText, size_t& outLen){
    outLen = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, text, len, NULL, 0);
    outText = new wchar_t[outLen + 1];
    MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, text, len, outText, outLen);
    outText[outLen] = L'\0';
    return true;
}

FT_Library Font::ftLib = NULL;

void Font::Init(){
    if (!ftLib)
        FT_Init_FreeType(&ftLib);
}

void Font::Uninit(){
    if (ftLib){
        FT_Done_FreeType(ftLib);
        ftLib = NULL;
    }
}

Font::Font(String& path){
    FT_New_Face(ftLib, path.GetString(), 0, &face);
    FT_Select_Charmap(face, FT_ENCODING_UNICODE);
}

Font::Font(String& path, uint width, uint height){
    FT_New_Face(ftLib, path.GetString(), 0, &face);
    FT_Select_Charmap(face, FT_ENCODING_UNICODE);
    FT_Set_Pixel_Sizes(face, width, height);
}

Font::~Font(){
    FT_Done_Face(face);
}

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

uint Font::LoadChar(uint c){
    FT_Glyph glyph;
    FT_BitmapGlyph bitmap;
    uint idx;
    uint width;
    uint height;
    uint advY;
    float invAdvY;
    uint bmpWidth;
    uint bmpRows;
    uchar* buf;
    uchar* bmpBuf;

    CharTexture& chTex = tex[c];
    if (chTex.tex)
        return chTex.tex;
    // 加了FT_LOAD_FORCE_AUTOHINT以后会报错
    idx = FT_Get_Char_Index(face, c);
    FT_Load_Glyph(face, idx, FT_LOAD_TARGET_NORMAL);
    FT_Get_Glyph(face->glyph, &glyph);
    FT_Render_Glyph(face->glyph, FT_RENDER_MODE_LCD);
    FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, NULL, TRUE);
    bitmap = (FT_BitmapGlyph)glyph;

    width = bitmap->bitmap.width;
    height = bitmap->bitmap.rows;
    advY = face->size->metrics.y_ppem;
    invAdvY = 1.0f / (float)advY;

    chTex.width = (float)width * invAdvY;
    chTex.height = (float)height * invAdvY;
    chTex.deltaX = (float)bitmap->left * invAdvY;
    chTex.deltaY = ((float)bitmap->top - height) * invAdvY;
    chTex.advX = (float)(face->glyph->advance.x >> 6) * invAdvY;

    bmpWidth = bitmap->bitmap.width;
    bmpRows = bitmap->bitmap.rows;
    bmpBuf = bitmap->bitmap.buffer;

    glGenTextures(1, &chTex.tex);
    glBindTexture(GL_TEXTURE_2D, chTex.tex);

    buf = new uchar[(width * height) << 2];

    for (uint i = 0; i < width; i++){
        for (uint j = 0; j < height; j++){
            uchar alpha = ((i >= bmpWidth || j >= bmpRows) ? 0 : bmpBuf[i + bmpWidth * j]);
            uint idx = ((i + (height - j - 1) * width) << 2);
            buf[idx] = 0xFF;
            buf[idx | 1] = 0xFF;
            buf[idx | 2] = 0xFF;
            buf[idx | 3] = alpha;
        }
    }

    FT_Done_Glyph(glyph);

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

void Font::DrawString(const char* text, float x, float y, float h, float lineX, float depth){
    WString s(text);
    DrawString(s, x, y, h, lineX, depth);
}

void Font::DrawString(const wchar_t* text, float x, float y, float h, float lineX, float depth){
    WString s(text);
    DrawString(s, x, y, h, lineX, depth);
}

void Font::DrawString(const WString& text, float x, float y, float h, float lineX, float depth){
    float sx = x;
    float sy = y - 2.0f * h;
    size_t len = text.GetLength();

    glEnable(GL_TEXTURE_2D);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    for (uint i = 0; i < len; i++){
        if (text[i] == '\n'){
            sx = x;
            sy -= h;
            continue;
        }

        LoadChar(text[i]);
        CharTexture& chTex = tex[text[i]];

        glBindTexture(GL_TEXTURE_2D, chTex.tex);

        float chX = sx + chTex.deltaX * h;
        float chY = sy + chTex.deltaY * h;
        float chX2 = chX + chTex.width * h;
        float chY2 = chY + chTex.height * h;

        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(chX, chY, depth);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(chX2, chY, depth);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(chX2, chY2, depth);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(chX, chY2, depth);
        glEnd();

        sx += chTex.advX * h;

        if(sx > x + lineX){
            sx = x;
            sy -= h;
        }
    }

    glDisable(GL_TEXTURE_2D);
}

GLFont::GLFont(String path, FontType type) : type(type) {
    CreateFTGLFont(path);
}

GLFont::GLFont(String path, uint size, uint res) : type(FONT_PIXMAP) {
    CreateFTGLFont(path);
    FTGL::ftglSetFontFaceSize(font, size, res);
}

GLFont::GLFont(String path, uint size, FontType type, uint res) : type(type) {
    CreateFTGLFont(path);
    FTGL::ftglSetFontFaceSize(font, size, res);
}

GLFont::~GLFont(){
    FTGL::ftglDestroyFont(font);
}

void GLFont::CreateFTGLFont(String& path){
    switch (type){
    case FONT_BITMAP: font = FTGL::ftglCreateBitmapFont(path.GetString()); break;
    case FONT_BUFFER: font = FTGL::ftglCreateBufferFont(path.GetString()); break;
    case FONT_EXTRUDE: font = FTGL::ftglCreateExtrudeFont(path.GetString()); break;
    case FONT_OUTLINE: font = FTGL::ftglCreateOutlineFont(path.GetString()); break;
    case FONT_PIXMAP: font = FTGL::ftglCreatePixmapFont(path.GetString()); break;
    case FONT_POLYGON: font = FTGL::ftglCreatePolygonFont(path.GetString()); break;
    case FONT_TEXTURE: font = FTGL::ftglCreateTextureFont(path.GetString()); break;
    case FONT_CUSTOM:
        DebugError("GLFont::CreateFTGLFont Custom Font Should Be Created By GLFont::GLFont(..., void* data, FTGLglyph*(*makeglyphCallback)(FT_GlyphSlot, void*))");
        break;
    default:
        DebugError("GLFont::CreateFTGLFont Unrecognized Font Type %d", type);
        break;
    }
}

GLFont::FontType GLFont::GetType(){
    return type;
}

void GLFont::SetSize(uint size, uint res){
    FTGL::ftglSetFontFaceSize(font, size, res);
}

void GLFont::DrawString(const char* text){
    FTGL::ftglRenderFont(font, text, FTGL::RENDER_ALL);
}

void GLFont::DrawString(const wchar_t* text){
    String s(text);
    DrawString(s);
}

void GLFont::DrawString(const String& text){
    FTGL::ftglRenderFont(font, text.GetString(), FTGL::RENDER_ALL);
}