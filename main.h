#include <windows.h>
#ifndef __MAIN__
#define __MAIN__

#include "define.h"

#include <windows.h>
#include <windowsx.h>

#include <gl/gl.h>
#include <gl/glu.h>

#include "geodef.h"
#include "menu.h"
#include "mesh.h"
#include "container.h"
#include "colorboard.h"

class MainWindow;
class Main;

class MainWindow : public IWindow {
public:
    Vector2 cursorPos = Vector2::zero;
    Vector3 cursorDir = Vector3::zero;
    Vector2 cliSize, cliInvSize;
    float aspect;

    HWND hWnd;
    HINSTANCE hInst;

    bool focus = true;

private:
    Vector3 camLookat = Vector3::up;
    Quaternion camDir = Quaternion::one;
    float camDis = 5.0f;
    Vector3 camPos = Vector3::back * 5.0f + Vector3::up;
    Vector3 camRight = Vector3::right;
    Vector3 camUp = Vector3::up;
    Vector3 camForward = Vector3::forward;
    float camRange = 100.0f;

    Menu* menu = NULL;
    Vector2 menuPos = Vector2::zero;

    Menu* basicMenu;

    UIManager* uiMgr;
    Mesh* mesh;

    List<Vertex*> selectedPoints;

    IOperation* curOp = NULL;

    ColorBoard* colorBoard = NULL;

    ITool* curTool = NULL;

    wchar_t inputText[MAX_PATH + 1];
    bool inputConfirm;

    class MoveButton : public IButton {
    private:
        Vector2 center;
        float radius;
        Vector3 start;
        MainWindow* main;
    public:
        MoveButton(Vector2 center, float radius, MainWindow* main);
        virtual ~MoveButton() override;
        virtual bool Trigger(Vector2 pos) override;
        virtual void Render() override;
        virtual void Click() override;
        virtual void Drag(Vector2 dir) override;
        virtual void ClickEnd() override;
    };

    class RotateButton : public IButton {
    private:
        Vector2 center;
        float radius;
        Quaternion start;
        MainWindow* main;
    public:
        RotateButton(Vector2 center, float radius, MainWindow* main);
        virtual ~RotateButton() override;
        virtual bool Trigger(Vector2 pos) override;
        virtual void Render() override;
        virtual void Click() override;
        virtual void Drag(Vector2 dir) override;
        virtual void ClickEnd() override;
    };

    class MoveOperation : public IOperation {
    private:
        struct MoveInfo {
            Vertex* vert;
            Vector3 pos;
        };

        Vector2 start;
        List<MoveInfo> moveInfo;
        bool x, y, z;
        MainWindow* main;
    public:
        MoveOperation(MainWindow* main);
        virtual ~MoveOperation() override;
        virtual void OnEnter() override;
        virtual void OnMove() override;
        virtual void OnCommand(UINT id) override;
        virtual void OnConfirm() override;
        virtual void OnUndo() override;
    };

    class EmptyTool : public ITool {
    private:
        MainWindow* window;

    public:
        EmptyTool(MainWindow* window);
        ~EmptyTool() override;
        virtual void OnLeftDown() override;
    };

    class SelectTool : public ITool {
    private:
        MainWindow* window;
        Vector2 start;
        Vector2 end;
        bool leftDown;
        
    public:
        SelectTool(MainWindow* window);
        virtual ~SelectTool() override;
        virtual void OnLeftDown() override;
        virtual void OnLeftUp() override;
        virtual void OnMove() override;
        virtual void OnRender() override;
    };

public:
    MainWindow(HINSTANCE hInstance);
    virtual ~MainWindow() override;

    void InitCamera();
    void InitLight0();

    void RenderModelView();
    void SetMenu(Menu* m);
    void SetTool(ITool* tool);

    void UpdateWindowSize(int x, int y);
    void UpdateCursor(int x, int y);
    void UpdateLookAtLocation();
    void UpdateRotation();
    void UpdateDistance();

    virtual bool IsFocus() override;
    virtual void OnRender() override;
    virtual void OnCreate(HWND hWnd) override;
    virtual void OnClose() override;
    virtual void OnResize(int x, int y) override;
    virtual void OnMouseMove(int x, int y) override;
    virtual void OnLeftDown(int x, int y) override;
    virtual void OnLeftUp(int x, int y) override;
    virtual void OnRightDown(int x, int y) override;
    virtual void OnRightUp(int x, int y) override;
    virtual void OnMouseWheel(int delta) override;
    virtual void OnFocus() override;
    virtual void OnKillFocus() override;
    virtual void OnMenuAccel(int id, bool accel) override;

    void OnInsSave();
    void OnInsLoad();
    void OnInsMove();
    void OnInsTopology();
    void OnInsSelectColor();

    void GetTextInput();
    void AddPoint();
    void DeletePoint();
    bool SaveMesh(Mesh* mesh);
    bool LoadMesh(Mesh* mesh);
    void AboutBox();
    Vector3 GetLookPosition(Vector3 pos);
};

class Main {
public:
    static HINSTANCE hInst;
    static HWND hWnd;
    static HDC hDC;
    static HGLRC hRC;
    static Main* inst;

    IWindow* mainWnd;
    RECT mainRect;
    // 测试一下面向对象的结果，成功实现双屏
    IWindow* mainWnd2;
    RECT mainRect2;

    LRContainer* container;

    Main();
    ~Main();
    ATOM RegClass();
    HWND CreateWnd();
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT LocalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void FireEvent(IWindow* window, RECT rect, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void FireEvent(IWindow* window, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void OnResize(int x, int y);
    void OnRender();

    int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
};

#endif