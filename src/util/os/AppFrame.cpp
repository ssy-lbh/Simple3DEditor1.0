#include <util/os/AppFrame.h>

#include <main.h>
#include <res.h>
#include <util/os/Font.h>
#include <util/os/Time.h>
#include <util/os/Resource.h>
#include <editor/main/ViewManager.h>
#include <editor/main/ViewObject.h>
#include <editor/gui/UIManager.h>

#include <thread>

#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_impl_glfw.h>
#include <lib/imgui/imgui_impl_opengl3.h>

#include <lib/glfw/GLFW/glfw3.h>

namespace simple3deditor {

thread_local AppFrame* appFrame;

const char* AppFrame::GLSL_VERSION = "#version 130";

static void ThreadStart(AppFrame* frame){
    appFrame = frame;

    frame->InitWindow();
    frame->MainLoop();

    delete frame;
}

AppFrame::AppFrame(String name, AWindow* mainFrame, size_t height, size_t width, bool async) : name(name), mainFrame(mainFrame), height(height), width(width) {
    DebugLog("AppFrame %s Launched", name.GetString());

    if (async){
        std::thread t(ThreadStart, this);
        t.detach();
        return;
    }

    appFrame = this;
    InitWindow();
}

AppFrame::~AppFrame(){
    DebugLog("AppFrame %p Destoryed", this);
    if (mainFrame) delete mainFrame;
    if (data) delete data;
    if (viewMgr) delete viewMgr;
    if (fontCache) delete[] fontCache;
    
    ImGui::SetCurrentContext(imguiCtx);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(imguiCtx);

    glfwDestroyWindow(window);
}

static void glfwErrorCallback(int error, const char* description){
    DebugError("GLFW Error %d: %s", error, description);
}

bool AppFrame::Initialize(){
    IMGUI_CHECKVERSION();

    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit())
        return false;
    
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 0);

    return true;
}

void AppFrame::Uninitialize(){
    glfwTerminate();
}

AppFrame* AppFrame::GetLocalInst(){
    return appFrame;
}

String AppFrame::GetCaption(){
    return name;
}

void AppFrame::SetSize(size_t height, size_t width){
    glfwSetWindowSize(window, (int)width, (int)height);
}

Rect AppFrame::GetClientRect(){
    int left, top, right, bottom;

    glfwGetWindowFrameSize(window, &left, &top, &right, &bottom);

    return Rect(left, right, bottom, top);
}

size_t AppFrame::GetHeight(){
    int top, bottom;

    glfwGetWindowFrameSize(window, nullptr, &top, nullptr, &bottom);

    return (size_t)(top - bottom);
}

size_t AppFrame::GetWidth(){
    int left, right;

    glfwGetWindowFrameSize(window, &left, nullptr, &right, nullptr);

    return (size_t)(right - left);
}

AWindow* AppFrame::GetMainFrame(){
    return mainFrame;
}

LocalData* AppFrame::GetLocalData(){
    return data;
}

ViewManager* AppFrame::GetViewManager(){
    return viewMgr;
}

void AppFrame::Show(){
    glfwShowWindow(window);
}

void AppFrame::Hide(){
    glfwHideWindow(window);
}

bool AppFrame::WaitHandleEvent(){
    glfwWaitEvents();
    glfwPollEvents();
    return true;
}

bool AppFrame::HandleEvents(){
    glfwPollEvents();
    return true;
}

void AppFrame::Render(){
    //viewMgr->Reset();
    //viewMgr->EnableScissor();

    mainFrame->OnRender();

    //viewMgr->Reset();

    //data->Render();
}

void AppFrame::SwapBuffer(){
    glfwSwapBuffers(window);
}

int AppFrame::GetExitCode(){
    return 0;
}

int AppFrame::MainLoop(){
    LocalData* localData = LocalData::GetLocalInst();

    localData->recTime = Time::GetTime();
    localData->deltaTime = 0.0f;

    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        this->Render();

        // Rendering
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

        float time = Time::GetTime();
        localData->deltaTime = time - localData->recTime;
        localData->recTime = time;
    }

    return 0;
}

void AppFrame::InitWindow(){
    data = new LocalData();
    viewMgr = new ViewManager(this);

    window = glfwCreateWindow(width, height, name.GetString(), nullptr, nullptr);

    imguiCtx = ImGui::CreateContext();
    ImGui::SetCurrentContext(imguiCtx);
    ImGui::StyleColorsDark();
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);

    //DebugLog("OpenGL Enabled");
    //DebugLog("OpenGL Version %s", glGetString(GL_VERSION));
    //DebugLog("OpenGL Renderer %s", glGetString(GL_RENDERER));
    //DebugLog("OpenGL Vendor %s", glGetString(GL_VENDOR));
    //DebugLog("OpenGL Extensions %s", glGetString(GL_EXTENSIONS));

    //glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}

void AppFrame::SetCursor(int id){}

void AppFrame::SetCursor(const char* res){}

void AppFrame::SaveImage(const String& file, Rect rect){
    size_t width = (size_t)rect.GetWidth();
    size_t height = (size_t)rect.GetHeight();
    size_t size = (width * height) << 2;
    char* buffer = new char[size];

    DebugLog("SaveImage %s", file.GetString());

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(rect.left, rect.bottom, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    Resource::StoreImage(file, buffer, width, height, 4);

    delete[] buffer;
}

}