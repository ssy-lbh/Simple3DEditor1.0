#ifndef __MENU__
#define __MENU__

#include <gl/gl.h>

#include "vecmath.h"
#include "list.h"

#define MENUITEM_LAMBDA_TRANS(t) (void(*)(void*))(void(*)(t*))

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
    MenuItem(const wchar_t* name, void(*click)(void*));
    MenuItem(const wchar_t* name, void(*click)(void*), void* userData);
    MenuItem(const wchar_t* name, Menu* menu);
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
    Vector2 cursorPos;

    Menu* curMenu = NULL;
    Vector2 menuPos;
public:
    Menu();
    ~Menu();
    void AddItem(MenuItem* item);
    void SetClientSize(Vector2 size);
    bool InMenu(Vector2 relaPos);
    bool InChainMenu(Vector2 relaPos);
    void CursorMove(Vector2 relaPos);
    void Click();
    void RenderItem(MenuItem* item);
    void Render(float x, float y);
    void ResetSelect();
};

#endif