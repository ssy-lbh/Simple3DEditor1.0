#ifndef __UTIL_GL_GLSIMPLIFIED__
#define __UTIL_GL_GLSIMPLIFIED__

#ifndef __GL_H__
#include <windows.h>
#include <gl/gl.h>
#endif

#include <util/math3d/Math.h>
#include <util/math3d/LinearAlgebra.h>

namespace simple3deditor {

// ��������û�з��ţ��޷����붨����ʵ�֣��ҿ���ͷ�ļ�����
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

}

#endif