#include "main.h"

#include "res.h"

#include "log.h"
#include "font.h"
#include "gltools.h"

Main::MoveButton::MoveButton(Vector2 center, float radius) : center(center), radius(radius) {}

Main::MoveButton::~MoveButton(){}

bool Main::MoveButton::Trigger(Vector2 pos){
    return (pos - center).SqrMagnitude() <= radius * radius;
}

void Main::MoveButton::Render(){
    glColor3f(0.3f, 0.3f, 0.3f);
    GLUtils::DrawCorner(center.x, center.y, 0.0f, 360.0f, radius, 0.05f);
}

void Main::MoveButton::Click(){
    DebugLog("MoveButton Click");
    start = inst->camLookat;
}

void Main::MoveButton::Drag(Vector2 dir){
    DebugLog("MoveButton Drag %f %f", dir.x, dir.y);
    inst->camLookat = start - (inst->camRight * dir.x + inst->camUp * dir.y) * inst->camDis;
    inst->UpdateLookAtLocation();
}

Main::RotateButton::RotateButton(Vector2 center, float radius) : center(center), radius(radius) {}

Main::RotateButton::~RotateButton(){}

bool Main::RotateButton::Trigger(Vector2 pos){
    return (pos - center).SqrMagnitude() <= radius * radius;
}

void Main::RotateButton::Render(){
    glColor3f(0.3f, 0.3f, 0.3f);
    GLUtils::DrawCorner(center.x, center.y, 0.0f, 360.0f, radius, 0.05f);
}

void Main::RotateButton::Click(){
    DebugLog("RotateButton Click");
    start = inst->camDir;
}

void Main::RotateButton::Drag(Vector2 dir){
    DebugLog("RotateButton Drag %f %f", dir.x, dir.y);
    inst->camDir = Quaternion::AxisAngle(inst->camUp, -dir.x * 100.0f) *
                            Quaternion::AxisAngle(inst->camRight, dir.y * 100.0f) * start;
    dragged = true;
}

void Main::RotateButton::Leave(){
    if (dragged){
        DebugLog("RotateButton Leave");
        dragged = false;
        inst->UpdateRotation();
    }
}

Main::MoveOperation::MoveOperation(){}

Main::MoveOperation::~MoveOperation(){}

void Main::MoveOperation::OnEnter(){
    x = y = z = true;
    start = inst->cursorPos;
    if (inst->selectedPoints.Size() > 0){
        if (inst->selectedPoints.Size() > 1){
            DebugLog("MoveOperation OnEnter [Warning]: Selected Multiple Points");
        }
        target = inst->selectedPoints.GetFront();
        startPos = target->pos;
    }else{
        target = NULL;
    }
}

void Main::MoveOperation::OnMove(){
    Vector2 mov;
    Vector3 delta;
    if (target){
        mov = (inst->cursorPos - start) * inst->camDis;
        delta = inst->camRight * mov.x + inst->camUp * mov.y;
        target->pos = startPos + Vector3(delta.x * x, delta.y * y, delta.z * z);
        DebugLog("MoveOperation OnMove %f %f %f", delta.x * x, delta.y * y, delta.z * z);
    }
}

void Main::MoveOperation::OnCommand(UINT id){
    switch (id){
    case IDM_OP_X: x = true; y = z = false; break;
    case IDM_OP_Y: y = true; z = x = false; break;
    case IDM_OP_Z: z = true; x = y = false; break;
    case IDM_OP_PLANE_X: x = false; y = z = true; break;
    case IDM_OP_PLANE_Y: y = false; z = x = true; break;
    case IDM_OP_PLANE_Z: z = false; x = y = true; break;
    }
}

Main::Main(){}

Main::~Main(){
    if(testMenu) delete testMenu;
    if(uiMgr) delete uiMgr;
    if(mesh) delete mesh;
    if(curOp) delete curOp;
    if(colorBoard) delete colorBoard;
}

void Main::InitCamera(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, viewportMgr->GetAspect(), 0.1, 100.0);

    Vector3 camPos = camLookat - camDir * Vector3::forward * camDis;
    Vector3 camUp = camDir * Vector3::up;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPos.x, camPos.y, camPos.z,
            camLookat.x, camLookat.y, camLookat.z,
            camUp.x, camUp.y, camUp.z);
}

void Main::InitLight0(){
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };

    GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };  //最后一个参数为0表示该光源是directional的

    GLfloat light_ambient[] = { 0.5, 0.5, 0.5, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void Main::RenderFrame(HWND hWnd, HDC hDC){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glDisable(GL_BLEND);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    //glDisable(GL_DITHER); 这几个就一直不用
    //glDisable(GL_FOG);
    //glDisable(GL_LOGIC_OP);

    viewportMgr->Reset(hWnd);

    InitCamera();

    // xyz坐标轴
    glDisable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_LINE_STIPPLE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f); glVertex3f(100.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f); glVertex3f(0.0f, 100.0f, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -100.0f); glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
    glDisable(GL_LINE_SMOOTH);

    InitLight0(); //TODO 后续光照设置法线
    glEnable(GL_LIGHTING);     //开启光照系统
    glEnable(GL_LIGHT0);       //开启GL_LIGHT0光源

    mesh->Render();

    // 已选择点绘制
    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(6.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);
    selectedPoints.Foreach([](Vertex* p){
        glVertex3f(p->pos.x, p->pos.y, p->pos.z);
    });
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    // UI绘制
    uiMgr->Render(viewportMgr->GetAspect());

    // 菜单绘制
    //glFlush();// 仅仅执行drawcall但不阻塞
    //glFinish();// 执行drawcall且阻塞直到执行完毕
    if (menu != NULL){
        menu->Render(menuPos.x, menuPos.y);
    }

    SwapBuffers(hDC);
}

void Main::SetMenu(Menu* m){
    menu = m;
    if (menu != NULL){
        menu->SetClientSize(cliSize);
        menu->CursorMove(cursorPos);
        menuPos = cursorPos;
    }
}

void Main::UpdateWindowSize(HWND hWnd){
    GetClientRect(hWnd, &cliRect);
    cliSize.x = cliRect.right - cliRect.left;
    cliSize.y = cliRect.bottom - cliRect.top;
    cliInvSize.x = 1.0f / cliSize.x;
    cliInvSize.y = 1.0f / cliSize.y;
    aspect = (float)cliSize.x / cliSize.y;
    if (menu != NULL){
        menu->SetClientSize(cliSize);
    }
}

//TODO 添加一些UI范围检测
void Main::UpdateCursor(int x, int y){
    cursorPos.x = 2.0f * x / cliSize.x - 1.0f;
    cursorPos.y = 1.0f - 2.0f * y / cliSize.y;
    cursorDir = camForward + camRight * cursorPos.x * aspect + camUp * cursorPos.y;
    if (menu != NULL){
        menu->CursorMove(cursorPos - menuPos);
        return;
    }
    uiMgr->CursorMove(cursorPos);
    if (curOp){
        curOp->OnMove();
    }
}

void Main::UpdateLookAtLocation(){
    camPos = camLookat - camForward * camDis;
}

void Main::UpdateRotation(){
    camForward = camDir * Vector3::forward;
    camUp = camDir * Vector3::up;
    camRight = camDir * Vector3::right;
    camPos = camLookat - camForward * camDis;
    cursorDir = camForward + camRight * cursorPos.x * aspect + camUp * cursorPos.y;
}

void Main::UpdateDistance(){
    camPos = camLookat - camForward * camDis;
}

void Main::GetTextInput(){
    DialogBox(hInst, MAKEINTRESOURCE(IDD_TEXT), hWnd, [](HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
        switch (uMsg){
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;
        case WM_CLOSE:
            GetDlgItemText(hDlg, IDC_TEXT_EDIT, inst->inputText, MAX_PATH);
            EndDialog(hDlg, LOWORD(wParam));
            break;
        }
        return (INT_PTR)FALSE;
    });
}

LRESULT CALLBACK Main::LocalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg){
    case WM_CREATE:
        UpdateWindowSize(hWnd);
        uiMgr->AddButton(new RotateButton(Vector2(0.85f, 0.85f), 0.12f));
        uiMgr->AddButton(new MoveButton(Vector2(0.55f, 0.85f), 0.12f));
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_SIZE:
        UpdateWindowSize(hWnd);
        UpdateCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_MOUSEMOVE:
        UpdateCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_MOUSEWHEEL:
        camDis *= Pow(0.998f, GET_WHEEL_DELTA_WPARAM(wParam));
        break;
    case WM_MOUSELEAVE:
        break;
    case WM_MOUSEHOVER:
        break;
    case WM_SETFOCUS:
        break;
    case WM_KILLFOCUS:
        break;
    case WM_LBUTTONDOWN:{
        // 菜单消失
        if (menu){
            if (menu->InMenu(cursorPos - menuPos)){
                menu->Click();
            }
            SetMenu(NULL);
            break;
        }
        // UI交互
        if (uiMgr->LeftDown()){
            break;
        }
        // 操作
        if (curOp){
            curOp->OnConfirm();
            delete curOp;
            curOp = NULL;
            break;
        }
        // 选择点
        Vertex* v = mesh->Find(camPos, cursorDir);
        if (v == NULL){
            selectedPoints.Clear();
            DebugLog("No Point Selected");
        }else{
            selectedPoints.Add(v);
            DebugLog("Select Point %f %f %f", v->pos.x, v->pos.y, v->pos.z);
        }
    }
        break;
    case WM_LBUTTONUP:
        uiMgr->LeftUp();
        break;
    case WM_RBUTTONDOWN:
        SetMenu(testMenu);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)){
        case IDM_EXIT:
            PostQuitMessage(0);
            break;
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, [](HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
                switch (uMsg){
                case WM_INITDIALOG:
                    return (INT_PTR)TRUE;
                case WM_COMMAND:
                    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL){
                        EndDialog(hDlg, LOWORD(wParam));
                        return (INT_PTR)TRUE;
                    }
                    break;
                }
                return (INT_PTR)FALSE;
            });
            break;
        case IDM_ROTATE_CCW:
            camDir *= Quaternion::AxisAngle(Vector3::forward, -10.0f);
            UpdateRotation();
            break;
        case IDM_ROTATE_CW:
            camDir *= Quaternion::AxisAngle(Vector3::forward, 10.0f);
            UpdateRotation();
            break;
        case IDM_POINT:{
            Vector3 pos = camLookat + (camUp * cursorPos.y + camRight * cursorPos.x * aspect) * camDis;
            mesh->AddVertex(pos);
            DebugLog("Point at %f %f %f", pos.x, pos.y, pos.z);
        }
            break;
        case IDM_TOPOLOGY:
            switch (selectedPoints.Size()){
            case 0:
                break;
            case 1:
                break;
            case 2:
                mesh->AddEdge(selectedPoints.GetItem(0), selectedPoints.GetItem(1));
                break;
            case 3:
                mesh->AddTriFace(selectedPoints.GetItem(0), selectedPoints.GetItem(1), selectedPoints.GetItem(2));
                break;
            default:
                break;
            }
            break;
        case IDM_MOVE:
            curOp = new MoveOperation();
            curOp->OnEnter();
            break;
        case IDM_ROTATE:
            break;
        case IDM_SIZE:
            break;
        case IDM_SELECT_COLOR:{
            if (colorBoard == NULL){
                colorBoard = new ColorBoard();
            }
            Vector3 color = colorBoard->RunAndGetColor();
            selectedPoints.Foreach<Vector3*>([](Vertex* v, Vector3* c){
                v->color = *c;
            }, &color);
        }
            break;
        case IDM_DELETE:
            selectedPoints.Foreach<Mesh*>([](Vertex* v, Mesh* m){
                m->DeleteVertex(v);
            }, mesh);
            selectedPoints.Clear();
            break;
        }
        // 当前操作的命令
        if (curOp){
            curOp->OnCommand(LOWORD(wParam));
        }
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Main::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    return inst->LocalWndProc(hWnd, uMsg, wParam, lParam);
}

ATOM Main::RegClass(HINSTANCE hInstance){
    WNDCLASSEXA wc;

    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wc.hIcon = LoadIconA(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hIconSm = LoadIconA(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "ModelView";
    wc.lpszMenuName = "";
    wc.style = 0;

    return RegisterClassExA(&wc);
}

int Main::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;
    MSG Msg;

    hInst = hInstance;

    RegClass(hInstance);
    ColorBoard::RegClass(hInstance);

    hWnd = CreateWindowExA(
        0,
        "ModelView",
        "ModelView",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        750, 750,
        NULL,
        LoadMenu(hInstance, MAKEINTRESOURCE(IDC_MENU)),
        hInstance,
        NULL
    );

    this->hWnd = hWnd;

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    DebugLog("Main Window Created");

    GLUtils::EnableOpenGL(hWnd, &hDC, &hRC);

    DebugLog("OpenGL Enabled");
    DebugLog("OpenGL Version %s", glGetString(GL_VERSION));
    DebugLog("OpenGL Vendor %s", glGetString(GL_VENDOR));

    glInitASCIIFont();
    glSelectFont(12, GB2312_CHARSET, "楷体_GB2312");

    DebugLog("OpenGL Use Font %s", "GB2312");

    testMenu->AddItem(new MenuItem(L"你好"));
    testMenu->AddItem(new MenuItem(L"这是"));
    testMenu->AddItem(new MenuItem(L"一个"));
    testMenu->AddItem(new MenuItem(L"测试用"));
    testMenu->AddItem(new MenuItem(L"右键菜单"));

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MENU));

    while (GetMessageA(&Msg, NULL, 0, 0)){
        if (!TranslateAccelerator(Msg.hwnd, hAccelTable, &Msg)){
            if (Msg.message == WM_QUIT){
                break;
            } else {
                TranslateMessage(&Msg);
                DispatchMessageA(&Msg);
                wglMakeCurrent(hDC, hRC);
                RenderFrame(hWnd, hDC);
            }
        }
    }

    GLUtils::DisableOpenGL(hWnd, hDC, hRC);

    DestroyWindow(hWnd);
    
    return Msg.wParam;
}

Main* Main::inst = new Main();

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    int ret = Main::inst->WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    delete Main::inst;
    return ret;
}