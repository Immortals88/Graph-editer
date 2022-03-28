#include "mainwindow.h"
#include <fstream>
#include <vector>

using std::vector;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("图形编辑器");
    this->setWindowIcon(QIcon(":/new/prefix1/Images/ps.ico"));
    // init Window
    this->resize(800, 800);
    // init MenuBar
    this->initMenuBar();
    // init ToolBar
    this->initToolBar();
    // init DockWidget
    QWidget *central = this->takeCentralWidget();
    if(central) delete central;
    this->initDock();
    // init StatusBar
    this->initStatusBar();
    ActionList actionlist;
    vector<ActionList *> l;
    l.push_back(&actionlist);
    Layernum=0;
    curLayer=0;
    connect(label_image, &PaintWidget::sendop,this,&MainWindow::receiveop);
    penWidth=1;
}

void MainWindow::initMenuBar(){
    QMenuBar *menuBar = this->menuBar();
    // Menu File
    QMenu *menuFile = new QMenu("文件");
    QAction *action_open = new QAction("打开");
    action_open->setShortcut(QKeySequence::Open);
    connect(action_open, SIGNAL(triggered(bool)), this, SLOT(file_open()));

    QAction *action_save = new QAction("保存");
    action_save->setShortcut(QKeySequence::Save);
    connect(action_save, SIGNAL(triggered(bool)), this, SLOT(file_save()));

    QAction *action_new = new QAction("新建");
    action_new->setShortcut(QKeySequence::New);
    connect(action_new, SIGNAL(triggered(bool)), this, SLOT(file_new()));

    QAction *action_saveas = new QAction("另存为");
    action_saveas->setShortcut(QKeySequence::SaveAs);
    connect(action_saveas, SIGNAL(triggered(bool)), this, SLOT(file_saveas()));

    QList<QAction*> actionList_file;
    actionList_file << action_new << action_open <<action_save <<action_saveas;
    menuFile->addActions(actionList_file);
    menuBar->addMenu(menuFile);
    // Menu Others
    QMenu *menuView = new QMenu("窗口");
    QAction *action_imageOpen = new QAction("图形编辑窗口");
    connect(action_imageOpen, &QAction::triggered, this, &MainWindow::showImageWindow);

    QAction *action_layerOpen = new QAction("图层窗口");
    connect(action_layerOpen, &QAction::triggered, this, &MainWindow::showLayerWindow);

    menuView->addAction(action_imageOpen);
    menuView->addAction(action_layerOpen);
    menuBar->addMenu(menuView);
    // To-do List: add icons of each action
}

void MainWindow::initToolBar(){
    // Tool Pen
    QToolBar *toolBar_pen = this->addToolBar(tr("Pen"));

    QLabel *label_penWidth = new QLabel();
    label_penWidth->setText("画笔宽度");
    toolBar_pen->addWidget(label_penWidth);


    box_penWidth = new QSpinBox();
    box_penWidth->setValue(1);
    connect(box_penWidth, SIGNAL(valueChanged(int)), this, SLOT(penWidthChange(int)));
    toolBar_pen->addWidget(box_penWidth);

    QPushButton *button_color = new QPushButton();
    button_color->setText("画笔颜色");
    connect(button_color,SIGNAL(clicked(bool)),this,SLOT(colorSelect()));
    toolBar_pen->addWidget(button_color);

    frame_color = new QFrame();
    frame_color->setFrameShape(QFrame::Box);
    frame_color->setAutoFillBackground(true);
    frame_color->setPalette(Qt::black);
    frame_color->setFixedSize(25,25);
    toolBar_pen->addWidget(frame_color);

    QLabel *label_padding = new QLabel();
    label_padding->setText("填充:");
    toolBar_pen->addWidget(label_padding);

    box_padding = new QComboBox;
    box_padding->addItem("NULL");
    box_padding->addItem("填充");
    connect(box_padding, SIGNAL(currentIndexChanged(int)), this, SLOT(padMode(int)));
    toolBar_pen->addWidget(box_padding);

    button_fill = new QPushButton();
    button_fill->setText("填充颜色");
    button_fill->setEnabled(false);
    connect(button_fill,SIGNAL(clicked(bool)),this,SLOT(fillColorSelect()));
    toolBar_pen->addWidget(button_fill);


    frame_fill = new QFrame();
    frame_fill->setFrameShape(QFrame::Box);
    frame_fill->setAutoFillBackground(true);
    frame_fill->setPalette(Qt::black);
    frame_fill->setFixedSize(25,25);
    toolBar_pen->addWidget(frame_fill);

    // Tool Figure
    QToolBar *toolBar_figure = this->addToolBar(tr("Figure"));
    ActionList *action_line = new ActionList();
    action_line->setIcon(QIcon(":/new/prefix1/Images/line.ico"));
    action_line->setToolTip("直线");
    ActionList *action_circle = new ActionList();
    action_circle->setToolTip("圆形");
    action_circle->setIcon(QIcon(":/new/prefix1/Images/circle.ico"));
    ActionList *action_free = new ActionList();
    action_free->setToolTip("自由线条");
    action_free->setIcon(QIcon(":/new/prefix1/Images/freeline.ico"));
    ActionList *action_ellipse = new ActionList();
    action_ellipse->setToolTip("椭圆");
    action_ellipse->setIcon(QIcon(":/new/prefix1/Images/ellipse.ico"));
    ActionList *action_triangle = new ActionList();
    action_triangle->setToolTip("三角形");
    action_triangle->setIcon(QIcon(":/new/prefix1/Images/triangle.ico"));
    ActionList *action_rhombus = new ActionList();
    action_rhombus->setToolTip("菱形");
    action_rhombus->setIcon(QIcon(":/new/prefix1/Images/rhombus.ico"));
    ActionList *action_rect = new ActionList();
    action_rect->setToolTip("矩形");
    action_rect->setIcon(QIcon(":/new/prefix1/Images/rect.ico"));
    ActionList *action_square = new ActionList();
    action_square->setToolTip("正方形");
    action_square->setIcon(QIcon(":/new/prefix1/Images/square.ico"));
    ActionList *action_hexagon = new ActionList();
    action_hexagon->setToolTip("六边形");
    action_hexagon->setIcon(QIcon(":/new/prefix1/Images/hexagon.ico"));

    QList<ActionList*> actionList_figure;
    QList<QAction*> QAction_figure;
    actionList_figure << action_free << action_line << action_ellipse << action_circle
                      << action_triangle << action_rhombus << action_rect << action_square << action_hexagon;
    for(int i = 0;i < actionList_figure.size();i++){
        actionList_figure[i]->setIndex(i);
        connect(actionList_figure[i],SIGNAL(triggered(bool)),this,SLOT(figureSelect()));
        QAction_figure.append(static_cast<QAction*>(actionList_figure[i]));
    }
    toolBar_figure->addActions(QAction_figure);

    QAction *action_trans = new QAction();
    action_trans->setToolTip("移动图形");
    toolBar_figure->addAction(action_trans);
    connect(action_trans, &QAction::triggered, this, &MainWindow::selectMode);
    action_trans->setIcon(QIcon(":/new/prefix1/Images/mouse.ico"));

    QAction *action_eraser = new QAction();
    action_eraser->setToolTip("橡皮");
    toolBar_figure->addAction(action_eraser);
    connect(action_eraser, &QAction::triggered, this, &MainWindow::selectEraser);
    action_eraser->setIcon(QIcon(":/new/prefix1/Images/eraser.ico"));

}

void MainWindow::initDock(){
    //setDockNestingEnabled(True);
    // Dock Image
    initImage();

    // Dock layer
    dock_layer = new QDockWidget("图层", this);
    dock_layer->setFeatures(QDockWidget::DockWidgetClosable);
    // widget
    QWidget *widget_layer = new QWidget();
    // layout
    QVBoxLayout *layer_layout = new QVBoxLayout;
    layer_list = new myQList(dock_layer);
    connect(layer_list, &myQList::listChanged, this, &MainWindow::receiveListChange);
    connect(layer_list, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectLayer(QListWidgetItem*)));

    QHBoxLayout *layer_button_layout = new QHBoxLayout;
    add_layer = new QPushButton();
    add_layer->setText("添加图层");
    connect(add_layer, SIGNAL(clicked(bool)), this, SLOT(addLayer()));
    add_layer->setEnabled(false);
    delete_layer=new QPushButton();
    delete_layer->setText("删除图层");
    connect(delete_layer,SIGNAL(clicked(bool)),this ,SLOT(deleteLayer()));
    delete_layer->setEnabled(false);

    layer_button_layout->addWidget(add_layer);
    layer_button_layout->addWidget(delete_layer);

    QLabel *label_name = new QLabel("图层");
    QLabel *label_hide = new QLabel("|隐藏");
    QHBoxLayout *h = new QHBoxLayout;
    h->addWidget(label_name);
    h->addWidget(label_hide);
    layer_layout->addLayout(h);
    layer_layout->addWidget(layer_list);
    layer_layout->addLayout(layer_button_layout);

    widget_layer->setLayout(layer_layout);
    dock_layer->setWidget(widget_layer);

    // layout
    setCentralWidget(dock_image);
    addDockWidget(Qt::RightDockWidgetArea, dock_layer);
}

void MainWindow::receiveop(Operation op){
    if(op.Type!=-1){
        if(op.Type==4||op.Type==8){
            if(op.x2>op.x1&&op.y2<=op.y1){//first
                op.y2=op.y1+op.x2-op.x1;
            }
            else if(op.y2>op.y1&&op.x2<=op.x1){//third
                op.x2=op.x1+op.y2-op.y1;
            }
            else if(op.x2<op.x1&&op.y2<op.y1){//cond
                op.y2=op.y1+op.x2-op.x1;
            }
            else if(op.x1<op.x2&&op.y1<op.y2){//fourth
                int m=max((op.x2-op.x1),(op.y2-op.y1));
                op.x2=op.x1+m;
                op.y2=op.y1+m;
            }
        }
        op.x1 = int(op.x1 / ratio);
        op.x2 = int(op.x2 / ratio);
        op.y1 = int(op.y1 / ratio);
        op.y2 = int(op.y2 / ratio);
        op.W = int(op.W / ratio);
        if(op.W < 1) op.W = 1;
        curLayer = static_cast<unsigned int>(layer_list->getLayerNo(layer_list->currentRow()));
        LayerList[curLayer].createNode(op);
    }
    if(op.Type != 1) this->Flush();
}


void MainWindow::file_new(){
    LayerList.clear();
    layer_list->clear();
    QImage image = QImage(500, 500, QImage::Format_RGB32);
    Layernum = 0;
    image.fill(qRgb(255, 255, 255));
    LayerList.push_back(Layer(0));
    layer_list->add_layer(int(Layernum));
    raw=image;
    label_image->setImage(image);
    label_image->resize(image.width(), image.height());
    currentPath = "";
    // init picture size
    this->image_width = image.width();
    this->image_height = image.height();
    // init slider in Status Bar
    slider_scale->show();
    slider_scale->setValue(100);
    // init layer button
    add_layer->setEnabled(true);
    delete_layer->setEnabled(true);
    this->Flush();
}

void MainWindow::file_open(){
    QString path = QFileDialog::getOpenFileName(this, tr("Choose"), ".", tr("Images(*.jpg *.png *.bmp)")); // 文件选择框
    if (!path.isEmpty()){
        LayerList.clear();
        layer_list->clear();
        //qDebug()<<path<<endl;
        QImage* img = new QImage();
        if (!(img->load(path))) {
            QMessageBox::information(this, tr("Error"), tr("Failed to open picture"));
            delete img;
            return;
        }
        raw=*img;
        Layernum = 0;
        LayerList.push_back(Layer(0));
        layer_list->add_layer(int(Layernum));
        label_image->setImage(*img);
        label_image->resize(img->width(),img->height());
        // init picture size
        this->image_width = img->width();
        this->image_height = img->height();
        currentPath = path;
        // init slider in Status Bar
        slider_scale->show();
        slider_scale->setValue(100);
        // init layer button
        add_layer->setEnabled(true);
        delete_layer->setEnabled(true);
    }
    else{
        return;
    }
}

void MainWindow::file_save(){
    int ret=QMessageBox::warning(this, "Warning", "是否保存文件？如果是打开的文件，可能会被重写", QMessageBox::Yes | QMessageBox::No);
    if(ret== QMessageBox::Yes){
       slider_scale->setValue(100);
        if (currentPath.isEmpty()){
            QString path = QFileDialog::getSaveFileName(this, tr("Save"), ".", tr("Images(*.jpg *.png *.bmp)"));
                if (!path.isEmpty())
                    currentPath = path;
        }
        QImage img = label_image->getImage();
        img.save(currentPath);
    }
}

void MainWindow::file_saveas(){
    QString path = QFileDialog::getSaveFileName(this, tr("Saveas"), ".", tr("Images(*.jpg *.png *.bmp)"));
    if (!path.isEmpty()){
        QImage img = label_image->getImage();
        img.save(path);
        currentPath = path;
    }
}

void MainWindow::initImage(){
    // Dock Image
    dock_image = new QDockWidget("编辑",this);
    dock_image->setFeatures(QDockWidget::DockWidgetClosable);
    dock_image->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    //dock_image->setMinimumSize(500, 500);
    // Label Image
    label_image = new PaintWidget();
    label_image->setScaledContents(true);
    label_image->setPenWidth(1);
    label_image->setPenColor(Qt::black);
    label_image->setShape(PaintWidget::shape(1));

    // scroll
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setWidget(label_image);
    dock_image->setWidget(scrollArea);
}

void MainWindow::initStatusBar(){
    // init bar
    status_bar = this->statusBar();
    // add slide
    slider_scale = new QSlider(status_bar);
    slider_scale->setOrientation(Qt::Horizontal);
    slider_scale->setMinimum(20);
    slider_scale->setMaximum(100);
    slider_scale->setSingleStep(5);
    connect(slider_scale,SIGNAL(valueChanged(int)),this,SLOT(imageResize(int)));
    // set a pointer to show whether the slider should be open or not
    slider_scale->hide();
}

void MainWindow::imageResize(int value){
    ratio = value / 100.0;
    label_image->setRatio(ratio);
    this->Flush();
}


MainWindow::~MainWindow(){

}

void MainWindow::figureSelect(){
    ActionList *action = static_cast<ActionList*>(sender());
    label_image->setShape(PaintWidget::shape(action->getIndex()+1));
    label_image->setMode(PaintWidget::Draw);
    label_image->setErase(false);
}

void MainWindow::colorSelect(){
    color = QColorDialog::getColor();
    if(color.isValid()){
        frame_color->setPalette(QPalette(color));
        label_image->setPenColor(color);
    }
}

void MainWindow::fillColorSelect(){
    fillColor = QColorDialog::getColor();
    if(fillColor.isValid()){
        frame_fill->setPalette(QPalette(fillColor));
        label_image->setFillColor(fillColor);
    }
}

void MainWindow::selectLayer(QListWidgetItem* item){
    layer_list->setCurrentItem(item);
    this->Flush();
}

void MainWindow::penWidthChange(int value){
    if(value < 1){
        box_penWidth->setValue(1);
        return;
    }
    if(value > 10){
        box_penWidth->setValue(10);
        return;
    }
    label_image->setPenWidth(value);
    penWidth=value;
}

void MainWindow::addLayer(){
    Layernum++;
    LayerList.push_back(Layer(Layernum));
    layer_list->add_layer(int(Layernum));
    this->Flush();
}

void MainWindow::deleteLayer(){
        int num=layer_list->currentRow();
        int number=layer_list->getLayerNo(num);
        if (number==0) {
            QMessageBox::warning(this,"warning","图层0为锁定图层，无法删除");
            return;
        }
        curLayer = static_cast<unsigned int>(layer_list->delete_current_layer());
        if (curLayer==Layernum){
            LayerList.pop_back();
            Layernum--;
            while(!LayerList[Layernum].isvalid()){
                LayerList.pop_back();
                Layernum--;
            }  
        }
        else if(curLayer<Layernum){
            LayerList[curLayer].disable();
        }
        Flush();
}

void MainWindow::Flush(){
    if(layer_list->count()>0){
        QImage result;
        result = raw.scaled(int(raw.width() * ratio), int(raw.height() * ratio), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        label_image->setImage(result);
        label_image->resize(int(raw.width() * ratio), int(raw.height() * ratio));
        for(int i=0;i<layer_list->count();i++){
            if(layer_list->getCheckBox(i)) continue;
            curLayer =static_cast<unsigned int>(layer_list->getLayerNo(i));
            Layer l=LayerList[curLayer];
            for(unsigned int j=0;j<l.getSize();j++){
                label_image->PaintOp(l.getOp(j), ratio);
            }
        }
        int num1 = layer_list->getLayerNo(layer_list->currentRow());
        unsigned int num=static_cast<unsigned int>(num1);
        label_image->setCurrentLayer(&(LayerList[num]));
        label_image->setPenColor(color);
        label_image->setFillColor(fillColor);
        label_image->setPenWidth(penWidth);
    }
}

void MainWindow::receiveListChange(){
    this->Flush();
}

void MainWindow::selectMode(){
    label_image->setMode(PaintWidget::Choose);
}

void MainWindow::selectEraser(){
    label_image->setShape(PaintWidget::Pen);
    label_image->setErase(true);
    label_image->setMode(PaintWidget::Draw);
}

void MainWindow::padMode(int index){
    if(index == 1){
        button_fill->setEnabled(true);
        label_image->setFill(true);
    }else if(index == 0){
        button_fill->setEnabled(false);
        label_image->setFill(false);
    }
}

void MainWindow::showImageWindow(){
    dock_image->setVisible(true);
}

void MainWindow::showLayerWindow(){
    dock_layer->setVisible(true);
}
