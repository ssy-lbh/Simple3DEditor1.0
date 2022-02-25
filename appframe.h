#ifndef __APPFRAME__
#define __APPFRAME__

#include "define.h"

#include "uimgr.h"
#include "utils.h"

// 应保证为单例
class AppFrame final : public Object {
#ifdef PLATFORM_WINDOWS
private:
    static bool init;

    bool cursorSelected = false;

public:
    HWND hWnd;
    HACCEL hAccel;
    HDC hDC;
    HGLRC hRC;
    MSG Msg;

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK LocalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void FireEvent(IWindow* window, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif
public:
    static AppFrame* inst;

private:
    String name;
    IWindow* mainFrame;
    size_t width;
    size_t height;

public:
    enum MessageType {
        MESSAGE_TIMER,
        MESSAGE_LEFTDOWN,
        MESSAGE_LEFTUP,
        MESSAGE_RIGHTDOWN,
        MESSAGE_RIGHTUP,
        MESSAGE_OTHERS
    };

    AppFrame(String name, IWindow* mainFrame, size_t height, size_t width);
    ~AppFrame();

    String GetCaption();
    void SetSize(size_t height, size_t width);
    size_t GetHeight();
    size_t GetWidth();
    IWindow* GetMainFrame();

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

    MessageType GetLastMessageType();
    int GetExitCode();
};

#endif