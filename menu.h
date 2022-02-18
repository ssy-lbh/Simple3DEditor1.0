#ifndef __MENU__
#define __MENU__

#include <gl/gl.h>

#include "vecmath.h"
#include "list.h"

#define MENUITEM_LAMBDA_TRANS(t) (void(*)(void*))(void(*)(t*))

class IMenuItem : public Object {
public:
    enum class ItemType {
        DEFAULT,
        GROUP,
        SEPERATOR
    };

    IMenuItem();
    virtual ~IMenuItem();

    virtual IMenuItem::ItemType GetType();
    virtual const wchar_t* GetName();
    virtual Menu* GetMenu();

    virtual void OnClick();
};

class MenuItem : public IMenuItem {
public:
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
    virtual ~MenuItem() override;

    virtual IMenuItem::ItemType GetType() override;
    virtual const wchar_t* GetName() override;
    virtual Menu* GetMenu() override;

    virtual void OnClick() override;
};

class Menu : public Object {
private:
    List<IMenuItem*> items;
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
    void AddItem(IMenuItem* item);
    void SetClientSize(Vector2 size);
    bool InMenu(Vector2 relaPos);
    bool InChainMenu(Vector2 relaPos);
    void CursorMove(Vector2 relaPos);
    void Click();
    void RenderItem(IMenuItem* item);
    void Render(float x, float y);
    void ResetSelect();
};

#endif