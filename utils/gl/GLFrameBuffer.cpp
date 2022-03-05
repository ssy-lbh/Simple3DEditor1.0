#include <utils/gl/GLFrameBuffer.h>

#include <lib/opengl/gl/gl.h>

#include <editor/gui/ViewManager.h>
#include <utils/os/GLFunc.h>
#include <utils/gl/GLRenderTexture2D.h>

GLFrameBuffer::GLFrameBuffer(){
    glGenFramebuffers(1, &frame);
}

GLFrameBuffer::GLFrameBuffer(GLRenderTexture2D* texture){
    glGenFramebuffers(1, &frame);
    BindTexture(texture);
}

GLFrameBuffer::~GLFrameBuffer(){
    glDeleteFramebuffers(1, &frame);
}

void GLFrameBuffer::BindTexture(GLRenderTexture2D* texture){
    BindTexture(texture, GL_COLOR_ATTACHMENT0);
}

void GLFrameBuffer::BindTexture(GLRenderTexture2D* texture, uenum attachment){
    uint tex;

    if (!texture){
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, 0, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        SetSize(GLRect::zero);
        return;
    }

    tex = texture->GetTexture();
    glBindTexture(GL_TEXTURE_2D, tex);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    SetSize(texture);
}

void GLFrameBuffer::SetSize(GLRect rect){
    this->rect = rect;
}

void GLFrameBuffer::SetSize(GLRenderTexture2D* texture){
    if (texture){
        rect = GLRect(0.0f, texture->GetWidth(), 0.0f, texture->GetHeight());
    }else{
        rect = GLRect::zero;
    }
}

void GLFrameBuffer::Enable(){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame);
    ViewManager::GetLocalInst()->PushView(rect);
}

void GLFrameBuffer::Disable(){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    ViewManager::GetLocalInst()->PopView();
}