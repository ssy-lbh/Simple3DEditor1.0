#ifndef __UTILS_GL_GLRENDERTEXTURE2D__
#define __UTILS_GL_GLRENDERTEXTURE2D__

#include <define.h>

class GLRenderTexture2D final : public Object {
private:
    uint tex;
    int x, y;
    uenum format;
    
public:
    GLRenderTexture2D(int x, int y, uenum format);
    ~GLRenderTexture2D();

    bool Enable();
    // 同glDisable(GL_TEXTURE_2D);
    static void Disable();
    uint GetTexture();
    int GetWidth();
    int GetHeight();
    uenum GetFormat();
};

#endif