#include "deluser.h"
#include "ui_deluser.h"
#include<qsqlquery.h>
#include<qmessagebox.h>
deluser::deluser(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::deluser)
{
    ui->setupUi(this);
}

deluser::~deluser()
{
    delete ui;
}

void deluser::on_delfind_pushButton_clicked()
{
    QString id = ui->delID_lineEdit->text();
    QString findstr = "SELECT *FROM 用户表 WHERE 用户编号='" + id + "';";
    QSqlQuery find;
    find.exec(findstr);
    find.next();
    if (find.value(0).toString() == "") {
        QMessageBox::information(this, "提示", "未找到该用户!");
        ui->delID_lineEdit->clear();
        return;
    }
    else {
        ui->ID_label->setText("工号：" + find.value("用户编号").toString());
        ui->name_label->setText("用户名：" + find.value("用户名").toString());
        ui->type_label->setText("角色：" + find.value("角色").toString());
    }
}


void deluser::on_DEL_pushButton_clicked()
{
    if (maybe_del()) {
        del_user();
    }
}


void deluser::on_exit_pushButton_clicked()
{
    this->close();
}

bool deluser::maybe_del()
{
    QMessageBox maybe;
    maybe.setWindowTitle("警告");
    maybe.setIcon(QMessageBox::Warning);
    maybe.setText("确认删除该用户吗？");
   QPushButton*yes= maybe.addButton("确定", QMessageBox::YesRole);
   maybe.addButton("取消", QMessageBox::NoRole);
 maybe.exec();
    if (maybe.clickedButton()==(QAbstractButton*)yes) {
        return true;
    }
    return false;
}

void deluser::del_user()
{
    QString id = ui->delID_lineEdit->text();
    if (id == "000") {
        QMessageBox::warning(this, "提示", "无法删除超级管理员", QMessageBox::Ok);
        return;
    }
    QString delstr = "DELETE FROM 用户表 WHERE 用户编号='" + id + "';";
    QSqlQuery del;
    del.exec(delstr);
    QMessageBox::information(this, "提示", "删除成功!");
    ui->delID_lineEdit->clear();
    ui->ID_label->setText("工号：" );
    ui->name_label->setText("用户名：");
    ui->type_label->setText("角色：" );
}

