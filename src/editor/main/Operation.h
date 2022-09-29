#ifndef __EDITOR_MAIN_OPERATION__
#define __EDITOR_MAIN_OPERATION__

#include <define.h>

namespace simple3deditor {

// ����Object��ʵ���Ͽ�������������
class IOperation : public Object {
public:
    IOperation();
    virtual ~IOperation();

    virtual void OnEnter();
    virtual void OnConfirm();
    virtual void OnUndo();
    virtual void OnRightDown();
    virtual void OnRightUp();
    virtual void OnMove();
    virtual void OnCommand(int id);
};

}

#endif