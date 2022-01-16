#ifndef __MENU__
#define __MENU__

#include <gl/gl.h>

#include "vecmath.h"
#include "list.h"

class TopMenu;
class Menu;
class MenuItem;

class MenuItem {
public:
    enum class ItemType {
        DEFAULT,
        GROUP,
        SEPERATOR
    };

    ItemType type;
    const wchar_t* name = NULL;
    Menu* menu = NULL;

    void(*click)(void*) = NULL;
    void* userData;

    MenuItem();
    MenuItem(const wchar_t* name);
    MenuItem(Menu* menu);
    ~MenuItem();
};

class Menu {
private:
    List<MenuItem*> items;
    size_t size = 0;

    Vector2 minPos;
    size_t drawCounter;
    size_t selected = -1;

    Vector2 cliSize, cliInvSize;
public:
    Menu();
    ~Menu();
    void AddItem(MenuItem* item);
    void SetClientSize(Vector2 size);
    bool InMenu(Vector2 relaPos);
    void CursorMove(Vector2 relaPos);
    void Click();
    void RenderItem(MenuItem* item);
    void Render(float x, float y);
};

#endif