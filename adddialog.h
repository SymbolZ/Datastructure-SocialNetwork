#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QMessageBox>

#include "person.h"

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = 0);
    ~AddDialog();

private slots:

    void on_pushButton_clicked();//确认按钮

    void on_pushButton_2_clicked();//取消按钮

private:
    Ui::AddDialog *ui;

signals:
    void send_person_data(Person *p);
};

#endif // ADDDIALOG_H
