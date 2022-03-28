#ifndef OPERATION_H
#define OPERATION_H
#include <QColor>

class Operation
{
public:
    Operation();
    Operation(int t, int w, QColor cc, int xx1, int yy1, int xx2, int yy2, bool b, QColor fc,bool eraser);
    bool isInside(int x, int y);
    int Type;   //shape
    int W;      //pen width
    QColor C;   //pencolor
    QColor FC;  //fillcolor
    int x1;     //points
    int y1;
    int x2;
    int y2;
    bool isFill;
    bool isErase;
};

#endif // OPERATION_H
