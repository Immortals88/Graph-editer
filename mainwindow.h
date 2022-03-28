#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QString>
#include <QLabel>
#include <QScrollArea>
#include <QStatusBar>
#include <QSlider>
#include <QDebug>
#include <QFrame>
#include <QColor>
#include <QPalette>
#include <QString>
#include <QImage>
#include <QImageReader>
#include <QFileDialog>
#include <QFile>
#include <QFileDevice>
#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include "paintwidget.h"
#include "operation.h"
#include "layer.h"
#include "mywidget.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Flush();

private:
    QDockWidget *dock_image;
    QDockWidget *dock_layer;
    myQList *layer_list;
    PaintWidget *label_image;
    QStatusBar *status_bar;
    QSlider *slider_scale;
    QFrame *frame_color;
    QFrame *frame_fill;
    QString currentPath;
    QPushButton *add_layer;
    QPushButton *delete_layer;
    QPushButton *button_fill;
    QComboBox *box_padding;
    QSpinBox *box_penWidth;
    QImage raw;
    int image_width;
    int image_height;
    double ratio;
    unsigned int Layernum;
    unsigned int curLayer;
    vector<Layer>LayerList;
    QColor color;
    QColor fillColor;
    int penWidth;

    void initMenuBar();
    void initToolBar();
    void initDock();
    void initImage();
    void initStatusBar();

private slots:
    void file_new();
    void file_open();
    void file_save();
    void file_saveas();
    void imageResize(int value);
    void figureSelect();
    void colorSelect();
    void fillColorSelect();
    void selectLayer(QListWidgetItem *item);
    void penWidthChange(int value);
    void addLayer();
    void deleteLayer();
    void receiveop(Operation op);
    void receiveListChange();
    void selectMode();
    void selectEraser();
    void padMode(int index);
    void showImageWindow();
    void showLayerWindow();
};

#endif // MAINWINDOW_H
