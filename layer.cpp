#include "layer.h"

Layer::Layer(unsigned int no)
{
    No=no;
    valid=true;
}
unsigned int Layer::getNo(){
    return No;
}
unsigned int Layer::getSize(){
    return static_cast<unsigned int>(OperationList.size());
}
void Layer::createNode(Operation nop){
    OperationList.push_back(nop);
}

Operation Layer::getOp(unsigned int i){
    return OperationList[i];
}
bool Layer::isvalid(){
    return valid;
}
void Layer::disable(){
    valid=false;
}

void Layer::setOp(unsigned int i, Operation op){
    OperationList[i] = op;
}
