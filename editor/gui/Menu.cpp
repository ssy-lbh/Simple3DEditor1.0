#include <editor/gui/Menu.h>

#include <lib/opengl/gl/gl.h>

#include <utils/os/Font.h>
#include <utils/os/Log.h>
#include <utils/gl/GLUtils.h>

IMenuItem::IMenuItem(){}
IMenuItem::~IMenuItem(){}

IMenuItem::ItemType IMenuItem::GetType(){ return ItemType::DEFAULT; }
const wchar_t* IMenuItem::GetName(){ return L""; }
Menu* IMenuItem::GetMenu(){ return NULL; }

void IMenuItem::OnClick(){}

MenuItem::MenuItem() : type(ItemType::SEPERATOR) {}
MenuItem::MenuItem(const wchar_t* name) : type(ItemType::DEFAULT), name(name) {}
MenuItem::MenuItem(const wchar_t* name, void(*click)(void*)) : type(ItemType::DEFAULT), name(name), click(click) {}
MenuItem::MenuItem(const wchar_t* name, void(*click)(void*), void* userData)
    : type(ItemType::DEFAULT), name(name), click(click), userData(userData) {}
MenuItem::MenuItem(const wchar_t* name, Menu* menu) : type(ItemType::GROUP), name(name), menu(menu) {}

MenuItem::~MenuItem(){
    if (menu) delete menu;
}

IMenuItem::ItemType MenuItem::GetType(){
    return type;
}

const wchar_t* MenuItem::GetName(){
    return name;
}

Menu* MenuItem::GetMenu(){
    return menu;
}

void MenuItem::OnClick(){
    if (click)
        click(userData);
}

SwitchMenuItem::SwitchMenuItem(const wchar_t* nameOn, const wchar_t* nameOff)
    : nameOn(nameOn), nameOff(nameOff) {}
SwitchMenuItem::SwitchMenuItem(const wchar_t* nameOn, const wchar_t* nameOff, bool state)
    : nameOn(nameOn), nameOff(nameOff), state(state) {}
SwitchMenuItem::SwitchMenuItem(const wchar_t* nameOn, const wchar_t* nameOff, void(*click)(bool, void*))
    : nameOn(nameOn), nameOff(nameOff), click(click) {}
SwitchMenuItem::SwitchMenuItem(const wchar_t* nameOn, const wchar_t* nameOff, void(*click)(bool, void*), bool state)
    : nameOn(nameOn), nameOff(nameOff), click(click), state(state) {}
SwitchMenuItem::SwitchMenuItem(const wchar_t* nameOn, const wchar_t* nameOff, void(*click)(bool, void*), void* userData)
    : nameOn(nameOn), nameOff(nameOff), click(click), userData(userData) {}
SwitchMenuItem::SwitchMenuItem(const wchar_t* nameOn, const wchar_t* nameOff, void(*click)(bool, void*), void* userData, bool state)
    : nameOn(nameOn), nameOff(nameOff), click(click), userData(userData), state(state) {}
SwitchMenuItem::~SwitchMenuItem(){}

IMenuItem::ItemType SwitchMenuItem::GetType(){
    return ItemType::DEFAULT;
}

const wchar_t* SwitchMenuItem::GetName(){
    return state ? nameOn : nameOff;
}

void SwitchMenuItem::OnClick(){
    state = !state;
    if (click)
        click(state, userData);
}

ConditionalMenuItem::ConditionalMenuItem(const wchar_t* nameTrue, const wchar_t* nameFalse, bool(*judge)(void*))
    : nameTrue(nameTrue), nameFalse(nameFalse), judge(judge) {}
ConditionalMenuItem::ConditionalMenuItem(const wchar_t* nameTrue, const wchar_t* nameFalse, bool(*judge)(void*), void(*click)(bool, void*))
    : nameTrue(nameTrue), nameFalse(nameFalse), judge(judge), click(click) {}
ConditionalMenuItem::ConditionalMenuItem(const wchar_t* nameTrue, const wchar_t* nameFalse, bool(*judge)(void*), void(*click)(bool, void*), void* data)
    : nameTrue(nameTrue), nameFalse(nameFalse), judge(judge), click(click), judgeData(data), userData(data) {}
ConditionalMenuItem::ConditionalMenuItem(const wchar_t* nameTrue, const wchar_t* nameFalse, bool(*judge)(void*), void* judgeData, void(*click)(bool, void*), void* userData)
    : nameTrue(nameTrue), nameFalse(nameFalse), judge(judge), click(click), judgeData(judgeData), userData(userData) {}
ConditionalMenuItem::~ConditionalMenuItem(){}

IMenuItem::ItemType ConditionalMenuItem::GetType(){
    return ItemType::DEFAULT;
}

const wchar_t* ConditionalMenuItem::GetName(){
    return judge(judgeData) ? nameTrue : nameFalse;
}

void ConditionalMenuItem::OnClick(){
    if (click)
        click(judge(judgeData), userData);
}

const float Menu::WIDTH_PIXELS = 250.0f;
const float Menu::CORNER_PIXELS = 10.0f;
const float Menu::LINE_PIXELS = 30.0f;

Menu::Menu(){}

Menu::~Menu(){
    Free(items);
}

void Menu::AddItem(IMenuItem* item){
    items.Add(item);
}

void Menu::SetClientSize(Vector2 size){
    cliSize = size;
    cliInvSize.x = 1.0f / cliSize.x;
    cliInvSize.y = 1.0f / cliSize.y;
    if (curMenu){
        curMenu->SetClientSize(size);
    }
}

bool Menu::InMenu(Vector2 relaPos){
    Vector2 cornWidth = cliInvSize * CORNER_PIXELS;
    float x = 2.0f * cornWidth.x + WIDTH_PIXELS * cliInvSize.x;
    float y = 2.0f * cornWidth.y + items.Size() * LINE_PIXELS * cliInvSize.y;
    return relaPos.x >= 0.0f && relaPos.x <= x && relaPos.y <= 0.0f && relaPos.y >= -y;
}

bool Menu::InChainMenu(Vector2 relaPos){
    if (curMenu && curMenu->InChainMenu(relaPos - menuPos)){
        return true;
    }
    return InMenu(relaPos);
}

void Menu::CursorMove(Vector2 relaPos){
    if (InMenu(relaPos)){
        selected = (int)((-CORNER_PIXELS * cliInvSize.y - relaPos.y) / (LINE_PIXELS * cliInvSize.y));
        openMenu = true;
    }
    cursorPos = relaPos;
    if (curMenu){
        curMenu->CursorMove(cursorPos - menuPos);
    }
}

void Menu::Click(){
    DebugLog("Menu::Click %p", this);
    if (curMenu){
        curMenu->Click();
    }
    if (selected >= 0 && selected < items.Size()){
        IMenuItem* item = items.GetItem(selected);
        item->OnClick();
    }
}

void Menu::RenderItem(IMenuItem* item){
    minPos.y -= LINE_PIXELS * cliInvSize.y;
    switch (item->GetType()){
    case IMenuItem::ItemType::DEFAULT:{
        if (drawCounter == selected){
            SetMenu(NULL, -1);
            float height = LINE_PIXELS * cliInvSize.y, width = WIDTH_PIXELS * cliInvSize.x;
            glColor3f(0.1f, 0.4f, 1.0f);
            glBegin(GL_TRIANGLES);
            glVertex2f(minPos.x, minPos.y); glVertex2f(minPos.x + width, minPos.y); glVertex2f(minPos.x + width, minPos.y + height);
            glVertex2f(minPos.x, minPos.y); glVertex2f(minPos.x, minPos.y + height); glVertex2f(minPos.x + width, minPos.y + height);
            glEnd();
        }
        glColor3f(1.0f, 0.5f, 0.0f);
        glRasterPos2f(minPos.x + (0.5f * CORNER_PIXELS) * cliInvSize.x, minPos.y + (0.2f * LINE_PIXELS) * cliInvSize.y);
        glDrawCNString(item->GetName());
    }
        break;
    case IMenuItem::ItemType::SEPERATOR:
        if (drawCounter == selected){
            SetMenu(NULL, -1);
        }
        glColor3f(0.5f, 0.5f, 0.5f);
        glLineWidth(3.0f);
        glBegin(GL_LINES);
        glVertex2f(minPos.x, minPos.y + (0.5f * LINE_PIXELS) * cliInvSize.y);
        glVertex2f(minPos.x + WIDTH_PIXELS * cliInvSize.x, minPos.y + (0.5f * LINE_PIXELS) * cliInvSize.y);
        glEnd();
        glLineWidth(1.0f);
        break;
    case IMenuItem::ItemType::GROUP:{
        float height = LINE_PIXELS * cliInvSize.y, width = WIDTH_PIXELS * cliInvSize.x;
        if (drawCounter == selected){
            glColor3f(0.1f, 0.4f, 1.0f);
            glBegin(GL_TRIANGLES);
            glVertex2f(minPos.x, minPos.y); glVertex2f(minPos.x + width, minPos.y); glVertex2f(minPos.x + width, minPos.y + height);
            glVertex2f(minPos.x, minPos.y); glVertex2f(minPos.x, minPos.y + height); glVertex2f(minPos.x + width, minPos.y + height);
            glEnd();
            Menu* itemMenu = item->GetMenu();
            if (itemMenu == NULL){
                DebugError("Menu::RenderItem %llu:%p NullPointerException", drawCounter, item);
            }else{
                if (openMenu){
                    SetMenu(itemMenu, drawCounter);
                    itemMenu->Render(Vector2(minPos.x + menuPos.x, minPos.y + (LINE_PIXELS + CORNER_PIXELS) * cliInvSize.y));
                }
            }
        }
        glColor3f(1.0f, 0.5f, 0.0f);
        glRasterPos2f(minPos.x + (0.5f * CORNER_PIXELS) * cliInvSize.x, minPos.y + (0.2f * LINE_PIXELS) * cliInvSize.y);
        glDrawCNString(item->GetName());
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_TRIANGLES);
        glVertex2f(minPos.x + width * 0.9f, minPos.y + height * 0.2f);
        glVertex2f(minPos.x + width * 0.95f, minPos.y + height * 0.5f);
        glVertex2f(minPos.x + width * 0.9f, minPos.y + height * 0.8f);
        glEnd();
    }
        break;
    }    
    drawCounter++;
}

void Menu::Render(Vector2 pos){
    Vector2 cornWidth = cliInvSize * CORNER_PIXELS;

    float width = cornWidth.x * 2.0f + WIDTH_PIXELS * cliInvSize.x;
    float height = cornWidth.y * 2.0f + items.Size() * LINE_PIXELS * cliInvSize.y;

    float xmin = pos.x + cornWidth.x;
    float ymax = pos.y - cornWidth.y;

    renderPos = pos;

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glColor3f(0.1f, 0.1f, 0.1f);
    GLUtils::DrawRoundRect(
        pos.x,
        pos.y - height,
        width,
        height,
        cornWidth
    );
    
    minPos = Vector2(xmin, ymax);
    drawCounter = 0;

    items.Foreach<Menu*>([](IMenuItem* item, Menu* menu){
        menu->RenderItem(item);
    }, this);

    glLineWidth(1.0f);
}

void Menu::ResetSelect(){
    selected = -1;
    openMenu = false;
}

void Menu::SetMenu(Menu* m, size_t pos){
    if (m == NULL){
        if (curMenu)
            curMenu->ResetSelect();
        curMenu = NULL;
        return;
    }
    if (curMenu != m){
        if (curMenu)
            curMenu->ResetSelect();
        curMenu = m;
        menuPos = Vector2((WIDTH_PIXELS + CORNER_PIXELS) * cliInvSize.x,
                        -(pos * LINE_PIXELS * cliInvSize.y) - CORNER_PIXELS * cliInvSize.y);
        // size_t pos; 无正负号，需要先与浮点数相乘再取负
        if (menuPos.x + renderPos.x >= 1.0f)
            menuPos.x -= 2.0f * (WIDTH_PIXELS + 2.0f * CORNER_PIXELS) * cliInvSize.x;
        m->SetClientSize(cliSize);
        m->CursorMove(cursorPos - menuPos);
    }
}

void Menu::PressUp(){
    if (curMenu){
        curMenu->PressUp();
        return;
    }
    if (selected > 0 && selected < items.Size()){
        selected--;
        openMenu = false;
    }
}

void Menu::PressDown(){
    if (curMenu){
        curMenu->PressDown();
        return;
    }
    if ((selected < items.Size() - 1 && items.Size() != 0) || selected == -1){
        selected++;
        openMenu = false;
    }
}

void Menu::PressLeft(){
    if (curMenu){
        if (curMenu->curMenu){
            curMenu->PressLeft();
            return;
        }
        SetMenu(NULL, -1);
        openMenu = false;
    }
}

void Menu::PressRight(){
    if (curMenu){
        curMenu->PressRight();
        return;
    }
    openMenu = true;
    if (selected < items.Size()){
        IMenuItem* item = items[selected];
        if (item->GetType() == IMenuItem::ItemType::GROUP){
            if (!item->GetMenu()){
                DebugError("Menu::PressRight %llu:%p NullPointerException", selected, item);
                return;
            }
            SetMenu(item->GetMenu(), selected);
            curMenu->selected = 0;
        }
    }
}