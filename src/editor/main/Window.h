#ifndef __EDITOR_MAIN_WINDOW__
#define __EDITOR_MAIN_WINDOW__

#include <define.h>

#include <io/Serializable.h>
#include <util/math3d/LinearAlgebra.h>

namespace simple3deditor {

#define WINDOW_INFO_DEF()\
    static const char* WINDOW_ID;\
    static const wchar_t* WINDOW_DISPLAY_NAME

//! ע��:�ݶ�clazz������[����namespace��'::'����]+[����]�ϸ����֣�ID����ű�ͬ��Ψһ
#define WINDOW_INFO_DECL(clazz, displayName)\
    const char* clazz::WINDOW_ID = #clazz;\
    const wchar_t* clazz::WINDOW_DISPLAY_NAME = displayName;

class AWindow : public Object, public IMemorable {
protected:
    bool focus = false;

    Point2 cursorCoord;
    Point2 cursorPos = Point2::zero;
    Vector2 cliSize;
    Vector2 cliInvSize;
    float aspect;

    void UpdateCursor(int x, int y);
    void UpdateWindowSize(int x, int y);

public:
    WINDOW_INFO_DEF();

    AWindow();
    virtual ~AWindow();

    virtual bool IsFocus();
    // Ŀǰ�ƻ���ʵ��һ�׿��ύ�����λ�ú�֡�����ȫ����Ⱦ��ʵ����Ӱ��ȵĲɼ�
    virtual void OnForwardRender();
    // ����OpenGL����Ⱦ�������Ⱦȫ���������ó���ɻ���API
    virtual void OnRender();
    // �ƻ����ں��ڴ�������MC��������Ч
    virtual void OnPostRender();
    virtual void OnTimer(int id);
    virtual void OnChar(char c);
    virtual void OnUnichar(wchar_t c);
    // ���� (0, 0) ����Ч
    virtual void OnResize(int x, int y);
    virtual void OnMouseMove(int x, int y);
    virtual void OnLeftDown(int x, int y);
    virtual void OnLeftUp(int x, int y);
    virtual void OnRightDown(int x, int y);
    virtual void OnRightUp(int x, int y);
    virtual void OnFocus();
    virtual void OnKillFocus();
    virtual void OnMouseWheel(int delta);
    virtual void OnMenuAccel(int id, bool accel);
    virtual void OnDropFileA(const char* path, uint len);
    virtual void OnDropFileW(const wchar_t* path, uint len);

    // �������л�ʱӦ��o["id"] = WINDOW_ID�����������ڷ����л�ʱʶ�𴰿�
    virtual void Serialize(nlohmann::json& o) override;
    // ����WINDOW_ID���������ݰ�˳�����ζ�ȡ����
    virtual void Deserialize(nlohmann::json& o) override;
};

class AGUIWindow : public AWindow {
protected:
    GUIManagerObject* guiMgr;

public:
    WINDOW_INFO_DEF();

    AGUIWindow();
    virtual ~AGUIWindow() override;

    virtual void OnRender() override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;

    virtual void Serialize(nlohmann::json& o) override;
    using AWindow::Deserialize;
};

}

#endif