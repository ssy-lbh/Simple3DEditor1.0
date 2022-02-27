#ifndef __UTILS_GL_GLTEXTURE2D__
#define __UTILS_GL_GLTEXTURE2D__

#include <define.h>

class GLTexture2D final : public Object {
private:
    uint tex;
    bool delTex = true;
    
public:
    GLTexture2D(const char* path);
    GLTexture2D(const wchar_t* path);
    GLTexture2D(int resid);
    GLTexture2D(uint texture, bool delTex = false);
    ~GLTexture2D();
    bool Enable();
    static void Disable();
};

#endif