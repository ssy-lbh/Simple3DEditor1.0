#include <util/os/AppFrame.h>

#include <main.h>
#include <res.h>
#include <util/os/Font.h>
#include <util/os/Time.h>
#include <util/math3d/Mesh.h>
#include <util/os/Resource.h>
#include <editor/main/ViewManager.h>
#include <editor/main/ViewObject.h>
#include <editor/gui/UIManager.h>

#include <cstdio>
#include <thread>

#include <lib/imgui/imgui.h>
#include <lib/imgui/imgui_impl_glfw.h>
#include <lib/imgui/imgui_impl_opengl3.h>

#include <lib/glfw/GLFW/glfw3.h>

namespace simple3deditor {

thread_local AppFrame* appFrame;

const char* AppFrame::GLSL_VERSION = "#version 430";

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

    return Rect((float)left, (float)right, (float)bottom, (float)top);
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

void AppFrame::RenderMainMenu(){
    LocalData* data = LocalData::GetLocalInst();

    if (ImGui::BeginMainMenuBar()){
        if (ImGui::BeginMenu("File")){
            if (ImGui::MenuItem("Save Model", "Ctrl+S"));
            if (ImGui::MenuItem("Load Model", "Ctrl+L"));
            ImGui::Separator();
            if (ImGui::MenuItem("Save Workspace", "Ctrl+Alt+S"));
            if (ImGui::MenuItem("Load Workspace", "Ctrl+Alt+L"));
            ImGui::Separator();
            if (ImGui::MenuItem("Save Settings"))
                Main::data->SaveSettings("settings.json");
            if (ImGui::MenuItem("Load Settings")){
                Main::data->LoadSettings("settings.json");
                Main::data->ApplySettings();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4"))
                glfwSetWindowShouldClose(window, true);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void AppFrame::Render(){
    RenderMainMenu();

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
    ImGuiIO& io = ImGui::GetIO();

    localData->recTime = glfwGetTime();
    io.DeltaTime = localData->deltaTime = 1.0f / 60.0f;

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

        float time = (float)glfwGetTime();
        io.DeltaTime = localData->deltaTime = time - localData->recTime;
		localData->recTime = time;
    }

    return 0;
}

void AppFrame::InitWindow(){
    data = new LocalData();
    viewMgr = new ViewManager(this);

    window = glfwCreateWindow((int)width, (int)height, name.GetString(), nullptr, nullptr);

    imguiCtx = ImGui::CreateContext();
    ImGui::SetCurrentContext(imguiCtx);
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

    // TEMP
    // io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
    // io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
    // io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
    // io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
    // io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
    // io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
    // io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
    // io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
    // io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
    // io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
    // io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
    // io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
    // io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
    // io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
    // io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
    // io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
    // io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
    // io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
    // io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
    // io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
    // io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);

    // glfwSetCharCallback();
    // glfwSetCursorPosCallback();
    // glfwSetDropCallback();
    // glfwSetMouseButtonCallback();
    // glfwSetWindowFocusCallback();
    // glfwSetWindowSizeCallback();

    DebugLog("OpenGL Enabled");
    DebugLog("OpenGL Version %s", glGetString(GL_VERSION));
    DebugLog("OpenGL Renderer %s", glGetString(GL_RENDERER));
    DebugLog("OpenGL Vendor %s", glGetString(GL_VENDOR));
    //DebugLog("OpenGL Extensions %s", glGetString(GL_EXTENSIONS));

    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
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
    glReadPixels((GLint)rect.left, (GLint)rect.bottom, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    Resource::StoreImage(file, buffer, width, height, 4);

    delete[] buffer;
}

}