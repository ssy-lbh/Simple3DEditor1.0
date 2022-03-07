#ifndef __EDITOR_GUI_GUIMANAGER__
#define __EDITOR_GUI_GUIMANAGER__

#include <define.h>

#include <utils/math3d/LinearAlgebra.h>

//TODO 设计未完成，计划应用2x4矩阵
class GUITransform {
public:
    Vector2 translation;

    GUITransform();
    ~GUITransform();
};

//TODO 设计未完成，尚未应用于程序
class GUIManager {

};

#endif