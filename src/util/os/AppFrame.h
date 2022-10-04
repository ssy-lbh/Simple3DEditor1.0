#ifndef __UTIL_OS_APPFRAME__
#define __UTIL_OS_APPFRAME__

#include <define.h>

#include <util/String.h>
#include <util/os/Font.h>

namespace simple3deditor {

// 应保证单个线程内为单例
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

    // 这些字体List由wglDeleteContext回收
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
    int GetExitCode();

    void SetCursor(int id);
    void SetCursor(const char* res);

    void SaveImage(const String& file, Rect rect);
};

}

#endif