#ifndef __EDITOR_OBJECT_GUIOBJECT__
#define __EDITOR_OBJECT_GUIOBJECT__

#include <define.h>

#include <editor/main/ViewObject.h>

namespace simple3deditor {

class AGUIObject : public AViewObject {
protected:
    GUIManagerObject* parentGUIMgr = NULL;

    AGUIObject(ViewObjectType type);
    AGUIObject(const wchar_t* name, ViewObjectType type);
    AGUIObject(WString name, ViewObjectType type);

public:
    OBJECT_INFO_DEF();

    AGUIObject();
    AGUIObject(const wchar_t* name);
    AGUIObject(WString name);
    virtual ~AGUIObject() override;

    // 获取管理自身的GUIManagerObject
    GUIManagerObject* GetGUIManager();
    void SetGUIManager(GUIManagerObject* guiMgr);
};

}

#endif