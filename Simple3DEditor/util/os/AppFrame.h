#ifndef __UTIL_OS_APPFRAME__
#define __UTIL_OS_APPFRAME__

#include <define.h>

#include <util/String.h>
#include <util/os/Font.h>

#include <windows.h>

namespace simple3deditor {

// Ӧ��֤�����߳���Ϊ����
class AppFrame final : public Object {
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

    // ��Щ����List��wglDeleteContext����
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
    // ���ط�Ϊ�˳�
    bool WaitHandleEvent();
    bool HandleEvents();
    void Render();
    void SwapBuffer();
    // ˵������֡���棬�������ֱͬ��
    // ��������ܴ�˵����ʾ��������������Ⱦ�����Ļ�����Ҳ������
    // ���һ���˺�ѣ��˷�CPU��GPU��Դ
    // �������ò��û�п���ֱͬ�����һ����Լ��ҷ�������

    int MainLoop();
    bool WaitQuit();

    MessageType GetLastMessageType();
    int GetExitCode();
};

}

#endif