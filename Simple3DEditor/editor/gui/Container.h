#ifndef __EDITOR_GUI_CONTAINER__
#define __EDITOR_GUI_CONTAINER__

#include <define.h>

#include <editor/main/Window.h>

namespace simple3deditor {

class AContainer : public AWindow {
protected:
    // 以下对象用于窗口归并等功能
    SelectionWindow* selWindow = NULL;

    friend class SelectionWindow;

    AContainer();
    AContainer(SelectionWindow* selWindow);
};

//TODO 容器期望集成选择夹、属性窗口等容器
class LRContainer : public AContainer {
public:
    WINDOW_INFO_DEF();

    static constexpr int INIT_DIS = 50;
    static constexpr Vector2 INIT_SIZE = Vector2(2.0f * INIT_DIS, 0.0f);

private:
    AWindow* lWindow;
    AWindow* rWindow;
    int dis = INIT_DIS;
    AWindow* focusWindow = NULL;
    bool right = false;
    bool adjustPos = false;
    bool dragEnable = true;

    Menu* joinMenu = NULL;

    void InitMenu();

public:
    LRContainer();
    LRContainer(AWindow* lWindow, AWindow* rWindow);
    LRContainer(AWindow* lWindow, AWindow* rWindow, SelectionWindow* selWindow);
    ~LRContainer();

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnTimer(int id) override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;
    virtual void OnDropFileA(const char* path, uint len) override;
    virtual void OnDropFileW(const wchar_t* path, uint len) override;

    virtual void Serialize(nlohmann::json& o) override;
    virtual void Deserialize(nlohmann::json& o) override;

    void UpdateFocus();
    void FreeWindow();
    AWindow* GetLeftWindow();
    AWindow* GetRightWindow();
    void EnableDrag();
    void DisableDrag();
    bool DragEnabled();
};

class UDContainer : public AContainer {
public:
    WINDOW_INFO_DEF();

    static constexpr int INIT_DIS = 50;
    static constexpr Vector2 INIT_SIZE = Vector2(0.0f, 2.0f * INIT_DIS);

private:
    AWindow* uWindow;
    AWindow* dWindow;
    int dis = INIT_DIS;
    AWindow* focusWindow = NULL;
    bool up = false;
    bool adjustPos = false;
    bool dragEnable = true;

    Menu* joinMenu = NULL;

    void InitMenu();

public:
    UDContainer();
    UDContainer(AWindow* uWindow, AWindow* dWindow);
    UDContainer(AWindow* uWindow, AWindow* dWindow, SelectionWindow* selWindow);
    virtual ~UDContainer() override;

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnTimer(int id) override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;
    virtual void OnDropFileA(const char* path, uint len) override;
    virtual void OnDropFileW(const wchar_t* path, uint len) override;

    virtual void Serialize(nlohmann::json& o) override;
    virtual void Deserialize(nlohmann::json& o) override;

    void UpdateFocus();
    void FreeWindow();
    AWindow* GetUpWindow();
    AWindow* GetDownWindow();
    void EnableDrag();
    void DisableDrag();
    bool DragEnabled();
};

class SelectionWindow : public AWindow {
private:
    AWindow* curWindow;
    Menu* selMenu;

    void InitMenu();

public:
    WINDOW_INFO_DEF();

    SelectionWindow();
    SelectionWindow(AWindow* initialWnd);
    virtual ~SelectionWindow() override;

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnTimer(int id) override;
    virtual void OnChar(char c) override;
    virtual void OnUnichar(wchar_t c) override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnMenuAccel(int id, bool accel) override;
    virtual void OnDropFileA(const char* path, uint len) override;
    virtual void OnDropFileW(const wchar_t* path, uint len) override;

    virtual void Serialize(nlohmann::json& o) override;
    virtual void Deserialize(nlohmann::json& o) override;

    AWindow* GetWindow();
    void SetWindow(AWindow* window, bool del = true);
};

}

#endif