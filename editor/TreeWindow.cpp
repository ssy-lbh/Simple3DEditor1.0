#include <editor/TreeWindow.h>

#include <lib/opengl/gl/gl.h>

#include <main.h>
#include <res.h>
#include <editor/gui/Menu.h>
#include <editor/dialog/Tips.h>
#include <utils/os/Font.h>
#include <utils/gl/GLUtils.h>
#include <utils/math3d/ViewObject.h>

TreeWindow::TreeWindow(){
    DebugLog("TreeWindow Launched");

    basicMenu = new Menu();

    Menu* objectMenu = new Menu();
    objectMenu->AddItem(new MenuItem(L"空对象", MENUITEM_LAMBDA_TRANS(TreeWindow)[](TreeWindow* window){
        window->AddObject(new AViewObject());
    }, this));
    objectMenu->AddItem(new MenuItem(L"网格体", MENUITEM_LAMBDA_TRANS(TreeWindow)[](TreeWindow* window){
        window->AddObject(new MeshObject());
    }, this));
    objectMenu->AddItem(new MenuItem(L"三次贝塞尔曲线", MENUITEM_LAMBDA_TRANS(TreeWindow)[](TreeWindow* window){
        window->AddObject(new BezierCurveObject());
    }, this));
    objectMenu->AddItem(new MenuItem(L"点光源", MENUITEM_LAMBDA_TRANS(TreeWindow)[](TreeWindow* window){
        window->AddObject(new PointLightObject());
    }, this));
    objectMenu->AddItem(new MenuItem(L"音频收听者", MENUITEM_LAMBDA_TRANS(TreeWindow)[](TreeWindow* window){
        LocalData::GetLocalInst()->CreateAudioListener();
    }, this));
    objectMenu->AddItem(new MenuItem(L"摄像机", MENUITEM_LAMBDA_TRANS(TreeWindow)[](TreeWindow* window){
        LocalData::GetLocalInst()->CreateCamera();
    }, this));
    basicMenu->AddItem(new MenuItem(L"添加对象", objectMenu));

    basicMenu->AddItem(new MenuItem(L"删除对象", MENUITEM_LAMBDA_TRANS(TreeWindow)[](TreeWindow* window){
        if (!window->selObject){
            DebugError("No Object Selected");
            return;
        }
        Main::DeleteObject(window->selObject);
    }, this));
    basicMenu->AddItem(new MenuItem(L"重命名", MENUITEM_LAMBDA_TRANS(TreeWindow)[](TreeWindow* window){
        wchar_t name[DEFAULT_STRING_LENGTH];

        if (!window->selObject){
            DebugError("No Object Selected");
            return;
        }
        DialogTextInput(name, DEFAULT_STRING_LENGTH);
        if (*name == L'\0'){
            DebugLog("Rename Operation Cancelled");
            return;
        }
        window->selObject->name = name;
    }, this));
}

TreeWindow::~TreeWindow(){
    DebugLog("TreeWindow Destroyed");
    if (basicMenu) delete basicMenu;
}

void TreeWindow::AddObject(AViewObject* o){
    if (selObject){
        selObject->AddChild(o);
        return;
    }
    Main::AddObject(o);
}

bool TreeWindow::IsFocus(){
    return focus;
}

void TreeWindow::OnRender(){
    glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_AUTO_NORMAL);
    glEnable(GL_BLEND);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_ALPHA_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    objectList.Clear();

    depth = 0;
    RenderItem(Main::data->scene);
}

void TreeWindow::RenderItem(AViewObject* o){
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
        o->GetChildren().Foreach<TreeWindow*>([](AViewObject* o, TreeWindow* window){
            window->RenderItem(o);
        }, this);
        depth--;
    }
}

void TreeWindow::OnClose(){}

void TreeWindow::OnChar(char c){}

void TreeWindow::OnUnichar(wchar_t c){}

void TreeWindow::OnResize(int x, int y){
    UpdateWindowSize(x, y);
}

void TreeWindow::UpdateCursor(int x, int y){
    cursorPos.x = 2.0f * x / cliSize.x - 1.0f;
    cursorPos.y = 2.0f * y / cliSize.y - 1.0f;
}

void TreeWindow::UpdateWindowSize(int x, int y){
    cliSize.x = x;
    cliSize.y = y;
    cliInvSize.x = 1.0f / cliSize.x;
    cliInvSize.y = 1.0f / cliSize.y;
}

void TreeWindow::OnMouseMove(int x, int y){
    UpdateCursor(x, y);

    if (dragObject)
        Main::SetCursor(IDC_UPARROW);
}

void TreeWindow::OnLeftDown(int x, int y){
    size_t selected;

    UpdateCursor(x, y);

    selected = (size_t)((1.0f - cursorPos.y) / (30.0f * cliInvSize.y));
    dragObject = (selected < objectList.Size() ? objectList[selected] : NULL);
}

void TreeWindow::OnLeftUp(int x, int y){
    size_t selected;

    UpdateCursor(x, y);

    if (!dragObject)
        return;

    selected = (size_t)((1.0f - cursorPos.y) / (30.0f * cliInvSize.y));
    if (selected < objectList.Size()){
        AViewObject* o = objectList[selected];
        if (o == dragObject){
            if (Main::data->curObject != o){
                DebugLog("TreeWindow Select Object %S", o->name.GetString());
                Main::SelectObject(o);
                dragObject = NULL;
                return;
            }
            o->unfold = !o->unfold;
            DebugLog("TreeWindow %s Object %S", o->unfold ? "Unfold" : "Fold", o->name.GetString());
        }else{
            if (!o || o->HasAncestor(dragObject)){
                dragObject = NULL;
                return;
            }
            o->AddChild(dragObject);
            DebugLog("TreeWindow Set %S As Child Of %S", dragObject->name.GetString(), o->name.GetString());
        }
    }
    dragObject = NULL;
}

void TreeWindow::OnRightDown(int x, int y){
    size_t selPos;

    UpdateCursor(x, y);
    
    selPos = (size_t)((1.0f - cursorPos.y) / (30.0f * cliInvSize.y));
    selObject = (selPos < objectList.Size() ? objectList[selPos] : NULL);
    Main::SetMenu(basicMenu);
}

void TreeWindow::OnRightUp(int x, int y){
    UpdateCursor(x, y);
}

void TreeWindow::OnMouseHover(int key, int x, int y){}

void TreeWindow::OnMouseLeave(){}

void TreeWindow::OnFocus(){
    focus = true;
}

void TreeWindow::OnKillFocus(){
    focus = false;
}

void TreeWindow::OnMouseWheel(int delta){}

void TreeWindow::OnMenuAccel(int id, bool accel){
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