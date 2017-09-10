#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QGridLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QMenuBar>
#include <iostream>
#include <sstream>

#include "adddialog.h"
#include "linklist.h"
#include "person.h"
#include "def.h"
#include "renderarea.h"
#include "dialog_in_ar.h"
#include "network.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    //界面和布局
    Ui::MainWindow *ui;
    QGridLayout *layout;

    //好友推荐类型
    REC_TYPE type;

    //详细信息列表
    QTableWidget  *table;
    //主界面列表
    QTableWidget  *table_main;

    //绘图区域
    RenderArea *area;

    //菜单栏
    QMenuBar *m_bar;

    //更新主界面表格
    void UpdateTable();

    //读取存储数据函数
    LinkList<Person *> * ReadData();
    void SavePerson(fstream &file, Person *p1);
    void SaveData( LinkList<Person*>* list);

    //邻接链表
    LinkList<Person *> *list;
    LinkList<Person *> *friends;
    LinkList<Person *> *strangers;

    //寻找相关的人
    void FindRelated(Person * per);

    //相关度计算函数
    double CosSimi(Person *p1, Person *p2);
    double JacSimi(Person *p1, Person *p2);

    int selected_row;

    //好友推荐窗口创建函数
    void ShowCosSimi();
    void ShowJacSimi();

    //根据Jaccard系数推荐好友
    void RecBySchJac();
    void RecByAreaJac();
    void RecByComJac();
    void RecByLikeJac();
    void RecJac();

    //根据Cos系数推荐好友
    void RecBySchCos();
    void RecByAreaCos();
    void RecByComCos();
    void RecByLikeCos();
    void RecCos();


    //根据好友推荐
    void RecommendByFriends(int t);

private slots:
    //数据存取、添加好友、弧按钮响应函数
    void on_bt_readData_clicked();
    void on_bt_writeData_clicked();
    void on_bt_addPer_clicked();
    void on_bt_addArc_clicked();

    //列表响应函数
    void on_item_double_clicked(int a,int b);
    void on_item_clicked(int a,int b);

    //显示详细信息相应函数
    void on_bt_detail_clicked();

    //推荐相应函数
    void on_bt_rec_by_fri_cos_clicked();
    void on_bt_rec_by_fri_jac_clicked();

    //帮助栏
    void Help();

public slots:
    void rec_person_clicked(Person *per);
    void rec_arc_data(int , int );

};

#endif // MAINWINDOW_H
