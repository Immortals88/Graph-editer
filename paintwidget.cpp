#include "paintwidget.h"
#include <QtGui>
#include <QtWidgets>
#include <QMainWindow>

PaintWidget::PaintWidget(QWidget *parent) :
    QLabel(parent)
{
    isDrawing = false;
    isChosen = false;
    isFilling = false;
    isErase=false;
    OperIndex = 0;
}

void PaintWidget::setImage(QImage img)              // 读入图像
{
    image = img;
    tempImage = image;
    update();
}

QImage PaintWidget::getImage()                      // 外部获取图像
{
    if (image.isNull() != true)
        return image;
    QImage i;
    return i;
}

void PaintWidget::setShape(PaintWidget::shape t)    // 设置绘制类型
{
    type = t;
}

void PaintWidget::setMode(PaintWidget::drawMode m)  // 设置选择还是画图
{
    mode = m;
}

void PaintWidget::setCurrentLayer(Layer *l){
    currentLayer = l;
}

void PaintWidget::setPenWidth(int w){
    penWidth = w;
}

void PaintWidget::setPenColor(QColor c){
    penColor = c;
}

void PaintWidget::setFillColor(QColor c){
    fillColor = c;
}

void PaintWidget::setFill(bool b){
    isFilling = b;
}

void PaintWidget::setErase(bool b){
    isErase=b;
}

void PaintWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    if (isDrawing == true) {
        painter.drawImage(0, 0, tempImage);// 如果正在绘图，既鼠标点击或者正在移动，画在tempImage上
    }
    else {
        painter.drawImage(0, 0, image);// 如果鼠标释放，将图保存在image上
    }
}

void PaintWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if(mode == PaintWidget::Draw){
            lastPoint = event->pos();
            isDrawing = true;// 鼠标点击开始绘图，移动表示正在绘图
        }
        else if(mode == PaintWidget::Choose){
            isDrawing = true;
            lastPoint = event->pos();
            isChosen = findOp();
        }
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {// 鼠标为左键且正在移动
        endPoint = event->pos();
        tempImage = image;
        if(mode == PaintWidget::Draw){
            if (type == Pen) {// 如果工具为画笔，不用双缓冲直接画在画板上
                paint(image);
            }
            else { // 否则用双缓冲绘图
                paint(tempImage);
            }
        }
        else if(mode == PaintWidget::Choose){
            if(isChosen){
                type = PaintWidget::shape(op.Type);
                penWidth = op.W;
                penColor = op.C;
                isFilling=op.isFill;
                if(isFilling) fillColor=op.FC;
                paint(tempImage);
            }
        }
    }
}
void PaintWidget::mouseReleaseEvent(QMouseEvent *event) {
    if(isDrawing && event->pos() == lastPoint && type != Pen){
        isDrawing = false;
        return;
    }
    isDrawing = false;
    if(mode == PaintWidget::Draw){
        if (type != Pen) {
            paint(image);
            Operation  op(type,penWidth,penColor,lastPoint.x(),lastPoint.y(),endPoint.x(),endPoint.y(),isFilling, fillColor,false);
            sendop(op);
        }else{
            Operation  op(type,penWidth,penColor,0,0,0,0,isFilling, fillColor,false);
            if(isErase){
                op.C=QColor(255,255,255);
                op.isErase=true;
                if (penWidth<5){
                    op.W=5;
                }
            }
            for(unsigned int i = 0; i < pointList.size() - 1; i++){
                op.x1 = pointList[i].x();
                op.x2 = pointList[i + 1].x();
                op.y1 = pointList[i].y();
                op.y2 = pointList[i + 1].y();

                sendop(op);
            }
            pointList.clear();
        }
    }
    else if(mode == PaintWidget::Choose){
        if (isChosen){
            isChosen = false;
            op.x1 += endPoint.x() - lastPoint.x();
            op.x2 += endPoint.x() - lastPoint.x();
            op.y1 += endPoint.y() - lastPoint.y();
            op.y2 += endPoint.y() - lastPoint.y();
            op.x1 = int(op.x1 / ratio);
            op.x2 = int(op.x2 / ratio);
            op.y1 = int(op.y1 / ratio);
            op.y2 = int(op.y2 / ratio);
            op.W = int(op.W / ratio);
            currentLayer->setOp(OperIndex, op);
            sendop(Operation());
        }
    }
}

bool PaintWidget::findOp(){
    Operation tempOp;
    bool found = false;
    int x = lastPoint.x();
    int y = lastPoint.y();
    for(int i = currentLayer->getSize() - 1; i >= 0; i--){
        tempOp = currentLayer->getOp(i);
        if(tempOp.isInside(int(x/ratio), int(y/ratio))){
            OperIndex = static_cast<unsigned int> (i);
            found = true;
            op = tempOp;
            op.x1 = int(op.x1 * ratio);
            op.y1 = int(op.y1 * ratio);
            op.x2 = int(op.x2 * ratio);
            op.y2 = int(op.y2 * ratio);
            op.W = int(op.W * ratio);
            if(op.W < 1) op.W = 1;
            break;
        }
    }
    return found;
}

void PaintWidget::paint(QImage &theImage) {
    QPainter p(&theImage);
    QPen apen;
    //apen.setStyle(Qt::DashDotLine);
    apen.setWidth(penWidth);    // 设置画笔宽度
    apen.setColor(penColor);    // 设置画笔颜色
    if(isFilling){
        p.setBrush(fillColor);
    }
    if(isErase){
        apen.setColor(QColor(255,255,255));
        if(penWidth<5){
            apen.setWidth(5);
        }
    }
    p.setPen(apen);// 设置绘图工具画笔线条宽度为4
    p.setRenderHint(QPainter::Antialiasing, true); // 反走样

    int x1, y1, x2, y2;
    x1 = lastPoint.x();
    y1 = lastPoint.y();
    x2 = endPoint.x();
    y2 = endPoint.y();

    if(isChosen){
        x1 = op.x1 + endPoint.x() - lastPoint.x();
        y1 = op.y1 + endPoint.y() - lastPoint.y();
        x2 = op.x2 + endPoint.x() - lastPoint.x();
        y2 = op.y2 + endPoint.y() - lastPoint.y();
    }

    switch (type) {                 // 画图
    case PaintWidget::Pen:{
        p.drawLine(lastPoint, endPoint);
        lastPoint = endPoint;
        pointList.push_back(lastPoint);
        break;
    }
    case  PaintWidget::Line:{
        p.drawLine(QPoint(x1,y1), QPoint(x2,y2));
        break;
    }
    case PaintWidget::Ellipse:{
        p.drawEllipse(x1, y1, x2 - x1, y2 - y1);
        break;
    }
    case PaintWidget::Circle:{
        int length = (x2 - x1) > (y2 - y1) ? (x2 - x1) : (y2 - y1);
        p.drawEllipse(x1, y1, length, length);
        break;
    }
    case PaintWidget::Triangle:{
        int top, buttom, left, right;
        top = (y1 < y2) ? y1 : y2;
        buttom = (y1 > y2) ? y1 : y2;
        left = (x1 < x2) ? x1 : x2;
        right = (x1 > x2) ? x1 : x2;

        if (y1 < y2){
            QPoint points[3] = { QPoint(left,buttom),   QPoint(right,buttom),   QPoint((right + left) / 2,top) };
            p.drawPolygon(points, 3);
        }
        else{
            QPoint points[3] = { QPoint(left,top),  QPoint(right,top),  QPoint((left + right) / 2,buttom) };
            p.drawPolygon(points, 3);
        }
        break;
    }
    case PaintWidget::Rhombus: {
        int top, buttom, left, right;
        top = (y1 < y2) ? y1 : y2;
        buttom = (y1 > y2) ? y1 : y2;
        left = (x1 < x2) ? x1 : x2;
        right = (x1 > x2) ? x1 : x2;

        QPoint points[4] = {
            QPoint(left,(top + buttom) / 2),
            QPoint((left + right) / 2,buttom),
            QPoint(right,(top + buttom) / 2),
            QPoint((left + right) / 2,top) };
        p.drawPolygon(points, 4);
        break;
    }
    case PaintWidget::Rect:{
        p.drawRect(x1, y1, x2 - x1, y2 - y1);
        break;
    }
    case PaintWidget::Square:{
        int length = (x2 - x1) > (y2 - y1) ? (x2 - x1) : (y2 - y1);
        p.drawRect(x1, y1, length, length);
        break;
    }
    case PaintWidget::Hexagon:{
        QPoint points[6] = {
            QPoint(x1,y1),
            QPoint(x2,y1),
            QPoint((3 * x2 - x1) / 2,(y1 + y2) / 2),
            QPoint(x2,y2),
            QPoint(x1,y2),
            QPoint((3 * x1 - x2) / 2,(y1 + y2) / 2) };
        p.drawPolygon(points, 6);
        break;
    }
    case PaintWidget::Null:{
        break;
    }
    default:
        break;
    }
    update();// 重绘
}

void PaintWidget::PaintOp(Operation op, double ratio){
    lastPoint.setX(int(op.x1 * ratio));
    lastPoint.setY(int(op.y1 * ratio));
    endPoint.setX(int(op.x2 * ratio));
    endPoint.setY(int(op.y2 * ratio));
    penWidth=int(op.W * ratio);
    if(penWidth < 1) penWidth = 1;
    penColor=op.C;
    if(op.Type == 1) type = PaintWidget::shape(op.Type + 1);
    else type=PaintWidget::shape(op.Type);
    isFilling = op.isFill;
    isErase=op.isErase;
    if(isFilling){
        fillColor = op.FC;
    }
    this->paint(image);
}

void PaintWidget::setRatio(double value){
    ratio = value;
}
