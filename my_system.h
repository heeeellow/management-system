#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_my_system.h"
#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QPainter>
class my_system : public QMainWindow
{
    Q_OBJECT

protected:
    virtual void closeEvent(QCloseEvent* event) override;
public:
    my_system(QWidget *parent = nullptr);
    ~my_system();
   static  bool exit_flag ;
    //主窗口显示用户信息
    void dis_information();
private slots:


    void on_input_Button_clicked();

    void on_new_Button_clicked();

    void on_outfind_pushButton_clicked();

    void on_output_pushButton_clicked();

    void on_place_comboBox_activated(int index);

    void on_findRecord_pushButton_clicked();

    void on_finduser_pushButton_clicked();

    void on_newuser_pushButton_clicked();

    void on_deluser_pushButton_clicked();

    //计时
    void dis_time();


    void on_tabWidget_currentChanged(int index);

    void on_infoFind_pushButton_clicked();

    void on_infoOut_pushButton_clicked();

    void on_information_tabWidget_currentChanged(int index);

    void on_compFind_pushButton_clicked();

    void on_compOut_pushButton_clicked();

    void on_supplierOut_pushButton_clicked();

    void on_pushButton_updatemin_clicked();

    void on_pushButton_updatefind_clicked();

    void on_pushButton_update_clicked();

    void on_comboBox_updatetype_currentTextChanged(const QString &arg1);

    void on_pushButton_newsupplier_clicked();

    void on_changeComp_tabWidget_currentChanged(int index);

private:
    Ui::my_systemClass ui;
    //入库
    void input_data();
    //入库记录
    void input_record();
    void clear_input();
    //添加新元器件
    void new_comps();
    void clear_new();

    //出库
    void output_data();
    void output_find();
    void dis_label(QString name,QString num,QString min);
    void clear_output();

    //处理数据
    void process_data(QList<QList<QString>>& data, QTableWidget* table);
};

//改变tabwidget样式
class CustomTabStyle :public QProxyStyle {
public:
    CustomTabStyle() {}
    QSize sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
};
