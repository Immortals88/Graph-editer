#include "operation.h"
#include <algorithm>
#include<math.h>
using namespace std;

Operation::Operation(){
    Type = -1;
}

Operation::Operation(int t, int w, QColor cc, int xx1, int yy1, int xx2, int yy2, bool b, QColor fc,bool eraser)
{
    Type=t;
    W=w;
    C=cc;
    x1=xx1;
    y1=yy1;
    x2=xx2;
    y2=yy2;
    isFill = b;
    FC = fc;
    isErase=eraser;
}


bool Operation::isInside(int x, int y){
    if(Type == 1)  // pen
       {
           return false;
       }
       else if (Type==2) {
           double length=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
           double l1=sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
           double l2=sqrt((x-x2)*(x-x2)+(y-y2)*(y-y2));
           if (length*1.05>l1+l2){
               return true;
           }
           return false;
       }
       else{
           if (x >= min(x1, x2) && x <= max(x1, x2) && y >= min(y1, y2) && y <= max(y1, y2))
               return true;
       }
       return false;
}
