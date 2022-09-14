#include <editor/windows/TreeWindowObject.h>

#include <main.h>
#include <res.h>
#include <editor/gui/Menu.h>
#include <editor/gui/GUIUtils.h>
#include <editor/dialog/Tips.h>
#include <util/os/Font.h>
#include <util/gl/GLUtils.h>
#include <util/gl/GLSimplified.h>
#include <editor/main/ViewObject.h>
#include <editor/object/AllObjects.h>
#include <editor/windows/AllWindows.h>

namespace simple3deditor {

OBJECT_INFO_DECL(simple3deditor::TreeWindowObject, L"树形图窗口对象");

TreeWindowObject::TreeWindowObject() : AWindowObject(L"TreeWindow") {
    DebugLog("TreeWindowObject Launched");

    basicMenu = new Menu();

    Menu* objectMenu = new Menu();
    objectMenu->AddItem(new MenuItem(L"空对象", [=]{ this->AddObject(new AViewObject()); }));
    objectMenu->AddItem(new MenuItem(L"网格体", [=]{ this->AddObject(new MeshObject()); }));
    objectMenu->AddItem(new MenuItem(L"三次贝塞尔曲线", [=]{ this->AddObject(new CubicBezierObject()); }));
    objectMenu->AddItem(new MenuItem(L"点光源", [=]{ this->AddObject(new PointLightObject()); }));
    objectMenu->AddItem(new MenuItem(L"GUI管理器", [=]{ this->AddObject(new GUIManagerObject()); }));
    objectMenu->AddItem(new MenuItem(L"GUI网格体", [=]{ this->AddObject(new GUIMeshObject()); }));

    Menu* objGUIMenu = new Menu();
    objGUIMenu->AddItem(new MenuItem(L"图标按钮", [=]{ this->AddObject(new IconButton()); }));
    objGUIMenu->AddItem(new MenuItem(L"编辑框", [=]{ this->AddObject(new GUIEditA()); }));
    objGUIMenu->AddItem(new MenuItem(L"横进度条", [=]{ this->AddObject(new HorizontalProgressBar()); }));
    objGUIMenu->AddItem(new MenuItem(L"竖进度条", [=]{ this->AddObject(new VerticalProgressBar()); }));
    objectMenu->AddItem(new MenuItem(L"GUI", objGUIMenu));

    Menu* windowsMenu = new Menu();
    windowsMenu->AddItem(new MenuItem(L"音频播放器", [=]{ this->AddObject(new AudioPlayerWindowObject()); }));
    windowsMenu->AddItem(new MenuItem(L"树形图", [=]{ this->AddObject(new TreeWindowObject()); }));
    objectMenu->AddItem(new MenuItem(L"窗口", windowsMenu));

    objectMenu->AddItem(new MenuItem(L"音频收听者", [=]{ LocalData::GetLocalInst()->CreateAudioListener(); }));
    objectMenu->AddItem(new MenuItem(L"摄像机", [=]{ LocalData::GetLocalInst()->CreateCamera(); }));
    basicMenu->AddItem(new MenuItem(L"添加对象", objectMenu));

    basicMenu->AddItem(new MenuItem(L"删除对象", [=]{
        if (!this->selObject){
            DebugError("No Object Selected");
            return;
        }
        Main::DeleteObject(this->selObject);
    }));
    basicMenu->AddItem(new MenuItem(L"重命名", [=]{
        wchar_t name[DEFAULT_STRING_LENGTH];

        if (!this->selObject){
            DebugError("No Object Selected");
            return;
        }
        DialogTextInput(name, DEFAULT_STRING_LENGTH);
        if (*name == L'\0'){
            DebugLog("Rename Operation Cancelled");
            return;
        }
        this->selObject->name = name;
    }));
}

TreeWindowObject::~TreeWindowObject(){
    DebugLog("TreeWindowObject Destroyed");
    if (basicMenu) delete basicMenu;
}

void TreeWindowObject::AddObject(AViewObject* o){
    if (selObject){
        selObject->AddChild(o);
        return;
    }
    Main::AddObject(o);
}

bool TreeWindowObject::OnHit2D(Point2 pos){
    return pos.x >= -1.0f && pos.x <= 1.0f &&
            pos.y >= -1.0f && pos.y <= 1.0f;
}

void TreeWindowObject::OnRender(){
    glColor4f(0.2f, 0.2f, 0.2f, 0.5f);
    GLUtils::DrawRect(-1.0f, -1.0f, 1.0f, 1.0f);

    objectList.Clear();

    depth = 0;
    RenderItem(Main::data->scene);
    RenderItem(Main::data->screen);
}

void TreeWindowObject::RenderItem(AViewObject* o){
    float xbase = -1.0f + 30.0f * (depth + 1) * cliInvSize.x;
    float ybase = 1.0f - 30.0f * objectList.Size() * cliInvSize.y;

    if (Main::data->curObject == o){
        glColor3f(0.0f, 0.0f, 1.0f);
        GLUtils::DrawRect(Vector2(-1.0f, ybase), Vector2(1.0f, ybase - 30.0f * cliInvSize.y));
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(xbase, ybase - 24.0f * cliInvSize.y);
    glDrawCNString(o->name.GetString());

    if (o->GetChildren().Size() > 0){
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLES);
        if (o->unfold){
            glVertex2f(xbase - 6.0f * cliInvSize.x, ybase - 10.0f * cliInvSize.y);
            glVertex2f(xbase - 15.0f * cliInvSize.x, ybase - 20.0f * cliInvSize.y);
            glVertex2f(xbase - 24.0f * cliInvSize.x, ybase - 10.0f * cliInvSize.y);
        }else{
            glVertex2f(xbase - 20.0f * cliInvSize.x, ybase - 6.0f * cliInvSize.y);
            glVertex2f(xbase - 10.0f * cliInvSize.x, ybase - 15.0f * cliInvSize.y);
            glVertex2f(xbase - 20.0f * cliInvSize.x, ybase - 24.0f * cliInvSize.y);
        }
        glEnd();
    }
    
    objectList.Add(o);

    if (o->unfold){
        depth++;
        o->GetChildren().Foreach([=](AViewObject* o){
            this->RenderItem(o);
        });
        depth--;
    }
}

void TreeWindowObject::OnChar(char c){}

void TreeWindowObject::OnUnichar(wchar_t c){}

void TreeWindowObject::OnResize(Vector2 size){
    UpdateWindowSize(size);
}

void TreeWindowObject::UpdateCursor(Point2 pos){
    AWindowObject::UpdateCursor(pos);
}

void TreeWindowObject::UpdateWindowSize(Vector2 size){
    AWindowObject::UpdateWindowSize(size);
}

void TreeWindowObject::OnMouseMove2D(Point2 pos){
    UpdateCursor(pos);
    if (dragObject)
        Main::SetCursor(IDC_UPARROW);
}

void TreeWindowObject::OnLeftDown2D(Point2 pos){
    size_t selected;

    UpdateCursor(pos);

    selected = (size_t)((1.0f - cursorPos.y) / (30.0f * cliInvSize.y));
    dragObject = (selected < objectList.Size() ? objectList[selected] : NULL);
}

void TreeWindowObject::OnLeftUp2D(Point2 pos){
    size_t selected;

    UpdateCursor(pos);

    if (!dragObject)
        return;

    selected = (size_t)((1.0f - cursorPos.y) / (30.0f * cliInvSize.y));
    if (selected < objectList.Size()){
        AViewObject* o = objectList[selected];
        if (o == dragObject){
            if (Main::data->curObject != o){
                DebugLog("TreeWindowObject Select Object %S", o->name.GetString());
                Main::SelectObject(o);
                dragObject = NULL;
                return;
            }
            o->unfold = !o->unfold;
            DebugLog("TreeWindowObject %s Object %S", o->unfold ? "Unfold" : "Fold", o->name.GetString());
        }else{
            if (!o || !Main::AddObjectChild(o, dragObject)){
                dragObject = NULL;
                return;
            }
            DebugLog("TreeWindowObject Set %S As Child Of %S", dragObject->name.GetString(), o->name.GetString());
        }
    }
    dragObject = NULL;
}

void TreeWindowObject::OnRightDown2D(Point2 pos){
    size_t selPos;

    UpdateCursor(pos);
    
    selPos = (size_t)((1.0f - cursorPos.y) / (30.0f * cliInvSize.y));
    selObject = (selPos < objectList.Size() ? objectList[selPos] : NULL);
    Main::SetMenu(basicMenu);
}

void TreeWindowObject::OnRightUp2D(Point2 pos){
    UpdateCursor(pos);
}

void TreeWindowObject::OnMouseWheel(int delta){}

void TreeWindowObject::OnMenuAccel(int id, bool accel){
    switch (id){
    case IDM_DELETE:
        if (Main::data->curObject)
            Main::DeleteObject(Main::data->curObject);
        break;
    case IDM_MENU_BASIC:
        selObject = Main::data->curObject;
        Main::SetMenu(basicMenu);
        break;
    }
}

}