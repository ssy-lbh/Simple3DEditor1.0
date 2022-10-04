#ifndef __UTIL_OS_APPFRAME__
#define __UTIL_OS_APPFRAME__

#include <define.h>

#include <util/String.h>
#include <util/os/Font.h>

namespace simple3deditor {

// Ӧ��֤�����߳���Ϊ����
class AppFrame final : public Object {
public:
    void InitWindow();

private:
    String name;

    ::ImGuiContext* imguiCtx;
    ::GLFWwindow* window;

    AWindow* mainFrame;
    size_t width;
    size_t height;

    LocalData* data;
    ViewManager* viewMgr;

    void RenderMainMenu();

public:
    static const char* GLSL_VERSION;

    // ��Щ����List��wglDeleteContext����
    uint fontASCII = 0;
    uint* fontCache = NULL;

    AppFrame(String name, AWindow* mainFrame, size_t height, size_t width, bool async = false);
    ~AppFrame();

    static bool Initialize();
    static void Uninitialize();
    static AppFrame* GetLocalInst();

    String GetCaption();
    void SetSize(size_t height, size_t width);
    Rect GetClientRect();
    size_t GetHeight();
    size_t GetWidth();
    AWindow* GetMainFrame();
    LocalData* GetLocalData();
    ViewManager* GetViewManager();
    inline ::GLFWwindow* GetWindow(){ return window; }

    void Show();
    void Hide();
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
    int GetExitCode();

    void SetCursor(int id);
    void SetCursor(const char* res);

    void SaveImage(const String& file, Rect rect);
};

}

#endif