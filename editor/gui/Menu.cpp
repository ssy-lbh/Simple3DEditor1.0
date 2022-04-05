#include <editor/gui/Menu.h>

#include <lib/opengl/gl/gl.h>

#include <utils/os/Font.h>
#include <utils/os/Log.h>
#include <utils/gl/GLUtils.h>

MenuItem::MenuItem() : type(MenuItemType::SEPERATOR) {}
MenuItem::MenuItem(Value<const wchar_t*>&& name) : type(MenuItemType::DEFAULT), name(name) {}

MenuItem::MenuItem(Value<const wchar_t*>&& name, std::function<void()>&& click) :
    type(MenuItemType::DEFAULT), name(name) {
    onClick += click;
}

MenuItem::MenuItem(Value<const wchar_t*>&& name, Menu* menu) : type(MenuItemType::GROUP), name(name), menu(menu) {
    onDestory += [](MenuItem* item){
        if (item->menu())
            delete item->menu();
    };
}

MenuItem::~MenuItem(){
    onDestory(this);
}

const float Menu::WIDTH_PIXELS = 250.0f;
const float Menu::CORNER_PIXELS = 10.0f;
const float Menu::LINE_PIXELS = 30.0f;

Menu::Menu(){}

Menu::~Menu(){
    Free(items);
}

void Menu::AddItem(MenuItem* item){
    items.Add(item);
}

void Menu::SetClientSize(Vector2 size){
    cliSize = size;
    cliInvSize.x = 1.0f / cliSize.x;
    cliInvSize.y = 1.0f / cliSize.y;
    if (curMenu)
        curMenu->SetClientSize(size);
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
        MenuItem* item = items[selected];
        item->onClick();
    }
}

void Menu::RenderItem(MenuItem* item){
    minPos.y -= LINE_PIXELS * cliInvSize.y;
    switch (item->type()){
    case MenuItemType::DEFAULT:{
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
        glDrawCNString(item->name());
    }
        break;
    case MenuItemType::SEPERATOR:
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
    case MenuItemType::GROUP:{
        float height = LINE_PIXELS * cliInvSize.y, width = WIDTH_PIXELS * cliInvSize.x;
        if (drawCounter == selected){
            glColor3f(0.1f, 0.4f, 1.0f);
            glBegin(GL_TRIANGLES);
            glVertex2f(minPos.x, minPos.y); glVertex2f(minPos.x + width, minPos.y); glVertex2f(minPos.x + width, minPos.y + height);
            glVertex2f(minPos.x, minPos.y); glVertex2f(minPos.x, minPos.y + height); glVertex2f(minPos.x + width, minPos.y + height);
            glEnd();
            Menu* itemMenu = item->menu();
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
        glDrawCNString(item->name());
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

    items.Foreach([=](MenuItem* item){
        this->RenderItem(item);
    });

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
        MenuItem* item = items[selected];
        if (item->type() == MenuItemType::GROUP){
            if (!item->menu()){
                DebugError("Menu::PressRight %llu:%p NullPointerException", selected, item);
                return;
            }
            SetMenu(item->menu(), selected);
            curMenu->selected = 0;
        }
    }
}