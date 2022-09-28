#include <editor/main/Operation.h>

namespace simple3deditor {

IOperation::IOperation(){}
IOperation::~IOperation(){}
void IOperation::OnEnter(){}
void IOperation::OnConfirm(){}
void IOperation::OnUndo(){}
void IOperation::OnRightDown(){}
void IOperation::OnRightUp(){}
void IOperation::OnMove(){}
void IOperation::OnCommand(int id){}

}