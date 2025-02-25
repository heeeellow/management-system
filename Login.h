#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class LoginD;
}

class LoginD : public QDialog
{
    Q_OBJECT

public:
    explicit LoginD(QWidget *parent = nullptr);
    ~LoginD();
    static bool admin;
    static QString user;
    static bool database_state;
    bool check_login(QString id);
    bool exit_user();
    void update_login(QString id, QString state, QString mac);
    QString getMac();

private slots:


    void on_pushButton_dataopen_clicked();

    void on_pushButton_savedata_clicked();

    void on_pushButton_login_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::LoginD *ui;
    bool link_database();
    void logins();
    void database_init();

};

#endif // LOGIN_H
