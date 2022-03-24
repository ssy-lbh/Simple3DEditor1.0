#ifndef __EDITOR_OBJECT_GUIOBJECT__
#define __EDITOR_OBJECT_GUIOBJECT__

#include <define.h>

#include <editor/main/ViewObject.h>

class AGUIObject : public AViewObject {
protected:
    GUIManagerObject* guiMgr = NULL;

    AGUIObject(ViewObjectType type);
    AGUIObject(const wchar_t* name, ViewObjectType type);
    AGUIObject(WString name, ViewObjectType type);

public:
    AGUIObject();
    AGUIObject(const wchar_t* name);
    AGUIObject(WString name);
    virtual ~AGUIObject() override;

    GUIManagerObject* GetGUIManager();
    void SetGUIManager(GUIManagerObject* guiMgr);
};

#endif