#include <utils/gl/GLFrameBuffer.h>

#include <lib/opengl/gl/gl.h>
#include <utils/os/GLFunc.h>
#include <editor/gui/ViewManager.h>

GLFrameBuffer::GLFrameBuffer(){
    glGenFramebuffers(1, &frame);
}

GLFrameBuffer::GLFrameBuffer(uint texture){
    glGenFramebuffers(1, &frame);
    BindTexture(texture);
}

GLFrameBuffer::~GLFrameBuffer(){
    glDeleteFramebuffers(1, &frame);
}

void GLFrameBuffer::BindTexture(uint texture){
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    hasScale = false;
}

void GLFrameBuffer::BindTexture(uint texture, int x, int y){
    this->x = x; this->y = y;
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    hasScale = true;
}

void GLFrameBuffer::BindTexture(uint texture, uenum attachment){
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    hasScale = false;
}

void GLFrameBuffer::BindTexture(uint texture, uenum attachment, int x, int y){
    this->x = x; this->y = y;
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    hasScale = true;
}

void GLFrameBuffer::Enable(){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame);
    if (hasScale)
        ViewManager::GetLocalInst()->PushView(GLRect(0, x, 0, y));
}

void GLFrameBuffer::Disable(){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    if (hasScale)
        ViewManager::GetLocalInst()->PopView();
}