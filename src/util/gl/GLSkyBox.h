#ifndef __UTILS_GL_SKYBOX__
#define __UTILS_GL_SKYBOX__

#include <define.h>

class GLSkyBox {
private:
    GLTexture2D* left = NULL;
    GLTexture2D* right = NULL;
    GLTexture2D* front = NULL;
    GLTexture2D* back = NULL;
    GLTexture2D* top = NULL;
    GLTexture2D* down = NULL;

public:
    enum Direction {
        LEFT,
        RIGHT,
        FRONT,
        BACK,
        TOP,
        DOWN
    };

    GLSkyBox();
    ~GLSkyBox();

    void Set(Direction dir, GLTexture2D* tex);
    GLTexture2D* Get(Direction dir);
    // 在最前渲染，应取消GL_DEPTH_TEST，调用后glDepthMask为TRUE
    void Render(Vector3 pos, float dis);
};

#endif