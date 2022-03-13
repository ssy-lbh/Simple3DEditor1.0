#include <utils/gl/GLSkyBox.h>

#include <lib/glew/gl/glew.h>

#include <utils/gl/GLTexture2D.h>
#include <utils/gl/GLSimplified.h>

static inline void Set(GLTexture2D** tex, GLTexture2D* newTex){
    if (*tex)
        delete *tex;
    *tex = newTex;
    if (newTex){
        newTex->SetXWarp(GL_CLAMP_TO_EDGE);
        newTex->SetYWarp(GL_CLAMP_TO_EDGE);
    }
}

static inline void Draw(GLTexture2D* tex, Vector3& ld, Vector3& rd, Vector3& ru, Vector3& lu){
    if (tex && tex->Enable()){
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLE_FAN);
        glTexCoord2f(0.0f, 1.0f); glVertexv3(ld);
        glTexCoord2f(1.0f, 1.0f); glVertexv3(rd);
        glTexCoord2f(1.0f, 0.0f); glVertexv3(ru);
        glTexCoord2f(0.0f, 0.0f); glVertexv3(lu);
        glEnd();
        GLTexture2D::Disable();
    }else{
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
        glVertexv3(ld);
        glVertexv3(rd);
        glVertexv3(ru);
        glVertexv3(lu);
        glEnd();
    }
}

GLSkyBox::GLSkyBox(){}

GLSkyBox::~GLSkyBox(){
    if (left) delete left;
    if (right) delete right;
    if (front) delete front;
    if (back) delete back;
    if (top) delete top;
    if (down) delete down;
}

void GLSkyBox::Set(Direction dir, GLTexture2D* tex){
    switch (dir){
    case LEFT: ::Set(&left, tex); break;
    case RIGHT: ::Set(&right, tex); break;
    case FRONT: ::Set(&front, tex); break;
    case BACK: ::Set(&back, tex); break;
    case TOP: ::Set(&top, tex); break;
    case DOWN: ::Set(&down, tex); break;
    }
}

GLTexture2D* GLSkyBox::Get(Direction dir){
    switch (dir){
    case LEFT: return left;
    case RIGHT: return right;
    case FRONT: return front;
    case BACK: return back;
    case TOP: return top;
    case DOWN: return down;
    }
    return NULL;
}

void GLSkyBox::Render(Vector3 pos, float dis){
    Vector3 v000 = pos + Vector3(-dis, -dis, -dis);
    Vector3 v100 = pos + Vector3( dis, -dis, -dis);
    Vector3 v010 = pos + Vector3(-dis,  dis, -dis);
    Vector3 v110 = pos + Vector3( dis,  dis, -dis);
    Vector3 v001 = pos + Vector3(-dis, -dis,  dis);
    Vector3 v101 = pos + Vector3( dis, -dis,  dis);
    Vector3 v011 = pos + Vector3(-dis,  dis,  dis);
    Vector3 v111 = pos + Vector3( dis,  dis,  dis);

    glDepthMask(GL_FALSE);

    ::Draw(left, v010, v000, v001, v011);
    ::Draw(right, v100, v110, v111, v101);
    ::Draw(front, v110, v010, v011, v111);
    ::Draw(back, v000, v100, v101, v001);
    ::Draw(top, v111, v011, v001, v101);
    ::Draw(down, v100, v000, v010, v110);

    glDepthMask(GL_TRUE);
}