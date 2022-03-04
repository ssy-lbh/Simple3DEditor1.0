#ifndef __UTILS_OS_APPFRAME__
#define __UTILS_OS_APPFRAME__

#include <define.h>

#include <utils/String.h>

#ifdef PLATFORM_WINDOWS
#include <windef.h>
#include <winuser.h>
#include <handleapi.h>
#endif

// 应保证单个线程内为单例
class AppFrame final : public Object {
#ifdef PLATFORM_WINDOWS
private:
    static bool init;

public:
    HINSTANCE hInst;
    HWND hWnd;
    HACCEL hAccel;
    HDC hDC;
    HGLRC hRC;
    MSG Msg;
    HANDLE hThread = INVALID_HANDLE_VALUE;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void FireEvent(IWindow* window, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void InitWindow();
#endif

private:
    String name;
    IWindow* mainFrame;
    size_t width;
    size_t height;

    LocalData* data;
    ViewManager* viewMgr;

    static void Initialize();

public:
    enum MessageType {
        MESSAGE_TIMER,
        MESSAGE_LEFTDOWN,
        MESSAGE_LEFTUP,
        MESSAGE_RIGHTDOWN,
        MESSAGE_RIGHTUP,
        MESSAGE_OTHERS
    };

    bool reqRender = false;
    bool cursorSelected = false;

    uint fontASCII;

    AppFrame(String name, IWindow* mainFrame, size_t height, size_t width, bool async = false);
    ~AppFrame();

    static AppFrame* GetLocalInst();

    String GetCaption();
    void SetSize(size_t height, size_t width);
    size_t GetHeight();
    size_t GetWidth();
    IWindow* GetMainFrame();
    LocalData* GetLocalData();
    ViewManager* GetViewManager();

    void Show();
    void Hide();
    void EnableOpenGL();
    void EnableOpenGLEXT();
    void DisableOpenGL();
    // 返回否为退出
    bool WaitHandleEvent();
    bool HandleEvents();
    void Render();
    void SwapBuffer();

    int MainLoop();
    bool WaitQuit();

    MessageType GetLastMessageType();
    int GetExitCode();
};

#endif