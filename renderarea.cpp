#include "renderarea.h"

RenderArea::RenderArea(QWidget *parent) :
    QWidget(parent)
{
    //设置背景颜色和窗口大小
    QPalette p;
    p.setBrush(this->backgroundRole(),QBrush(QColor(255,255,255)));
    this->setPalette(p);
    this->setAutoFillBackground(true);
    //ui->setupUi(this);
    this->resize(QSize(400,300));
    this->list = nullptr;

}

void RenderArea::MyUpdate()
{
    update();
}

void RenderArea::GetList(LinkList<Person *> *list)
{
    this->list = list;
}

void RenderArea::GetNum(int num)
{
    this->num = num;
}

//绘制总的社会关系网络
void RenderArea::paintEvent(QPaintEvent *)
{

    if(!list)
        return;

    QPainter *QP = new QPainter(this);
    //坐标变换
    QP->translate( GRAPH_OFFSET_X , GRAPH_OFFSET_Y );

    QP->save();
    QP->translate( 0 , 0 );
    QP->setBrush(Qt::SolidPattern);

    //人物数量
    int number = list->ListLength();

    //临时变量
    Person* per;
    edge ed;

    //好友列表
    LinkList<edge>* fri;

    double deg_gap;
    if(number)
        deg_gap= 360/number;
    else
        deg_gap = 0;

    double deg = 0;
    QPoint qpoint;
    QPoint qpoint_end;


    //设置画刷，用于圆内部
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor::fromRgb(255,255,255) );

    //绘制直线与箭头
    int arrow_order = 0;
    int col;
    for(int i = 0;i<number;++i)
    {
        list->GetElem(i,&per);

        fri = per->GetFriendsList();
        col = fri->ListLength();

        for(int j = 0;j<col ;++j)
        {
            fri->GetElem(j,&ed);
            arrow_order  = ed.dest - 1;
            //绘制直线
            qpoint.setX(cos( (deg+ i*deg_gap) *PI/180 ) * DISTANCE);
            qpoint.setY(sin( (deg+ i*deg_gap) *PI/180 ) * DISTANCE);

            qpoint_end.setX(cos( (deg+ arrow_order*deg_gap) *PI/180 ) * DISTANCE);
            qpoint_end.setY(sin( (deg+ arrow_order*deg_gap) *PI/180 ) * DISTANCE);

            QP->drawLine(qpoint,qpoint_end);
        }
    }

    for(int i = 0;i<number;++i)
    {
        //外部黑色实心圆
        qpoint.setX(cos( (deg+ i*deg_gap) *PI/180 ) * DISTANCE);
        qpoint.setY(sin( (deg+ i*deg_gap) *PI/180 ) * DISTANCE);
        QP->drawEllipse(qpoint,RADIUS,RADIUS);
        //内部白色实心圆，用于擦除线条
        QP->setBrush(brush);
        QP->drawEllipse(qpoint,RADIUS-1,RADIUS-1);
        //QP->set
        QP->drawText(qpoint,iToQString(i+1));
    }


    QP->restore();
}

RenderArea::~RenderArea()
{
    delete ui;
}
