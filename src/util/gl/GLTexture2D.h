#ifndef __UTIL_GL_GLTEXTURE2D__
#define __UTIL_GL_GLTEXTURE2D__

#include <define.h>

namespace simple3deditor {

class GLTexture2D : public Object {
protected:
    uint tex;
    bool delTex = true;
    int x, y;
    uenum format;

    GLTexture2D();

    void InitData(DataBuffer& data, int x, int y, int channel);
    
public:
    GLTexture2D(const char* path);
    GLTexture2D(const wchar_t* path);
    GLTexture2D(int resid);
    GLTexture2D(GLRenderTexture2D* tex);
    ~GLTexture2D();

    bool Enable();
    // 同glDisable(GL_TEXTURE_2D);
    static void Disable();
    uint GetTexture();
    int GetWidth();
    int GetHeight();
    uenum GetFormat();
    void SetXWarp(uenum val);
    void SetYWarp(uenum val);
    void SetMinFilter(uenum val);
    void SetMagFilter(uenum val);
};

}

#endif