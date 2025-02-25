#ifndef TABLE_H
#define TABLE_H

#include <QWidget>
namespace Ui {
class table;
}

class table : public QWidget
{
    Q_OBJECT

public:
    explicit table(QWidget *parent = nullptr);
    ~table();
    void init_table(QString find_name);
    void find_data(QString find_text);
private slots:
    void on_find_pushButton_clicked();

    void on_savealltable_pushButton_clicked();

    void on_find_comboBox_activated(int index);

private:
    Ui::table *ui;
};

#endif // TABLE_H
