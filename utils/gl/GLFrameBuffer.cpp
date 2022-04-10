#include <utils/gl/GLFrameBuffer.h>

#include <lib/glew/gl/glew.h>

#include <editor/main/ViewManager.h>
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
    if (glIsRenderbuffer(buffer))
        glDeleteRenderbuffers(1, &buffer);
}

void GLFrameBuffer::BindTexture(GLRenderTexture2D* texture){
    BindTexture(texture, GL_COLOR_ATTACHMENT0);
}

void GLFrameBuffer::BindTexture(GLRenderTexture2D* texture, uenum attachment){
    uint tex;

    if (!texture){
        glBindFramebuffer(GL_FRAMEBUFFER, frame);
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, 0, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        SetSize(Rect::zero);
        return;
    }

    tex = texture->GetTexture();
    glBindTexture(GL_TEXTURE_2D, tex);
    glBindFramebuffer(GL_FRAMEBUFFER, frame);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    SetSize(texture);
}

void GLFrameBuffer::SetSize(Rect rect){
    this->rect = rect;
}

void GLFrameBuffer::SetSize(GLRenderTexture2D* texture){
    if (texture){
        rect = Rect(0.0f, texture->GetWidth(), 0.0f, texture->GetHeight());
    }else{
        rect = Rect::zero;
    }
}

void GLFrameBuffer::Enable(){
    glBindFramebuffer(GL_FRAMEBUFFER, frame);
    ViewManager::GetLocalInst()->PushView(rect);
}

void GLFrameBuffer::Disable(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ViewManager::GetLocalInst()->PopView();
}

void GLFrameBuffer::EnableRenderBuffer(){
    if (!buffer){
        glGenRenderbuffers(1, &buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, rect.GetWidth(), rect.GetHeight());
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, frame);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFrameBuffer::DisableRenderBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, frame);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}