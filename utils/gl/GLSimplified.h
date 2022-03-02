#ifndef __UTILS_GL_GLSIMPLIFIED__
#define __UTILS_GL_GLSIMPLIFIED__

#include <lib/opengl/gl/gl.h>

#include <utils/math3d/Math.h>
#include <utils/math3d/LinearAlgebra.h>

// 内联函数没有符号，无法分离定义与实现，且可在头文件定义
inline void glVertexv2(Vector2 v){
    glVertex2f(v.x, v.y);
}

inline void glVertexv3(Vector3 v){
    glVertex3f(v.x, v.y, v.z);
}

inline void glVertexv4(Vector4 v){
    glVertex4f(v.x, v.y, v.z, v.w);
}

inline void glColorv3(Vector3 v){
    glColor3f(v.x, v.y, v.z);
}

inline void glColorv4(Vector4 v){
    glColor4f(v.x, v.y, v.z, v.w);
}

inline void glRotateq(Quaternion q){
    Vector3 axis = q.GetAxis().Normal();
    glRotatef(ToAngle(2.0f * Acos(q.w)), axis.x, axis.y, axis.z);
}

inline void glScalev3(Vector3 v){
    glScalef(v.x, v.y, v.z);
}

inline void glLoadMatrix(Matrix4x4 mat){
    glLoadMatrixf((GLfloat*)&mat);
}

inline void glMultMatrix(Matrix4x4 mat){
    glMultMatrixf((GLfloat*)&mat);
}

#endif