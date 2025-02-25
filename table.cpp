#include "table.h"
#include "ui_table.h"
#include<qsqlquery.h>
#include<qmessagebox.h>
#include<qfiledialog.h>
#include"table_control.h"
#include<qlist.h>
table::table(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::table)
{
    ui->setupUi(this);
    
}

table::~table()
{
    delete ui;
}

void table::on_find_pushButton_clicked()
{
    find_data(ui->find_comboBox->currentText());
}



void table::on_savealltable_pushButton_clicked()
{
    QString filename= QFileDialog::getSaveFileName(this, "另存为", this->windowTitle(), "(*.xlsx);;(*.xls)");
    if (!filename.isEmpty()) {
        QList<QList<QString>> data;
        data.resize(ui->record_tableWidget->rowCount() + 1);

        for (int i = 0; i < ui->record_tableWidget->rowCount() + 1; i++) {
            data[i].resize(ui->record_tableWidget->columnCount());
            for (int j = 0; j < ui->record_tableWidget->columnCount(); j++) {
                if (i == 0) {
                    data[i][j] = ui->record_tableWidget->horizontalHeaderItem(j)->text();
                }
                else {
                    data[i][j] = ui->record_tableWidget->item(i - 1, j)->text();
                }
            }
        }
        
        QString title = this->windowTitle();
        table_control tab;
        tab.output_table(filename, title, data);
        QMessageBox::information(this, "提示", "导出成功!", QMessageBox::Yes);
    }
}

void table::init_table(QString find_name)
{
    QStringList str;
   
    if (find_name == "入库记录表") {
        str << "全部" << "元器件编号" << "存储位置" << "入库日期"
            << "操作员" << "备注";
    }
    else if (find_name == "出库记录表") {
        str << "全部" << "元器件编号" << "存储位置" << "出库日期"
            << "操作员" << "接收人" << "备注";
    }
    ui->find_comboBox->addItems(str);
  
    QString find_text = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='"
        + find_name + "';";
    QStringList header;
    QSqlQuery find;
    find.exec(find_text);
    while (find.next()) {
        header << find.value(0).toString();
    }
    ui->record_tableWidget->setColumnCount(find.size());
    ui->record_tableWidget->setHorizontalHeaderLabels(header);
    //隔行交叉颜色
    //ui->record_tableWidget->setAlternatingRowColors(true);
    //选择一行
    ui->record_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    //无法编辑
    ui->record_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //标签水平拓展
    ui->record_tableWidget->horizontalHeader()->setStretchLastSection(true);
    //列宽自动适应
    ui->record_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void table::find_data(QString find_type)
{
    QString table_name = this->windowTitle();
    QString str;
    if (find_type == "全部") {
        str = "SELECT*FROM " + table_name + ";";
    }
    else {
        str = "SELECT*FROM " + table_name + " WHERE " + find_type
            + "='" + ui->find_lineEdit->text() + "';";
    }
    QSqlQuery find_Data;
    find_Data.exec(str);
    ui->record_tableWidget->setRowCount(find_Data.numRowsAffected());

    int i = 0;
    while (find_Data.next()) {
        for (int j = 0; j < ui->record_tableWidget->columnCount(); j++) {
            QString temp = find_Data.value(j).toString();
            ui->record_tableWidget->setItem(i, j, new QTableWidgetItem(temp));
            ui->record_tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
        i++;
    }
    if (i> 0) {
        QMessageBox::information(this, "提示", "查询成功!", QMessageBox::Yes);
    }
    else {
        QMessageBox::information(this, "提示", "未查询到内容!", QMessageBox::Yes);
        int temp= ui->record_tableWidget->rowCount();
        //清除表格内容
        while (temp--) {
            ui->record_tableWidget->removeRow(temp);
        }
    }
    ui->find_lineEdit->clear();
}


void table::on_find_comboBox_activated(int index)
{
    if (ui->find_comboBox->currentText() == "入库日期"
        || ui->find_comboBox->currentText() == "出库日期") {
        ui->find_lineEdit->setPlaceholderText("XXXX-XX-XX");
    }
}

