#ifndef NETWORK_H
#define NETWORK_H

#include <QWidget>
#include <QPainter>


#include "linklist.h"
#include "person.h"

const int FRIENDS_OFFSET_X = 400;
const int FRIENDS_OFFSET_Y = -50;

const int STRANGER_OFFSET_X = 400;
const int STRANGER_OFFSET_Y = 30;



namespace Ui {
class Network;
}

class Network : public QWidget
{
    Q_OBJECT

public:
    explicit Network(QWidget *parent = 0);
    ~Network();
    //更新绘图
    void MyUpdate();

    //获得绘图数据
    void GetList(LinkList<Person *> *list);
    void GetFriends(LinkList<Person *> *friends);
    void GetStrangers(LinkList<Person *> *strangers);
    void GetCenter(Person *center);

    //设置标题
    void setTitle();

private:
    Ui::Network *ui;

    Person *center;
    LinkList<Person *> *list;//记录社会关系网络
    LinkList<Person *> *friends;//记录好友
    LinkList<Person *> *strangers;//记录陌生人

    //得到某人位于图上的位置
    QPoint GetPos(Person *per);

    //绘制一个结点
    void DrawNode(QPainter *pt,QPoint qpoint,int number);
    //根据中心人物设置标题

protected:
    void paintEvent(QPaintEvent *);
};

#endif // NETWORK_H
