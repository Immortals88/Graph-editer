#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QtGui>
#include <QtWidgets>
#include <QMainWindow>
#include "operation.h"
#include "layer.h"


class PaintWidget : public QLabel
{
    Q_OBJECT

public:
    explicit PaintWidget(QWidget *parent = nullptr);
    enum shape {
        Pen = 1,Line,Ellipse,Circle, Triangle, Rhombus,
        Rect, Square, Hexagon, Null
    };
    enum drawMode{
        Choose, Draw
    };

    void paint(QImage &theImage);
    void setImage(QImage img);      // 设置图片
    QImage getImage();              // 外部获取图像

    void setShape(PaintWidget::shape );      // 设置绘制类型
    void setMode(PaintWidget::drawMode);
    void setPenWidth(int w);                 // 设置画笔宽度
    void setPenColor(QColor c);              // 设置画笔颜色
    void setFillColor(QColor c);        //设置填充颜色
    void setFill(bool b);               //
    void setCurrentLayer(Layer *l);
    void PaintOp(Operation op, double ratio);
    void setRatio(double value);
    void setErase(bool b);

signals:
    void sendop(Operation op);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);    //鼠标点击
    void mouseMoveEvent(QMouseEvent *);     //鼠标移动
    void mouseReleaseEvent(QMouseEvent *);  //鼠标释放
    bool findOp();      //寻找当前图层中是否有被选中的图形

private:
    PaintWidget::shape type;
    PaintWidget::drawMode mode;
    int penWidth;
    double ratio;
    QColor penColor;
    QColor fillColor;
    QImage image;       //图片
    QImage tempImage;   //临时绘制图片
    QPoint lastPoint;
    QPoint endPoint;
    bool isDrawing;     //是否正在移动
    bool isChosen;      //是否有图形被选中，需要被移动
    bool isFilling;     //是否设置为填充
    bool isErase;       //是否为橡皮擦
    Layer *currentLayer;    //当前正在绘制的图层
    Operation op;
    unsigned int OperIndex;
    vector<QPoint> pointList;//点序列，用在线段绘制
};

#endif // PAINTWIDGET_H
