#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QListWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QAction>

class myQList : public QListWidget{
    Q_OBJECT
public:
    myQList(QWidget* parent);
    ~myQList();
    void add_layer(int num);
    int delete_current_layer();
    int getLayerNo(int index);
    bool getCheckBox(int index);
    void viewChange();

protected:
    void dropEvent(QDropEvent *event);

signals:
    void listChanged();
};

class myLayerListItem : public QWidget{
    Q_OBJECT
public:
    myLayerListItem(QString name, int No);
    int getNo();
    bool getCheck();
    QCheckBox *layer_show;

private:
    int No;
};

class ActionList : public QAction{
    Q_OBJECT

public:
    using QAction::QAction;
    void setIndex(int index);
    int getIndex();

private:
    int index;
};

#endif // MYWIDGET_H
