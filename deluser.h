#ifndef DELUSER_H
#define DELUSER_H

#include <QDialog>

namespace Ui {
class deluser;
}

class deluser : public QDialog
{
    Q_OBJECT

public:
    explicit deluser(QWidget *parent = nullptr);
    ~deluser();

private slots:
    void on_delfind_pushButton_clicked();

    void on_DEL_pushButton_clicked();

    void on_exit_pushButton_clicked();

private:
    Ui::deluser *ui;
    bool maybe_del();
    void del_user();
};

#endif // DELUSER_H
