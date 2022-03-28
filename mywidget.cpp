#include "mywidget.h"
#include <QDebug>

int ActionList::getIndex(){
    return this->index;
}

void ActionList::setIndex(int index){
    this->index = index;
}

myQList::myQList(QWidget *parent)
    :QListWidget(parent)
{
    this->setViewMode(QListView::ListMode);
    this->setDragEnabled(true);
    this->setDragDropMode(QAbstractItemView::DragDrop);
    this->setDefaultDropAction(Qt::MoveAction);
    this->setAcceptDrops(true);
}

myQList::~myQList()
{

}

void myQList::dropEvent(QDropEvent *event){
    QListWidget::dropEvent(event);
    listChanged();
}

void myQList::add_layer(int num){
    QListWidgetItem *item;
    item = new QListWidgetItem();
    this->addItem(item);
    myLayerListItem *w = new myLayerListItem("图层"+QString::number(num), num);
    QSize s = item->sizeHint();
    w->resize(s.width(), s.height());
    this->setItemWidget(item, w);
    this->setCurrentItem(item);
    connect(w->layer_show, &QCheckBox::stateChanged, this, &myQList::viewChange);
}

int myQList::delete_current_layer(){
    QListWidgetItem *item = this->currentItem();
    myLayerListItem *w = dynamic_cast<myLayerListItem *>(this->itemWidget(item));
    if(w){
        int No = w->getNo();
        int row = this->currentRow();
        this->removeItemWidget(item);
        this->takeItem(row);
        //this->setCurrentItem(this->item(this->count() - 1));
        return No;
    }else{
        qDebug() << "Pointer transformation illegal" << endl;
        return -1;
    }
}

int myQList::getLayerNo(int index){
    QListWidgetItem *item = this->item(index);
    myLayerListItem *w = dynamic_cast<myLayerListItem*>(this->itemWidget(item));
    return w->getNo();
}

bool myQList::getCheckBox(int index){
    QListWidgetItem *item = this->item(index);
    myLayerListItem *w = dynamic_cast<myLayerListItem*>(this->itemWidget(item));
    return w->getCheck();
}

void myQList::viewChange(){
    listChanged();
}

myLayerListItem::myLayerListItem(QString name, int No){
    QHBoxLayout *layout = new QHBoxLayout;
    QLabel *layer_name = new QLabel();
    layer_name->setText(name);
    layer_show = new QCheckBox;
    layout->addWidget(layer_name);
    layout->addWidget(layer_show);
    layout->setContentsMargins(1, 0, 0, 1);
    this->setLayout(layout);
    this->No = No;
}

int myLayerListItem::getNo(){
    return this->No;
}

bool myLayerListItem::getCheck(){
    return layer_show->isChecked();
}
