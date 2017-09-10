#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QWidget>
#include <QPainter>
#include <cmath>
#include <QMessageBox>
#include "linklist.h"
#include "person.h"

//图的半径、偏移量、距离中心长度
const int RADIUS = 20;
const int DISTANCE = 130;
const double PI = 3.1415926;
const int GRAPH_OFFSET_X = 300;
const int GRAPH_OFFSET_Y = 200;

namespace Ui {
class RenderArea;
}

class RenderArea : public QWidget
{
    Q_OBJECT

public:
    explicit RenderArea(QWidget *parent = 0);
    ~RenderArea();

    void MyUpdate();
    void GetList(LinkList<Person *> *list);
    void GetNum(int num);
private:
    Ui::RenderArea *ui;
    LinkList<Person *> *list;//记录社会关系网络
    int num;//记录当前绘制的社会网络人物编号
protected:
    void paintEvent(QPaintEvent *);
};


#endif // RENDERAREA_H
