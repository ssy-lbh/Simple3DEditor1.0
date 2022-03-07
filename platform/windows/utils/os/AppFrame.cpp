#include <utils/os/AppFrame.h>

#include <windows.h>
#include <windowsx.h>
#include <dbt.h>

#include <main.h>
#include <res.h>
#include <utils/os/GLFunc.h>
#include <utils/os/Font.h>
#include <utils/os/Thread.h>
#include <utils/math3d/ViewObject.h>
#include <editor/gui/UIManager.h>
#include <editor/gui/ViewManager.h>

bool AppFrame::init = false;

AppFrame::AppFrame(String name, IWindow* mainFrame, size_t height, size_t width, bool async) : name(name), mainFrame(mainFrame), height(height), width(width) {
    DebugLog("AppFrame %p Launched", this);
    
    hInst = GetModuleHandleA(NULL);
    
    Initialize();

    if (async){
        hThread = CreateThread(NULL, 0, [] CALLBACK (LPVOID data) -> DWORD {
            AppFrame* frame = (AppFrame*)data;
            DWORD ret;

            frame->InitWindow();
            ret = frame->MainLoop();

            delete frame;

            return ret;
        }, this, 0, NULL);
        CloseHandle(hThread);
        return;
    }

    InitWindow();
}

AppFrame::~AppFrame(){
    DebugLog("AppFrame %p Destoryed", this);
    if (mainFrame) delete mainFrame;
    if (data) delete data;
    if (viewMgr) delete viewMgr;
    DestroyWindow(hWnd);
}

void AppFrame::Initialize(){
    HINSTANCE hInst;

    if (!init){
        WNDCLASSEXA wc;

        init = true;

        hInst = GetModuleHandleA(NULL);

        wc.cbSize = sizeof(WNDCLASSEXA);
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hCursor = LoadCursorA(hInst, MAKEINTRESOURCEA(IDC_NORMAL));
        wc.hIcon = LoadIconA(hInst, MAKEINTRESOURCE(IDI_ICON1));
        wc.hIconSm = LoadIconA(hInst, MAKEINTRESOURCE(IDI_ICON1));
        wc.hInstance = hInst;
        wc.lpfnWndProc = AppFrame::WndProc;
        wc.lpszClassName = "ModelView.MainWindow";
        wc.lpszMenuName = "";
        wc.style = 0;

        RegisterClassExA(&wc);

        ThreadLocal::Alloc(THREAD_LOCAL_APPFRAME);
        ThreadLocal::Alloc(THREAD_LOCAL_LOCALDATA);
        ThreadLocal::Alloc(THREAD_LOCAL_VIEWMGR);
    }
}

AppFrame* AppFrame::GetLocalInst(){
    return (AppFrame*)ThreadLocal::Get(THREAD_LOCAL_APPFRAME);
}

String AppFrame::GetCaption(){
    return name;
}

void AppFrame::SetSize(size_t height, size_t width){
    SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, width, height, SWP_NOMOVE);
}

size_t AppFrame::GetHeight(){
    return height;
}

size_t AppFrame::GetWidth(){
    return width;
}

IWindow* AppFrame::GetMainFrame(){
    return mainFrame;
}

LocalData* AppFrame::GetLocalData(){
    return data;
}

ViewManager* AppFrame::GetViewManager(){
    return viewMgr;
}

void AppFrame::Show(){
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
}

void AppFrame::Hide(){
    ShowWindow(hWnd, SW_HIDE);
}

void AppFrame::EnableOpenGL(){
    PIXELFORMATDESCRIPTOR pfd;

    hDC = GetDC(hWnd);

    RtlZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int iFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, iFormat, &pfd);

    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);
}

void AppFrame::EnableOpenGLEXT(){
    glInitEXTFunctions();
}

void AppFrame::DisableOpenGL(){
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}

bool AppFrame::WaitHandleEvent(){
    GetMessageA(&Msg, NULL, 0, 0);
    //DebugLog("%p %d %d %d", Msg.hwnd, Msg.message, Msg.wParam, Msg.lParam);
    if (!TranslateAcceleratorA(Msg.hwnd, hAccel, &Msg)){
        if (Msg.message == WM_QUIT){
            return false;
        } else {
            TranslateMessage(&Msg);
            DispatchMessageA(&Msg);
        }
    }
    return true;
}

bool AppFrame::HandleEvents(){
    while (PeekMessageA(&Msg, NULL, 0, 0, PM_REMOVE)){
        if (!TranslateAcceleratorA(Msg.hwnd, hAccel, &Msg)){
            if (Msg.message == WM_QUIT){
                return false;
            } else {
                TranslateMessage(&Msg);
                DispatchMessageA(&Msg);
            }
        }
    }
    return true;
}

void AppFrame::Render(){
    wglMakeCurrent(hDC, hRC);

    viewMgr->Reset();
    viewMgr->EnableScissor();

    mainFrame->OnRender();

    viewMgr->Reset();

    data->Render();
}

void AppFrame::SwapBuffer(){
    SwapBuffers(hDC);
}

AppFrame::MessageType AppFrame::GetLastMessageType(){
    switch (Msg.message){
    case WM_TIMER: return MESSAGE_TIMER;
    case WM_LBUTTONDOWN: return MESSAGE_LEFTDOWN;
    case WM_LBUTTONUP: return MESSAGE_LEFTUP;
    case WM_RBUTTONDOWN: return MESSAGE_RIGHTDOWN;
    case WM_RBUTTONUP: return MESSAGE_RIGHTUP;
    default: return MESSAGE_OTHERS;
    }
}

int AppFrame::GetExitCode(){
    return Msg.wParam;
}

int AppFrame::MainLoop(){
    EnableOpenGL();
    EnableOpenGLEXT();

    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glFontSize(12);
    glInitASCIIFont();

    // 放在这里是为了让OpenGL初始化
    mainFrame->OnCreate();

    Show();
    while (GetMessageA(&Msg, NULL, 0, 0)){
        if (!TranslateAcceleratorA(Msg.hwnd, hAccel, &Msg)){
            if (Msg.message == WM_QUIT){
                break;
            } else {
                TranslateMessage(&Msg);
                DispatchMessageA(&Msg);
                if (reqRender || Msg.message != WM_TIMER){
                    reqRender = false;
                    Render();
                    SwapBuffer();
                }
            }
        }
    }

    DisableOpenGL();

    return Msg.wParam;
}

bool AppFrame::WaitQuit(){
    if (hThread == INVALID_HANDLE_VALUE)
        return false;
    return WaitForSingleObject(hThread, INFINITE) == WAIT_OBJECT_0;
}

LRESULT CALLBACK AppFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    AppFrame* inst = (AppFrame*)ThreadLocal::Get(THREAD_LOCAL_APPFRAME);
    return inst->LocalWndProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK AppFrame::LocalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg){
    case WM_CREATE:
        DragAcceptFiles(hWnd, TRUE);
        // 非零ID才有效
        // 传入TimerProc时可能不同步但是实时性好
        SetTimer(hWnd, 1, 10, NULL);
        break;
    case WM_CLOSE:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    case WM_TIMER:
        Main::data->scene->OnTimer(wParam);
        break;
    case WM_SIZE:
        data->UpdateWindowSize(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_MOUSEMOVE:
        data->UpdateCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_LBUTTONDOWN:
        // 这一段是临时补的，为了出现菜单时的左键事件不发送至窗口
        if (data->menu){
            data->OnLeftDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            if (!cursorSelected)
                Main::SetCursor(IDC_CLICKED);
            cursorSelected = false;
            return DefWindowProcA(hWnd, uMsg, wParam, lParam);
        }
        data->OnLeftDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        SetCapture(hWnd);
        break;
    case WM_LBUTTONUP:
        data->OnLeftUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        ReleaseCapture();
        break;
    case WM_RBUTTONDOWN:
        data->OnRightDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_RBUTTONUP:
        data->OnRightUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_CHAR:
        Main::data->scene->OnChar(wParam);
        break;
    case WM_UNICHAR:
        if (wParam == UNICODE_NOCHAR)
            break;
        Main::data->scene->OnUnichar(wParam);
        break;
    case WM_COMMAND:
        switch (HIWORD(wParam)){
        case 0:
            data->OnMenuAccel(LOWORD(wParam), false);
            Main::data->scene->OnMenuAccel(LOWORD(wParam), false);
            break;
        case 1:
            data->OnMenuAccel(LOWORD(wParam), true);
            Main::data->scene->OnMenuAccel(LOWORD(wParam), true);
            break;
        }
        break;
    case WM_DEVICECHANGE:
        switch (wParam){
        case DBT_DEVNODES_CHANGED:
            DebugLog("System Device Change %d", wParam);
            break;
        case DBT_DEVICEARRIVAL:
            break;
        case DBT_DEVICEREMOVEPENDING:
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            break;
        }
        break;
    }

    //将事件发送至对应整个窗口的组件容器
    FireEvent(mainFrame, hWnd, uMsg, wParam, lParam);

    switch (uMsg){
    case WM_LBUTTONDOWN:
        if (!cursorSelected)
            SetCursor(LoadCursorA(hInst, MAKEINTRESOURCE(IDC_CLICKED)));
        break;
    case WM_UNICHAR:
        // 当其他软件试图使用UNICODE_NOCHAR测试本软件是否处理unicode字符时
        // 给予TRUE作为回应并忽视此消息
        if (wParam == UNICODE_NOCHAR){
            return TRUE;
        }
        break;
    }

    cursorSelected = false;

    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

void AppFrame::FireEvent(IWindow* window, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    RECT cliRect;
    GetClientRect(hWnd, &cliRect);
    // 事件中鼠标坐标上下需反转
    int x = GET_X_LPARAM(lParam), y = cliRect.bottom - GET_Y_LPARAM(lParam);
    switch (uMsg){
    case WM_CLOSE:
        window->OnClose();
        break;
    case WM_TIMER:
        window->OnTimer(wParam);
        break;
    case WM_SIZE:
        window->OnResize(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_MOUSEMOVE:
        window->OnMouseMove(x, y);
        break;
    case WM_MOUSEWHEEL:
        window->OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
        break;
    case WM_MOUSELEAVE:
        window->OnMouseLeave();
        break;
    case WM_MOUSEHOVER:
        window->OnMouseHover(wParam, x, y);
        break;
    case WM_SETFOCUS:
        window->OnFocus();
        break;
    case WM_KILLFOCUS:
        window->OnKillFocus();
        break;
    case WM_LBUTTONDOWN:
        window->OnLeftDown(x, y);
        break;
    case WM_LBUTTONUP:
        window->OnLeftUp(x, y);
        break;
    case WM_RBUTTONDOWN:
        window->OnRightDown(x, y);
        break;
    case WM_RBUTTONUP:
        window->OnRightUp(x, y);
        break;
    case WM_CHAR:
        window->OnChar(wParam);
        // UTF-16格式字符(GB2312)
        break;
    case WM_UNICHAR:
        // UTF-32格式字符
        window->OnUnichar(wParam);
        break;
    case WM_COMMAND:
        switch (HIWORD(wParam)){
        case 0:
            window->OnMenuAccel(LOWORD(wParam), false);
            break;
        case 1:
            window->OnMenuAccel(LOWORD(wParam), true);
            break;
        }
        break;
    case WM_DROPFILES:{
        UINT cnt;
        char path[MAX_PATH];
        wchar_t wpath[MAX_PATH];

        cnt = DragQueryFileA((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
        for (UINT i = 0; i < cnt; i++){
            DragQueryFileA((HDROP)wParam, i, path, MAX_PATH);
            window->OnDropFileA(path);
            DragQueryFileW((HDROP)wParam, i, wpath, MAX_PATH);
            window->OnDropFileW(wpath);
        }
        DragFinish((HDROP)wParam);
    }
        break;
    }
}

void AppFrame::InitWindow(){
    data = new LocalData();
    viewMgr = new ViewManager(this);

    ThreadLocal::Set(THREAD_LOCAL_APPFRAME, this);
    ThreadLocal::Set(THREAD_LOCAL_LOCALDATA, data);
    ThreadLocal::Set(THREAD_LOCAL_VIEWMGR, viewMgr);

    hWnd = CreateWindowExA(
        0,
        "ModelView.MainWindow",
        name.GetString(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL,
        LoadMenuA(hInst, MAKEINTRESOURCE(IDC_MENU)),
        hInst,
        NULL
    );

    hAccel = LoadAcceleratorsA(hInst, MAKEINTRESOURCE(IDC_MENU));
}