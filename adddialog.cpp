#include "adddialog.h"
#include "ui_adddialog.h"

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);


}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::on_pushButton_clicked()
{
    //获取文本框内容
    std::string name = ui->lineEdit_name->text().toStdString();
    std::string area = ui->lineEdit_area->text().toStdString();
    std::string pri = ui->lineEdit_pri->text().toStdString();
    std::string jun = ui->lineEdit_jun->text().toStdString();
    std::string high = ui->lineEdit_high->text().toStdString();
    std::string univ = ui->lineEdit_univ->text().toStdString();
    std::string work = ui->lineEdit_work->text().toStdString();
    std::string like = ui->lineEdit_like->text().toStdString();
    //判断是否为空
    if(name.empty() || area.empty() || pri.empty() ||
       jun.empty() || high.empty() || univ.empty() ||
       work.empty() || like.empty() )
    {
        QMessageBox::critical(this,
                       "输入错误" ,
                       "存在空内容，请输入完整",
                       QMessageBox::Ok | QMessageBox::Default ,
                       0,0 );
        return;
    }
    Person *per = new Person(name,area,pri,jun,high,univ,
                             work);

    string::size_type pos1, pos2;
    string c = ",";
    string subs;
    pos2 = like.find(c);
    pos1 = 0;
    LinkList<string>* likelist = per->GetHobbyList();

    while (std::string::npos != pos2)
    {
        subs = like.substr(pos1, pos2 - pos1);
        likelist->Append_elem(subs);

        pos1 = pos2 + c.size();
        pos2 = like.find(c, pos1);
    }
    if (pos1 != like.length())
        likelist->Append_elem(like.substr(pos1));

    emit(send_person_data(per));
    close();

}

void AddDialog::on_pushButton_2_clicked()
{
    close();
}
