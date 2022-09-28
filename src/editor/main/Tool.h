#ifndef __EDITOR_MAIN_TOOL__
#define __EDITOR_MAIN_TOOL__

#include <define.h>

namespace simple3deditor {

// 含有Object，实际上可以算作抽象类
class ITool : public Object {
public:
    ITool();
    virtual ~ITool();

    virtual void OnSelect();
    virtual void OnUnselect();
    virtual void OnLeftDown();
    virtual void OnLeftUp();
    virtual void OnRightDown();
    virtual void OnRightUp();
    virtual void OnMove();
    virtual void OnRender();
    virtual void OnCommand(int id);
};

}

#endif