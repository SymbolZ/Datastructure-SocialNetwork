#include "dialog_in_ar.h"
#include "ui_dialog_in_ar.h"

Dialog_in_ar::Dialog_in_ar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_in_ar)
{
    ui->setupUi(this);
    this->setWindowTitle("添加弧");
}

Dialog_in_ar::~Dialog_in_ar()
{
    delete ui;
}




void Dialog_in_ar::on_pushButton_clicked()
{
    //获取文本框内容
    std::string start = ui->lineEdit->text().toStdString();
    std::string end = ui->lineEdit_2->text().toStdString();
    //判断是否为空
    if(start.empty() || end.empty()  )
    {
        QMessageBox::critical(this,
                       "输入错误" ,
                       "存在空内容，请输入完整",
                       QMessageBox::Ok | QMessageBox::Default ,
                       0,0 );
        return;

    }

        //判断输入内容是否为数字
    for(unsigned int i = 0;i<start.length();++i)
    {
        if(start[i]>='0'&&start[i]<='9')
            continue;
        QMessageBox::critical(this,
                       "输入错误" ,
                       "请输入数字",
                       QMessageBox::Ok | QMessageBox::Default ,
                       0,0 );
        return;

    }
    for(unsigned int i = 0;i<end.length();++i)
    {
        if(end[i]>='0'&&end[i]<='9')
            continue;
        QMessageBox::critical(this,
                       "输入错误" ,
                       "请输入数字",
                       QMessageBox::Ok | QMessageBox::Default ,
                       0,0 );
        return;

    }

    int i_s = std::stoi(start);
    int i_e = std::stoi(end);

    //判断输入内容是否越界
    if(i_s<1 || i_s> ver_num
       || i_e<1 || i_e> ver_num )
    {
        QMessageBox::critical(this,
                       "输入错误" ,
                       "输入越界,请输入范围内顶点编号",
                       QMessageBox::Ok | QMessageBox::Default ,
                       0,0 );
        return;
    }
    emit( send_arc_data(i_s,i_e) );
    qDebug("OK clicked\n");
    //将数据回传


    close();
}

void Dialog_in_ar::on_pushButton_2_clicked()
{
    close();
}

void Dialog_in_ar::set_ver_num(int i)
{
    ver_num = i;
}
