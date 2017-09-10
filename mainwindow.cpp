#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
{

    m_bar = new QMenuBar;
    area = new RenderArea;
    table = nullptr;
    selected_row = -1;//初始选中人物为空标记
    friends = nullptr;
    strangers = nullptr;

    list = new  LinkList<Person *>;
    area->GetList(this->list);

    //创建菜单栏
    QMenu *menu1= new QMenu("好友推荐");
    QMenu *menu2= new QMenu("添加人物");
    QMenu *menu3= new QMenu("帮助");


    QMenu *menu1_sub1= new QMenu("根据就读学校推荐");
    menu1_sub1->addAction("余弦相似度",this,MainWindow::RecBySchCos);
    menu1_sub1->addAction("Jaccard相似度",this,MainWindow::RecBySchJac);



    QMenu *menu1_sub2= new QMenu("根据地区推荐");
    menu1_sub2->addAction("余弦相似度",this,MainWindow::RecByAreaCos);
    menu1_sub2->addAction("Jaccard相似度",this,MainWindow::RecByAreaJac);

    QMenu *menu1_sub3= new QMenu("根据公司推荐");
    menu1_sub3->addAction("余弦相似度",this,MainWindow::RecByComCos);
    menu1_sub3->addAction("Jaccard相似度",this,MainWindow::RecByComJac);


    QMenu *menu1_sub4= new QMenu("根据爱好以及团体推荐");
    menu1_sub4->addAction("余弦相似度",this,MainWindow::RecByLikeCos);
    menu1_sub4->addAction("Jaccard相似度",this,MainWindow::RecByLikeJac);

    QMenu *menu1_sub5= new QMenu("根据好友推荐");
    menu1_sub5->addAction("余弦相似度",this,MainWindow::on_bt_rec_by_fri_cos_clicked);
    menu1_sub5->addAction("Jaccard相似度",this,MainWindow::on_bt_rec_by_fri_jac_clicked);

    QMenu *menu1_sub6= new QMenu("总推荐排名");
    menu1_sub6->addAction("余弦相似度",this,MainWindow::RecCos);
    menu1_sub6->addAction("Jaccard相似度",this,MainWindow::RecJac);

    menu2->addAction("导入文件数据",this,MainWindow::on_bt_readData_clicked);
    menu2->addAction("添加人物",this,MainWindow::on_bt_addPer_clicked);

    menu3->addAction("使用方法",this,MainWindow::Help);

    menu1->addMenu(menu1_sub1);
    menu1->addMenu(menu1_sub2);
    menu1->addMenu(menu1_sub3);
    menu1->addMenu(menu1_sub4);
    menu1->addMenu(menu1_sub5);
    menu1->addMenu(menu1_sub6);

    m_bar->addMenu(menu1);
    m_bar->addMenu(menu2);
    m_bar->addMenu(menu3);

    m_bar->setStyleSheet("background-color: QColor(160,0,225);");


    //设置布局
    layout = new QGridLayout;
    setLayout(layout);

    this->resize(800,600);

    //创建表格
    QTableWidgetItem *item0;
    QString txt;
    Person *per;
    int rows = 0;
    if(list)
        rows =list->ListLength();
    table_main = new QTableWidget(rows,1);
    //mark
    table_main->setColumnWidth(1,100);
    //设置表格标题
    QStringList headLabels;
    headLabels<<"姓名";
    table_main->setHorizontalHeaderLabels(headLabels);
    for (int i = 0; i < rows; ++i)
    {
        list->GetElem(i, &per);
        txt = QString::fromStdString(per->name);
        item0->setText(txt);
        table_main->setItem(i, 1, item0);
    }

    //创建按钮
    QPushButton *bt_readData = new QPushButton;
    QPushButton *bt_writeData = new QPushButton;
    QPushButton *bt_addPer = new QPushButton;
    QPushButton *bt_addArc = new QPushButton;
    QPushButton *bt_detail= new QPushButton;

    bt_readData->setText("读取数据");
    connect(bt_readData,SIGNAL(clicked()),
                          this, SLOT(on_bt_readData_clicked())
                          );

    bt_writeData->setText("存储数据");
    connect(bt_writeData,SIGNAL(clicked()),
                          this, SLOT(on_bt_writeData_clicked())
                          );


    bt_addPer->setText("添加人物");
    connect(bt_addPer,SIGNAL(clicked()),
                          this, SLOT(on_bt_addPer_clicked())
                          );

    bt_addArc->setText("添加好友关系");
    connect(bt_addArc,SIGNAL(clicked()),
                          this, SLOT(on_bt_addArc_clicked())
                          );

    bt_detail->setText("查看详细信息");
    connect(bt_detail,SIGNAL(clicked()),
                          this, SLOT(on_bt_detail_clicked()) );
    //设置布局
    layout->addWidget(bt_readData,16,1,1,1);
    layout->addWidget(bt_writeData,16,2,1,1);
    layout->addWidget(bt_addPer,16,3,1,1);
    layout->addWidget(bt_addArc,16,4,1,1);
    layout->addWidget(bt_detail,16,5,1,1);

    layout->addWidget(area,1,1,15,25);
    layout->addWidget(m_bar,0,0,1,26);

    layout->addWidget(table_main,1,0,15,1);

    //layout->setColumnStretch(0,20);
    //设置标题
    setWindowTitle("数据结构课程设计-综合应用5");
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString iToQString(int i)
{

    char c[6];
    itoa(i,c,10);
    return QString(c);
}

void MainWindow::on_bt_readData_clicked()
{
    if(list)
        delete list;
    list = ReadData();
    area->MyUpdate();

    on_bt_detail_clicked();

    this->UpdateTable();
}
void MainWindow::on_item_double_clicked(int a,int b)
{

    Network *n = new Network;

    //将好友列表和List传入绘图类实例
    n->GetList(list);
    n->GetFriends(friends);

    //显示好友需要显示与中心人物连通的人
    //所以需要重新搜索陌生人列表,缩小范围

    if(strangers)
        delete strangers;

    strangers = new LinkList<Person *>;

    //标志位归0
    Person *p;
    for(int i = 0;i<list->ListLength();++i)
    {
        list->GetElem(i,&p);
        p->flag = 0;
    }

    //得到中心人物
    list->GetElem(a,&p);
    n->GetCenter(p);

    //根据中心人物，设置标题
    n->setTitle();
    //根据中心人物，递归寻找相关的人
    FindRelated(p);
    //递归以中心结点开始，全部加入strangers链表
    //删除中心结点
    strangers->DeFirst(p);

    n->GetStrangers(strangers);

    n->show();
}

void MainWindow::FindRelated(Person * per)
{
    if(per->flag)
        return;

    //qDebug("%d,%s\n",per->number,per->name.c_str() );

    per->flag = 1;

    if(!friends->Exist(per))
        strangers->Append_elem(per);

    LinkList<edge>* pFriends = per->GetFriendsList();
    Person *p;
    edge e;

    for(int i = 0;i<pFriends->ListLength();++i)
    {
        pFriends->GetElem(i,&e);
        list->GetElem(e.dest - 1,&p);
        if(p)
            FindRelated(p);
    }


}

void MainWindow::on_item_clicked(int a,int b)
{
    //获得人物编号
    selected_row = a;


    //创建临时变量
    Person *per;
    Person *per2;

    //清空之前的列表
    if(friends)
        delete friends;

    if(strangers)
        delete strangers;

    friends = new LinkList<Person *>;
    strangers = new LinkList<Person *>;

    //得到中心点人物
    list->GetElem(a,&per);

    LinkList<edge>* friends_list = per->GetFriendsList();

    for(int i = 0;i<list->ListLength();++i)
    {
        list->GetElem(i,&per2);
        //根据编号，判断是否为中心点人物的朋友
        if(friends_list->Exist( edge{i + 1} ) )
        {
            friends->Append_elem(per2);
        }
        else if(per->number != i + 1)
        {
            strangers->Append_elem(per2);
        }
    }

}

void MainWindow::on_bt_writeData_clicked()
{
    SaveData(list);
}

void MainWindow::on_bt_addPer_clicked()
{
    AddDialog *ad = new AddDialog;
    connect(ad,SIGNAL(send_person_data(Person*) ),
            this,SLOT(rec_person_clicked(Person*) ) );
    ad->show();
}


LinkList<Person *> * MainWindow::ReadData()
{
    fstream file;

    file.open("people.data", fstream::in);
    if (!file.is_open())
    {
        cout << "文件打开失败" << endl;
        return nullptr;
    }
    LinkList<Person *> *peopleList = new LinkList<Person *>;


    string str;
    string subs;
    string c = ",";
    string::size_type pos1, pos2;
    int tp = NAME;
    Person *p1;
    LinkList<string>* like;

    while(getline(file,str))
    {
        if (str == "#")
        {
            cout << "到达分隔" << endl;
            break;
        }
        p1 = new Person();
        like = p1->GetHobbyList();
        pos2 = str.find(c);
        pos1 = 0;
        tp = NAME;
        while (std::string::npos != pos2)
        {
            subs = str.substr(pos1, pos2 - pos1);
            switch (tp++)
            {
            case NAME:
                p1->name = subs;
                break;
            case AREA:
                p1->area = subs;
                break;
            case JUNSHC:
                p1->junSch = subs;
                break;
            case PRISCH:
                p1->priSch = subs;
                break;
            case HIGHSCH:
                p1->highSch = subs;
                break;
            case UNIV:
                p1->univ = subs;
                break;
            case COM:
                p1->com = subs;
                break;
            default:
                like->Append_elem(subs);
                break;
            }

            pos1 = pos2 + c.size();
            pos2 = str.find(c, pos1);
        }
        if (pos1 != str.length())
            like->Append_elem(str.substr(pos1));
        peopleList->Append_elem(p1);
        p1->number = peopleList->ListLength();

    }

    //读取好友关系
    int pos = 0;
    c = " ";
    LinkList<edge>* fri;
    edge e;
    while (getline(file, str))
    {
        peopleList->GetElem(pos++, &p1);
        fri = p1->GetFriendsList();

        pos2 = str.find(c);
        pos1 = 0;
        while (std::string::npos != pos2)
        {
            subs = str.substr(pos1, pos2 - pos1);
            e.dest = atoi(subs.c_str());
            fri->Append_elem(e);
            pos1 = pos2 + c.size();
            pos2 = str.find(c, pos1);
        }
        if (pos1 != str.length())
        {
            e.dest = atoi(subs.c_str());
            fri->Append_elem(e);
        }
    }
    return peopleList;
}

void MainWindow::SavePerson(fstream &file, Person *p1)
{
    file << p1->name << "," << p1->area << "," << p1->priSch << ","
        << p1->junSch << "," << p1->highSch << "," << p1->univ << ","
        << p1->com<<",";

    string str;
    LinkList<string>* like;
    like = p1->GetHobbyList();
    int length = like->ListLength();
    for (int i = 0; i < length; ++i)
    {
        like->GetElem(i, &str);
        if (i == length - 1)
            file << str;
        else
            file << str << ",";
    }
    file << endl;
}

void MainWindow::SaveData( LinkList<Person*>* list)
{
    fstream file;
    file.open("people.data", fstream::out);
    if (!file.is_open())
    {
        cout << "文件打开失败" << endl;
        return;
    }
    Person *p;
    //存储个人信息
    for (int i = 0; i < list->ListLength(); ++i)
    {
        list->GetElem(i, &p);
        SavePerson(file, p);
    }

    //作为个人信息和好友关系的分割
    file << "#" << endl;
    //存储好友关系
    LinkList<edge>* fri;
    edge e;
    for (int i = 0; i < list->ListLength(); ++i)
    {
        list->GetElem(i, &p);
        fri = p->GetFriendsList();
        for (int j = 0; j < fri->ListLength(); ++j)
        {
            fri->GetElem(j, &e);

            file << e.dest << " ";
        }
        file << endl;
    }
    file.close();
}

void MainWindow::rec_person_clicked(Person *per)
{
    //将表格新增人物添加至链表内
    list->Append_elem(&per);
    per->number = list->ListLength();
    area->MyUpdate();
    this->UpdateTable();
}
void MainWindow::UpdateTable()
{
    QTableWidgetItem* item0;
    Person *per;
    QString txt;
    //创建表格
    int rows = 0;
    if(list)
        rows =list->ListLength();
    if(table_main)
        delete table_main;

    QStringList headLabels;
    headLabels<<"姓名";

    table_main = new QTableWidget(rows,1);
    //table_main->
    table_main->setHorizontalHeaderLabels(headLabels);
    table_main->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //将表格与响应信号连接
    connect(table_main,SIGNAL(cellDoubleClicked(int,int) ),
                          this, SLOT(on_item_double_clicked(int,int) )
                          );

    connect(table_main,SIGNAL(cellClicked(int,int) ),
                          this, SLOT(on_item_clicked(int,int) )
                          );

    for (int i = 0; i < rows; ++i)
    {
        item0 = new QTableWidgetItem;

        list->GetElem(i, &per);
        txt = QString::fromStdString(per->name);
        item0->setText(txt);
        table_main->setItem(i, 0, item0);
    }
    layout->addWidget(table_main,1,0,15,1);
}


void MainWindow::on_bt_addArc_clicked()
{
    Dialog_in_ar *d = new Dialog_in_ar;
    d->set_ver_num(list->ListLength());
    connect(d,SIGNAL(send_arc_data(int,int) ),
            this,SLOT(rec_arc_data(int,int) )
                     );
    d->show();
}
void MainWindow::rec_arc_data(int a, int b)
{
    //qDebug("receive success1234");

    Person *p1,*p2;
    //只有访问的时候需要做-1映射
    list->GetElem(a - 1, &p1);
    list->GetElem(b - 1, &p2);

    LinkList<edge>* l1,* l2;

    l1 = p1->GetFriendsList();
    l2 = p2->GetFriendsList();

    l1->Append_elem(edge{b});
    l2->Append_elem(edge{a});

    area->MyUpdate();
}
void MainWindow::on_bt_detail_clicked()
{
    area->GetList(this->list);
    table = new QTableWidget(list->ListLength(),8);

    //设置为自动适应表格行列数
    table->setSizeAdjustPolicy(QTableWidget::AdjustToContents);

    //设置为不能编辑
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //设置标题
    table->setWindowTitle("详细信息");

    QStringList headLabels;
    headLabels<<"姓名"<<"区域"<<"就读小学"<<"就读初中"<<"就读高中"<<"就读大学"<<"工作单位"<<"爱好和加入的组织";
    table->setHorizontalHeaderLabels(headLabels);

    //声明临时变量
    QTableWidgetItem *item0, *item1,*item2,*item3;
    QTableWidgetItem *item4, *item5,*item6,*item7;
    QString txt;

    Person *per;

    LinkList<string>* like;
    string str;

    LinkList<edge>* fri;
    for (int i = 0; i < list->ListLength(); ++i)
    {
        item0 = new QTableWidgetItem;
        item1 = new QTableWidgetItem;
        item2 = new QTableWidgetItem;
        item3 = new QTableWidgetItem;
        item4 = new QTableWidgetItem;
        item5 = new QTableWidgetItem;
        item6 = new QTableWidgetItem;
        item7 = new QTableWidgetItem;

        list->GetElem(i, &per);
        //cout<<per->name<<endl;
        txt = QString::fromStdString(per->name);
        item0->setText(txt);
        item0->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 0, item0);


        txt = QString::fromStdString(per->area);
        item1->setText(txt);
        item1->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 1, item1);

        txt = QString::fromStdString(per->priSch);
        item2->setText(txt);
        item2->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 2, item2);

        txt = QString::fromStdString(per->junSch);
        item3->setText(txt);
        item3->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 3, item3);

        txt = QString::fromStdString(per->highSch);
        item4->setText(txt);
        item4->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 4, item4);

        txt = QString::fromStdString(per->univ);
        item5->setText(txt);
        item5->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 5, item5);

        txt = QString::fromStdString(per->com);
        item6->setText(txt);
        item6->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 6, item6);

        fri = per->GetFriendsList();
        like = per->GetHobbyList();

        txt = "";
        for (int j = 0; j < like->ListLength(); ++j)
        {
            like->GetElem(j, &str);
            txt.append(QString::fromStdString(str));
            if(j != like->ListLength() - 1)
                txt.append(",");

        }
        item7->setText(txt);
        table->setItem(i, 7, item7);
    }

    //设置列宽
    for(int i = 0;i<7;++i)
    {
        table->setColumnWidth(i,80);
    }
    table->setColumnWidth(5,120);
    table->setColumnWidth(7,200);
    table->show();
}

void MainWindow::ShowCosSimi()
{
    if(selected_row == -1)
    {
        QMessageBox::critical(this,
                       "错误" ,
                       "请先在下方单击列表选择要查看的人物",
                       QMessageBox::Ok | QMessageBox::Default ,
                       0,0 );
        return;
    }

    //根据推荐条件选出的推荐好友列表
    LinkList<Person *> *filter_strangers = new  LinkList<Person *>;

    //获取关系网所选的中心人物
    Person *center;
    list->GetElem(selected_row,&center);

    //其他人物
    Person *per;
    string keyWord;
    for(int i = 0;i<strangers->ListLength();++i)
    {
        strangers->GetElem(i,&per);
        per->value = CosSimi(center,per);

        switch(type)
        {
        case R_NAME:
            if(per->name == center->name)
                filter_strangers->Append_elem(per);
            keyWord = "姓名";
            break;
        case R_AREA:
            if(per->area == center->area)
                filter_strangers->Append_elem(per);
            keyWord = "地区";
            break;

        case R_SCH:
            if(per->priSch == center->priSch || per->junSch == center->junSch
                    || per->highSch == center->highSch)
                filter_strangers->Append_elem(per);
            keyWord = "学校";
            break;

        case R_COM:
            if(per->com == center->com)
                filter_strangers->Append_elem(per);
            keyWord = "公司";
            break;
        case R_LIKE:

            break;

        case R_ALL:
            filter_strangers->Append_elem(per);
            break;

        default:
            filter_strangers->Append_elem(per);
            break;

        }

    }

    QTableWidget *qt;
    QTableWidgetItem *item0,*item1;
    QString txt;
    string str;
    std::stringstream ss;
    //创建表格
    qt = new QTableWidget(filter_strangers->ListLength(),2);

    for (int i = 0; i < filter_strangers->ListLength(); ++i)
    {
        item0 = new QTableWidgetItem;
        item1 = new QTableWidgetItem;

        filter_strangers->GetElem(i, &per);
        txt = QString::fromStdString(per->name);
        item0->setText(txt);

        str = "";
        ss.clear();
        ss<<per->value;
        ss>>str;
        txt = QString::fromStdString(str);
        item1->setText(txt);

        qt->setItem(i, 0, item0);
        qt->setItem(i, 1, item1);
    }
    qt->sortByColumn(1,Qt::DescendingOrder);

    //设置自动适应
    qt->setSizeAdjustPolicy(QTableWidget::AdjustToContents);
    //设置无法编辑
    qt->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //设置标题
    qt->setWindowTitle(QString::fromStdString("根据"+keyWord+"推荐"+center->name+"的可能好友") );

    qt->show();

}

void MainWindow::ShowJacSimi()
{
    if(selected_row == -1)
    {
        QMessageBox::critical(this,
                       "错误" ,
                       "请先在下方单击列表选择要查看的人物",
                       QMessageBox::Ok | QMessageBox::Default ,
                       0,0 );
        return;
    }

    Person *per;
    Person *center;
    list->GetElem(selected_row,&center);


    //根据推荐条件选出目标

    LinkList<Person *> *filter_strangers = new  LinkList<Person *>;
    string keyWord;
    for(int i = 0;i<strangers->ListLength();++i)
    {

        strangers->GetElem(i,&per);
        per->value = JacSimi(center,per);
        switch(type)
        {
        case R_NAME:
            if(per->name == center->name)
                filter_strangers->Append_elem(per);
            keyWord = "姓名";
            break;
        case R_AREA:
            if(per->area == center->area)
                filter_strangers->Append_elem(per);
            keyWord = "地区";
            break;

        case R_SCH:
            if(per->priSch == center->priSch || per->junSch == center->junSch
                    || per->highSch == center->highSch)
                filter_strangers->Append_elem(per);
            keyWord = "学校";
            break;

        case R_COM:
            if(per->com == center->com)
                filter_strangers->Append_elem(per);
            keyWord = "公司";
            break;
        case R_LIKE:

            break;

        case R_ALL:
            filter_strangers->Append_elem(per);
            break;

        default:
            filter_strangers->Append_elem(per);
            break;

        }


    }

    QTableWidget *qt;
    QTableWidgetItem *item0,*item1;
    QString txt;
    string str;
    std::stringstream ss;
    //创建表格
    qt = new QTableWidget(filter_strangers->ListLength(),2);

    for (int i = 0; i < filter_strangers->ListLength(); ++i)
    {
        item0 = new QTableWidgetItem;
        item1 = new QTableWidgetItem;

        filter_strangers->GetElem(i, &per);
        txt = QString::fromStdString(per->name);
        item0->setText(txt);

        str = "";
        ss.clear();
        ss<<per->value;
        ss>>str;
        txt = QString::fromStdString(str);
        item1->setText(txt);

        qt->setItem(i, 0, item0);
        qt->setItem(i, 1, item1);
    }
    qt->sortByColumn(1,Qt::DescendingOrder);

    //设置自动适应
    qt->setSizeAdjustPolicy(QTableWidget::AdjustToContents);
    //设置无法编辑
    qt->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //设置标题
    qt->setWindowTitle(QString::fromStdString("根据"+keyWord+"推荐"+center->name+"的可能好友") );

    qt->show();
}

//计算两个人的余弦相似度
double MainWindow::CosSimi(Person *p1, Person *p2)
{
    LinkList<string>* f1 = p1->GetHobbyList();
    LinkList<string>* f2 = p2->GetHobbyList();
    LinkList<string>* f3 = f1->UnionList(*f2);

    int n1 = f1->ListLength();
    int n2 = f2->ListLength();

    //两集合交集
    int union_length = f3->ListLength();

    int inter_length = n1 + n2 - union_length;

    int simi_num = 0;

    //计算相同的属性数量

    if (p1->area == p2->area)
        ++simi_num;
    if (p1->highSch == p2->highSch)
        ++simi_num;
    if (p1->junSch == p2->junSch)
        ++simi_num;
    if (p1->priSch == p2->priSch)
        ++simi_num;
    if(p1->com == p2->com)
        ++simi_num;
    if(p1->univ == p2->univ)
        ++simi_num;

    return double(simi_num + inter_length) / (sqrt(VARIA_NUM + n1) * sqrt(VARIA_NUM + n2));
}

double MainWindow::JacSimi(Person *p1, Person *p2)
{
    //f3为f1与f2并集
    LinkList<string>* f1 = p1->GetHobbyList();
    LinkList<string>* f2 = p2->GetHobbyList();
    LinkList<string>* f3 = f1->UnionList(*f2);

    int n1 = f1->ListLength();
    int n2 = f2->ListLength();
    int union_length = f3->ListLength();

    //两集合非固定属性部分交集
    int inter_length = n1 + n2 - union_length;

    int simi_num = 0;

    //计算相同的固定属性数量

    if (p1->area == p2->area)
        ++simi_num;
    if (p1->highSch == p2->highSch)
        ++simi_num;
    if (p1->junSch == p2->junSch)
        ++simi_num;
    if (p1->priSch == p2->priSch)
        ++simi_num;
    if (p1->com == p2->com)
        ++simi_num;
    if (p1->univ == p2->univ)
        ++simi_num;

    return double(simi_num + inter_length) / (VARIA_NUM + n1 + VARIA_NUM + n2 - simi_num - inter_length);
}


void MainWindow::RecBySchJac()
{
    type = R_SCH;
    ShowJacSimi();
}

void MainWindow::RecByAreaJac()
{
    type = R_AREA;
    ShowJacSimi();
}

void MainWindow::RecByComJac()
{
    type = R_COM;
    ShowJacSimi();
}

void MainWindow::RecByLikeJac()
{
    type = R_LIKE;
    ShowJacSimi();
}

void MainWindow::RecBySchCos()
{
    type = R_SCH;
    ShowCosSimi();

}

void MainWindow::RecByAreaCos()
{
    type = R_AREA;
    ShowCosSimi();
}

void MainWindow::RecByComCos()
{
    type = R_COM;
    ShowCosSimi();
}

void MainWindow::RecByLikeCos()
{
    type = R_LIKE;
    ShowCosSimi();
}

void MainWindow::RecommendByFriends(int t)
{
    if(selected_row == -1)
    {
        QMessageBox::critical(this,
                       "错误" ,
                       "请先在下方单击列表选择要查看的人物",
                       QMessageBox::Ok | QMessageBox::Default ,
                       0,0 );
        return;
    }

    //清空陌生人列表
    if(strangers)
        delete strangers;
    strangers = new LinkList<Person *>;

    //标志位归0
    Person *p,*p2,*center;
    for(int i = 0;i<list->ListLength();++i)
    {
        list->GetElem(i,&p);
        p->flag = 0;
    }

    //从中心人物，递归连通的陌生人
    list->GetElem(selected_row,&center);
    FindRelated(center);
    //递归以中心结点开始，全部加入strangers链表
    //删除中心结点
    strangers->DeFirst(p);

    //计算共同好友数量
    edge e;
    for(int i = 0;i<strangers->ListLength();++i)
    {
        strangers->GetElem(i,&p);
        if(t == 0)
            p->value = JacSimi(p,center);
        else if(t == 1)
            p->value = JacSimi(p,center);
        else
            p->value = -1;

        p->commonfriends = 0;
        LinkList<edge>* fr = p->GetFriendsList();
        for(int j =0;j<p->GetFriendsList()->ListLength();++j)
        {
            fr->GetElem(j,&e);
            list->GetElem(e.dest - 1,&p2);
            if(friends->Exist(p2))
            {
                ++p->commonfriends;
            }
        }
    }

    QTableWidget *qt;
    Person *per;
    QTableWidgetItem *item0,*item1,*item2;
    QString txt;
    string str;
    std::stringstream ss;
    //创建表格
    qt = new QTableWidget(strangers->ListLength(),3);

    //用于int到QString转换
    char c[6];


    //绘制表格
    for (int i = 0; i < strangers->ListLength(); ++i)
    {
        item0 = new QTableWidgetItem;
        item1 = new QTableWidgetItem;
        item2 = new QTableWidgetItem;

        strangers->GetElem(i, &per);
        txt = QString::fromStdString(per->name);
        item0->setText(txt);


        //double转换为QString
        str = "";
        ss.clear();
        ss<<per->value;
        ss>>str;
        txt = QString::fromStdString(str);
        item1->setText(txt);

        itoa(per->commonfriends,c,10);
        item2->setText(c);

        qt->setItem(i, 0, item0);
        qt->setItem(i, 1, item1);
        qt->setItem(i, 2, item2);
    }
    //根据相似度排序
    qt->sortByColumn(1,Qt::DescendingOrder);

    //设置窗口大小自动适应
    qt->setSizeAdjustPolicy(QTableWidget::AdjustToContents);
    //设置无法编辑
    qt->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //设置headLabels
    QStringList headLabels;
    headLabels<<"姓名"<<"相似度"<<"共同好友数";
    qt->setHorizontalHeaderLabels(headLabels);

    qt->setWindowTitle(QString::fromStdString("根据好友推荐"+center->name+"的潜在好友") );

    qt->show();

}

void MainWindow::on_bt_rec_by_fri_cos_clicked()
{
    RecommendByFriends(0);
}

void MainWindow::on_bt_rec_by_fri_jac_clicked()
{
    RecommendByFriends(1);
}
void MainWindow::RecJac()
{
    type = R_ALL;
    ShowCosSimi();
}

void MainWindow::RecCos()
{
    type = R_ALL;
    ShowCosSimi();
}
void MainWindow::Help()
{
    QMessageBox::information(this,
                   "帮助" ,
                   "点击读取数据或者添加人物导入数据\n双击左侧表格内人物姓名查看该人物社会网络\n单机左侧姓名，再点击菜单栏好友推荐查看好友相似度和推荐",
                   QMessageBox::Ok | QMessageBox::Default ,
                   0,0 );
}
