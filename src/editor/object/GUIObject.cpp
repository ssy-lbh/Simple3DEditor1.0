#include <editor/object/GUIObject.h>

namespace simple3deditor {

AGUIObject::AGUIObject() : AViewObject(L"GUI", ViewObjectType::OBJECT_GUI) {}
AGUIObject::AGUIObject(const wchar_t* name) : AViewObject(name) {}
AGUIObject::AGUIObject(WString name) : AViewObject(name) {}
AGUIObject::AGUIObject(ViewObjectType type) : AViewObject(L"GUI", type) {}
AGUIObject::AGUIObject(const wchar_t* name, ViewObjectType type) : AViewObject(name, type) {}
AGUIObject::AGUIObject(WString name, ViewObjectType type) : AViewObject(name, type) {}
AGUIObject::~AGUIObject(){}

GUIManagerObject* AGUIObject::GetGUIManager(){ return parentGUIMgr; }
void AGUIObject::SetGUIManager(GUIManagerObject* guiMgr){ this->parentGUIMgr = guiMgr; }

}