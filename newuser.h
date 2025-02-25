#ifndef NEWUSER_H
#define NEWUSER_H

#include <QDialog>

namespace Ui {
class newUser;
}

class newUser : public QDialog
{
    Q_OBJECT

public:
    explicit newUser(QWidget *parent = nullptr);
    ~newUser();
    
private slots:
    void on_addpushButton_clicked();

    void on_exitpushButton_clicked();

private:
    Ui::newUser *ui;
    void add_user();
};

#endif // NEWUSER_H
