#include <editor/MainWindow.h>

#include <lib/opengl/gl/gl.h>
#include <lib/opengl/gl/glu.h>

#include <main.h>
#include <res.h>
#include <editor/gui/ViewManager.h>
#include <editor/dialog/ColorBoard.h>
#include <editor/dialog/Tips.h>
#include <utils/File.h>
#include <utils/AudioUtils.h>
#include <utils/math3d/Math.h>
#include <utils/math3d/LinearAlgebra.h>
#include <utils/math3d/Mesh.h>
#include <utils/math3d/ViewObject.h>
#include <utils/os/Shell.h>
#include <utils/gl/GLTexture2D.h>
#include <utils/gl/GLSkyBox.h>
#include <utils/gl/GLSimplified.h>

MainWindow::MoveButton::MoveButton(Vector2 center, float radius, MainWindow* main) : center(center), radius(radius), main(main) {}

MainWindow::MoveButton::~MoveButton(){
    if (texture) delete texture;
}

bool MainWindow::MoveButton::Trigger(Vector2 pos){
    return (pos - center).SqrMagnitude() <= radius * radius;
}

void MainWindow::MoveButton::Render(){
    glColor3f(1.0f, 1.0f, 1.0f);
    if (!texture)
        texture = new GLTexture2D(IDT_BUTTON_MOVE);
    texture->Enable();
    GLUtils::DrawCornerWithUV(center.x, center.y, 0.0f, 360.0f, radius, 0.05f);
    GLTexture2D::Disable();
}

void MainWindow::MoveButton::Click(Vector2 pos){
    DebugLog("MoveButton Click");
    start = main->camLookat;
}

void MainWindow::MoveButton::Drag(Vector2 dir){
    //DebugLog("MoveButton Drag %f %f", dir.x, dir.y);
    main->camLookat = start - (main->camRight * dir.x + main->camUp * dir.y) * main->camDis;
    main->UpdateLookAtLocation();
}

void MainWindow::MoveButton::ClickEnd(Vector2 pos, IButton* end){
    DebugLog("MoveButton ClickEnd");
    main->UpdateLookAtLocation();
}

MainWindow::RotateButton::RotateButton(Vector2 center, float radius, MainWindow* main) : center(center), radius(radius), main(main) {}
MainWindow::RotateButton::~RotateButton(){}

bool MainWindow::RotateButton::Trigger(Vector2 pos){
    return (pos - center).SqrMagnitude() <= radius * radius;
}

void MainWindow::RotateButton::Render(){
    Quaternion q = -main->camDir;
    Vector2 right = (q * Vector3::right).XZ() * radius;
    Vector2 forward = (q * Vector3::forward).XZ() * radius;
    Vector2 up = (q * Vector3::up).XZ() * radius;

    glColor3f(0.1f, 0.1f, 0.1f);
    GLUtils::DrawCornerWithUV(center.x, center.y, 0.0f, 360.0f, radius, 0.05f);

    glEnable(GL_LINE_SMOOTH);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertexv2(center - right); glVertexv2(center + right);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertexv2(center - forward); glVertexv2(center + forward);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertexv2(center - up); glVertexv2(center + up);
    glEnd();
    glDisable(GL_LINE_SMOOTH);
}

void MainWindow::RotateButton::Click(Vector2 pos){
    DebugLog("RotateButton Click");
    start = main->camDir;
    up = main->camUp;
    right = main->camRight;
}

void MainWindow::RotateButton::Drag(Vector2 dir){
    //DebugLog("RotateButton Drag %f %f", dir.x, dir.y);
    main->camDir = Quaternion::AxisAngle(up, -dir.x * 100.0f) *
                            Quaternion::AxisAngle(right, dir.y * 100.0f) * start;
    main->UpdateRotation();
}

void MainWindow::RotateButton::ClickEnd(Vector2 pos, IButton* end){
    DebugLog("RotateButton ClickEnd");
    main->UpdateRotation();
}

MainWindow::MoveOperation::MoveOperation(MainWindow* main) : main(main) {}
MainWindow::MoveOperation::~MoveOperation(){}

void MainWindow::MoveOperation::OnEnter(){
    DebugLog("MoveOperation OnEnter");

    x = y = z = true;
    start = main->cursorPos;

    switch (Main::data->selType){
    case GlobalData::SELECT_OBJECT:{
        MoveInfo info;
        info.obj = Main::data->curObject;
        info.pos = info.obj->GetWorldPos();
        moveInfo.Add(info);
    }
        break;
    case GlobalData::SELECT_VERTICES:
        if (Main::data->selPoints.Empty())
            break;
        Main::data->selPoints.Foreach<MoveOperation*>([](Vertex* v, MoveOperation* op){
            MoveInfo info;
            info.vert = v;
            info.pos = v->GetWorldPos();
            op->moveInfo.Add(info);
        }, this);
        break;
    case GlobalData::SELECT_EDGES:
        break;
    case GlobalData::SELECT_FACES:
        break;
    }
}

void MainWindow::MoveOperation::OnMove(){
    Vector2 mov;
    Vector3 delta;

    if (moveInfo.Empty())
        return;

    mov = (main->cursorPos - start) * main->camDis;
    delta = main->camRight * mov.x * main->aspect + main->camUp * mov.y;
    delta = Vector3(x ? delta.x : 0.0f, y ? delta.y : 0.0f, z ? delta.z : 0.0f);

    DebugLog("MoveOperation OnMove %f %f %f", delta.x, delta.y, delta.z);

    switch (Main::data->selType){
    case GlobalData::SELECT_OBJECT:
        moveInfo.Foreach<Vector3*>([](MoveInfo info, Vector3* offset){
            info.obj->SetWorldPos(info.pos + *offset);
        }, &delta);
        break;
    case GlobalData::SELECT_VERTICES:
        moveInfo.Foreach<Vector3*>([](MoveInfo info, Vector3* offset){
            info.vert->SetWorldPos(info.pos + *offset);
        }, &delta);
        break;
    case GlobalData::SELECT_EDGES:
        break;
    case GlobalData::SELECT_FACES:
        break;
    }
}

void MainWindow::MoveOperation::OnConfirm(){
    DebugLog("MoveOperation OnConfirm");
    moveInfo.Clear();
}

void MainWindow::MoveOperation::OnUndo(){
    DebugLog("MoveOperation OnUndo");

    switch (Main::data->selType){
    case GlobalData::SELECT_OBJECT:
        moveInfo.Foreach([](MoveInfo info){
            info.obj->SetWorldPos(info.pos);
        });
        break;
    case GlobalData::SELECT_VERTICES:
        moveInfo.Foreach([](MoveInfo info){
            info.vert->SetWorldPos(info.pos);
        });
        break;
    case GlobalData::SELECT_EDGES:
        break;
    case GlobalData::SELECT_FACES:
        break;
    }

    moveInfo.Clear();
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
    List<Vertex*> copies;
    size_t cnt;
    Mesh* mesh;

    if (Main::data->selType != GlobalData::SELECT_VERTICES){
        DebugError("ExcludeOperation Unsupported Select Mode");
        return;
    }

    mesh = Main::GetMesh();

    if (!mesh)
        return;

    cnt = Main::data->selPoints.Size();

    x = y = z = true;
    start = main->cursorPos;

    for (size_t i = 0; i < cnt; i++){
        copies.Add(Main::data->selPoints[i]);
        Main::data->selPoints[i] = mesh->AddVertex(Main::data->selPoints[i]->pos);
        mesh->AddEdge(copies[i], Main::data->selPoints[i]);
    }
    for (size_t i = 0; i < cnt; i++){
        for (size_t j = i + 1; j < cnt; j++){
            if (copies[i]->EdgeRelateTo(copies[j])){
                mesh->AddTriFace(copies[i], copies[j], Main::data->selPoints[j]);
                mesh->AddTriFace(copies[i], Main::data->selPoints[i], Main::data->selPoints[j]);
            }
        }
    }
    if (Main::data->selPoints.Size() > 0){
        Main::data->selPoints.Foreach<ExcludeOperation*>([](Vertex* v, ExcludeOperation* op){
            op->moveInfo.Add({v, v->GetWorldPos()});
        }, this);
    }
}

void MainWindow::ExcludeOperation::OnMove(){
    Vector2 mov;
    Vector3 delta;

    if (moveInfo.Empty())
        return;

    mov = (main->cursorPos - start) * main->camDis;
    delta = main->camRight * mov.x * main->aspect + main->camUp * mov.y;
    delta = Vector3(x ? delta.x : 0.0f, y ? delta.y : 0.0f, z ? delta.z : 0.0f);

    moveInfo.Foreach<Vector3*>([](MoveInfo info, Vector3* offset){
        info.vert->SetWorldPos(info.pos + *offset);
    }, &delta);
    DebugLog("ExcludeOperation OnMove %f %f %f", delta.x, delta.y, delta.z);
}

void MainWindow::ExcludeOperation::OnConfirm(){
    DebugLog("ExcludeOperation OnConfirm");
    moveInfo.Clear();
}

void MainWindow::ExcludeOperation::OnUndo(){
    DebugLog("ExcludeOperation OnUndo");
    Mesh* mesh = Main::GetMesh();

    if (!mesh)
        return;
    
    Main::data->selPoints.Clear();
    moveInfo.Foreach<Mesh*>([](MoveInfo info, Mesh* mesh){
        mesh->DeleteVertex(info.vert);
    }, mesh);
    moveInfo.Clear();
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

    mode = MODE_CAMERA;
    start = main->cursorPos;
    center = Vector3::zero;

    switch (Main::data->selType){
    case GlobalData::SELECT_OBJECT:{
        RotateInfo info;
        info.obj = Main::data->curObject;
        // 欧拉角任意轴向旋转未实现
        if (info.obj->transform.rotationMode != Transform::ROT_QUATERNION){
            mode = MODE_X;
            info.rot = info.obj->transform.rotation.Get();
        }else{
            info.pos = info.obj->transform.rotationXYZ.Get();
        }
        rotateInfo.Add(info);
        center = info.obj->GetParentChainMat() * Point3::zero;
        screenCenter = main->GetScreenPosition(center);
    }
        break;
    case GlobalData::SELECT_VERTICES:
        if (Main::data->selPoints.Empty())
            break;
        Main::data->selPoints.Foreach<RotateOperation*>([](Vertex* v, RotateOperation* op){
            RotateInfo info;
            info.vert = v;
            info.pos = v->GetWorldPos();
            op->rotateInfo.Add(info);
            op->center += info.pos;
        }, this);
        center /= Main::data->selPoints.Size();
        screenCenter = main->GetScreenPosition(center);
        break;
    case GlobalData::SELECT_EDGES:
        break;
    case GlobalData::SELECT_FACES:
        break;
    }

    dis = (main->cursorPos - screenCenter).Magnitude();
}

void MainWindow::RotateOperation::OnMove(){
    float delta;

    if (rotateInfo.Empty())
        return;
    
    delta = ((main->cursorPos - screenCenter).Magnitude() - dis) * 360.0f;

    DebugLog("RotateOperation Rotate %f Degree", delta);

    switch (Main::data->selType){
    case GlobalData::SELECT_OBJECT:{
        Vector3 rotVec;
        if (Main::data->curObject->transform.rotationMode != Transform::ROT_QUATERNION){
            switch (mode){
            case MODE_CAMERA: rotVec = Vector3::zero; break;
            case MODE_X: rotVec = Vector3(delta, 0.0f, 0.0f); break;
            case MODE_Y: rotVec = Vector3(0.0f, delta, 0.0f); break;
            case MODE_Z: rotVec = Vector3(0.0f, 0.0f, delta); break;
            }
            rotateInfo.Foreach<Vector3*>([](RotateInfo info, Vector3* rot){
                info.obj->transform.rotationXYZ.Set(info.pos + *rot);
            }, &rotVec);
        }else{
            switch (mode){
            case MODE_CAMERA: rotate = Quaternion::AxisAngle(main->camForward, delta); break;
            case MODE_X: rotate = Quaternion::AxisAngle(Vector3(1.0f, 0.0f, 0.0f), delta); break;
            case MODE_Y: rotate = Quaternion::AxisAngle(Vector3(0.0f, 1.0f, 0.0f), delta); break;
            case MODE_Z: rotate = Quaternion::AxisAngle(Vector3(0.0f, 0.0f, 1.0f), delta); break;
            }
            rotateInfo.Foreach<Quaternion*>([](RotateInfo info, Quaternion* rot){
                info.obj->transform.rotation.Set(*rot * info.rot);
            }, &rotate);
        }
    }
        break;
    case GlobalData::SELECT_VERTICES:
        switch (mode){
        case MODE_CAMERA: rotate = Quaternion::AxisAngle(main->camForward, delta); break;
        case MODE_X: rotate = Quaternion::AxisAngle(Vector3(1.0f, 0.0f, 0.0f), delta); break;
        case MODE_Y: rotate = Quaternion::AxisAngle(Vector3(0.0f, 1.0f, 0.0f), delta); break;
        case MODE_Z: rotate = Quaternion::AxisAngle(Vector3(0.0f, 0.0f, 1.0f), delta); break;
        }
        rotateInfo.Foreach<RotateOperation*>([](RotateInfo info, RotateOperation* op){
            info.vert->SetWorldPos(op->center + op->rotate * (info.pos - op->center));
        }, this);
        break;
    case GlobalData::SELECT_EDGES:
        break;
    case GlobalData::SELECT_FACES:
        break;
    }
}

void MainWindow::RotateOperation::OnConfirm(){
    DebugLog("RotateOperation OnConfirm");
    rotateInfo.Clear();
}

void MainWindow::RotateOperation::OnUndo(){
    DebugLog("RotateOperation OnUndo");
    
    switch (Main::data->selType){
    case GlobalData::SELECT_OBJECT:
        rotateInfo.Foreach([](RotateInfo info){
            info.obj->transform.rotationXYZ.Set(info.pos);
        });
        break;
    case GlobalData::SELECT_VERTICES:
        rotateInfo.Foreach([](RotateInfo info){
            info.vert->SetWorldPos(info.pos);
        });
        break;
    case GlobalData::SELECT_EDGES:
        break;
    case GlobalData::SELECT_FACES:
        break;
    }

    rotateInfo.Clear();
}

void MainWindow::RotateOperation::OnCommand(int id){
    switch (id){
    case IDM_OP_X: mode = MODE_X; break;
    case IDM_OP_Y: mode = MODE_Y; break;
    case IDM_OP_Z: mode = MODE_Z; break;
    case IDM_OP_PLANE_X: mode = MODE_X; break;
    case IDM_OP_PLANE_Y: mode = MODE_Y; break;
    case IDM_OP_PLANE_Z: mode = MODE_Z; break;
    }
}

MainWindow::SizeOperation::SizeOperation(MainWindow* main) : main(main) {}
MainWindow::SizeOperation::~SizeOperation(){}

void MainWindow::SizeOperation::OnEnter(){
    DebugLog("SizeOperation OnEnter");

    x = y = z = true;
    start = main->cursorPos;
    center = Vector3::zero;

    switch (Main::data->selType){
    case GlobalData::SELECT_OBJECT:{
        SizeInfo info;
        info.obj = Main::data->curObject;
        info.vec = info.obj->transform.scale.Get();
        sizeInfo.Add(info);
        center = info.obj->GetParentChainMat() * Point3::zero;
        screenCenter = main->GetScreenPosition(center);
    }
        break;
    case GlobalData::SELECT_VERTICES:
        if (Main::data->selPoints.Empty())
            break;
        Main::data->selPoints.Foreach<SizeOperation*>([](Vertex* v, SizeOperation* op){
            SizeInfo info;
            info.vert = v;
            info.vec = v->GetWorldPos();
            op->sizeInfo.Add(info);
            op->center += info.vec;
        }, this);
        center /= Main::data->selPoints.Size();
        screenCenter = main->GetScreenPosition(center);
        break;
    case GlobalData::SELECT_EDGES:
        break;
    case GlobalData::SELECT_FACES:
        break;
    }

    startSize = (main->cursorPos - screenCenter).Magnitude();
}

void MainWindow::SizeOperation::OnMove(){
    if (sizeInfo.Empty())
        return;
    
    scale = (main->cursorPos - screenCenter).Magnitude() / startSize;

    DebugLog("SizeOperation Scale %f", scale);
    
    switch (Main::data->selType){
    case GlobalData::SELECT_OBJECT:
        sizeInfo.Foreach<SizeOperation*>([](SizeInfo info, SizeOperation* op){
            info.obj->transform.scale.Set(Vector3(
                op->x ? info.vec.x * op->scale : info.vec.x,
                op->y ? info.vec.y * op->scale : info.vec.y,
                op->z ? info.vec.z * op->scale : info.vec.z
            ));
        }, this);
        break;
    case GlobalData::SELECT_VERTICES:
        sizeInfo.Foreach<SizeOperation*>([](SizeInfo info, SizeOperation* op){
            Vector3 res = op->center + (info.vec - op->center) * op->scale;
            info.vert->SetWorldPos(Vector3(
                op->x ? res.x : info.vec.x,
                op->y ? res.y : info.vec.y,
                op->z ? res.z : info.vec.z
            ));
        }, this);
        break;
    case GlobalData::SELECT_EDGES:
        break;
    case GlobalData::SELECT_FACES:
        break;
    }
}

void MainWindow::SizeOperation::OnConfirm(){
    DebugLog("SizeOperation OnConfirm");
    sizeInfo.Clear();
}

void MainWindow::SizeOperation::OnUndo(){
    DebugLog("SizeOperation OnUndo");

    switch (Main::data->selType){
    case GlobalData::SELECT_OBJECT:
        sizeInfo.Foreach([](SizeInfo info){
            info.obj->transform.scale.Set(info.vec);
        });
        break;
    case GlobalData::SELECT_VERTICES:
        sizeInfo.Foreach([](SizeInfo info){
            info.vert->SetWorldPos(info.vec);
        });
        break;
    case GlobalData::SELECT_EDGES:
        break;
    case GlobalData::SELECT_FACES:
        break;
    }
    
    sizeInfo.Clear();
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
    if (Main::data->curObject){
        Main::data->curObject->OnSelect(window->camPos, window->cursorDir);
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
        Main::data->selPoints.Clear();
        return;
    }
    if (Main::data->curObject){
        SelectInfo info;
        info.camPos = window->camPos;
        info.camDir = window->camDir;
        info.zBound = Vector2(window->camDis * 0.02, window->camDis * 20.0);
        info.rect = GLRect(Vector2(start.x * window->aspect, start.y), Vector2(end.x * window->aspect, end.y));
        
        Main::data->curObject->OnSelect(&info);
    }
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
    DebugLog("MainWindow Launched");
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
        DialogVersionInfo();
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

    Menu* selTypeMenu = new Menu();
    selTypeMenu->AddItem(new MenuItem(L"对象(部分实现)", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        Main::SelectType(GlobalData::SELECT_OBJECT);
    }, this));
    selTypeMenu->AddItem(new MenuItem(L"顶点", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        Main::SelectType(GlobalData::SELECT_VERTICES);
    }, this));
    selTypeMenu->AddItem(new MenuItem(L"边(部分实现)", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        Main::SelectType(GlobalData::SELECT_EDGES);
    }, this));
    selTypeMenu->AddItem(new MenuItem(L"面(未实现)", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        Main::SelectType(GlobalData::SELECT_FACES);
    }, this));
    basicMenu->AddItem(new MenuItem(L"选择类型", selTypeMenu));

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

    Menu* objectMenu = new Menu();
    objectMenu->AddItem(new MenuItem(L"空对象", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        Main::AddObject(new AViewObject());
    }, this));
    objectMenu->AddItem(new MenuItem(L"网格体", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        Main::AddObject(new MeshObject());
    }, this));
    objectMenu->AddItem(new MenuItem(L"三次贝塞尔曲线", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        Main::AddObject(new BezierCurveObject());
    }, this));
    objectMenu->AddItem(new MenuItem(L"点光源", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        Main::AddObject(new PointLightObject());
    }, this));
    objectMenu->AddItem(new MenuItem(L"音频收听者", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        LocalData::GetLocalInst()->CreateAudioListener();
    }, this));
    objectMenu->AddItem(new MenuItem(L"摄像机", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        LocalData::GetLocalInst()->CreateCamera();
    }, this));
    basicMenu->AddItem(new MenuItem(L"添加对象", objectMenu));

    basicMenu->AddItem(new MenuItem());
    basicMenu->AddItem(new SwitchMenuItem(L"光照:开", L"光照:关", SWITCHMENUITEM_LAMBDA_TRANS(MainWindow)[](bool state, MainWindow* window){
        window->lightEnabled = state;
        DebugLog("MainWindow Light State %s", window->lightEnabled ? "On" : "Off");
    }, this, lightEnabled));

    basicMenu->AddItem(new MenuItem());
    basicMenu->AddItem(new SwitchMenuItem(L"音频控制:开", L"音频控制:关", SWITCHMENUITEM_LAMBDA_TRANS(MainWindow)[](bool state, MainWindow* window){
        window->audioControl = state;
        DebugLog("MainWindow Audio Control %s", window->audioControl ? "On" : "Off");
        if (state){
            // 重置速度并记录当前位置
            alListenerPosAutoVelv3(window->camPos);
            alListenerVelocityv3(Vector3::zero);
            alListenerDirv3(window->camForward, window->camUp);
        }else{
            alListenerPosv3(Vector3::zero);
            alListenerVelocityv3(Vector3::zero);
            alListenerDirv3(Vector3::forward, Vector3::up);
        }
    }, this, audioControl));
    basicMenu->AddItem(new SwitchMenuItem(L"多普勒效应:开", L"多普勒效应:关", SWITCHMENUITEM_LAMBDA_TRANS(MainWindow)[](bool state, MainWindow* window){
        window->dopplerEffect = state;
        DebugLog("MainWindow Doppler Effect %s", window->dopplerEffect ? "On" : "Off");
        if (state){
            // 重置速度并记录当前位置
            alListenerPosAutoVelv3(window->camPos);
            alListenerVelocityv3(Vector3::zero);
        }else{
            alListenerVelocityv3(Vector3::zero);
        }
    }, this, dopplerEffect));

    insertMenu = new Menu();
    insertMenu->AddItem(new MenuItem(L"位置", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        Main::data->curObject->transform.InsertPos(Main::data->animFrame);
    }, this));
    insertMenu->AddItem(new MenuItem(L"旋转", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        Main::data->curObject->transform.InsertRot(Main::data->animFrame);
    }, this));
    insertMenu->AddItem(new MenuItem(L"大小", MENUITEM_LAMBDA_TRANS(MainWindow)[](MainWindow* window){
        Main::data->curObject->transform.InsertScale(Main::data->animFrame);
    }, this));

    uiMgr->AddButton(new RotateButton(Vector2(0.85f, 0.85f), 0.12f, this));
    uiMgr->AddButton(new MoveButton(Vector2(0.55f, 0.85f), 0.12f, this));

    if (audioControl){
        alListenerDirv3(camForward, camUp);
    }else{
        alListenerDirv3(Vector3::forward, Vector3::up);
    }
}

MainWindow::~MainWindow(){
    DebugLog("MainWindow Destroyed");
    if (basicMenu) delete basicMenu;
    if (insertMenu) delete insertMenu;
    if (uiMgr) delete uiMgr;
    if (curOp) delete curOp;
    if (skyBox) delete skyBox;
}

void MainWindow::InitCamera(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, ViewManager::GetLocalInst()->GetAspect(), camDis * 0.02, camDis * 20.0);

    Vector3 camPos = camLookat - camDir * Vector3::forward * camDis;
    Vector3 camUp = camDir * Vector3::up;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camPos.x, camPos.y, camPos.z,
            camLookat.x, camLookat.y, camLookat.z,
            camUp.x, camUp.y, camUp.z);
}

void MainWindow::RenderModelView(){
    // 这些暂定是默认设置
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_ALPHA_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    glDisable(GL_DITHER);
    glDisable(GL_FOG);
    glDisable(GL_LOGIC_OP);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    InitCamera();

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glDisable(GL_DEPTH_TEST);
    skyBox->Render(camPos, camDis);
    glEnable(GL_DEPTH_TEST);

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

    RenderOptions options;
    options.vertex = true;
    options.edge = true;
    options.face = true;
    options.light = lightEnabled;
    options.editor = true;
    options.objOp = objOp;
    //TODO 后续光照设置法线
    Main::data->scene->OnChainRender(&options);

    // 已选择点绘制
    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(8.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);
    Main::data->selPoints.Foreach([](Vertex* p){
        Vector3 pos = p->GetWorldPos();
        glVertex3f(pos.x, pos.y, pos.z);
    });
    glEnd();
    glDisable(GL_POINT_SMOOTH);

    // 已选择边绘制
    glDisable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(8.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    Main::data->selEdges.Foreach([](Edge* p){
        Vector3 pos = p->v1->GetWorldPos();
        glVertex3f(pos.x, pos.y, pos.z);
        pos = p->v2->GetWorldPos();
        glVertex3f(pos.x, pos.y, pos.z);
    });
    glEnd();
    glLineWidth(1.0f);
    glDisable(GL_LINE_SMOOTH);
}

void MainWindow::OnRender(){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderModelView();

    // 工具绘制
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLUtils::ResetProjection();
    GLUtils::ResetModelView();
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
    if (audioControl)
        alListenerDirv3(camForward, camUp);
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

void MainWindow::AddPoint(){
    Point3 pos;
    AViewObject* obj;
    Mesh* mesh;

    obj = Main::data->curObject;
    if (!obj)
        return;
    mesh = obj->GetMesh();
    if (!mesh)
        return;
    
    pos = camLookat + (camUp * cursorPos.y + camRight * cursorPos.x * aspect) * camDis;
    mesh->AddVertex(obj->transform.chainInvMat * pos);
    DebugLog("Point at %f %f %f", pos.x, pos.y, pos.z);
}

void MainWindow::DeletePoint(){
    Mesh* mesh = Main::GetMesh();
    if (!mesh)
        return;
    Main::data->selPoints.Foreach<Mesh*>([](Vertex* v, Mesh* m){
        m->DeleteVertex(v);
    }, mesh);
    Main::data->selPoints.Clear();
}

bool MainWindow::SaveMesh(Mesh* mesh){
    if (!mesh)
        return false;
    WString file = ShellFileSelectWindow(WString(IDS_OBJFILE_FILTER), FILESELECT_REQ_PATH, true);
    if (file.GetLength() == 0){
        DebugError("Stop Saving");
        return false;
    }
    File target(file);
    if (!target.CreateNew()){
        DebugError("CreateFile %S Error!", file.GetString());
        return false;
    }
    DebugLog("Saving Object");
    mesh->WriteToOBJ(target, true, false);
    target.Close();
    return true;
}

bool MainWindow::LoadMesh(AViewObject* obj){
    if (!obj->GetMesh())
        return false;
    WString file = ShellFileSelectWindow(WString(IDS_OBJFILE_FILTER), FILESELECT_REQ_FILE | FILESELECT_REQ_PATH);
    if (file.GetLength() == 0){
        DebugLog("Stop Loading");
        return false;
    }
    return LoadMesh(obj, file);
}

bool MainWindow::LoadMesh(AViewObject* obj, WString path){
    BY_HANDLE_FILE_INFORMATION fileInfo;
    char* fileData;
    size_t fileLen, filePtr = 0;
    List<Vertex*> vert;
    List<Vector2> vertUV;
    List<Vector3> vertNormal;
    Mesh* mesh;

    mesh = obj->GetMesh();

    if (!mesh)
        return false;
    
    File file(path);
    if (!file.Open()){
        DebugError("OpenFile %S Error!", path.GetString());
        return false;
    }

    DebugLog("Loading Object");
    fileLen = file.GetSize();
    fileData = new char[fileLen + 1];
    fileData[fileLen] = '\0';

    file.Read(fileData, fileLen);

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
                vert.Add(mesh->AddVertex(vec));
            }else if (__builtin_sscanf(fileData + filePtr, "vt %f %f", &vec.x, &vec.y) == 2){
                vertUV.Add(Vector2(vec.x, vec.y));
            }else if (__builtin_sscanf(fileData + filePtr, "vn %f %f %f", &vec.x, &vec.y, &vec.z) == 3){
                vertNormal.Add(vec);
            }else if (__builtin_sscanf(fileData + filePtr, "f %d/%d %d/%d %d/%d", &v1, &vt1, &v2, &vt2, &v3, &vt3) == 6){
                mesh->AddTriFace(vert[v1 - 1], vert[v2 - 1], vert[v3 - 1]);
                vert[v1 - 1]->uv = vertUV[vt1 - 1];
                vert[v2 - 1]->uv = vertUV[vt2 - 1];
                vert[v3 - 1]->uv = vertUV[vt3 - 1];
            }else if (__builtin_sscanf(fileData + filePtr, "f %d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3) == 6){
                mesh->AddTriFace(vert[v1 - 1], vert[v2 - 1], vert[v3 - 1]);
                vert[v1 - 1]->normal = vertNormal[vn1 - 1];
                vert[v2 - 1]->normal = vertNormal[vn2 - 1];
                vert[v3 - 1]->normal = vertNormal[vn3 - 1];
            }else if (__builtin_sscanf(fileData + filePtr, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3) == 9){
                mesh->AddTriFace(vert[v1 - 1], vert[v2 - 1], vert[v3 - 1]);
                vert[v1 - 1]->uv = vertUV[vt1 - 1];
                vert[v2 - 1]->uv = vertUV[vt2 - 1];
                vert[v3 - 1]->uv = vertUV[vt3 - 1];
                vert[v1 - 1]->normal = vertNormal[vn1 - 1];
                vert[v2 - 1]->normal = vertNormal[vn2 - 1];
                vert[v3 - 1]->normal = vertNormal[vn3 - 1];
            }else if (__builtin_sscanf(fileData + filePtr, "f %d %d %d", &v1, &v2, &v3) == 3){
                mesh->AddTriFace(vert[v1 - 1], vert[v2 - 1], vert[v3 - 1]);
            }else{
                DebugError("Object File Unknown Line %s", fileData + filePtr);
            }
            filePtr = i + 1;
        }
    }
    delete[] fileData;
    file.Close();

    return true;
}

void MainWindow::OnCreate(){
    skyBox = new GLSkyBox();
    skyBox->Set(GLSkyBox::LEFT, new GLTexture2D(IDT_SKYBOX_LEFT));
    skyBox->Set(GLSkyBox::RIGHT, new GLTexture2D(IDT_SKYBOX_RIGHT));
    skyBox->Set(GLSkyBox::FRONT, new GLTexture2D(IDT_SKYBOX_FRONT));
    skyBox->Set(GLSkyBox::BACK, new GLTexture2D(IDT_SKYBOX_BACK));
    skyBox->Set(GLSkyBox::TOP, new GLTexture2D(IDT_SKYBOX_TOP));
    skyBox->Set(GLSkyBox::DOWN, new GLTexture2D(IDT_SKYBOX_DOWN));
}

void MainWindow::OnClose(){}

void MainWindow::OnTimer(int id){
    if (audioControl){
        if (dopplerEffect){
            alListenerPosAutoVelv3(camPos);
        }else{
            alListenerPosv3(camPos);
        }
    }
}

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
    SaveMesh(Main::GetMesh());
}

void MainWindow::OnInsLoad(){
    LoadMesh(Main::data->curObject);
}

void MainWindow::OnInsTopology(){
    Mesh* mesh = Main::GetMesh();
    if (!mesh)
        return;
    switch (Main::data->selPoints.Size()){
    case 2:
        mesh->AddEdge(Main::data->selPoints[0], Main::data->selPoints[1]);
        break;
    case 3:
        mesh->AddTriFace(Main::data->selPoints[0], Main::data->selPoints[1], Main::data->selPoints[2]);
        break;
    }
}

void MainWindow::OnInsSelectColor(){
    Vector3 color = ColorBoard::GetColor();
    Main::data->selPoints.Foreach<Vector3*>([](Vertex* v, Vector3* c){
        v->color = *c;
    }, &color);
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
        Mesh* mesh = Main::GetMesh();
        if (!mesh)
            break;
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
        Mesh* mesh = Main::GetMesh();
        if (!mesh)
            break;
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
        Mesh* mesh = Main::GetMesh();
        if (!mesh)
            break;
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
        Mesh* mesh = Main::GetMesh();
        if (!mesh)
            break;
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
        Mesh* mesh = Main::GetMesh();
        if (!mesh)
            break;
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
    case IDM_MENU_INSERT:
        Main::SetMenu(insertMenu);
        break;
    case IDM_TOOL_EMPTY:
        SetTool(new EmptyTool(this));
        break;
    case IDM_TOOL_SELECTBOX:
        SetTool(new SelectTool(this));
        break;
    case IDM_TEXTURE_ENABLE:{
        Mesh* mesh = Main::GetMesh();
        if (!mesh)
            break;
        mesh->SetTexture(IDT_EARTH_WATER);
    }
        break;
    case IDM_TEXTURE_DISABLE:{
        Mesh* mesh = Main::GetMesh();
        if (!mesh)
            break;
        mesh->ResetTexture();
    }
        break;
    case IDM_TEXTURE_LOAD:{
        Mesh* mesh = Main::GetMesh();
        if (!mesh)
            break;
        WString file = ShellFileSelectWindow(WString(IDS_PICFILE_FILTER), FILESELECT_REQ_FILE | FILESELECT_REQ_PATH);
        if (file.GetLength() == 0){
            DebugLog("Stop Loading");
            break;
        }
        DebugLog("Load Texture %S", file.GetString());
        mesh->SetTexture(new GLTexture2D(file.GetString()));
    }
    }
    // 当前操作的命令
    if (curOp)
        curOp->OnCommand(id);
    if (curTool)
        curTool->OnCommand(id);
    if (Main::data->curObject)
        Main::data->curObject->OnMenuAccel(id, accel);
}

void MainWindow::OnDropFileW(const wchar_t* path){
    wchar_t* suffix = wcsrchr(path, L'.');
    if (wcscmp(suffix, L".obj")){
        WString message(IDS_OBJFILE_FORM_WARNING);
        WString caption(IDS_OBJFILE_FORM_WARNING_CAPTION);
        if (ShellMsgBox(caption, message) != MSGBOX_YES){
            DebugLog("MainWindow::OnDropFileW Stop Load File");
            return;
        }
    }
    LoadMesh(Main::data->curObject, path);
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

Point3 MainWindow::GetLookPosition(Point3 pos){
    return (-camDir) * (pos - camPos);
}

Point2 MainWindow::GetScreenPosition(Point3 pos){
    Vector3 lookPos = (-camDir) * (pos - camPos);
    return Point2((lookPos.x / lookPos.y) / aspect, lookPos.z / lookPos.y);
}