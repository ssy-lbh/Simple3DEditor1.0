#include <editor/MainWindow.h>

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include <main.h>
#include <res.h>
#include <io/File.h>
#include <util/AudioUtils.h>
#include <util/math3d/Math.h>
#include <util/math3d/LinearAlgebra.h>
#include <util/math3d/Mesh.h>
#include <util/os/Shell.h>
#include <util/os/Resource.h>
#include <util/gl/GLTexture2D.h>
#include <util/gl/GLSkyBox.h>
#include <util/gl/GLSimplified.h>
#include <editor/main/Tool.h>
#include <editor/main/Operation.h>
#include <editor/gui/Menu.h>
#include <editor/gui/Menu.h>
#include <editor/gui/GUIUtils.h>
#include <editor/dialog/ColorBoard.h>
#include <editor/dialog/Tips.h>
#include <editor/main/ViewManager.h>
#include <editor/main/ViewObject.h>
#include <editor/object/AllObjects.h>

#include <lib/json/nlohmann/json.hpp>

#include <lib/imgui/imgui.h>
#include <util/math3d/ImVec.h>

namespace simple3deditor {

class MainWindow::MoveOperation : public IOperation {
private:
    struct MoveInfo {
        union {
            AViewObject* obj;
            Vertex* vert;
            Edge* e;
            Face* f;
        };
        Vector3 pos;
    };

    Point2 start;
    List<MoveInfo> moveInfo;
    bool x, y, z;
    MainWindow* main;

public:
    MoveOperation(MainWindow* main) : main(main) {}
    virtual ~MoveOperation() override{}

    virtual void OnEnter() override{
        DebugLog("MoveOperation OnEnter");

        x = y = z = true;
        start = main->cursorPos;

        switch (Main::data->selType){
        case SelectionType::SELECT_OBJECT:{
            MoveInfo info;
            info.obj = Main::data->curObject;
            info.pos = info.obj->GetWorldPos();
            moveInfo.Add(info);
        }
            break;
        case SelectionType::SELECT_VERTICES:
            if (Main::data->selPoints.Empty())
                break;
            Main::data->selPoints.Foreach([=](Vertex* v){
                MoveInfo info;
                info.vert = v;
                info.pos = v->GetWorldPos();
                this->moveInfo.Add(info);
            });
            break;
        case SelectionType::SELECT_EDGES:
            break;
        case SelectionType::SELECT_FACES:
            break;
        }
    }

    virtual void OnMove() override{
        Vector2 mov;
        Vector3 delta;

        if (moveInfo.Empty())
            return;

        mov = (main->cursorPos - start) * main->camDis;
        delta = main->camRight * mov.x * main->aspect + main->camUp * mov.y;
        delta = Vector3(x ? delta.x : 0.0f, y ? delta.y : 0.0f, z ? delta.z : 0.0f);

        //DebugLog("MoveOperation OnMove %f %f %f", delta.x, delta.y, delta.z);

        switch (Main::data->selType){
        case SelectionType::SELECT_OBJECT:
            moveInfo.Foreach([=](MoveInfo info){
                info.obj->SetWorldPos(info.pos + delta);
            });
            break;
        case SelectionType::SELECT_VERTICES:
            moveInfo.Foreach([=](MoveInfo info){
                info.vert->SetWorldPos(info.pos + delta);
            });
            break;
        case SelectionType::SELECT_EDGES:
            break;
        case SelectionType::SELECT_FACES:
            break;
        }
    }
    
    virtual void OnCommand(int id) override{
        switch (id){
        case IDM_OP_X: x = true; y = z = false; break;
        case IDM_OP_Y: y = true; z = x = false; break;
        case IDM_OP_Z: z = true; x = y = false; break;
        case IDM_OP_PLANE_X: x = false; y = z = true; break;
        case IDM_OP_PLANE_Y: y = false; z = x = true; break;
        case IDM_OP_PLANE_Z: z = false; x = y = true; break;
        }
    }

    virtual void OnConfirm() override{
        DebugLog("MoveOperation OnConfirm");
        moveInfo.Clear();
    }

    virtual void OnUndo() override{
        DebugLog("MoveOperation OnUndo");

        switch (Main::data->selType){
        case SelectionType::SELECT_OBJECT:
            moveInfo.Foreach([](MoveInfo info){
                info.obj->SetWorldPos(info.pos);
            });
            break;
        case SelectionType::SELECT_VERTICES:
            moveInfo.Foreach([](MoveInfo info){
                info.vert->SetWorldPos(info.pos);
            });
            break;
        case SelectionType::SELECT_EDGES:
            break;
        case SelectionType::SELECT_FACES:
            break;
        }

        moveInfo.Clear();
    }
};

class MainWindow::ExcludeOperation : public IOperation {
private:
    struct MoveInfo {
        Vertex* vert;
        Point3 pos;
    };

    Point2 start;
    List<MoveInfo> moveInfo;
    bool x, y, z;
    MainWindow* main;

public:
    ExcludeOperation(MainWindow* main) : main(main) {}
    virtual ~ExcludeOperation() override{}

    virtual void OnEnter() override{
        DebugLog("ExcludeOperation OnEnter");
        List<Vertex*> copies;
        size_t cnt;
        Mesh* mesh;

        if (Main::data->selType != SelectionType::SELECT_VERTICES){
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
            Main::data->selPoints.Foreach([=](Vertex* v){
                this->moveInfo.Add({v, v->GetWorldPos()});
            });
        }
    }

    virtual void OnMove() override{
        Vector2 mov;
        Vector3 delta;

        if (moveInfo.Empty())
            return;

        mov = (main->cursorPos - start) * main->camDis;
        delta = main->camRight * mov.x * main->aspect + main->camUp * mov.y;
        delta = Vector3(x ? delta.x : 0.0f, y ? delta.y : 0.0f, z ? delta.z : 0.0f);

        moveInfo.Foreach([=](MoveInfo info){
            info.vert->SetWorldPos(info.pos + delta);
        });
        //DebugLog("ExcludeOperation OnMove %f %f %f", delta.x, delta.y, delta.z);
    }

    virtual void OnCommand(int id) override{
        switch (id){
        case IDM_OP_X: x = true; y = z = false; break;
        case IDM_OP_Y: y = true; z = x = false; break;
        case IDM_OP_Z: z = true; x = y = false; break;
        case IDM_OP_PLANE_X: x = false; y = z = true; break;
        case IDM_OP_PLANE_Y: y = false; z = x = true; break;
        case IDM_OP_PLANE_Z: z = false; x = y = true; break;
        }
    }

    virtual void OnConfirm() override{
        DebugLog("ExcludeOperation OnConfirm");
        moveInfo.Clear();
    }

    virtual void OnUndo() override{
        DebugLog("ExcludeOperation OnUndo");
        Mesh* mesh = Main::GetMesh();

        if (!mesh)
            return;
        Main::data->selPoints.Clear();
        moveInfo.Foreach([=](MoveInfo info){
            mesh->DeleteVertex(info.vert);
        });
        moveInfo.Clear();
    }
};

class MainWindow::RotateOperation : public IOperation {
private:
    struct RotateInfo {
        union {
            AViewObject* obj;
            Vertex* vert;
            Edge* e;
            Face* f;
        };
        Point3 pos;
        Quaternion rot;
    };

    enum RotateMode {
        MODE_CAMERA,
        MODE_X,
        MODE_Y,
        MODE_Z
    };

    Point2 start;
    List<RotateInfo> rotateInfo;
    RotateMode mode;
    MainWindow* main;
    Point3 center;
    Point2 screenCenter;
    float dis;
    Quaternion rotate;

public:
    RotateOperation(MainWindow* main) : main(main) {}
    virtual ~RotateOperation() override{}

    virtual void OnEnter() override{
        DebugLog("RotateOperation OnEnter");

        mode = MODE_CAMERA;
        start = main->cursorPos;
        center = Vector3::zero;

        switch (Main::data->selType){
        case SelectionType::SELECT_OBJECT:{
            RotateInfo info;
            info.obj = Main::data->curObject;
            // ŷ��������������תδʵ��
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
        case SelectionType::SELECT_VERTICES:
            if (Main::data->selPoints.Empty())
                break;
            Main::data->selPoints.Foreach([=](Vertex* v){
                RotateInfo info;
                info.vert = v;
                info.pos = v->GetWorldPos();
                this->rotateInfo.Add(info);
                this->center += info.pos;
            });
            center /= (float)Main::data->selPoints.Size();
            screenCenter = main->GetScreenPosition(center);
            break;
        case SelectionType::SELECT_EDGES:
            break;
        case SelectionType::SELECT_FACES:
            break;
        }

        dis = (main->cursorPos - screenCenter).Magnitude();
    }

    virtual void OnMove() override{
        float delta;

        if (rotateInfo.Empty())
            return;
        
        delta = ((main->cursorPos - screenCenter).Magnitude() - dis) * 360.0f;

        //DebugLog("RotateOperation Rotate %f Degree", delta);

        switch (Main::data->selType){
        case SelectionType::SELECT_OBJECT:{
            Vector3 rotVec;
            if (Main::data->curObject->transform.rotationMode != Transform::ROT_QUATERNION){
                switch (mode){
                case MODE_CAMERA: rotVec = Vector3::zero; break;
                case MODE_X: rotVec = Vector3(delta, 0.0f, 0.0f); break;
                case MODE_Y: rotVec = Vector3(0.0f, delta, 0.0f); break;
                case MODE_Z: rotVec = Vector3(0.0f, 0.0f, delta); break;
                }
                rotateInfo.Foreach([=](RotateInfo info){
                    info.obj->transform.SetRotationXYZ(info.pos + rotVec);
                });
            }else{
                switch (mode){
                case MODE_CAMERA: rotate = Quaternion::AxisAngle(main->camForward, delta); break;
                case MODE_X: rotate = Quaternion::AxisAngle(Vector3(1.0f, 0.0f, 0.0f), delta); break;
                case MODE_Y: rotate = Quaternion::AxisAngle(Vector3(0.0f, 1.0f, 0.0f), delta); break;
                case MODE_Z: rotate = Quaternion::AxisAngle(Vector3(0.0f, 0.0f, 1.0f), delta); break;
                }
                rotateInfo.Foreach([=](RotateInfo info){
                    info.obj->transform.SetRotation(rotate * info.rot);
                });
            }
        }
            break;
        case SelectionType::SELECT_VERTICES:
            switch (mode){
            case MODE_CAMERA: rotate = Quaternion::AxisAngle(main->camForward, delta); break;
            case MODE_X: rotate = Quaternion::RotateX(delta); break;
            case MODE_Y: rotate = Quaternion::RotateY(delta); break;
            case MODE_Z: rotate = Quaternion::RotateZ(delta); break;
            }
            rotateInfo.Foreach([=](RotateInfo info){
                info.vert->SetWorldPos(this->center + this->rotate * (info.pos - this->center));
            });
            break;
        case SelectionType::SELECT_EDGES:
            break;
        case SelectionType::SELECT_FACES:
            break;
        }
    }

    virtual void OnCommand(int id) override{
        switch (id){
        case IDM_OP_X: mode = MODE_X; break;
        case IDM_OP_Y: mode = MODE_Y; break;
        case IDM_OP_Z: mode = MODE_Z; break;
        case IDM_OP_PLANE_X: mode = MODE_X; break;
        case IDM_OP_PLANE_Y: mode = MODE_Y; break;
        case IDM_OP_PLANE_Z: mode = MODE_Z; break;
        }
    }

    virtual void OnConfirm() override{
        DebugLog("RotateOperation OnConfirm");
        rotateInfo.Clear();
    }

    virtual void OnUndo() override{
        DebugLog("RotateOperation OnUndo");
        
        switch (Main::data->selType){
        case SelectionType::SELECT_OBJECT:
            rotateInfo.Foreach([](RotateInfo info){
                info.obj->transform.SetRotationXYZ(info.pos);
            });
            break;
        case SelectionType::SELECT_VERTICES:
            rotateInfo.Foreach([](RotateInfo info){
                info.vert->SetWorldPos(info.pos);
            });
            break;
        case SelectionType::SELECT_EDGES:
            break;
        case SelectionType::SELECT_FACES:
            break;
        }

        rotateInfo.Clear();
    }
};

class MainWindow::SizeOperation : public IOperation {
private:
    struct SizeInfo {
        union {
            AViewObject* obj;
            Vertex* vert;
            Edge* e;
            Face* f;
        };
        Vector3 vec;
    };

    Point3 center;
    Point2 start;
    List<SizeInfo> sizeInfo;
    bool x, y, z;
    MainWindow* main;
    Point2 screenCenter;
    float startSize;
    float scale;

public:
    SizeOperation(MainWindow* main) : main(main) {}
    virtual ~SizeOperation() override{}

    virtual void OnEnter() override{
        DebugLog("SizeOperation OnEnter");

        x = y = z = true;
        start = main->cursorPos;
        center = Vector3::zero;

        switch (Main::data->selType){
        case SelectionType::SELECT_OBJECT:{
            SizeInfo info;
            info.obj = Main::data->curObject;
            info.vec = info.obj->transform.scale.Get();
            sizeInfo.Add(info);
            center = info.obj->GetParentChainMat() * Point3::zero;
            screenCenter = main->GetScreenPosition(center);
        }
            break;
        case SelectionType::SELECT_VERTICES:
            if (Main::data->selPoints.Empty())
                break;
            Main::data->selPoints.Foreach([=](Vertex* v){
                SizeInfo info;
                info.vert = v;
                info.vec = v->GetWorldPos();
                this->sizeInfo.Add(info);
                this->center += info.vec;
            });
            center /= (float)Main::data->selPoints.Size();
            screenCenter = main->GetScreenPosition(center);
            break;
        case SelectionType::SELECT_EDGES:
            break;
        case SelectionType::SELECT_FACES:
            break;
        }

        startSize = (main->cursorPos - screenCenter).Magnitude();
    }

    virtual void OnMove() override{
        if (sizeInfo.Empty())
            return;
        
        scale = (main->cursorPos - screenCenter).Magnitude() / startSize;

        //DebugLog("SizeOperation Scale %f", scale);
        
        switch (Main::data->selType){
        case SelectionType::SELECT_OBJECT:
            sizeInfo.Foreach([=](SizeInfo info){
                info.obj->transform.scale.Set(Vector3(
                    this->x ? info.vec.x * this->scale : info.vec.x,
                    this->y ? info.vec.y * this->scale : info.vec.y,
                    this->z ? info.vec.z * this->scale : info.vec.z
                ));
            });
            break;
        case SelectionType::SELECT_VERTICES:
            sizeInfo.Foreach([=](SizeInfo info){
                Vector3 res = this->center + (info.vec - this->center) * this->scale;
                info.vert->SetWorldPos(Vector3(
                    this->x ? res.x : info.vec.x,
                    this->y ? res.y : info.vec.y,
                    this->z ? res.z : info.vec.z
                ));
            });
            break;
        case SelectionType::SELECT_EDGES:
            break;
        case SelectionType::SELECT_FACES:
            break;
        }
    }

    virtual void OnCommand(int id) override{
        switch (id){
        case IDM_OP_X: x = true; y = z = false; break;
        case IDM_OP_Y: y = true; z = x = false; break;
        case IDM_OP_Z: z = true; x = y = false; break;
        case IDM_OP_PLANE_X: x = false; y = z = true; break;
        case IDM_OP_PLANE_Y: y = false; z = x = true; break;
        case IDM_OP_PLANE_Z: z = false; x = y = true; break;
        }
    }

    virtual void OnConfirm() override{
        DebugLog("SizeOperation OnConfirm");
        sizeInfo.Clear();
    }

    virtual void OnUndo() override{
        DebugLog("SizeOperation OnUndo");

        switch (Main::data->selType){
        case SelectionType::SELECT_OBJECT:
            sizeInfo.Foreach([](SizeInfo info){
                info.obj->transform.scale.Set(info.vec);
            });
            break;
        case SelectionType::SELECT_VERTICES:
            sizeInfo.Foreach([](SizeInfo info){
                info.vert->SetWorldPos(info.vec);
            });
            break;
        case SelectionType::SELECT_EDGES:
            break;
        case SelectionType::SELECT_FACES:
            break;
        }
        
        sizeInfo.Clear();
    }
};

class MainWindow::EmptyTool : public ITool {
private:
    MainWindow* window;

public:
    EmptyTool(MainWindow* window) : window(window) {}
    ~EmptyTool() override{}

    virtual void OnLeftDown() override{
        if (Main::data->curObject){
            Main::data->curObject->OnSelect(window->camPos, window->cursorDir);
        }
    }
};

class MainWindow::SelectTool : public ITool {
private:
    MainWindow* window;
    Point2 start;
    Point2 end;
    bool leftDown;
    
public:
    SelectTool(MainWindow* window) : window(window) {}
    virtual ~SelectTool() override{}
    
    virtual void OnLeftDown() override{
        start = window->cursorPos;
        end = window->cursorPos;
        leftDown = true;
    }

    virtual void OnLeftUp() override{
        leftDown = false;
        if (start.x == end.x && start.y == end.y){
            Main::data->selPoints.Clear();
            return;
        }
        if (Main::data->curObject){
            SelectInfo info;
            info.camPos = window->camPos;
            info.camDir = window->camDir;
            info.zBound = Vector2(window->camDis * 0.02f, window->camDis * 20.0f);
            info.rect = Rect(Vector2(start.x * window->aspect, start.y), Vector2(end.x * window->aspect, end.y));
            
            Main::data->curObject->OnSelect(&info);
        }
    }

    virtual void OnMove() override{
        end = window->cursorPos;
    }

    virtual void OnRender() override{
        if (leftDown){
            glColor4f(1.0f, 1.0f, 0.0f, 0.1f);
            GLUtils::DrawRect(start, end);
        }
    }
};

class MainWindow::MoveButton : public RoundButton {
public:
    Point3 startPos;

    MoveButton() : RoundButton(Vector2(0.55f, 0.85f), 0.12f) {}
};

class MainWindow::RotateButton : public RoundButton {
public:
    Quaternion start;
    Vector3 up;
    Vector3 right;
    MainWindow* window;

    RotateButton(MainWindow* window) : RoundButton(Vector2(0.85f, 0.85f), 0.12f), window(window) {}

    virtual void OnRender() override{
        Quaternion q = -window->camDir;
        Vector2 right = q.GetXAxis().XZ() * radius;
        Vector2 forward = q.GetYAxis().XZ() * radius;
        Vector2 up = q.GetZAxis().XZ() * radius;
        float light = (hover ? 1.0f : 0.8f);

        glColor3f(0.1f, 0.1f, 0.1f);
        GLUtils::DrawCornerWithUV(center.x, center.y, 0.0f, 360.0f, radius, 0.05f);

        glEnable(GL_LINE_SMOOTH);
        glBegin(GL_LINES);
        glColor3f(light, 0.0f, 0.0f);
        glVertexv2(center - right); glVertexv2(center + right);
        glColor3f(0.0f, light, 0.0f);
        glVertexv2(center - forward); glVertexv2(center + forward);
        glColor3f(0.0f, 0.0f, light);
        glVertexv2(center - up); glVertexv2(center + up);
        glEnd();
        glDisable(GL_LINE_SMOOTH);
    }
};

WINDOW_INFO_DECL(simple3deditor::MainWindow, L"������");

MainWindow::MainWindow() : CCamera(Point3(0.0f, -5.0f, 1.0f), Point3(0.0f, 0.0f, 1.0f), Vector3::up, 5.0f) {
    DebugLog("MainWindow Launched");

    guiMgr = new GUIManagerObject();

    SetTool(new EmptyTool(this));

    basicMenu = new Menu();
    basicMenu->AddItem(new MenuItem(L"��Ӷ���", [=]{ this->AddPoint(); }));
    basicMenu->AddItem(new MenuItem(L"ɾ������", [=]{ this->DeletePoint(); }));
    basicMenu->AddItem(new MenuItem());
    basicMenu->AddItem(new MenuItem(L"����", [=]{ this->OnInsSave(); }));
    basicMenu->AddItem(new MenuItem(L"����", [=]{ this->OnInsLoad(); }));
    basicMenu->AddItem(new MenuItem());
    basicMenu->AddItem(new MenuItem(L"����", [=]{ DialogVersionInfo(); }));
    basicMenu->AddItem(new MenuItem());

    Menu* subMenu = new Menu();
    subMenu->AddItem(new MenuItem(L"����", [=]{ this->OnMenuAccel(IDM_MESH_BASIC_BLOCK, false); }));
    subMenu->AddItem(new MenuItem(L"ƽ��", [=]{ this->OnMenuAccel(IDM_MESH_BASIC_PLANE, false); }));
    subMenu->AddItem(new MenuItem(L"Բ����", [=]{ this->OnMenuAccel(IDM_MESH_BASIC_CYLINDER, false); }));
    subMenu->AddItem(new MenuItem(L"����", [=]{ this->OnMenuAccel(IDM_MESH_BASIC_SPHERE, false); }));
    subMenu->AddItem(new MenuItem(L"������", [=]{ this->OnMenuAccel(IDM_MESH_BASIC_CAPSULE, false); }));
    basicMenu->AddItem(new MenuItem(L"���", subMenu));

    Menu* toolMenu = new Menu();
    toolMenu->AddItem(new MenuItem(L"��ѡ", [=]{ this->OnMenuAccel(IDM_TOOL_EMPTY, false); }));
    toolMenu->AddItem(new MenuItem(L"��ѡ", [=]{ this->OnMenuAccel(IDM_TOOL_SELECTBOX, false); }));
    basicMenu->AddItem(new MenuItem(L"����", toolMenu));

    Menu* selTypeMenu = new Menu();
    selTypeMenu->AddItem(new MenuItem(L"����(����ʵ��)", [=]{ Main::SelectType(SelectionType::SELECT_OBJECT); }));
    selTypeMenu->AddItem(new MenuItem(L"����", [=]{ Main::SelectType(SelectionType::SELECT_VERTICES); }));
    selTypeMenu->AddItem(new MenuItem(L"��(����ʵ��)", [=]{ Main::SelectType(SelectionType::SELECT_EDGES); }));
    selTypeMenu->AddItem(new MenuItem(L"��(δʵ��)", [=]{ Main::SelectType(SelectionType::SELECT_FACES); }));
    basicMenu->AddItem(new MenuItem(L"ѡ������", selTypeMenu));

    Menu* texMenu = new Menu();
    texMenu->AddItem(new MenuItem(L"����", [=]{ this->OnMenuAccel(IDM_TEXTURE_ENABLE, false); }));
    texMenu->AddItem(new MenuItem(L"ͣ��", [=]{ this->OnMenuAccel(IDM_TEXTURE_DISABLE, false); }));
    texMenu->AddItem(new MenuItem(L"����", [=]{ this->OnMenuAccel(IDM_TEXTURE_LOAD, false); }));
    basicMenu->AddItem(new MenuItem(L"����", texMenu));

    Menu* objectMenu = new Menu();
    objectMenu->AddItem(new MenuItem(L"�ն���", [=]{ Main::AddObject(new AViewObject()); }));
    objectMenu->AddItem(new MenuItem(L"������", [=]{ Main::AddObject(new MeshObject()); }));
    objectMenu->AddItem(new MenuItem(L"���α���������", [=]{ Main::AddObject(new SquareBezierObject()); }));
    objectMenu->AddItem(new MenuItem(L"���α���������", [=]{ Main::AddObject(new CubicBezierObject()); }));
    objectMenu->AddItem(new MenuItem(L"���Դ", [=]{ Main::AddObject(new PointLightObject()); }));
    objectMenu->AddItem(new MenuItem(L"GUI������", [=]{ Main::AddObject(new GUIManagerObject()); }));
    objectMenu->AddItem(new MenuItem(L"GUI������", [=]{ Main::AddObject(new GUIMeshObject()); }));

    Menu* objGUIMenu = new Menu();
    objGUIMenu->AddItem(new MenuItem(L"ͼ�갴ť", [=]{ Main::AddObject(new IconButton()); }));
    objGUIMenu->AddItem(new MenuItem(L"�༭��", [=]{ Main::AddObject(new GUIEditA()); }));
    objGUIMenu->AddItem(new MenuItem(L"�������", [=]{ Main::AddObject(new HorizontalProgressBar()); }));
    objGUIMenu->AddItem(new MenuItem(L"��������", [=]{ Main::AddObject(new VerticalProgressBar()); }));
    objectMenu->AddItem(new MenuItem(L"GUI", objGUIMenu));

    objectMenu->AddItem(new MenuItem(L"��Ƶ������", [=]{ LocalData::GetLocalInst()->CreateAudioListener(); }));
    objectMenu->AddItem(new MenuItem(L"�����", [=]{ LocalData::GetLocalInst()->CreateCamera(); }));
    basicMenu->AddItem(new MenuItem(L"��Ӷ���", objectMenu));

    basicMenu->AddItem(new MenuItem());
    basicMenu->AddItem(new MenuItem(
        Value<const wchar_t*>([=]{ return this->lightEnabled ? L"����:��" : L"����:��"; }),
        [=]{
            this->lightEnabled = !this->lightEnabled;
            DebugLog("MainWindow Light State %s", this->lightEnabled ? "On" : "Off");
        }
    ));

    basicMenu->AddItem(new MenuItem());
    basicMenu->AddItem(new MenuItem(
        Value<const wchar_t*>([=]{ return this->audioControl ? L"��Ƶ����:��" : L"��Ƶ����:��"; }),
        [=]{
            this->audioControl = !this->audioControl;
            DebugLog("MainWindow Audio Control %s", this->audioControl ? "On" : "Off");
            if (this->audioControl){
                // �����ٶȲ���¼��ǰλ��
                alListenerPosAutoVelv3(this->camPos);
                alListenerVelocityv3(Vector3::zero);
                alListenerDirv3(this->camForward, this->camUp);
            }else{
                alListenerPosv3(Vector3::zero);
                alListenerVelocityv3(Vector3::zero);
                alListenerDirv3(Vector3::forward, Vector3::up);
            }
        }
    ));
    basicMenu->AddItem(new MenuItem(
        Value<const wchar_t*>([=]{ return this->dopplerEffect ? L"������ЧӦ:��" : L"������ЧӦ:��"; }),
        [=]{
            this->dopplerEffect = !this->dopplerEffect;
            DebugLog("MainWindow Doppler Effect %s", this->dopplerEffect ? "On" : "Off");
            if (this->dopplerEffect){
                // �����ٶȲ���¼��ǰλ��
                alListenerPosAutoVelv3(this->camPos);
                alListenerVelocityv3(Vector3::zero);
            }else{
                alListenerVelocityv3(Vector3::zero);
            }
        }
    ));

    insertMenu = new Menu();
    insertMenu->AddItem(new MenuItem(L"λ��", [=]{ Main::data->curObject->transform.InsertPos(Main::data->animFrame); }));
    insertMenu->AddItem(new MenuItem(L"��ת", [=]{ Main::data->curObject->transform.InsertRot(Main::data->animFrame); }));
    insertMenu->AddItem(new MenuItem(L"��С", [=]{ Main::data->curObject->transform.InsertScale(Main::data->animFrame); }));

    if (audioControl){
        alListenerDirv3(camForward, camUp);
    }else{
        alListenerDirv3(Vector3::forward, Vector3::up);
    }

    skyBox = new GLSkyBox();
    skyBox->Set(GLSkyBox::LEFT, new GLTexture2D(IDT_SKYBOX_LEFT));
    skyBox->Set(GLSkyBox::RIGHT, new GLTexture2D(IDT_SKYBOX_RIGHT));
    skyBox->Set(GLSkyBox::FRONT, new GLTexture2D(IDT_SKYBOX_FRONT));
    skyBox->Set(GLSkyBox::BACK, new GLTexture2D(IDT_SKYBOX_BACK));
    skyBox->Set(GLSkyBox::TOP, new GLTexture2D(IDT_SKYBOX_TOP));
    skyBox->Set(GLSkyBox::DOWN, new GLTexture2D(IDT_SKYBOX_DOWN));

    // ��������Ϊ���ų�ͻ�����MoveButton������ȫ�֣������Ƴ�ͻʱ���ܳ�����
    MoveButton* moveBtn = new MoveButton();
    moveBtn->SetIcon(IDT_BUTTON_MOVE);
    moveBtn->onClick += [=]{
        moveBtn->startPos = this->camLookat;
    };
    moveBtn->onDrag += [=](Vector2 dir){
        this->SetLookAt(moveBtn->startPos - (this->camRight * dir.x + this->camUp * dir.y) * this->camDis);
    };
    guiMgr->AddChild(moveBtn);

    RotateButton* rotBtn = new RotateButton(this);
    rotBtn->onClick += [=]{
        rotBtn->start = this->camDir;
        rotBtn->up = this->camUp;
        rotBtn->right = this->camRight;
    };
    rotBtn->onDrag += [=](Vector2 dir){
        this->SetRotation(Quaternion::AxisAngle(rotBtn->up, -dir.x * 100.0f) *
                            Quaternion::AxisAngle(rotBtn->right, dir.y * 100.0f) * rotBtn->start);
    };
    guiMgr->AddChild(rotBtn);
}

MainWindow::~MainWindow(){
    DebugLog("MainWindow Destroyed");
    if (guiMgr) delete guiMgr;
    if (basicMenu) delete basicMenu;
    if (insertMenu) delete insertMenu;
    if (curOp) delete curOp;
    if (skyBox) delete skyBox;
}

void MainWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);
    guiMgr->OnMouseMove2D(cursorPos);
}

void MainWindow::OnRightDown(int x, int y){
    UpdateCursor(x, y);
    // UI����
    guiMgr->OnRightDown2D(cursorPos);
    if (guiMgr->GetCurrent())
        return;
    // ����
    if (curOp){
        curOp->OnUndo();
        delete curOp;
        curOp = NULL;
        return;
    }
    // ����
    if (curTool)
        curTool->OnRightDown();
    Main::SetMenu(basicMenu);
    // GUI����
    Main::OnRightDown(camPos, cursorDir);
}

void MainWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
    // UI����
    guiMgr->OnRightUp2D(cursorPos);
    if (guiMgr->GetCurrent())
        return;
    // ����
    if (curTool)
        curTool->OnRightUp();
    // GUI����
    Main::OnRightUp(camPos, cursorDir);
}

void MainWindow::InitCamera(){
    CCamera::InitCamera(aspect);
}

void MainWindow::RenderModelView(){
    // ��Щ�ݶ���Ĭ������
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
    glDepthFunc(GL_LEQUAL);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    InitCamera();

    glDisable(GL_DEPTH_TEST);
    skyBox->Render(camPos, camDis);
    glEnable(GL_DEPTH_TEST);

    // xyz������
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

    //TODO �����������÷���
    Main::RenderScene();

    glDepthFunc(GL_LESS);

    // ��ѡ������
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

    // ��ѡ��߻���
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

void MainWindow::AddMeshPlane(){
    Mesh* mesh = Main::GetMesh();
    if (!mesh)
        return;
    Vertex* v1 = new Vertex(Vector3(-1.0f, -1.0f, 0.0f) + camLookat);
    Vertex* v2 = new Vertex(Vector3( 1.0f, -1.0f, 0.0f) + camLookat);
    Vertex* v3 = new Vertex(Vector3(-1.0f,  1.0f, 0.0f) + camLookat);
    Vertex* v4 = new Vertex(Vector3( 1.0f,  1.0f, 0.0f) + camLookat);
    v1->uv = Vector2(0.0f, 0.0f);
    v2->uv = Vector2(1.0f, 0.0f);
    v3->uv = Vector2(0.0f, 1.0f);
    v4->uv = Vector2(1.0f, 1.0f);
    mesh->AddVertex(v1);
    mesh->AddVertex(v2);
    mesh->AddVertex(v3);
    mesh->AddVertex(v4);
    mesh->AddTriFace(v1, v2, v4);
    mesh->AddTriFace(v1, v3, v4);
}

void MainWindow::AddMeshBox(){
    Mesh* mesh = Main::GetMesh();
    if (!mesh)
        return;
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
    // XY�Խ�
    mesh->AddTriFace(v1, v2, v4);
    mesh->AddTriFace(v1, v3, v4);
    mesh->AddTriFace(v5, v6, v8);
    mesh->AddTriFace(v5, v7, v8);
    // XZ�Խ�
    mesh->AddTriFace(v1, v2, v6);
    mesh->AddTriFace(v1, v5, v6);
    mesh->AddTriFace(v3, v4, v8);
    mesh->AddTriFace(v3, v7, v8);
    // YZ�Խ�
    mesh->AddTriFace(v1, v3, v7);
    mesh->AddTriFace(v1, v5, v7);
    mesh->AddTriFace(v2, v4, v8);
    mesh->AddTriFace(v2, v6, v8);
}

void MainWindow::AddMeshSphere(int loops, int round){
    Mesh* mesh = Main::GetMesh();
    if (!mesh)
        return;
    Vertex** vert = new Vertex*[loops * round];
    Vertex* ceil;
    Vertex* floor;
    float angle = (2.0f * PI / round);
    ceil = mesh->AddVertex(Vector3(0.0f, 0.0f, 1.0f) + camLookat);
    floor = mesh->AddVertex(Vector3(0.0f, 0.0f, -1.0f) + camLookat);
    for (int i = 0; i < loops; i++){
        int off = i * round;
        for (int j = 0; j < round; j++){
            float height, radius;
            float vsin, vcos;
            SinCos(PI * (i + 1) / (loops + 1) - 0.5f * PI, &height, &radius);
            SinCos(j * angle, &vsin, &vcos);
            vert[off + j] = mesh->AddVertex(Vector3(vcos * radius, vsin * radius, height) + camLookat);
        }
    }
    for (int i = 0; i < loops - 1; i++){
        for (int j = 0; j < round; j++){
            int k = (j == round - 1 ? 0 : j + 1);
            mesh->AddTriFace(vert[i * round + j], vert[i * round + k], vert[(i + 1) * round + k]);
            mesh->AddTriFace(vert[i * round + j], vert[(i + 1) * round + j], vert[(i + 1) * round + k]);
        }
    }
    for (int i = 0, off = (loops - 1) * round; i < round; i++){
        int j = (i == round - 1 ? 0 : i + 1);
        mesh->AddTriFace(floor, vert[i], vert[j]);
        mesh->AddTriFace(ceil, vert[off + i], vert[off + j]);
    }
    delete[] vert;
}

void MainWindow::AddMeshCylinder(int loops, int round){
    Mesh* mesh = Main::GetMesh();
    if (!mesh)
        return;
    Vertex** vert = new Vertex*[loops * round];
    Vertex* ceil;
    Vertex* floor;
    float angle = (2.0f * PI / round);
    ceil = mesh->AddVertex(Vector3(0.0f, 0.0f, 1.0f) + camLookat);
    floor = mesh->AddVertex(Vector3(0.0f, 0.0f, -1.0f) + camLookat);
    for (int i = 0; i < loops; i++){
        for (int j = 0; j < round; j++){
            float vsin, vcos;
            SinCos(j * angle, &vsin, &vcos);
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
    for (int i = 0, off = (loops - 1) * round; i < round; i++){
        int j = (i == round - 1 ? 0 : i + 1);
        mesh->AddTriFace(floor, vert[i], vert[j]);
        mesh->AddTriFace(ceil, vert[off + i], vert[off + j]);
    }
    delete[] vert;
}

void MainWindow::AddMeshCapsule(int ballLoops, int cylinderLoops, int round){
    Mesh* mesh = Main::GetMesh();
    if (!mesh)
        return;
    const int loops = 2 * ballLoops + cylinderLoops;
    Vertex** vert = new Vertex*[loops * round];
    Vertex* ceil;
    Vertex* floor;
    float angle = (2.0f * PI / round);
    ceil = mesh->AddVertex(Vector3(0.0f, 0.0f, 2.0f) + camLookat);
    floor = mesh->AddVertex(Vector3(0.0f, 0.0f, -2.0f) + camLookat);
    for (int i = 0; i < loops; i++){
        if (i >= ballLoops && i < loops - ballLoops){
            int loopCnt = i - ballLoops;
            for (int j = 0; j < round; j++){
                float vsin, vcos;
                SinCos(j * angle, &vsin, &vcos);
                vert[i * round + j] = mesh->AddVertex(Vector3(vcos, vsin, 2.0f * loopCnt / (cylinderLoops - 1) - 1.0f) + camLookat);
            }
        }else if (i < ballLoops){
            for (int j = 0; j < round; j++){
                float height, radius;
                float vsin, vcos;
                SinCos(-0.5f * PI * (ballLoops - i - 1) / ballLoops, &height, &radius);
                SinCos(2.0f * PI * j / round, &vsin, &vcos);
                vert[i * round + j] = mesh->AddVertex(Vector3(vcos * radius, vsin * radius, height - 1.0f) + camLookat);
            }
        }else{
            for (int j = 0; j < round; j++){
                float height, radius;
                float vsin, vcos;
                SinCos(0.5f * PI * (i - ballLoops - cylinderLoops) / ballLoops, &height, &radius);
                SinCos(j * angle, &vsin, &vcos);
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
    for (int i = 0, off = (loops - 1) * round; i < round; i++){
        int j = (i == round - 1 ? 0 : i + 1);
        mesh->AddTriFace(floor, vert[i], vert[j]);
        mesh->AddTriFace(ceil, vert[off + i], vert[off + j]);
    }
    delete[] vert;
}

void MainWindow::OnGraphicsRenderCallback(const ImDrawList* list, const ImDrawCmd* cmd){
    static_cast<MainWindow*>(cmd->UserCallbackData)->OnGraphicsRender(list, cmd);
}

void MainWindow::OnGraphicsRender(const ImDrawList* list, const ImDrawCmd* cmd){
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // RenderOptions* options = &LocalData::GetLocalInst()->renderOptions;
    // options->editor = true;
    // options->vertex = true;
    // options->edge = true;
    // options->face = true;
    // options->light = lightEnabled;
    // options->objOp = objOp;

    // RenderModelView();

    // glEnable(GL_BLEND);
    // glEnable(GL_DEPTH_TEST);
    // glDisable(GL_LIGHTING);
    // glEnable(GL_ALPHA_TEST);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // GLUtils::ResetProjection();
    // GLUtils::ResetModelView();
    
    // // ��Ļ����
    // Main::RenderScreen();

    // glDisable(GL_DEPTH_TEST);
    // // ���߻���
    // if (curTool)
    //     curTool->OnRender();

    // // UI����
    // // ��֮ǰ����3D��Ⱦʹ��ͶӰ�任����Ҫ����aspect
    // guiMgr->OnChainRender();

    // if (audioControl){
    //     if (dopplerEffect){
    //         alListenerPosAutoVelv3(camPos);
    //     }else{
    //         alListenerPosv3(camPos);
    //     }
    // }
}

void MainWindow::OnRender(){
    ImGuiIO& io = ImGui::GetIO();

    if (!ImGui::Begin("MainWindow", &window_open, ImGuiWindowFlags_MenuBar)){
        ImGui::End();
        return;
    }

    ImDrawList* list = ImGui::GetWindowDrawList();
    list->PushClipRect(
        ImGui::GetCursorScreenPos(),
        ImGui::GetCursorScreenPos() + ImGui::GetContentRegionAvail()
    );
    list->AddCallback(OnGraphicsRenderCallback, this);
    list->PopClipRect();

    if (ImGui::BeginMenuBar()){
        if (ImGui::BeginMenu("Tools")){
            if (ImGui::MenuItem("Tap Click", "Alt+1"))
                SetTool(new EmptyTool(this));
            if (ImGui::MenuItem("Box Select", "Alt+2"))
                SetTool(new SelectTool(this));
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Mesh")){
            if (ImGui::MenuItem("Plane"))
                AddMeshPlane();
            if (ImGui::MenuItem("Box"))
                AddMeshBox();
            if (ImGui::MenuItem("Sphere"))
                AddMeshSphere(10, 10);
            if (ImGui::MenuItem("Cylinder"))
                AddMeshCylinder(2, 30);
            if (ImGui::MenuItem("Capsule"))
                AddMeshCapsule(5, 10, 10);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Right)){
        Main::SetMenu(basicMenu);
    }
    ImGui::End();
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

void MainWindow::UpdateCursor(int x, int y){
    AWindow::UpdateCursor(x, y);
    cursorDir = camForward + camRight * cursorPos.x * aspect + camUp * cursorPos.y;
    if (curOp)
        curOp->OnMove();
    if (curTool)
        curTool->OnMove();
    // GUI���Դ���
    Main::OnMouseMove(camPos, cursorDir);
}

void MainWindow::SetRotation(Quaternion rot){
    CCamera::SetRotation(rot);
    cursorDir = camForward + camRight * cursorPos.x * aspect + camUp * cursorPos.y;
    if (audioControl)
        alListenerDirv3(camForward, camUp);
}

void MainWindow::SetDistance(float dis){
    CCamera::SetDistance(dis);
    DebugLog("Camera Distance %f", camDis);
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

    mesh = Main::GetMesh(obj);

    if (mesh){
        pos = camLookat + (camUp * cursorPos.y + camRight * cursorPos.x * aspect) * camDis;
        mesh->AddVertex(obj->transform.chainInvMat * pos);
        DebugLog("Point at %f %f %f", pos.x, pos.y, pos.z);
    }
}

void MainWindow::DeletePoint(){
    Mesh* mesh = Main::GetMesh();
    if (!mesh)
        return;
    Main::data->selPoints.Foreach([=](Vertex* v){
        mesh->DeleteVertex(v);
    });
    Main::data->selPoints.Clear();
}

bool MainWindow::SaveMesh(Mesh* mesh){
    if (!mesh)
        return false;
    static const WString filter = Resource::GetWString(IDS_OBJFILE_FILTER);
    WString file = ShellFileSelectWindow(filter, FILESELECT_REQ_PATH, true);
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
    if (obj->GetType() != ViewObjectType::OBJECT_MESH)
        return false;
    static const WString filter = Resource::GetWString(IDS_OBJFILE_FILTER);
    WString file = ShellFileSelectWindow(filter, FILESELECT_REQ_FILE | FILESELECT_REQ_PATH);
    if (file.GetLength() == 0){
        DebugLog("Stop Loading");
        return false;
    }
    return LoadMesh(obj, file);
}

bool MainWindow::LoadMesh(AViewObject* obj, WString path){
    char* fileData;
    size_t fileLen, filePtr = 0;
    List<Vertex*> vert;
    List<Vector2> vertUV;
    List<Vector3> vertNormal;
    Mesh* mesh;

    if (obj->GetType() != ViewObjectType::OBJECT_MESH)
        return false;
    mesh = ((MeshObject*)obj)->GetMesh();
    
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
            }else if (sscanf_s(fileData + filePtr, "v %f %f %f", &vec.x, &vec.y, &vec.z) == 3){
                vert.Add(mesh->AddVertex(vec));
            }else if (sscanf_s(fileData + filePtr, "vt %f %f", &vec.x, &vec.y) == 2){
                vertUV.Add(Vector2(vec.x, vec.y));
            }else if (sscanf_s(fileData + filePtr, "vn %f %f %f", &vec.x, &vec.y, &vec.z) == 3){
                vertNormal.Add(vec);
            }else if (sscanf_s(fileData + filePtr, "f %d/%d %d/%d %d/%d", &v1, &vt1, &v2, &vt2, &v3, &vt3) == 6){
                mesh->AddTriFace(vert[v1 - 1], vert[v2 - 1], vert[v3 - 1]);
                vert[v1 - 1]->uv = vertUV[vt1 - 1];
                vert[v2 - 1]->uv = vertUV[vt2 - 1];
                vert[v3 - 1]->uv = vertUV[vt3 - 1];
            }else if (sscanf_s(fileData + filePtr, "f %d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3) == 6){
                mesh->AddTriFace(vert[v1 - 1], vert[v2 - 1], vert[v3 - 1]);
                vert[v1 - 1]->normal = vertNormal[vn1 - 1];
                vert[v2 - 1]->normal = vertNormal[vn2 - 1];
                vert[v3 - 1]->normal = vertNormal[vn3 - 1];
            }else if (sscanf_s(fileData + filePtr, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3) == 9){
                mesh->AddTriFace(vert[v1 - 1], vert[v2 - 1], vert[v3 - 1]);
                vert[v1 - 1]->uv = vertUV[vt1 - 1];
                vert[v2 - 1]->uv = vertUV[vt2 - 1];
                vert[v3 - 1]->uv = vertUV[vt3 - 1];
                vert[v1 - 1]->normal = vertNormal[vn1 - 1];
                vert[v2 - 1]->normal = vertNormal[vn2 - 1];
                vert[v3 - 1]->normal = vertNormal[vn3 - 1];
            }else if (sscanf_s(fileData + filePtr, "f %d %d %d", &v1, &v2, &v3) == 3){
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
    Main::data->selPoints.Foreach([=](Vertex* v){
        v->color = color;
    });
}

void MainWindow::OnLeftDown(int x, int y){
    UpdateCursor(x, y);
    // UI����
    guiMgr->OnLeftDown2D(cursorPos);
    if (guiMgr->GetCurrent())
        return;
    // ����
    if (curOp){
        curOp->OnConfirm();
        delete curOp;
        curOp = NULL;
        return;
    }
    // ����
    if (curTool){
        curTool->OnLeftDown();
    }
    // GUI����
    Main::OnLeftDown(camPos, cursorDir);
}

void MainWindow::OnLeftUp(int x, int y){
    UpdateCursor(x, y);
    // UI����
    guiMgr->OnLeftUp2D(cursorPos);
    if (guiMgr->GetCurrent())
        return;
    // ����
    if (curTool){
        curTool->OnLeftUp();
    }
    // GUI���Դ���
    Main::OnLeftUp(camPos, cursorDir);
}

void MainWindow::OnChar(char c){
    guiMgr->OnChar(c);
}

void MainWindow::OnUnichar(wchar_t c){
    guiMgr->OnUnichar(c);
}

void MainWindow::OnResize(int x, int y){
    AWindow::OnResize(x, y);
}

void MainWindow::OnMouseWheel(int delta){
    SetDistance(camDis * Pow(0.999f, (float)delta));
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
        static const WString filter = Resource::GetWString(IDS_PICFILE_FILTER);
        WString file = ShellFileSelectWindow(filter, FILESELECT_REQ_FILE | FILESELECT_REQ_PATH);
        if (file.GetLength() == 0){
            DebugLog("Stop Loading");
            break;
        }
        DebugLog("Load Texture %S", file.GetString());
        mesh->SetTexture(new GLTexture2D(file.GetString()));
    }
    }
    // ��ǰ����������
    if (curOp)
        curOp->OnCommand(id);
    if (curTool)
        curTool->OnCommand(id);
    if (Main::data->curObject)
        Main::data->curObject->OnMenuAccel(id, accel);
}

void MainWindow::OnDropFileW(const wchar_t* path, uint len){
    wchar_t* suffix = wcsrchr(const_cast<wchar_t*>(path), L'.');
    if (wcscmp(suffix, L".obj")){
        static const WString message = Resource::GetWString(IDS_OBJFILE_FORM_WARNING);
        static const WString caption = Resource::GetWString(IDS_OBJFILE_FORM_WARNING_CAPTION);
        if (ShellMsgBox(caption, message) != MSGBOX_YES){
            DebugLog("MainWindow::OnDropFileW Stop Load File");
            return;
        }
    }
    LoadMesh(Main::data->curObject, path);
}

void MainWindow::Serialize(nlohmann::json& o){
    o["id"] = WINDOW_ID;
}

void MainWindow::Deserialize(nlohmann::json& o){}

Point2 MainWindow::GetScreenPosition(Point3 pos){
    return CCamera::GetScreenPosition(pos, aspect);
}

}