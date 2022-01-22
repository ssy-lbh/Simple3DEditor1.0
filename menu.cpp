#include "menu.h"

#include "font.h"
#include "gltools.h"
#include "log.h"

MenuItem::MenuItem() : type(ItemType::SEPERATOR) {}

MenuItem::MenuItem(const wchar_t* name) : type(ItemType::DEFAULT), name(name) {}

MenuItem::MenuItem(const wchar_t* name, void(*click)(void*)) : type(ItemType::DEFAULT), name(name), click(click) {}

MenuItem::MenuItem(const wchar_t* name, void(*click)(void*), void* userData) : type(ItemType::DEFAULT), name(name), click(click), userData(userData) {}

MenuItem::MenuItem(const wchar_t* name, Menu* menu) : type(ItemType::GROUP), name(name), menu(menu) {}

MenuItem::~MenuItem(){
    if (menu) delete menu;
}

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
}

bool Menu::InMenu(Vector2 relaPos){
    float cornWidth = 10.0f * cliInvSize.y;
    float x = 2.0f * cornWidth + 250.0f * cliInvSize.y;
    float y = 2.0f * cornWidth + items.Size() * 30.0f * cliInvSize.y;
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
        selected = (int)((-10.0f * cliInvSize.y - relaPos.y) / (30.0f * cliInvSize.y));
    }
    cursorPos = relaPos;
}

void Menu::Click(){
    if (selected >= 0 && selected < items.Size()){
        MenuItem* item = items.GetItem(selected);
        if (item->click != NULL){
            item->click(item->userData);
        }
    }
}

void Menu::RenderItem(MenuItem* item){
    minPos.y -= 30.0f * cliInvSize.y;
    switch (item->type){
    case MenuItem::ItemType::DEFAULT:{
        if (drawCounter == selected){
            float height = 30.0f * cliInvSize.y, width = 250.0f * cliInvSize.y;
            glColor3f(0.1f, 0.4f, 1.0f);
            glBegin(GL_TRIANGLES);
            glVertex2f(minPos.x, minPos.y); glVertex2f(minPos.x + width, minPos.y); glVertex2f(minPos.x + width, minPos.y + height);
            glVertex2f(minPos.x, minPos.y); glVertex2f(minPos.x, minPos.y + height); glVertex2f(minPos.x + width, minPos.y + height);
            glEnd();
        }
        glColor3f(1.0f, 0.5f, 0.0f);
        glRasterPos2f(minPos.x + 5.0f * cliInvSize.y, minPos.y + 6.0f * cliInvSize.y);
        glDrawCNString(item->name);
    }
        break;
    case MenuItem::ItemType::SEPERATOR:
        glColor3f(0.5f, 0.5f, 0.5f);
        glLineWidth(3.0f);
        glBegin(GL_LINES);
        glVertex2f(minPos.x, minPos.y + 15.0f * cliInvSize.y);
        glVertex2f(minPos.x + 250.0f * cliInvSize.y, minPos.y + 15.0f * cliInvSize.y);
        glEnd();
        break;
    case MenuItem::ItemType::GROUP:{
        float height = 30.0f * cliInvSize.y, width = 250.0f * cliInvSize.y;
        if (drawCounter == selected){
            glColor3f(0.1f, 0.4f, 1.0f);
            glBegin(GL_TRIANGLES);
            glVertex2f(minPos.x, minPos.y); glVertex2f(minPos.x + width, minPos.y); glVertex2f(minPos.x + width, minPos.y + height);
            glVertex2f(minPos.x, minPos.y); glVertex2f(minPos.x, minPos.y + height); glVertex2f(minPos.x + width, minPos.y + height);
            glEnd();
            if (item->menu == NULL){
                DebugError("Menu::RenderItem %p NullPointerException", item);
            }else{
                item->menu->SetClientSize(cliSize);
                item->menu->CursorMove(Vector2(cursorPos.x - width - 10.0f * cliInvSize.y, cursorPos.y + drawCounter * 30.0f * cliInvSize.y + 10.0f * cliInvSize.y));
                item->menu->Render(minPos.x + width + 10.0f * cliInvSize.y, minPos.y + 40.0f * cliInvSize.y);
                curMenu = item->menu;
                menuPos = Vector2(width + 10.0f * cliInvSize.y, -drawCounter * 30.0f * cliInvSize.y - 10.0f * cliInvSize.y);
            }
        }
        glColor3f(1.0f, 0.5f, 0.0f);
        glRasterPos2f(minPos.x + 5.0f * cliInvSize.y, minPos.y + 6.0f * cliInvSize.y);
        glDrawCNString(item->name);
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

void Menu::Render(float x, float y){
    float cornWidth = 10.0f * cliInvSize.y;

    float xmin = x + cornWidth;
    float ymax = y - cornWidth;

    float width = cornWidth * 2.0f + 250.0f * cliInvSize.y;
    float height = cornWidth * 2.0f + items.Size() * 30.0f * cliInvSize.y;
    
    glColor3f(0.1f, 0.1f, 0.1f);
    GLUtils::DrawRoundRect(
        x,
        y - height,
        width,
        height,
        cornWidth,
        0.05f
    );
    
    minPos = Vector2(xmin, ymax);
    drawCounter = 0;

    items.Foreach<Menu*>([](MenuItem* item, Menu* menu){
        menu->RenderItem(item);
    }, this);
}

void Menu::ResetSelect(){
    selected = -1;
}