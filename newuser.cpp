#include "newuser.h"
#include "ui_newuser.h"
#include<qmessagebox.h>
#include<qsqlquery.h>
newUser::newUser(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::newUser)
{
    ui->setupUi(this);
    
}

newUser::~newUser()
{
    delete ui;
}

void newUser::add_user()
{
    QString newid = ui->newIDlineEdit->text();
    QString newname = ui->newNamelineEdit->text();
    QString newpwd = ui->newPwdlineEdit->text();
    QString pwdok = ui->pwdOklineEdit->text();
    QString newtype = ui->newTypelineEdit->text();
    if (newid == "") {
        QMessageBox::warning(this, "警告", "工号不能为空!", QMessageBox::Yes);
        return;
    }
    if (newname == "") {
        QMessageBox::warning(this, "警告", "用户名不能为空!", QMessageBox::Yes);
        return;
    }
    if (newpwd == "") {
        QMessageBox::warning(this, "警告", "密码不能为空!", QMessageBox::Yes);
        return;
    }
    if (newtype == "") {
        QMessageBox::warning(this, "警告", "角色不能为空!", QMessageBox::Yes);
        return;
    }

    if (newpwd != pwdok) {
        QMessageBox::warning(this, "警告", "两次输入密码不一致!", QMessageBox::Yes);
        ui->newPwdlineEdit->clear();
        ui->pwdOklineEdit->clear();
        return;
    }
    else {
        QString newfind = "SELECT * FROM 用户表 WHERE 用户编号='" + newid + "';";
        QSqlQuery find;
        find.exec(newfind);
        while (find.next()) {
            if (find.value(0).toString() == newid) {
                QMessageBox::warning(this, "警告", "该用户已存在!", QMessageBox::Yes);
                return;
            }
        }
        newfind = "INSERT INTO 用户表 VALUES('" + newid + "','" + newname
            + "','" + pwdok + "','" + newtype + "','离线',NOW(),'00:00:00:00:00:00');";
        find.exec(newfind);
        QMessageBox::information(this, "提示", "添加成功!", QMessageBox::Yes);
        ui->newIDlineEdit->clear();
        ui->newNamelineEdit->clear();
        ui->newPwdlineEdit->clear();
        ui->pwdOklineEdit->clear();
        ui->newTypelineEdit->clear();
    }
}

void newUser::on_addpushButton_clicked()
{
    add_user();
}


void newUser::on_exitpushButton_clicked()
{
    this->close();
}

