#include "network.h"
#include "ui_network.h"
const int RADIUS = 10;
const int DISTANCE = 300;
const int PI=3.1415926;


Network::Network(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Network)
{
    //设置背景颜色和窗口大小
    QPalette p;
    p.setBrush(this->backgroundRole(),QBrush(QColor(255,255,255)));
    this->setPalette(p);
    this->setAutoFillBackground(true);
    //ui->setupUi(this);
    this->resize(QSize(800,500));
    //类成员初始化
    this->list = nullptr;
    friends = nullptr;
    strangers = nullptr;

}

void Network::MyUpdate()
{
    update();
}

void Network::GetList(LinkList<Person *> *list)
{
    this->list = list;
}

Network::~Network()
{
    delete ui;
}

void Network::paintEvent(QPaintEvent *)
{
    if(!list)
        return;

    QPainter *QP = new QPainter(this);
    //坐标变换
    //QP->translate( GRAPH_OFFSET_X , GRAPH_OFFSET_Y );

    QP->save();
    QP->translate( 0 , 0 );
    QP->setBrush(Qt::SolidPattern);


    //好友列表
    LinkList<edge>* fri;


    QPoint qpoint;
    QPoint qpoint_end;

    //临时变量
    edge ed;
    Person *per;
    Person *per2;



    //绘制直线与箭头
    int arrow_order = 0;
    int col;
    for(int i = 0;i<friends->ListLength();++i)
    {
        friends->GetElem(i,&per);

        fri = per->GetFriendsList();
        col = fri->ListLength();

        for(int j = 0;j<col ;++j)
        {
            fri->GetElem(j,&ed);
            arrow_order  = ed.dest - 1;
            list->GetElem(arrow_order,&per2);

            //绘制直线
            qpoint = GetPos(per);
            qpoint_end = GetPos(per2);

            QP->drawLine(qpoint,qpoint_end);
        }
    }

    for(int i = 0;i<strangers->ListLength();++i)
    {
        strangers->GetElem(i,&per);

        fri = per->GetFriendsList();
        col = fri->ListLength();

        for(int j = 0;j<col ;++j)
        {
            fri->GetElem(j,&ed);
            arrow_order  = ed.dest - 1;
            list->GetElem(arrow_order,&per2);

            //绘制直线
            qpoint = GetPos(per);
            qpoint_end = GetPos(per2);

            QP->drawLine(qpoint,qpoint_end);
        }
    }

    //绘制陌生人
    for(int i = 0;i<strangers->ListLength();++i)
    {
        //获取位置
        strangers->GetElem(i,&per);
        qpoint = GetPos(per);

        DrawNode(QP,qpoint,per->number);
    }

    //绘制好友
    for(int i = 0;i<friends->ListLength();++i)
    {
        //获取位置
        friends->GetElem(i,&per);
        qpoint = GetPos(per);

        DrawNode(QP,qpoint,per->number);
    }

    qpoint = GetPos(this->center);
    DrawNode(QP,qpoint,this->center->number);

    qpoint.setY(qpoint.y() - RADIUS -3);
    qpoint.setX(qpoint.x() - RADIUS -3);
    QP->drawText(qpoint,QString::fromStdString(center->name) );

    QP->restore();
}

void Network::DrawNode(QPainter *pt,QPoint qpoint,int number)
{
    pt->save();



    //设置画刷，用于圆内部
    static QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor::fromRgb(255,255,255) );

    //外部黑色实心圆
    pt->drawEllipse(qpoint,RADIUS,RADIUS);

    //内部白色实心圆，用于擦除线条
    pt->setBrush(brush);
    pt->drawEllipse(qpoint,RADIUS-1,RADIUS-1);

    //绘制圆内数字
    char c[6];
    itoa(number,c,10);

    qpoint.setX(qpoint.x() - 3);
    qpoint.setY(qpoint.y() + 3);
    pt->drawText(qpoint,QString(c));

    pt->restore();
}


QPoint Network::GetPos(Person *per)
{
    //返回朋友位置
    Person *p;
    QPoint qpoint;
    const double deg = 50;//50度到130度
    double _deg_gap;
    if(friends->Exist(per))
    {
        _deg_gap = (90.0-deg)*2/(friends->ListLength()-1);
        for(int i = 0;i<friends->ListLength();++i)
        {
            friends->GetElem(i,&p);

            //找到对应的朋友位置
            if(p == per)
            {
                qpoint.setX(FRIENDS_OFFSET_X +
                            cos( (deg+ i*_deg_gap) *PI/180 ) * DISTANCE);
                qpoint.setY(FRIENDS_OFFSET_Y +
                            sin( (deg+ i*_deg_gap) *PI/180 ) * DISTANCE);
                return qpoint;
            }
        }
    }
    //返回陌生人位置
    else if(strangers->Exist(per))
    {
        _deg_gap = (90.0-deg)*2/(strangers->ListLength()-1);
        for(int i = 0;i<strangers->ListLength();++i)
        {
            strangers->GetElem(i,&p);

            //找到对应的朋友位置
            if(p == per)
            {
                qpoint.setX(STRANGER_OFFSET_X +
                            cos( (deg+ i*_deg_gap) *PI/180 ) * DISTANCE * 1.2);
                qpoint.setY(STRANGER_OFFSET_Y +
                            sin( (deg+ i*_deg_gap) *PI/180 ) * DISTANCE * 1.2);
                return qpoint;
            }
        }
    }

    //返回中心位置
    return QPoint(400,100);
}

void Network::GetFriends(LinkList<Person *> *friends)
{
    if(this->friends)
        delete this->friends;

    this->friends = new LinkList<Person *>(*friends);
}

void Network::GetStrangers(LinkList<Person *> *strangers)
{
    if(this->strangers)
        delete this->strangers;

    this->strangers = new LinkList<Person *>(*strangers);
}

void Network::GetCenter(Person *center)
{
    this->center = center;
}
void Network::setTitle()
{
    this->setWindowTitle(QString::fromStdString(center->name + "的社交网络") );
}
