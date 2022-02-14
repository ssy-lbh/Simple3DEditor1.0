#include "attrtable.h"

#include "log.h"
#include "gltools.h"
#include "font.h"

Attribute::Attribute(AttributeType type, const wchar_t* name) : type(type), name(name) {}
Attribute::~Attribute(){}

AttributeType Attribute::GetType(){
    return type;
}

const wchar_t* Attribute::GetName(){
    return name;
}

void Attribute::SetAttrTableReference(AttributeTable* tab, int index){
    this->tab = tab;
    this->index = index;
}

bool Attribute::Trigger(Vector2 pos){
    Vector2 start = Vector2(tab->pos.x + tab->textWidth, tab->pos.y - tab->lineHeight * index);
    if (pos.y > start.y || pos.y < start.y - tab->lineHeight){
        return false;
    }
    switch (type){
    case AttributeType::INTEGER:
    case AttributeType::FLOAT:
        return pos.x >= start.x && pos.x <= start.x + tab->comWidth;
    case AttributeType::BOOLEAN:
        return pos.x >= start.x && pos.x <= start.x + tab->lineHeight * 0.5f
                && pos.y <= start.y - tab->lineHeight * 0.25f && pos.y >= start.y - tab->lineHeight * 0.75f;
    }
    DebugError("Attribute Illegal Type %p %d", this, type);
    return false;
}

void Attribute::Click(Vector2 pos){
    if (type == AttributeType::BOOLEAN){
        current.boolData = ~origin.boolData;
        return;
    }
    current = origin;
}

void Attribute::Drag(Vector2 dir){
    switch (type){
    case AttributeType::INTEGER:
        current.intData = origin.intData + (int)(dir.x * 20.0f);
        break;
    case AttributeType::FLOAT:
        current.floatData = origin.floatData + (int)(dir.x * 20.0f);
        break;
    case AttributeType::BOOLEAN:
        break;
    }
}

void Attribute::ClickEnd(){
    origin = current;
}

void Attribute::Render(){
    //TODO 补全实现
    Vector2 start = Vector2(tab->pos.x + tab->textWidth, tab->pos.y - tab->lineHeight * index);
    wchar_t buffer[MAX_PATH];

    glColor3f(1.0f, 0.5f, 0.0f);
    glRasterPos2f(tab->pos.x, start.y);
    glDrawCNString(name);
    switch (type){
    case AttributeType::INTEGER:
        glColor3f(0.3f, 0.3f, 0.3f);
        GLUtils::DrawRect(start.x, start.y, start.x + tab->comWidth, start.y - tab->lineHeight);
        wsprintfW(buffer, L"%d", current.intData);
        glRasterPos2f(start.x, start.y);
        glDrawCNString(buffer);
        return;
    case AttributeType::FLOAT:
        glColor3f(0.3f, 0.3f, 0.3f);
        GLUtils::DrawRect(start.x, start.y, start.x + tab->comWidth, start.y - tab->lineHeight);
        wsprintfW(buffer, L"%f", current.floatData);
        glRasterPos2f(start.x, start.y);
        glDrawCNString(buffer);
        return;
    case AttributeType::BOOLEAN:
        if (current.boolData){
            glColor3f(0.3f, 0.3f, 0.3f);
        }else{
            glColor3f(1.0f, 1.0f, 1.0f);
        }
        GLUtils::DrawRect(start.x, start.y - tab->lineHeight * 0.25f, start.x + tab->lineHeight * 0.5f, start.y - tab->lineHeight * 0.75f);
        return;
    }
}

AttributeTable::AttributeTable(){}
AttributeTable::~AttributeTable(){}

void AttributeTable::AddAttribute(Attribute* attr){
    attr->SetAttrTableReference(this, attrs.Size());
    attrs.Add(attr);
}

Attribute* AttributeTable::GetAttribute(const wchar_t* name){
    size_t cnt = attrs.Size();
    for (size_t i = 0; i < cnt; i++){
        if (!wcscmp(name, attrs[i]->GetName())){
            return attrs[i];
        }
    }
    return NULL;
}

void AttributeTable::DeleteAttribute(Attribute* attr){
    attrs.Remove(attr);
}

void AttributeTable::DeleteAttribute(const wchar_t* name){
    size_t cnt = attrs.Size();
    for (size_t i = 0; i < cnt; i++){
        if (!wcscmp(name, attrs[i]->GetName())){
            attrs.Remove(attrs[i]);
            return;
        }
    }
}

void AttributeTable::EnableUI(UIManager* uiMgr){
    this->uiMgr = uiMgr;
    attrs.Foreach<UIManager*>([](Attribute* attr, UIManager* uiMgr){
        uiMgr->AddButton(attr);
    }, uiMgr);
}

void AttributeTable::DisableUI(){
    attrs.Foreach<UIManager*>([](Attribute* attr, UIManager* uiMgr){
        uiMgr->DeleteButton(attr);
    }, uiMgr);
    this->uiMgr = NULL;
}