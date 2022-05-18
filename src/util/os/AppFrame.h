#ifndef __UTIL_OS_APPFRAME__
#define __UTIL_OS_APPFRAME__

#include <define.h>

#include <util/String.h>
#include <util/os/Font.h>

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
    void FireEvent(AWindow* window, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void InitWindow();
#endif

private:
    String name;
    AWindow* mainFrame;
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

    bool cursorSelected = false;

    // 这些字体List由wglDeleteContext回收
    uint fontASCII = 0;
    uint* fontCache = NULL;

    AppFrame(String name, AWindow* mainFrame, size_t height, size_t width, bool async = false);
    ~AppFrame();

    static AppFrame* GetLocalInst();

    String GetCaption();
    void SetSize(size_t height, size_t width);
    Rect GetClientRect();
    size_t GetHeight();
    size_t GetWidth();
    AWindow* GetMainFrame();
    LocalData* GetLocalData();
    ViewManager* GetViewManager();

    void Show();
    void Hide();
    void EnableOpenGL();
    void DisableOpenGL();
    // 返回否为退出
    bool WaitHandleEvent();
    bool HandleEvents();
    void Render();
    void SwapBuffer();
    // 说到交换帧缓存，如果不垂直同步
    // 性能问题很大不说，显示器跟不上来，渲染出来的画面人也看不到
    // 而且画面撕裂，浪费CPU、GPU资源
    // 我这电脑貌似没有开垂直同步，我还得自己找方法开启

    int MainLoop();
    bool WaitQuit();

    MessageType GetLastMessageType();
    int GetExitCode();
};

#endif