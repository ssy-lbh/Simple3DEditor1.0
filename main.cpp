#include "main.h"

#include "res.h"

#include "log.h"
#include "font.h"
#include "gltools.h"
#include "nodemap.h"
#include "audio.h"
#include "paint.h"

#include "shell.h"

MainWindow::MoveButton::MoveButton(Vector2 center, float radius, MainWindow* main) : center(center), radius(radius), main(main) {}
MainWindow::MoveButton::~MoveButton(){}

bool MainWindow::MoveButton::Trigger(Vector2 pos){
    return (pos - center).SqrMagnitude() <= radius * radius;
}

void MainWindow::MoveButton::Render(){
    glColor3f(0.3f, 0.3f, 0.3f);
    GLUtils::DrawCorner(center.x, center.y, 0.0f, 360.0f, radius, 0.05f);
}

void MainWindow::MoveButton::Click(){
    DebugLog("MoveButton Click");
    start = main->camLookat;
}

void MainWindow::MoveButton::Drag(Vector2 dir){
    //DebugLog("MoveButton Drag %f %f", dir.x, dir.y);
    main->camLookat = start - (main->camRight * dir.x + main->camUp * dir.y) * main->camDis;
    main->UpdateLookAtLocation();
}

void MainWindow::MoveButton::ClickEnd(){
    DebugLog("MoveButton ClickEnd");
    main->UpdateLookAtLocation();
}

MainWindow::RotateButton::RotateButton(Vector2 center, float radius, MainWindow* main) : center(center), radius(radius), main(main) {}
MainWindow::RotateButton::~RotateButton(){}

bool MainWindow::RotateButton::Trigger(Vector2 pos){
    return (pos - center).SqrMagnitude() <= radius * radius;
}

void MainWindow::RotateButton::Render(){
    glColor3f(0.3f, 0.3f, 0.3f);
    GLUtils::DrawCorner(center.x, center.y, 0.0f, 360.0f, radius, 0.05f);
}

void MainWindow::RotateButton::Click(){
    DebugLog("RotateButton Click");
    start = main->camDir;
}

void MainWindow::RotateButton::Drag(Vector2 dir){
    //DebugLog("RotateButton Drag %f %f", dir.x, dir.y);
    main->camDir = Quaternion::AxisAngle(main->camUp, -dir.x * 100.0f) *
                            Quaternion::AxisAngle(main->camRight, dir.y * 100.0f) * start;
}

void MainWindow::RotateButton::ClickEnd(){
    DebugLog("RotateButton ClickEnd");
    main->UpdateRotation();
}

MainWindow::MoveOperation::MoveOperation(MainWindow* main) : main(main) {}
MainWindow::MoveOperation::~MoveOperation(){}

void MainWindow::MoveOperation::OnEnter(){
    DebugLog("MoveOperation OnEnter");
    x = y = z = true;
    start = main->cursorPos;
    if (Main::data->selectedPoints.Size() > 0){
        Main::data->selectedPoints.Foreach<MoveOperation*>([](Vertex* v, MoveOperation* op){
            op->moveInfo.Add({v, v->pos});
        }, this);
    }
}

void MainWindow::MoveOperation::OnMove(){
    Vector2 mov;
    Vector3 delta;
    if (moveInfo.Size() > 0){
        mov = (main->cursorPos - start) * main->camDis;
        delta = main->camRight * mov.x * main->aspect + main->camUp * mov.y;
        delta = Vector3(x ? delta.x : 0.0f, y ? delta.y : 0.0f, z ? delta.z : 0.0f);
        moveInfo.Foreach<Vector3*>([](MoveInfo info, Vector3* offset){
            info.vert->pos = info.pos + *offset;
        }, &delta);
        DebugLog("MoveOperation OnMove %f %f %f", x ? delta.x : 0.0f, y ? delta.y : 0.0f, z ? delta.z : 0.0f);
    }
}

void MainWindow::MoveOperation::OnConfirm(){
    DebugLog("MoveOperation OnConfirm");
}

void MainWindow::MoveOperation::OnUndo(){
    DebugLog("MoveOperation OnUndo");
    moveInfo.Foreach([](MoveInfo info){
        info.vert->pos = info.pos;
    });
}

void MainWindow::MoveOperation::OnCommand(int id){
    switch (id){
    case IDM_OP_X: x = true; y = z = false; break;
    case IDM_OP_Y: y = true; z = x = false; break;
    case IDM_OP_Z: z = true; x = y = false; break;
    case IDM_OP_PLANE_X: x = false; y = z = true; break;
    case IDM_OP_PLANE_Y: y = false; z = x = true; break;
    case IDM_OP_PLANE_Z: z = false; x = y = true; break;
    }
}

MainWindow::ExcludeOperation::ExcludeOperation(MainWindow* main) : main(main) {}
MainWindow::ExcludeOperation::~ExcludeOperation(){}

void MainWindow::ExcludeOperation::OnEnter(){
    DebugLog("ExcludeOperation OnEnter");
    x = y = z = true;
    start = main->cursorPos;
    List<Vertex*> copies;
    size_t cnt = Main::data->selectedPoints.Size();
    for (size_t i = 0; i < cnt; i++){
        copies.Add(Main::data->selectedPoints[i]);
        Main::data->selectedPoints[i] = Main::data->mesh->AddVertex(Main::data->selectedPoints[i]->pos);
        Main::data->mesh->AddEdge(copies[i], Main::data->selectedPoints[i]);
    }
    for (size_t i = 0; i < cnt; i++){
        for (size_t j = i + 1; j < cnt; j++){
            if (copies[i]->EdgeRelateTo(copies[j])){
                Main::data->mesh->AddTriFace(copies[i], copies[j], Main::data->selectedPoints[j]);
                Main::data->mesh->AddTriFace(copies[i], Main::data->selectedPoints[i], Main::data->selectedPoints[j]);
            }
        }
    }
    if (Main::data->selectedPoints.Size() > 0){
        Main::data->selectedPoints.Foreach<ExcludeOperation*>([](Vertex* v, ExcludeOperation* op){
            op->moveInfo.Add({v, v->pos});
        }, this);
    }
}

void MainWindow::ExcludeOperation::OnMove(){
    Vector2 mov;
    Vector3 delta;
    if (moveInfo.Size() > 0){
        mov = (main->cursorPos - start) * main->camDis;
        delta = main->camRight * mov.x * main->aspect + main->camUp * mov.y;
        delta = Vector3(x ? delta.x : 0.0f, y ? delta.y : 0.0f, z ? delta.z : 0.0f);
        moveInfo.Foreach<Vector3*>([](MoveInfo info, Vector3* offset){
            info.vert->pos = info.pos + *offset;
        }, &delta);
        DebugLog("ExcludeOperation OnMove %f %f %f", x ? delta.x : 0.0f, y ? delta.y : 0.0f, z ? delta.z : 0.0f);
    }
}

void MainWindow::ExcludeOperation::OnConfirm(){
    DebugLog("ExcludeOperation OnConfirm");
}

void MainWindow::ExcludeOperation::OnUndo(){
    DebugLog("ExcludeOperation OnUndo");
    Main::data->selectedPoints.Clear();
    moveInfo.Foreach<Mesh*>([](MoveInfo info, Mesh* mesh){
        mesh->DeleteVertex(info.vert);
    }, Main::data->mesh);
}

void MainWindow::ExcludeOperation::OnCommand(int id){
    switch (id){
    case IDM_OP_X: x = true; y = z = false; break;
    case IDM_OP_Y: y = true; z = x = false; break;
    case IDM_OP_Z: z = true; x = y = false; break;
    case IDM_OP_PLANE_X: x = false; y = z = true; break;
    case IDM_OP_PLANE_Y: y = false; z = x = true; break;
    case IDM_OP_PLANE_Z: z = false; x = y = true; break;
    }
}

MainWindow::RotateOperation::RotateOperation(MainWindow* main) : main(main) {}
MainWindow::RotateOperation::~RotateOperation(){}

void MainWindow::RotateOperation::OnEnter(){
    DebugLog("RotateOperation OnEnter");
    x = y = z = true;
    start = main->cursorPos;
    center = Vector3::zero;
    if (Main::data->selectedPoints.Size() > 0){
        Main::data->selectedPoints.Foreach<RotateOperation*>([](Vertex* v, RotateOperation* op){
            op->rotateInfo.Add({v, v->pos});
            op->center += v->pos;
        }, this);
        center /= Main::data->selectedPoints.Size();
        screenCenter = main->GetScreenPosition(center);
        dis = (main->cursorPos - screenCenter).Magnitude();
    }
}

void MainWindow::RotateOperation::OnMove(){
    if (rotateInfo.Size() > 0){
        Vector3 rotateVec = main->camForward * ((main->cursorPos - screenCenter).Magnitude() - dis);
        rotateVec = Vector3(x ? rotateVec.x : 0.0f, y ? rotateVec.y : 0.0f, z ? rotateVec.z : 0.0f);
        if (rotateVec.SqrMagnitude() > 0.0f){
            rotate = Quaternion::AxisAngle(rotateVec.Normal(), rotateVec.Magnitude() * 360.0f);
        }else{
            rotate = Quaternion::one;
        }
        rotateInfo.Foreach<RotateOperation*>([](RotateInfo info, RotateOperation* op){
            info.vert->pos = op->center + op->rotate * (info.pos - op->center);
        }, this);
        DebugLog("RotateOperation Rotate %f %f %f", x ? rotateVec.x : 0.0f, y ? rotateVec.y : 0.0f, z ? rotateVec.z : 0.0f);
    }
}

void MainWindow::RotateOperation::OnConfirm(){
    DebugLog("RotateOperation OnConfirm");
}

void MainWindow::RotateOperation::OnUndo(){
    DebugLog("RotateOperation OnUndo");
    rotateInfo.Foreach([](RotateInfo info){
        info.vert->pos = info.pos;
    });
}

void MainWindow::RotateOperation::OnCommand(int id){
    switch (id){
    case IDM_OP_X: x = true; y = z = false; break;
    case IDM_OP_Y: y = true; z = x = false; break;
    case IDM_OP_Z: z = true; x = y = false; break;
    case IDM_OP_PLANE_X: x = false; y = z = true; break;
    case IDM_OP_PLANE_Y: y = false; z = x = true; break;
    case IDM_OP_PLANE_Z: z = false; x = y = true; break;
    }
}

MainWindow::SizeOperation::SizeOperation(MainWindow* main) : main(main) {}
MainWindow::SizeOperation::~SizeOperation(){}

void MainWindow::SizeOperation::OnEnter(){
    DebugLog("SizeOperation OnEnter");
    x = y = z = true;
    start = main->cursorPos;
    center = Vector3::zero;
    if (Main::data->selectedPoints.Size() > 0){
        Main::data->selectedPoints.Foreach<SizeOperation*>([](Vertex* v, SizeOperation* op){
            op->sizeInfo.Add({v, v->pos});
            op->center += v->pos;
        }, this);
        center /= Main::data->selectedPoints.Size();
        screenCenter = main->GetScreenPosition(center);
        startSize = (main->cursorPos - screenCenter).Magnitude();
    }
}

void MainWindow::SizeOperation::OnMove(){
    if (sizeInfo.Size() > 0){
        scale = (main->cursorPos - screenCenter).Magnitude() / startSize;
        sizeInfo.Foreach<SizeOperation*>([](SizeInfo info, SizeOperation* op){
            Vector3 res = op->center + (info.pos - op->center) * op->scale;
            info.vert->pos = Vector3(
                op->x ? res.x : info.pos.x,
                op->y ? res.y : info.pos.y,
                op->z ? res.z : info.pos.z
            );
        }, this);
        DebugLog("SizeOperation Scale %f", scale);
    }
}

void MainWindow::SizeOperation::OnConfirm(){
    DebugLog("SizeOperation OnConfirm");
}

void MainWindow::SizeOperation::OnUndo(){
    DebugLog("SizeOperation OnUndo");
    sizeInfo.Foreach([](SizeInfo info){
        info.vert->pos = info.pos;
    });
}

void MainWindow::SizeOperation::OnCommand(int id){
    switch (id){
    case IDM_OP_X: x = true; y = z = false; break;
    case IDM_OP_Y: y = true; z = x = false; break;
    case IDM_OP_Z: z = true; x = y = false; break;
    case IDM_OP_PLANE_X: x = false; y = z = true; break;
    case IDM_OP_PLANE_Y: y = false; z = x = true; break;
    case IDM_OP_PLANE_Z: z = false; x = y = true; break;
    }
}

MainWindow::EmptyTool::EmptyTool(MainWindow* window) : window(window) {}
MainWindow::EmptyTool::~EmptyTool(){}

void MainWindow::EmptyTool::OnLeftDown(){
    Vertex* v = Main::data->mesh->Find(window->camPos, window->cursorDir);
    if (v == NULL){
        Main::data->selectedPoints.Clear();
        DebugLog("No Point Selected");
    }else{
        Main::data->selectedPoints.Add(v);
        DebugLog("Select Point %f %f %f", v->pos.x, v->pos.y, v->pos.z);
    }
}

MainWindow::SelectTool::SelectTool(MainWindow* window) : window(window) {}
MainWindow::SelectTool::~SelectTool(){}

void MainWindow::SelectTool::OnLeftDown(){
    start = window->cursorPos;
    end = window->cursorPos;
    leftDown = true;
}

void MainWindow::SelectTool::OnLeftUp(){
    leftDown = false;
    if (start.x == end.x && start.y == end.y){
        Main::data->selectedPoints.Clear();
        return;
    }
    //TODO 等待实现范围框选
    Main::data->mesh->FindScreenRect(
        window->camPos,
        window->camDir,
        window->camDis * 0.02,
        window->camDis * 20.0,
        start.x * window->aspect,
        end.x * window->aspect,
        start.y,
        end.y,
        Main::data->selectedPoints
    );
}

void MainWindow::SelectTool::OnMove(){
    end = window->cursorPos;
}

void MainWindow::SelectTool::OnRender(){
    if (leftDown){
        glColor4f(1.0f, 1.0f, 0.0f, 0.1f);
        GLUtils::DrawRect(start, end);
    }
}

MainWindow::MainWindow(){
    uiMgr = new UIManager();
    SetTool(new EmptyTool(this));

    basicMenu = new Menu();
    basicMenu->AddItem(new MenuItem(L"添加顶点", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        window->AddPoint();
    }, this));
    basicMenu->AddItem(new MenuItem(L"删除顶点", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        window->DeletePoint();
    }, this));
    basicMenu->AddItem(new MenuItem());
    basicMenu->AddItem(new MenuItem(L"保存", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        window->OnInsSave();
    }, this));
    basicMenu->AddItem(new MenuItem(L"加载", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        window->OnInsLoad();
    }, this));
    basicMenu->AddItem(new MenuItem());
    basicMenu->AddItem(new MenuItem(L"关于", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        window->AboutBox();
    }, this));
    basicMenu->AddItem(new MenuItem());

    Menu* subMenu = new Menu();
    subMenu->AddItem(new MenuItem(L"方块", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        window->OnMenuAccel(IDM_MESH_BASIC_BLOCK, false);
    }, this));
    subMenu->AddItem(new MenuItem(L"平面", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        window->OnMenuAccel(IDM_MESH_BASIC_PLANE, false);
    }, this));
    subMenu->AddItem(new MenuItem(L"圆柱体", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        window->OnMenuAccel(IDM_MESH_BASIC_CYLINDER, false);
    }, this));
    subMenu->AddItem(new MenuItem(L"球体", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        window->OnMenuAccel(IDM_MESH_BASIC_SPHERE, false);
    }, this));
    subMenu->AddItem(new MenuItem(L"胶囊体", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        window->OnMenuAccel(IDM_MESH_BASIC_CAPSULE, false);
    }, this));
    basicMenu->AddItem(new MenuItem(L"添加", subMenu));

    Menu* toolMenu = new Menu();
    toolMenu->AddItem(new MenuItem(L"点选", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        window->OnMenuAccel(IDM_TOOL_EMPTY, false);
    }, this));
    toolMenu->AddItem(new MenuItem(L"框选", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        window->OnMenuAccel(IDM_TOOL_SELECTBOX, false);
    }, this));
    basicMenu->AddItem(new MenuItem(L"工具", toolMenu));

    Menu* texMenu = new Menu();
    texMenu->AddItem(new MenuItem(L"启用", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        window->OnMenuAccel(IDM_TEXTURE_ENABLE, false);
    }, this));
    texMenu->AddItem(new MenuItem(L"停用", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        window->OnMenuAccel(IDM_TEXTURE_DISABLE, false);
    }, this));
    texMenu->AddItem(new MenuItem(L"加载", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        window->OnMenuAccel(IDM_TEXTURE_LOAD, false);
    }, this));
    basicMenu->AddItem(new MenuItem(L"纹理", texMenu));

    uiMgr->AddButton(new RotateButton(Vector2(0.85f, 0.85f), 0.12f, this));
    uiMgr->AddButton(new MoveButton(Vector2(0.55f, 0.85f), 0.12f, this));

    // 该组件测试未通过，实现未完成
    //uiMgr->AddButton(new UIEditA(Vector2(0.0f, 0.0f), Vector2(0.5f, 0.1f)));
}

MainWindow::~MainWindow(){
    if(basicMenu) delete basicMenu;
    if(uiMgr) delete uiMgr;
    if(curOp) delete curOp;
    if(colorBoard) delete colorBoard;
}

void MainWindow::InitCamera(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, ViewportManager::inst->GetAspect(), camDis * 0.02, camDis * 20.0);

    Vector3 camPos = camLookat - camDir * Vector3::forward * camDis;
    Vector3 camUp = camDir * Vector3::up;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPos.x, camPos.y, camPos.z,
            camLookat.x, camLookat.y, camLookat.z,
            camUp.x, camUp.y, camUp.z);
}

void MainWindow::InitLight0(){
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

void MainWindow::RenderModelView(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_ALPHA_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    //glDisable(GL_DITHER);
    //glDisable(GL_FOG);
    //glDisable(GL_LOGIC_OP);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    InitCamera();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // xyz坐标线
    glDisable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_LINE_STIPPLE);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-camRange, 0.0f, 0.0f); glVertex3f(camRange, 0.0f, 0.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -camRange, 0.0f); glVertex3f(0.0f, camRange, 0.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -camRange); glVertex3f(0.0f, 0.0f, camRange);
    glEnd();
    glDisable(GL_LINE_SMOOTH);

    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
    for (float i = 0.02f; i < 1.0f; i += 0.02f){
        glVertex3f(-camRange, i * camRange, 0.0f); glVertex3f(camRange, i * camRange, 0.0f);
        glVertex3f(-camRange, -i * camRange, 0.0f); glVertex3f(camRange, -i * camRange, 0.0f);
        glVertex3f(i * camRange, -camRange, 0.0f); glVertex3f(i * camRange, camRange, 0.0f);
        glVertex3f(-i * camRange, -camRange, 0.0f); glVertex3f(-i * camRange, camRange, 0.0f);
    }
    glEnd();

    InitLight0(); //TODO 后续光照设置法线
    glEnable(GL_LIGHTING);     //开启光照系统
    glEnable(GL_LIGHT0);       //开启GL_LIGHT0光源

    Main::data->mesh->Render();

    // 已选择点绘制
    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(8.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);
    Main::data->selectedPoints.Foreach([](Vertex* p){
        glVertex3f(p->pos.x, p->pos.y, p->pos.z);
    });
    glEnd();
    glDisable(GL_POINT_SMOOTH);
}

void MainWindow::OnRender(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //TODO 做好Container组件，实现UI尺寸坐标管理
    RenderModelView();

    // 工具绘制
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (curTool)
        curTool->OnRender();

    // UI绘制
    // 在之前进行3D渲染使用投影变换后，需要参数aspect
    uiMgr->Render();
}

void MainWindow::SetOperation(IOperation* op){
    if (curOp){
        DebugLog("MainWindow::SetOperation delete curOp");
        curOp->OnConfirm();
        delete curOp;
    }
    curOp = op;
    curOp->OnEnter();
}

void MainWindow::SetTool(ITool* tool){
    if (curTool){
        curTool->OnUnselect();
        delete curTool;
    }
    curTool = tool;
    tool->OnSelect();
}

void MainWindow::UpdateWindowSize(int x, int y){
    cliSize.x = x;
    cliSize.y = y;
    cliInvSize.x = 1.0f / cliSize.x;
    cliInvSize.y = 1.0f / cliSize.y;
    aspect = (float)cliSize.x / cliSize.y;
}

//TODO 添加一些UI范围检测
void MainWindow::UpdateCursor(int x, int y){
    cursorPos.x = 2.0f * x / cliSize.x - 1.0f;
    cursorPos.y = 2.0f * y / cliSize.y - 1.0f;
    cursorDir = camForward + camRight * cursorPos.x * aspect + camUp * cursorPos.y;
    uiMgr->CursorMove(cursorPos);
    if (curOp){
        curOp->OnMove();
    }
    if (curTool){
        curTool->OnMove();
    }
}

void MainWindow::UpdateLookAtLocation(){
    camPos = camLookat - camForward * camDis;
}

void MainWindow::UpdateRotation(){
    camForward = camDir * Vector3::forward;
    camUp = camDir * Vector3::up;
    camRight = camDir * Vector3::right;
    camPos = camLookat - camForward * camDis;
    cursorDir = camForward + camRight * cursorPos.x * aspect + camUp * cursorPos.y;
}

void MainWindow::UpdateDistance(){
    DebugLog("Camera Distance %f", camDis);
    camPos = camLookat - camForward * camDis;
    if (camRange < camDis * 8.0f){
        camRange *= 5.0f;
    }else if (camRange > camDis * 100.0f){
        camRange *= 0.2f;
    }
}

/** synchronized **/
void MainWindow::GetTextInput(){
    static MainWindow* window;
    window = this;
    DialogBox(Main::hInst, MAKEINTRESOURCE(IDD_TEXT), Main::hWnd,
        (DLGPROC)[]
#if (!_WIN64) && _WIN32
        __attribute__((__stdcall__))
#endif
        (HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
            switch (uMsg){
            case WM_INITDIALOG:
                return (INT_PTR)TRUE;
            case WM_CLOSE:
                window->inputConfirm = false;
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            case WM_COMMAND:
                switch (LOWORD(wParam)){
                case IDOK:
                    window->inputConfirm = true;
                    GetDlgItemTextW(hDlg, IDC_TEXT_EDIT, window->inputText, MAX_PATH);
                    EndDialog(hDlg, LOWORD(wParam));
                    break;
                case IDCANCEL:
                    window->inputConfirm = false;
                    EndDialog(hDlg, LOWORD(wParam));
                    break;
                }
                return (INT_PTR)TRUE;
            }
            return (INT_PTR)FALSE;
    });
    if (inputConfirm == false){
        *inputText = L'\0';
    }
}

void MainWindow::AddPoint(){
    Vector3 pos = camLookat + (camUp * cursorPos.y + camRight * cursorPos.x * aspect) * camDis;
    Main::data->mesh->AddVertex(pos);
    DebugLog("Point at %f %f %f", pos.x, pos.y, pos.z);
}

void MainWindow::DeletePoint(){
    Main::data->selectedPoints.Foreach<Mesh*>([](Vertex* v, Mesh* m){
        m->DeleteVertex(v);
    }, Main::data->mesh);
    Main::data->selectedPoints.Clear();
}

bool MainWindow::SaveMesh(Mesh* mesh){
    if (!ShellFileSelectWindowW(Main::hWnd, inputText, MAX_PATH, L"3D对象(*.obj)\0*.obj\0", OFN_PATHMUSTEXIST | OFN_EXPLORER)){
        DebugError("Stop Saving");
        return false;
    }
    inputText[MAX_PATH] = '\0';
    if (wcsncmp(inputText + wcslen(inputText) - 4, L".obj", 4)){
        wcsncat_s(inputText, MAX_PATH, L".obj", 4);
    }
    HANDLE hFile = CreateFileW(
        inputText,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE){
        DebugError("OpenFile Error!");
        return false;
    }
    DebugLog("Saving Object");
    mesh->WriteToOBJ(hFile, true, true);
    CloseHandle(hFile);
    return true;
}

bool MainWindow::LoadMesh(Mesh* mesh){
    if (!ShellFileSelectWindowW(Main::hWnd, inputText, MAX_PATH, L"3D对象(*.obj)\0*.obj\0", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER)){
        DebugLog("Stop Loading");
        return false;
    }
    inputText[MAX_PATH] = '\0';
    if (wcsncmp(inputText + wcslen(inputText) - 4, L".obj", 4)){
        wcsncat_s(inputText, MAX_PATH, L".obj", 4);
    }
    return LoadMeshW(mesh, inputText);
}

bool MainWindow::LoadMesh(Mesh* mesh, HANDLE hFile){
    BY_HANDLE_FILE_INFORMATION fileInfo;
    char* fileData;
    size_t fileLen, filePtr = 0;
    List<Vertex*> vert;
    List<Vector2> vertUV;
    List<Vector3> vertNormal;

    DebugLog("Loading Object");
    GetFileInformationByHandle(hFile, &fileInfo);
    //fileLen = ((size_t)fileInfo.nFileSizeHigh << 32) | fileInfo.nFileSizeLow;
    fileLen = fileInfo.nFileSizeLow;
    fileData = new char[fileLen + 1];
    fileData[fileLen] = '\0';

    ReadFile(hFile, fileData, fileLen, NULL, NULL);

    for (size_t i = 0; i < fileLen; i++){
        Vector3 vec;
        int v1, v2, v3;
        int vt1, vt2, vt3;
        int vn1, vn2, vn3;
        if (fileData[i] == '\n' || i == fileLen){
            fileData[i] = '\0';
            if (fileData[filePtr] == '#'){
                DebugLog("Object Annotation %s", fileData + filePtr + 1);
            }else if (__builtin_sscanf(fileData + filePtr, "v %f %f %f", &vec.x, &vec.y, &vec.z) == 3){
                vert.Add(mesh->AddVertex(vec + camLookat));
            }else if (__builtin_sscanf(fileData + filePtr, "f %d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3) == 6){
                mesh->AddTriFace(vert[v1 - 1], vert[v2 - 1], vert[v3 - 1]);
                vert[v1 - 1]->normal = vertNormal[vn1 - 1];
                vert[v2 - 1]->normal = vertNormal[vn2 - 1];
                vert[v3 - 1]->normal = vertNormal[vn3 - 1];
            }else if (__builtin_sscanf(fileData + filePtr, "f %d/%d %d/%d %d/%d", &v1, &vt1, &v2, &vt2, &v3, &vt3) == 6){
                mesh->AddTriFace(vert[v1 - 1], vert[v2 - 1], vert[v3 - 1]);
                vert[v1 - 1]->uv = vertUV[vn1 - 1];
                vert[v2 - 1]->uv = vertUV[vn2 - 1];
                vert[v3 - 1]->uv = vertUV[vn3 - 1];
            }else if (__builtin_sscanf(fileData + filePtr, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3) == 9){
                mesh->AddTriFace(vert[v1 - 1], vert[v2 - 1], vert[v3 - 1]);
                vert[v1 - 1]->uv = vertUV[vn1 - 1];
                vert[v2 - 1]->uv = vertUV[vn2 - 1];
                vert[v3 - 1]->uv = vertUV[vn3 - 1];
                vert[v1 - 1]->normal = vertNormal[vn1 - 1];
                vert[v2 - 1]->normal = vertNormal[vn2 - 1];
                vert[v3 - 1]->normal = vertNormal[vn3 - 1];
            }else if (__builtin_sscanf(fileData + filePtr, "f %d %d %d", &v1, &v2, &v3) == 3){
                mesh->AddTriFace(vert[v1 - 1], vert[v2 - 1], vert[v3 - 1]);
            }else if (__builtin_sscanf(fileData + filePtr, "vt %f %f", &vec.x, &vec.y) == 2){
                vertUV.Add(Vector2(vec.x, vec.y));
            }else if (__builtin_sscanf(fileData + filePtr, "vn %f %f %f", &vec.x, &vec.y, &vec.z) == 3){
                vertNormal.Add(Vector3(vec));
            }else{
                DebugError("Object File Unknown Line %s", fileData + filePtr);
            }
            filePtr = i + 1;
        }
    }
    delete[] fileData;

    return true;
}

bool MainWindow::LoadMeshA(Mesh* mesh, const char* path){
    BY_HANDLE_FILE_INFORMATION fileInfo;
    char* fileData;
    size_t fileLen, filePtr = 0;
    List<Vertex*> vert;

    HANDLE hFile = CreateFileA(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE){
        DebugError("OpenFile Error!");
        return false;
    }
    bool res = LoadMesh(mesh, hFile);

    CloseHandle(hFile);

    return res;
}

bool MainWindow::LoadMeshW(Mesh* mesh, const wchar_t* path){
    BY_HANDLE_FILE_INFORMATION fileInfo;
    char* fileData;
    size_t fileLen, filePtr = 0;
    List<Vertex*> vert;

    HANDLE hFile = CreateFileW(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE){
        DebugError("OpenFile Error!");
        return false;
    }
    bool res = LoadMesh(mesh, hFile);

    CloseHandle(hFile);

    return res;
}

void MainWindow::AboutBox(){
    DialogBox(Main::hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), Main::hWnd,
        (DLGPROC)[]
#if (!_WIN64) && _WIN32
        __attribute__((__stdcall__))
#endif
        (HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
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
}

void MainWindow::OnClose(){}

void MainWindow::OnTimer(int id){}

void MainWindow::OnResize(int x, int y){
    UpdateWindowSize(x, y);
}

void MainWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
}

void MainWindow::OnRightDown(int x, int y){
    // 操作
    if (curOp){
        curOp->OnUndo();
        delete curOp;
        curOp = NULL;
        return;
    }
    // 工具
    if (curTool){
        curTool->OnRightDown();
    }
    Main::SetMenu(basicMenu);
}

void MainWindow::OnRightUp(int x, int y){
    if (curTool){
        curTool->OnRightUp();
    }
}

void MainWindow::OnInsSave(){
    SaveMesh(Main::data->mesh);
}

void MainWindow::OnInsLoad(){
    LoadMesh(Main::data->mesh);
}

void MainWindow::OnInsSelectColor(){
    Vector3 color = ColorBoard::GetColor();
    Main::data->selectedPoints.Foreach<Vector3*>([](Vertex* v, Vector3* c){
        v->color = *c;
    }, &color);
}

void MainWindow::OnInsTopology(){
    switch (Main::data->selectedPoints.Size()){
    case 0:
        break;
    case 1:
        break;
    case 2:
        Main::data->mesh->AddEdge(Main::data->selectedPoints[0], Main::data->selectedPoints[1]);
        break;
    case 3:
        Main::data->mesh->AddTriFace(Main::data->selectedPoints[0], Main::data->selectedPoints[1], Main::data->selectedPoints[2]);
        break;
    default:
        break;
    }
}

void MainWindow::OnLeftDown(int x, int y){
    // UI交互
    if (uiMgr->LeftDown()){
        UpdateCursor(x, y);
        return;
    }
    // 操作
    if (curOp){
        curOp->OnConfirm();
        delete curOp;
        curOp = NULL;
        return;
    }
    // 工具
    if (curTool){
        curTool->OnLeftDown();
    }
}

void MainWindow::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
    uiMgr->LeftUp();
    if (curTool){
        curTool->OnLeftUp();
    }
}

void MainWindow::OnChar(char c){
    uiMgr->Char(c);
}

void MainWindow::OnUnichar(wchar_t c){
    uiMgr->Unichar(c);
}

void MainWindow::OnMouseWheel(int delta){
    camDis *= Pow(0.999f, delta);
    UpdateDistance();
}

void MainWindow::OnMenuAccel(int id, bool accel){
    DebugLog("MainWindow::OnMenuAccel %d %s", id, accel ? "true" : "false");
    switch (id){
    case IDM_SAVE:
        OnInsSave();
        break;
    case IDM_LOAD:
        OnInsLoad();
        break;
    case IDM_EXIT:
        PostQuitMessage(0);
        break;
    case IDM_ABOUT:
        AboutBox();
        break;
    case IDM_POINT:
        AddPoint();
        break;
    case IDM_TOPOLOGY:
        OnInsTopology();
        break;
    case IDM_MOVE:
        SetOperation(new MoveOperation(this));
        break;
    case IDM_ROTATE:
        SetOperation(new RotateOperation(this));
        break;
    case IDM_SIZE:
        SetOperation(new SizeOperation(this));
        break;
    case IDM_SELECT_COLOR:
        OnInsSelectColor();
        break;
    case IDM_DELETE:
        DeletePoint();
        break;
    case IDM_EXCLUDE:
        SetOperation(new ExcludeOperation(this));
        break;
    case IDM_MESH_BASIC_PLANE:{
        Mesh* mesh = Main::data->mesh;
        Vertex* v1 = new Vertex(Vector3(-1.0f, -1.0f, 0.0f) + camLookat);
        Vertex* v2 = new Vertex(Vector3( 1.0f, -1.0f, 0.0f) + camLookat);
        Vertex* v3 = new Vertex(Vector3(-1.0f,  1.0f, 0.0f) + camLookat);
        Vertex* v4 = new Vertex(Vector3( 1.0f,  1.0f, 0.0f) + camLookat);
        mesh->AddVertex(v1);
        mesh->AddVertex(v2);
        mesh->AddVertex(v3);
        mesh->AddVertex(v4);
        mesh->AddTriFace(v1, v2, v4);
        mesh->AddTriFace(v1, v3, v4);
    }
        break;
    case IDM_MESH_BASIC_BLOCK:{
        Mesh* mesh = Main::data->mesh;
        Vertex* v1 = new Vertex(Vector3(-1.0f, -1.0f, -1.0f) + camLookat);
        Vertex* v2 = new Vertex(Vector3( 1.0f, -1.0f, -1.0f) + camLookat);
        Vertex* v3 = new Vertex(Vector3(-1.0f,  1.0f, -1.0f) + camLookat);
        Vertex* v4 = new Vertex(Vector3( 1.0f,  1.0f, -1.0f) + camLookat);
        Vertex* v5 = new Vertex(Vector3(-1.0f, -1.0f,  1.0f) + camLookat);
        Vertex* v6 = new Vertex(Vector3( 1.0f, -1.0f,  1.0f) + camLookat);
        Vertex* v7 = new Vertex(Vector3(-1.0f,  1.0f,  1.0f) + camLookat);
        Vertex* v8 = new Vertex(Vector3( 1.0f,  1.0f,  1.0f) + camLookat);
        mesh->AddVertex(v1);
        mesh->AddVertex(v2);
        mesh->AddVertex(v3);
        mesh->AddVertex(v4);
        mesh->AddVertex(v5);
        mesh->AddVertex(v6);
        mesh->AddVertex(v7);
        mesh->AddVertex(v8);
        // XY对角
        mesh->AddTriFace(v1, v2, v4);
        mesh->AddTriFace(v1, v3, v4);
        mesh->AddTriFace(v5, v6, v8);
        mesh->AddTriFace(v5, v7, v8);
        // XZ对角
        mesh->AddTriFace(v1, v2, v6);
        mesh->AddTriFace(v1, v5, v6);
        mesh->AddTriFace(v3, v4, v8);
        mesh->AddTriFace(v3, v7, v8);
        // YZ对角
        mesh->AddTriFace(v1, v3, v7);
        mesh->AddTriFace(v1, v5, v7);
        mesh->AddTriFace(v2, v4, v8);
        mesh->AddTriFace(v2, v6, v8);
    }
        break;
    case IDM_MESH_BASIC_CYLINDER:{
        Mesh* mesh = Main::data->mesh;
        const int loops = 2;
        const int round = 30;
        Vertex** vert = new Vertex*[loops * round];
        Vertex* ceil;
        Vertex* floor;
        ceil = mesh->AddVertex(Vector3(0.0f, 0.0f, 1.0f) + camLookat);
        floor = mesh->AddVertex(Vector3(0.0f, 0.0f, -1.0f) + camLookat);
        for (int i = 0; i < loops; i++){
            for (int j = 0; j < round; j++){
                float vsin, vcos;
                SinCosf(2.0f * PI * j / round, &vsin, &vcos);
                vert[i * round + j] = mesh->AddVertex(Vector3(vcos, vsin, 2.0f * i / (loops - 1) - 1.0f) + camLookat);
            }
        }
        for (int i = 0; i < loops - 1; i++){
            for (int j = 0; j < round; j++){
                int k = (j == round - 1 ? 0 : j + 1);
                mesh->AddTriFace(vert[i * round + j], vert[i * round + k], vert[(i + 1) * round + k]);
                mesh->AddTriFace(vert[i * round + j], vert[(i + 1) * round + j], vert[(i + 1) * round + k]);
            }
        }
        for (int i = 0; i < round; i++){
            int j = (i == round - 1 ? 0 : i + 1);
            mesh->AddTriFace(floor, vert[i], vert[j]);
            mesh->AddTriFace(ceil, vert[(loops - 1) * round + i], vert[(loops - 1) * round + j]);
        }
    }
        break;
    case IDM_MESH_BASIC_SPHERE:{
        Mesh* mesh = Main::data->mesh;
        const int loops = 10;
        const int round = 10;
        Vertex** vert = new Vertex*[loops * round];
        Vertex* ceil;
        Vertex* floor;
        ceil = mesh->AddVertex(Vector3(0.0f, 0.0f, 1.0f) + camLookat);
        floor = mesh->AddVertex(Vector3(0.0f, 0.0f, -1.0f) + camLookat);
        for (int i = 0; i < loops; i++){
            for (int j = 0; j < round; j++){
                float height, radius;
                float vsin, vcos;
                SinCosf(PI * (i + 1) / (loops + 1) - 0.5f * PI, &height, &radius);
                SinCosf(2.0f * PI * j / round, &vsin, &vcos);
                vert[i * round + j] = mesh->AddVertex(Vector3(vcos * radius, vsin * radius, height) + camLookat);
            }
        }
        for (int i = 0; i < loops - 1; i++){
            for (int j = 0; j < round; j++){
                int k = (j == round - 1 ? 0 : j + 1);
                mesh->AddTriFace(vert[i * round + j], vert[i * round + k], vert[(i + 1) * round + k]);
                mesh->AddTriFace(vert[i * round + j], vert[(i + 1) * round + j], vert[(i + 1) * round + k]);
            }
        }
        for (int i = 0; i < round; i++){
            int j = (i == round - 1 ? 0 : i + 1);
            mesh->AddTriFace(floor, vert[i], vert[j]);
            mesh->AddTriFace(ceil, vert[(loops - 1) * round + i], vert[(loops - 1) * round + j]);
        }
    }
        break;
    case IDM_MESH_BASIC_CAPSULE:{
        Mesh* mesh = Main::data->mesh;
        const int ballLoops = 5;
        const int cylinderLoops = 10;
        const int loops = 2 * ballLoops + cylinderLoops;
        const int round = 10;
        Vertex** vert = new Vertex*[loops * round];
        Vertex* ceil;
        Vertex* floor;
        ceil = mesh->AddVertex(Vector3(0.0f, 0.0f, 2.0f) + camLookat);
        floor = mesh->AddVertex(Vector3(0.0f, 0.0f, -2.0f) + camLookat);
        for (int i = 0; i < loops; i++){
            if (i >= ballLoops && i < loops - ballLoops){
                int loopCnt = i - ballLoops;
                for (int j = 0; j < round; j++){
                    float vsin, vcos;
                    SinCosf(2.0f * PI * j / round, &vsin, &vcos);
                    vert[i * round + j] = mesh->AddVertex(Vector3(vcos, vsin, 2.0f * loopCnt / (cylinderLoops - 1) - 1.0f) + camLookat);
                }
            }else if (i < ballLoops){
                for (int j = 0; j < round; j++){
                    float height, radius;
                    float vsin, vcos;
                    SinCosf(-0.5f * PI * (ballLoops - i - 1) / ballLoops, &height, &radius);
                    SinCosf(2.0f * PI * j / round, &vsin, &vcos);
                    vert[i * round + j] = mesh->AddVertex(Vector3(vcos * radius, vsin * radius, height - 1.0f) + camLookat);
                }
            }else{
                for (int j = 0; j < round; j++){
                    float height, radius;
                    float vsin, vcos;
                    SinCosf(0.5f * PI * (i - ballLoops - cylinderLoops) / ballLoops, &height, &radius);
                    SinCosf(2.0f * PI * j / round, &vsin, &vcos);
                    vert[i * round + j] = mesh->AddVertex(Vector3(vcos * radius, vsin * radius, height + 1.0f) + camLookat);
                }
            }
        }
        for (int i = 0; i < loops - 1; i++){
            for (int j = 0; j < round; j++){
                int k = (j == round - 1 ? 0 : j + 1);
                mesh->AddTriFace(vert[i * round + j], vert[i * round + k], vert[(i + 1) * round + k]);
                mesh->AddTriFace(vert[i * round + j], vert[(i + 1) * round + j], vert[(i + 1) * round + k]);
            }
        }
        for (int i = 0; i < round; i++){
            int j = (i == round - 1 ? 0 : i + 1);
            mesh->AddTriFace(floor, vert[i], vert[j]);
            mesh->AddTriFace(ceil, vert[(loops - 1) * round + i], vert[(loops - 1) * round + j]);
        }
    }
        break;
    case IDM_MENU_BASIC:
        Main::SetMenu(basicMenu);
        break;
    case IDM_TOOL_EMPTY:
        SetTool(new EmptyTool(this));
        break;
    case IDM_TOOL_SELECTBOX:
        SetTool(new SelectTool(this));
        break;
    case IDM_TEXTURE_ENABLE:
        Main::data->mesh->SetTexture(IDB_EARTH_WATER);
        break;
    case IDM_TEXTURE_DISABLE:
        Main::data->mesh->ResetTexture();
        break;
    case IDM_TEXTURE_LOAD:
        if (!ShellFileSelectWindowW(Main::hWnd, inputText, MAX_PATH, L"任意图片格式(*.*)\0*.*\0", OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER)){
            DebugLog("Stop Loading");
            break;
        }
        inputText[MAX_PATH] = '\0';
        Main::data->mesh->SetTexture(new GLTexture2D(inputText));
        break;
    }
    // 当前操作的命令
    if (curOp)
        curOp->OnCommand(id);
    if (curTool)
        curTool->OnCommand(id);
}

void MainWindow::OnDropFileA(const char* path){
    char* suffix = strrchr(path, '.');
    char message[60];
    char caption[20];
    LoadString(Main::hInst, IDS_OBJFILE_FORM_WARNING, message, 60);
    LoadString(Main::hInst, IDS_OBJFILE_FORM_WARNING_CAPTION, caption, 20);
    if (strcmp(suffix, ".obj")){
        if (MessageBox(Main::hWnd, message, caption, MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL){
            DebugLog("MainWindow::OnDropFileA Stop Load File");
            return;
        }
    }
    LoadMeshA(Main::data->mesh, path);
}

bool MainWindow::IsFocus(){
    return focus;
}

void MainWindow::OnFocus(){
    focus = true;
}

void MainWindow::OnKillFocus(){
    focus = false;
}

Vector3 MainWindow::GetLookPosition(Vector3 pos){
    return (-camDir) * (pos - camPos);
}

Vector2 MainWindow::GetScreenPosition(Vector3 pos){
    Vector3 lookPos = (-camDir) * (pos - camPos);
    return Vector2((lookPos.x / lookPos.y) / aspect, lookPos.z / lookPos.y);
}

MainData::MainData(){
    mesh = new Mesh();
}

MainData::~MainData(){
    if (mesh) delete mesh;
}

void MainData::UpdateCursor(int x, int y){
    // 坐标反转
    cursorPos.x = 2.0f * x / cliSize.x - 1.0f;
    cursorPos.y = 1.0f - 2.0f * y / cliSize.y;
    if (menu)
        menu->CursorMove(cursorPos - menuPos);
}

void MainData::UpdateWindowSize(int x, int y){
    cliSize.x = x;
    cliSize.y = y;
    aspect = cliSize.x / cliSize.y;
    if (menu)
        menu->SetClientSize(cliSize);
}

void MainData::SetMenu(Menu* m){
    if (menu)
        menu->ResetSelect();
    menu = m;
    if (menu){
        menu->SetClientSize(cliSize);
        menu->CursorMove(cursorPos);
        menuPos = cursorPos;
    }
}

void MainData::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
    if (menu){
        if (menu->InChainMenu(cursorPos - menuPos)){
            menu->Click();
        }
        SetMenu(NULL);
        return;
    }
}

void MainData::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
}

void MainData::OnRightDown(int x, int y){
    UpdateCursor(x, y);
}

void MainData::OnRightUp(int x, int y){
    UpdateCursor(x, y);
}

Main::Main(){
    data = new MainData();
}

Main::~Main(){
    if (data) delete data;
}

ATOM Main::RegClass(){
    WNDCLASSEXA wc;

    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(hInst, MAKEINTRESOURCEA(IDC_NORMAL));
    wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    wc.hIconSm = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
    wc.hInstance = hInst;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "ModelView.MainWindow";
    wc.lpszMenuName = "";
    wc.style = 0;

    return RegisterClassExA(&wc);
}

HWND Main::CreateWnd(){
    return CreateWindowEx(
        0,
        "ModelView.MainWindow",
        "ModelView",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        1250, 750,
        NULL,
        LoadMenu(hInst, MAKEINTRESOURCE(IDC_MENU)),
        hInst,
        NULL
    );
}

void Main::FireEvent(IWindow* window, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    RECT cliRect;
    GetClientRect(hWnd, &cliRect);
    // 事件中鼠标坐标上下需反转
    int x = GET_X_LPARAM(lParam), y = cliRect.bottom - GET_Y_LPARAM(lParam);
    switch (uMsg){
    case WM_CREATE:
        window->OnCreate();
        break;
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
        break;
    case WM_UNICHAR:
        // 当其他软件试图使用UNICODE_NOCHAR测试本软件是否处理unicode字符时
        // 给予TRUE作为回应并忽视此消息
        if (wParam == UNICODE_NOCHAR){
            break;
        }
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

        cnt = DragQueryFile((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
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

LRESULT Main::LocalWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    //将事件发送至对应整个窗口的组件容器
    FireEvent(mainFrame, hWnd, uMsg, wParam, lParam);

    switch (uMsg){
    case WM_CREATE:
        DragAcceptFiles(hWnd, TRUE);
        SetTimer(hWnd, 0, 10, NULL);
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_SIZE:
        data->UpdateWindowSize(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_MOUSEMOVE:
        data->UpdateCursor(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        break;
    case WM_LBUTTONDOWN:
        data->OnLeftDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CLICKED)));
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
    case WM_UNICHAR:
        if (wParam == UNICODE_NOCHAR){
            return TRUE;
        }
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK Main::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    return Main::inst->LocalWndProc(hWnd, uMsg, wParam, lParam);
}

void CALLBACK Main::TimerProc(HWND hWnd, UINT uMsg, UINT_PTR wParam, DWORD lParam){
    return;
}

void Main::RequestRender(){
    inst->reqRender = true;
}

void Main::SetMenu(Menu* m){
    data->SetMenu(m);
}

void Main::OnRender(){
    ViewportManager::inst->Reset(hWnd);
    ViewportManager::inst->EnableScissor();

    mainFrame->OnRender();

    ViewportManager::inst->Reset(hWnd);

    if (data->menu)
        data->menu->Render(data->menuPos.x, data->menuPos.y);
}

int Main::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    MSG Msg;

    hInst = hInstance;

    AudioUtils::InitOpenAL();

    IWindow* mainWnd;
    IWindow* mainWnd2;
    IWindow* mainWnd3;
    IWindow* mainWnd4;
    IWindow* mainWnd5;
    IWindow* mainWnd6;

    LRContainer* lrCont1;

    UDContainer* udCont1;
    UDContainer* udCont2;
    UDContainer* udCont3;

    // mainWnd = new MainWindow();
    // mainWnd2 = new AudioPlayerWindow();
    // mainWnd3 = new PaintWindow();
    // mainWnd4 = new UVEditWindow();
    // mainWnd5 = new AudioCaptureWindow();
    // mainWnd6 = new NodeMapWindow();

    // udCont1 = new UDContainer(mainWnd, mainWnd3);
    // udCont2 = new UDContainer(mainWnd2, mainWnd4);
    // udCont3 = new UDContainer(mainWnd5, mainWnd6);
    // lrCont1 = new LRContainer(udCont2, udCont3);
    // mainFrame = new LRContainer(udCont1, lrCont1);

    mainFrame = new SelectionWindow();

    RegClass();
    ColorBoard::Init(hInstance);

    hWnd = CreateWnd();

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    DebugLog("Main Window Created");

    GLUtils::EnableOpenGL(hWnd, &hDC, &hRC);
    GLUtils::EnableOpenGLEXT();

    DebugLog("OpenGL Enabled");
    DebugLog("OpenGL Version %s", glGetString(GL_VERSION));
    DebugLog("OpenGL Renderer %s", glGetString(GL_RENDERER));
    DebugLog("OpenGL Vendor %s", glGetString(GL_VENDOR));
    //DebugLog("OpenGL Extensions %s", glGetString(GL_EXTENSIONS));

    glInitASCIIFont();
    glSelectFont(12, GB2312_CHARSET, "微软雅黑");
    // 楷体_GB2312

    DebugLog("OpenGL Use Encoding %s", "GB2312");

    HACCEL hAccelTable = LoadAccelerators(hInst, MAKEINTRESOURCE(IDC_MENU));

    while (GetMessage(&Msg, NULL, 0, 0)){
        if (!TranslateAccelerator(Msg.hwnd, hAccelTable, &Msg)){
            if (Msg.message == WM_QUIT){
                break;
            } else {
                TranslateMessage(&Msg);
                DispatchMessage(&Msg);
                if (reqRender || Msg.message != WM_TIMER){
                    reqRender = false;
                    wglMakeCurrent(hDC, hRC);
                    OnRender();
                    SwapBuffers(hDC);
                }
            }
        }
    }

    GLUtils::DisableOpenGL(hWnd, hDC, hRC);

    DestroyWindow(hWnd);

    AudioUtils::UninitOpenAL();
    
    return Msg.wParam;
}

HINSTANCE Main::hInst;
Main* Main::inst;
MainData* Main::data;
HWND Main::hWnd;
HDC Main::hDC;
HGLRC Main::hRC;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    Main::inst = new Main();
    int ret = Main::inst->WinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    delete Main::inst;
    return ret;
}