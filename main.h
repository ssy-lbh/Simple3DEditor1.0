#include <windows.h>
#include <windowsx.h>

//#include <gl/glew.h>

#include <gl/gl.h>
#include <gl/glu.h>

#include "geodef.h"
#include "menu.h"
#include "mesh.h"
#include "uimgr.h"
#include "colorboard.h"

class Main;

class Main {
public:
    Vector2 cursorPos = Vector2::zero;
    Vector3 cursorDir = Vector3::zero;
    RECT cliRect;
    Vector2 cliSize, cliInvSize;
    float aspect;

    HINSTANCE hInst;
    HWND hWnd;

private:
    Vector3 camLookat = Vector3::zero;
    Quaternion camDir = Quaternion::one;
    float camDis = 5.0f;
    Vector3 camPos = Vector3::back * 5.0f;
    Vector3 camRight = Vector3::right;
    Vector3 camUp = Vector3::up;
    Vector3 camForward = Vector3::forward;
    float camRange = 100.0f;

    Menu* menu = NULL;
    Vector2 menuPos = Vector2::zero;

    Menu* testMenu = new Menu();

    UIManager* uiMgr = new UIManager();
    ViewportManager* viewportMgr = new ViewportManager();
    Mesh* mesh = new Mesh();

    List<Vertex*> selectedPoints;

    IOperation* curOp = NULL;

    ColorBoard* colorBoard = NULL;

    char inputText[MAX_PATH + 1];

    class MoveButton : public IButton {
    private:
        Vector2 center;
        float radius;
        Vector3 start;
    public:
        MoveButton(Vector2 center, float radius);
        virtual ~MoveButton() override;
        virtual bool Trigger(Vector2 pos) override;
        virtual void Render() override;
        virtual void Click() override;
        virtual void Drag(Vector2 dir) override;
    };

    class RotateButton : public IButton {
    private:
        Vector2 center;
        float radius;
        Quaternion start;
        bool dragged;
    public:
        RotateButton(Vector2 center, float radius);
        virtual ~RotateButton() override;
        virtual bool Trigger(Vector2 pos) override;
        virtual void Render() override;
        virtual void Click() override;
        virtual void Drag(Vector2 dir) override;
        virtual void Leave() override;
    };

    class MoveOperation : public IOperation {
    private:
        Vector2 start;
        Vertex* target;
        Vector3 startPos;
        bool x, y, z;
    public:
        MoveOperation();
        virtual ~MoveOperation() override;
        virtual void OnEnter() override;
        virtual void OnMove() override;
        virtual void OnCommand(UINT id) override;
    };
public:
    static Main* inst;

    Main();
    ~Main();
    void InitCamera();
    void InitLight0();
    void RenderFrame(HWND hWnd, HDC hDC);
    void SetMenu(Menu* m);
    void UpdateWindowSize(HWND hWnd);
    void UpdateCursor(int x, int y);
    void UpdateLookAtLocation();
    void UpdateRotation();
    void UpdateDistance();
    LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    ATOM RegClass(HINSTANCE hInstance);
    int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
    void GetTextInput();
};