#include "menu.h"

#include "font.h"
#include "gltools.h"

MenuItem::MenuItem() : type(ItemType::SEPERATOR) {}

MenuItem::MenuItem(const wchar_t* name) : type(ItemType::DEFAULT), name(name) {}

MenuItem::MenuItem(const wchar_t* name, void(*click)(void*)) : type(ItemType::DEFAULT), name(name), click(click) {}

MenuItem::MenuItem(const wchar_t* name, void(*click)(void*), void* userData) : type(ItemType::DEFAULT), name(name), click(click), userData(userData) {}

MenuItem::MenuItem(Menu* menu) : type(ItemType::GROUP), menu(menu) {}

MenuItem::~MenuItem(){}

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

void Menu::CursorMove(Vector2 relaPos){
    if (InMenu(relaPos)){
        selected = (int)((-10.0f * cliInvSize.y - relaPos.y) / (30.0f * cliInvSize.y));
    }else{
        selected = -1;
    }
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
        glRasterPos2f(minPos.x, minPos.y + 6.0f * cliInvSize.y);
        glDrawCNString(item->name);
    }
        break;
    case MenuItem::ItemType::SEPERATOR:
        break;
    case MenuItem::ItemType::GROUP:
        break;
    }    
    drawCounter++;
}

void Menu::Render(float x, float y){
    float cornWidth = 10.0f * cliInvSize.y;
    float xmin = x + cornWidth, xmax = x + cornWidth + 250.0f * cliInvSize.y;
    float ymax = y - cornWidth, ymin = y - cornWidth - items.Size() * 30.0f * cliInvSize.y;

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(0.1f, 0.1f, 0.1f);
    GLUtils::DrawCorner(xmin, ymax, 90.0f, 180.0f, cornWidth, 0.05f);
    GLUtils::DrawCorner(xmax, ymax, 0.0f, 90.0f, cornWidth, 0.05f);
    GLUtils::DrawCorner(xmin, ymin, 180.0f, 270.0f, cornWidth, 0.05f);
    GLUtils::DrawCorner(xmax, ymin, 270.0f, 360.0f, cornWidth, 0.05f);

    glBegin(GL_TRIANGLES);
    glVertex2f(xmin, ymax + cornWidth); glVertex2f(xmin, ymin - cornWidth); glVertex2f(xmax, ymin - cornWidth);
    glVertex2f(xmin, ymax + cornWidth); glVertex2f(xmax, ymax + cornWidth); glVertex2f(xmax, ymin - cornWidth);
    glVertex2f(xmin - cornWidth, ymax); glVertex2f(xmin, ymax); glVertex2f(xmin, ymin);
    glVertex2f(xmin - cornWidth, ymax); glVertex2f(xmin - cornWidth, ymin); glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymax); glVertex2f(xmax + cornWidth, ymax); glVertex2f(xmax + cornWidth, ymin);
    glVertex2f(xmax, ymax); glVertex2f(xmax, ymin); glVertex2f(xmax + cornWidth, ymin);
    glEnd();

    minPos = Vector2(xmin, ymax);
    drawCounter = 0;

    items.Foreach<Menu*>([](MenuItem* item, Menu* menu){
        menu->RenderItem(item);
    }, this);
}