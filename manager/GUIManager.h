#ifndef __MANAGER_GUIMANAGER__
#define __MANAGER_GUIMANAGER__

#include <define.h>

#include <utils/math3d/LinearAlgebra.h>

//TODO 设计未完成，计划应用2x4矩阵
class GUITransform : public Object {
public:
    Vector2 translation;

    GUITransform();
    ~GUITransform();
};

//TODO 设计未完成，尚未应用于程序
class GUIManager : public Object {

};

#endif