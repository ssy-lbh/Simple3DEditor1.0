#ifndef __ATTRTABLE__
#define __ATTRTABLE__

#include "list.h"
#include "uimgr.h"

enum class AttributeType {
    INTEGER,
    FLOAT,
    BOOLEAN
};

class Attribute final : public IButton {
private:
    AttributeType type;
    const wchar_t* name;
    AttributeTable* tab;
    int index;

public:
    union {
        int intData;
        float floatData;
        bool boolData;
    } origin, current;

    Attribute(AttributeType type, const wchar_t* name);
    virtual ~Attribute() override;

    AttributeType GetType();
    const wchar_t* GetName();
    void SetAttrTableReference(AttributeTable* tab, int index);

    virtual bool Trigger(Vector2 pos) override;
    virtual void Click(Vector2 pos) override;
    virtual void Drag(Vector2 dir) override;
    virtual void ClickEnd() override;
    virtual void Render() override;
};

class AttributeTable final : public Object {
private:
    List<Attribute*> attrs;
    UIManager* uiMgr = NULL;

public:
    Vector2 pos;
    float lineHeight = 0.1f;
    float textWidth = 0.15f;
    float comWidth = 0.15f;

    AttributeTable();
    ~AttributeTable();

    void AddAttribute(Attribute* attr);
    Attribute* GetAttribute(const wchar_t* name);
    void DeleteAttribute(Attribute* attr);
    void DeleteAttribute(const wchar_t* name);
    void EnableUI(UIManager* uiMgr);
    void DisableUI();
};

#endif