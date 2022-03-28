#ifndef LAYER_H
#define LAYER_H


#include"operation.h"

#include<vector>
#include <QtGui>
#include <QtWidgets>
#include <QMainWindow>
using namespace std;


class Layer
{
public:
    Layer(unsigned int no);    //constructor
    void createNode(Operation op);   // add a new operation
    unsigned int getNo();       //return the layer number
    unsigned int getSize();     //return the number of operation
    Operation getOp(unsigned int i);    //layer[i]
    bool isvalid();
    void disable();
    void setOp(unsigned int i, Operation op);//layer[i]=op
private:
    vector<Operation> OperationList;
    unsigned int No;
    bool valid;
};


#endif // LAYER_H
