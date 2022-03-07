#include <utils/gl/GLTexture2D.h>

#include <lib/opengl/gl/gl.h>

#include <utils/String.h>
#include <utils/DataBuffer.h>
#include <utils/os/Resource.h>
#include <utils/os/Log.h>
#include <utils/os/GLFunc.h>
#include <utils/gl/GLRenderTexture2D.h>

GLTexture2D::GLTexture2D(const char* path){
    int channel;
    DataBuffer data = Resource::GetTexture(String(path), &x, &y, &channel, 0);
    InitData(data, x, y, channel);
}

GLTexture2D::GLTexture2D(const wchar_t* path){
    int channel;
    DataBuffer data = Resource::GetTexture(String(path), &x, &y, &channel, 0);
    InitData(data, x, y, channel);
}

GLTexture2D::GLTexture2D(int resid){
    int channel;
    DataBuffer data = Resource::GetTexture(resid, &x, &y, &channel, 0);
    InitData(data, x, y, channel);
}

GLTexture2D::GLTexture2D(GLRenderTexture2D* tex)
    : tex(tex->GetTexture()), x(tex->GetWidth()), y(tex->GetHeight()), format(tex->GetFormat()), delTex(false) {}

GLTexture2D::~GLTexture2D(){
    if (delTex && glIsTexture(tex))
        glDeleteTextures(1, &tex);
}

void GLTexture2D::InitData(DataBuffer& data, int x, int y, int channel){
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);// 纹理数据上传的对齐像素数
    //glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap.bmWidthBytes / channel);// 指定像素数据中原图的宽度

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// S方向上贴图
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// T方向上贴图
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// 放大纹理过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);// 缩小纹理过滤方式
	
    switch (channel){
    case 1: format = GL_R; break;
    case 3: format = GL_RGB; break;
    case 4: format = GL_RGBA; break;
    default:
        format = GL_NONE;
        DebugError("GLTexture2D::GLTexture2D Unknown Channel Count %d", channel);
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, channel, x, y, 0, format, GL_UNSIGNED_BYTE, data.Buffer());
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool GLTexture2D::Enable(){
    glEnable(GL_TEXTURE_2D);
    if (glIsTexture(tex)){
        glBindTexture(GL_TEXTURE_2D, tex);
        return true;
    }
    DebugError("GLTexture2D::Enable %d Is Not A Texture", tex);
    glBindTexture(GL_TEXTURE_2D, 0);
    return false;
}

void GLTexture2D::Disable(){
    glDisable(GL_TEXTURE_2D);
}

uint GLTexture2D::GetTexture(){
    return tex;
}

int GLTexture2D::GetWidth(){
    return x;
}

int GLTexture2D::GetHeight(){
    return y;
}

uenum GLTexture2D::GetFormat(){
    return format;
}

void GLTexture2D::SetXWarp(uenum val){
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, val);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture2D::SetYWarp(uenum val){
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, val);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture2D::SetMinFilter(uenum val){
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, val);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture2D::SetMagFilter(uenum val){
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, val);
    glBindTexture(GL_TEXTURE_2D, 0);
}