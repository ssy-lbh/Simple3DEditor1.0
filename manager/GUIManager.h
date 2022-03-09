#ifndef __MANAGER_GUIMANAGER__
#define __MANAGER_GUIMANAGER__

#include <define.h>

#include <utils/List.h>
#include <utils/math3d/LinearAlgebra.h>

//TODO 设计未完成，计划应用2x4矩阵
class Transform2D : public Object {
public:
    Vector2 translation;

    Transform2D();
    ~Transform2D();
};

//TODO 设计未完成，尚未应用于程序
class AGUIObject : public Object {
public:
    Transform2D transform;

private:
    List<AGUIObject*> children;
    AGUIObject* parent;

public:
    AGUIObject();
    ~AGUIObject();

    void CursorMove(Vector2 pos);
    void AddButton(IButton* btn);
    void DeleteButton(IButton* btn);
    void Render();
    void RenderWithDepth();
    bool LeftDown();
    bool LeftUp();
    bool RightDown();
    bool RightUp();
    bool Wheel(int delta);
    bool Char(char c);
    bool Unichar(wchar_t c);
    void Foreach(void(*func)(IButton*));
    void Foreach(void(*func)(IButton*, void*), void* user);

    IButton* GetCurrent();
    IButton* FindCurrent();
};

#endif