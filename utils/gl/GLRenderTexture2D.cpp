#include <utils/gl/GLRenderTexture2D.h>

#include <lib/opengl/gl/gl.h>

#include <utils/os/Log.h>
#include <utils/os/GLFunc.h>

GLRenderTexture2D::GLRenderTexture2D(int x, int y, uenum format) : x(x), y(y), format(format) {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexStorage2D(GL_TEXTURE_2D, 8, format, x, y);
}

GLRenderTexture2D::~GLRenderTexture2D(){
    glDeleteTextures(1, &tex);
}

bool GLRenderTexture2D::Enable(){
    glEnable(GL_TEXTURE_2D);
    if (glIsTexture(tex)){
        glBindTexture(GL_TEXTURE_2D, tex);
        return true;
    }
    DebugError("GLRenderTexture2D::Enable %d Is Not A Texture", tex);
    glBindTexture(GL_TEXTURE_2D, 0);
    return false;
}

void GLRenderTexture2D::Disable(){
    glDisable(GL_TEXTURE_2D);
}

uint GLRenderTexture2D::GetTexture(){
    return tex;
}

int GLRenderTexture2D::GetWidth(){
    return x;
}

int GLRenderTexture2D::GetHeight(){
    return y;
}

uenum GLRenderTexture2D::GetFormat(){
    return format;
}