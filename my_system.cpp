#include "my_system.h"
#include<qsqlquery.h>
#include<qmessagebox.h>
#include"Login.h"
#include<qtabwidget.h>
#include"table.h"
#include<qtimer.h>
#include"newuser.h"
#include"deluser.h"
#include<qfiledialog.h>
#include"table_control.h"
#include <QCloseEvent> 
void my_system::closeEvent(QCloseEvent* event)
{
        QMessageBox maybe;
        maybe.setWindowTitle("提示");
        maybe.setIcon(QMessageBox::Information);
        maybe.setText("确定要退出系统吗?");
        QPushButton* yes = maybe.addButton("确定", QMessageBox::YesRole);
        maybe.addButton("取消", QMessageBox::NoRole);
        maybe.exec();
        if (maybe.clickedButton() == (QAbstractButton*)yes) {
            LoginD close;
            close.update_login(LoginD::user, "离线", "00:00:00:00:00:00");
            event->accept();
        }
        else {
            event->ignore();
        }
    
}

my_system::my_system(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    this->setWindowTitle("电子元器件仓库管理系统");
    ui.record_comboBox->addItem("出库记录表");
    ui.record_comboBox->addItem("入库记录表");
   // ui.tabWidget->setTabPosition(QTabWidget::West);
    ui.tabWidget->tabBar()->setStyle(new CustomTabStyle);
    //设置验证器为纯数字输入
    ui.lineEdit_minsize->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]+$")));
    ui.outnum_lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]+$")));
    ui.inputnum_lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("[0-9]+$")));
}

my_system::~my_system()
{}

void my_system::on_input_Button_clicked()
{
    input_data();
}


void my_system::on_new_Button_clicked()
{
    new_comps();
}

void my_system::input_data()
{
    QString id = ui.inputid_lineEdit->text();
    int num = ui.inputnum_lineEdit->text().toInt();
    QString unit = ui.inputunit_lineEdit->text();
    QString placce = ui.inputplace_lineEdit->text();
    if (id == "" || unit == "" || placce == "" || num < 1) {
        QMessageBox::warning(this, "警告", "请正确填写入库信息!", QMessageBox::Ok);
        return;
    }
    QString idfind = "SELECT * FROM 元器件表 WHERE 元器件编号='" + id + "';";
    QSqlQuery input;
    input.exec(idfind);
    //查看元器件表中是否有相关元器件信息
    input.next();
        if (input.value("元器件编号").toString() == id) {
            idfind = "SELECT * FROM 库存表 WHERE 元器件编号='" + id + "';";
            input.exec(idfind);
            while (input.next()) {
                //库存表中相同存储位置有该元器件则更新
                if (input.value("元器件编号").toString() == id && input.value("存储位置").toString() == placce) {
                    int temp = num + input.value("库存数量").toInt();
                    QString num = QString::number(temp);
                    QString updata = "UPDATE 库存表 SET 库存数量="
                        + num + ",最后更新时间=DATE_FORMAT(NOW(),'%Y-%m-%d %H:%i:%s'),单位='" 
                        + unit + "' WHERE 存储位置 = '" + placce + "';";
                    input.exec(updata);
                    QMessageBox::information(this, "提示", "入库成功", QMessageBox::Ok);
                    input_record();
                    clear_input();
                    return;
                }
            }
                    //否则插入
                    QString temp = QString::number(num);
                    QString insert = "INSERT INTO 库存表(元器件编号,库存数量,存储位置,最后更新时间,单位) VALUES('"
                        + id + "'," + temp + ",'" + placce + "',DATE_FORMAT(NOW(),'%Y-%m-%d %H:%i:%s'),'" + unit + "');";
                    input.exec(insert);
                    QMessageBox::information(this, "提示", "入库成功", QMessageBox::Ok);
                    input_record();
                    clear_input();
                
   }
   else {
      int ret= QMessageBox::warning(this, "提示", "未找到该元器件信息，是否添加元器件?", QMessageBox::Yes|QMessageBox::No);
      if (ret == QMessageBox::Yes) {
          ui.tabWidget->setCurrentIndex(4);
      }
       clear_input();
    }
}

void my_system::input_record()
{
    QString input_id = ui.inputid_lineEdit->text();
    QString  num = ui.inputnum_lineEdit->text();
    QString note = ui.note_lineEdit->text();
    QString use = LoginD::user;
    QString placce = ui.inputplace_lineEdit->text();
    QString ID =  "DATE_FORMAT(NOW(),'%y%m%d%H%i%s')";
    QSqlQuery record;
    QString temp = "INSERT INTO 入库记录表 VALUES("
        + ID + ",'" + input_id + "'," + num +",'"+placce + "',CURDATE(),'" + use + "','" + note + "');";
    record.exec(temp);
}

void my_system::clear_input()
{
    ui.inputid_lineEdit->clear();
    ui.inputnum_lineEdit->clear();
    ui.inputplace_lineEdit->clear();
    ui.inputunit_lineEdit->clear();
}

void my_system::new_comps()
{

    QString newid = ui.newID_lineEdit->text();
    QString newname = ui.newName_lineEdit->text();
    QString newtype = ui.newType_lineEdit->text();
    QString newmaker = ui.newMaker_lineEdit->text();
    QString newsupplierid = ui.newsupplierID_lineEdit_2->text();
    QString newnote = ui.newnote_lineEdit->text();

    if (newid == "" || newname == "" || newtype == "" || newmaker == ""
        || newsupplierid == "" ) {
        QMessageBox::warning(this, "提示", "请检查填写信息格式是否正确!",QMessageBox::Ok);
        return;
    }
    QString insert_comp = "INSERT INTO 元器件表 VALUES('" + newid + "','" + newsupplierid + "','"
        + newname + "','" + newtype + "','" + newmaker + "','" + newnote + "');";

    QString idfind = "SELECT * FROM 元器件表 WHERE 元器件编号='" + newid + "';";
  
    
    QSqlQuery new_comp;
    
    new_comp.exec(idfind);
    new_comp.next();
    if (new_comp.value("元器件编号").toString() == newid) {
        QMessageBox::warning(this, "提示", "该元器件已存在!", QMessageBox::Ok);
        clear_new();
    }
    else {
        new_comp.exec(insert_comp);
        QMessageBox::information(this, "提示", "添加成功!", QMessageBox::Ok);
        idfind= "SELECT * FROM 供应商表 WHERE 供应商编号='" + newsupplierid + "';";
        new_comp.exec(idfind);
        new_comp.next();
        if (new_comp.value(0).toString() == "") {
            QMessageBox::information(this, "提示", "未查询到该元器件的供应商,请添加!", QMessageBox::Ok);
            ui.changeComp_tabWidget->setCurrentIndex(1);
        }
    clear_new();
    }
}

void my_system::clear_new()
{
    ui.newID_lineEdit->clear();
    ui.newLinkman_lineEdit->clear();
    ui.newMail_lineEdit->clear();
    ui.newMaker_lineEdit->clear();
    ui.newName_lineEdit->clear();
    ui.newPhone_lineEdit->clear();
    ui.newSite_lineEdit->clear();
    ui.newsupplierID_lineEdit->clear();
    ui.newsupplierName_lineEdit->clear();
    ui.newType_lineEdit->clear();
}

void my_system::output_data()
{
    QString outID = ui.outputID_lineEdit->text();
    int maxnum = ui.outnum_label->text().toInt();
    int minnum = ui.outminnum_label->text().toInt();
    QString outnum = ui.outnum_lineEdit->text();
    QString recipient = ui.recipient_lineEdit->text();
    QString outnote = ui.outnote_lineEdit->text();
    QString place = ui.place_comboBox->currentText();
    if (outnum == "") {
        QMessageBox::warning(this, "警告", "请填写出库数量!", QMessageBox::Ok);
        return;
    }
    if (recipient == "") {
        QMessageBox::warning(this, "警告", "请填写接收人!", QMessageBox::Ok);
        return;
    }
    if (maxnum - outnum.toInt() < minnum) {
        QMessageBox::warning(this, "警告", "库存不足!", QMessageBox::Ok);
        return;
    }
    else {
        QString lastnum = QString::number(maxnum - outnum.toInt());
        QString updata = "UPDATE 库存表 SET 库存数量=" + lastnum
            + ",最后更新时间=NOW() WHERE 存储位置='" + place + "';";
        QString ID = "DATE_FORMAT(NOW(),'%y%m%d%H%i%s')";
        QString insert = "INSERT INTO 出库记录表 VALUES(" + ID + ",'" + outID + "','"
            + outnum + "','" + place + "',CURDATE(),'" + LoginD::user + "','" + recipient + "','" + outnote + "');";
        QSqlQuery output;
        output.exec(updata);
        output.exec(insert);
        clear_output();
        QMessageBox::information(this, "提示", "出库成功!", QMessageBox::Ok);
    }
}

void my_system::output_find()
{
 
    if(ui.outputID_lineEdit->text().isEmpty()){
        QMessageBox::warning(this, "警告", "编号不能为空!", QMessageBox::Ok);
    }
    else {
        QString outID = ui.outputID_lineEdit->text();
        QString find= "SELECT * FROM 库存表 WHERE 元器件编号='" + outID + "';";
        QSqlQuery findID;
        findID.exec(find);
        ui.place_comboBox->clear();
        while (findID.next()) {
            ui.place_comboBox->addItem(findID.value("存储位置").toString());
        }
        QString text = ui.place_comboBox->currentText();
        if (text == "") {
            QMessageBox::warning(this, "警告", "未找到相关元器件!", QMessageBox::Ok);
            ui.outputID_lineEdit->clear();
        }
        else {
            QString dis = "SELECT *FROM 元器件表,库存表 WHERE 元器件表.元器件编号=库存表.元器件编号 AND 库存表.存储位置='"
                + text + "' AND 元器件表.元器件编号='" + outID + "';";
            findID.exec(dis);
            findID.next();
            dis_label(findID.value("元器件名称").toString(), findID.value("库存数量").toString(), findID.value("最低库存").toString());
        }
    }
}

void my_system::dis_label(QString name, QString num, QString min)
{
    ui.outname_label->clear();
    ui.outnum_label->clear();
    ui.outminnum_label->clear();

    ui.outname_label->setText(name);
    ui.outnum_label->setText(num);
    ui.outminnum_label->setText(min);
}

void my_system::clear_output()
{
    ui.outputID_lineEdit->clear();
    ui.place_comboBox->clear();

    ui.outname_label->clear();
    ui.outnum_label->clear();
    ui.outminnum_label->clear();

    ui.outnum_lineEdit->clear();
    ui.recipient_lineEdit->clear();
}

void my_system::process_data(QList<QList<QString>>& data, QTableWidget* table)
{
    data.resize(table->rowCount() + 1);

    for (int i = 0; i < table->rowCount() + 1; i++) {
        data[i].resize(table->columnCount());
        for (int j = 0; j < table->columnCount(); j++) {
            if (i == 0) {
                data[i][j] = table->horizontalHeaderItem(j)->text();
            }
            else {
                data[i][j] = table->item(i - 1, j)->text();
            }
        }
    }
}




void my_system::on_outfind_pushButton_clicked()
{
    output_find();
}


void my_system::on_output_pushButton_clicked()
{
    output_data();
}

void my_system::on_place_comboBox_activated(int index)
{
    QString ID = ui.outputID_lineEdit->text();
    QString text = ui.place_comboBox->currentText();
    QString dis = "SELECT *FROM 元器件表,库存表 WHERE 元器件表.元器件编号=库存表.元器件编号 AND 库存表.存储位置='"
        + text + "' AND 元器件表.元器件编号='" + ID + "';";
    QSqlQuery dislabel;
    dislabel.exec(dis);
    dislabel.next();
    dis_label(dislabel.value("元器件名称").toString(), dislabel.value("库存数量").toString(), dislabel.value("最低库存").toString());
}


void my_system::on_findRecord_pushButton_clicked()
{
    QString record_type = ui.record_comboBox->currentText();
    table *record=new table;
    record->setWindowFlags(Qt::Window);
    record->setWindowTitle(record_type);
    record->init_table(record_type);
    record->show();
    record->setAttribute(Qt::WA_DeleteOnClose);//设置标志位内存管理
}





void my_system::on_finduser_pushButton_clicked()
{
    if (!LoginD::admin) {
        QMessageBox::warning(this, "警告", "权限不足!", QMessageBox::Ok);
        return;
    }
    else {
        QString find_text = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='用户表';";
        QStringList header;
        QSqlQuery find;
        find.exec(find_text);
        while (find.next()) {
            header << find.value(0).toString();
        }
        ui.user_tableWidget->setColumnCount(find.size());
        ui.user_tableWidget->setHorizontalHeaderLabels(header);
        find_text = "SELECT * FROM 用户表;";
        int i = 0;
        find.exec(find_text);
        ui.user_tableWidget->setRowCount(find.numRowsAffected());
            //无法编辑
        ui.user_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //标签水平拓展
        ui.user_tableWidget->horizontalHeader()->setStretchLastSection(true);
    //列宽自动适应
        ui.user_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui.user_tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

        while (find.next()) {
            for (int j = 0; j < ui.user_tableWidget->columnCount(); j++) {
                QString temp = find.value(j).toString();
                ui.user_tableWidget->setItem(i, j, new QTableWidgetItem(temp));
                //居中显示
                ui.user_tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
            i++;
        }
        if (i > 0) {
            QMessageBox::information(this, "提示", "查询成功!", QMessageBox::Ok);
        }
        else {
            QMessageBox::information(this, "提示", "未查询到内容!", QMessageBox::Ok);
        }
    }
}


void my_system::on_newuser_pushButton_clicked()
{
    if (!LoginD::admin) {
        QMessageBox::warning(this, "警告", "权限不足!", QMessageBox::Ok);
        return;
    }
    else {
        newUser* add = new newUser(this);
        add->setWindowTitle("添加用户");
        add->show();
        add->setAttribute(Qt::WA_DeleteOnClose);//内存回收
    }
}


void my_system::on_deluser_pushButton_clicked()
{
    if (!LoginD::admin) {
        QMessageBox::warning(this, "警告", "权限不足!", QMessageBox::Ok);
        return;
    }
    else {
        deluser* del = new deluser(this);
        del->show();
        del->setAttribute(Qt::WA_DeleteOnClose);
    }
}

void my_system::dis_time()
{
    static int sec = 0;
    static int min = 0;
    static int hour = 0;
    sec++;
    if (sec == 60) {
        min++;
        sec = 0;
    }
    if (min == 60) {
        min = 0;
        hour++;
    }
    QString time ="在线时长："+ QString::asprintf("%02d:%02d:%02d", hour, min, sec);
    ui.time_label->setText(time);
    LoginD login;
    if (!login.check_login(LoginD::user)) {
        QMessageBox maybe;
        maybe.setWindowTitle("提示");
        maybe.setIcon(QMessageBox::Information);
        maybe.setText("您已离线，请退出系统!");
        QPushButton* yes = maybe.addButton("确定", QMessageBox::YesRole);
        maybe.addButton("取消", QMessageBox::NoRole);
        maybe.exec();
        if (maybe.clickedButton() == (QAbstractButton*)yes) {
            QCoreApplication::exit(0);
        }
        else {
            return;
        }
   }
}

void my_system::dis_information()
{
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &my_system::dis_time);
    timer->start(1000);
    QString userid = "工号：" + LoginD::user;
    ui.userID_label->setText(userid);
    QString user_find = "SELECT 角色 FROM 用户表 WHERE 用户编号='" + LoginD::user + "';";
    QSqlQuery use;
    use.exec(user_find);
    use.next();
    QString user = "角色：" + use.value(0).toString();
    ui.user_label->setText(user);
}



QSize CustomTabStyle::sizeFromContents(ContentsType type, const QStyleOption* option, const QSize& size, const QWidget* widget) const
{
    QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
    if (type == QStyle::CT_TabBarTab) {
        s.transposed();
        s.rwidth() = 120;
        s.rheight() = 80;
    }
    return s;
}

void CustomTabStyle::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const
{
    if (element == CE_TabBarTabLabel) {
        if (const QStyleOptionTab* tab = qstyleoption_cast<const QStyleOptionTab*>(option)) {
            QRect allRect = tab->rect;
            if (tab->state & QStyle::State_Selected) {
                painter->save();
                painter->setPen(0x89cfff);
                painter->setBrush(QBrush(0x89cfff));
                painter->drawRect(allRect.adjusted(6, 6, -6, -6));
                painter->restore();
            }
            QTextOption option;
            option.setAlignment(Qt::AlignCenter);
            if (tab->state & QStyle::State_Selected) {
                painter->setPen(0xf8fcff);
            }
            else {
                painter->setPen(0x000001);
            }
            painter->drawText(allRect, tab->text, option);
            return;
        }
    }
    if (element == CE_TabBarTab) {
        QProxyStyle::drawControl(element, option, painter, widget);
    }
}



void my_system::on_tabWidget_currentChanged(int index)
{
    //添加库存信息页面查询类型项目
    if (index == 2) {
        ui.infoFind_comboBox->clear();
        QStringList find_name;
        QString find_str = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='库存表';";
        QSqlQuery find;
        find.exec(find_str);
        find_name << "全部"<<"元器件编号"<<"存储位置"<<"元器件名称";
        ui.infoFind_comboBox->addItems(find_name);
        find_name.clear();
        while (find.next()) {
            find_name << find.value(0).toString();
        }

        ui.info_tableWidget->setColumnCount(find.size());
        ui.info_tableWidget->setHorizontalHeaderLabels(find_name);
  
        //选择一行
        ui.info_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        //无法编辑
        ui.info_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        //标签水平拓展
        ui.info_tableWidget->horizontalHeader()->setStretchLastSection(true);
        //列宽自动适应
        ui.info_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
}


void my_system::on_infoFind_pushButton_clicked()
{
    QString find_type = ui.infoFind_comboBox->currentText();
    QString find_str;
    QString input = ui.infoFind_lineEdit->text();
    if (find_type == "全部") {
        find_str = "SELECT *FROM 库存表;";
    }
    else {
        find_str = "SELECT *FROM 库存表 WHERE " + find_type + "='" + input + "';";
    }
    QSqlQuery find;
    find.exec(find_str);
    ui.info_tableWidget->setRowCount(find.numRowsAffected());
    int i = 0;
    while (find.next()) {
        for (int j = 0; j < ui.info_tableWidget->columnCount(); j++) {
            QString temp = find.value(j).toString();
            ui.info_tableWidget->setItem(i, j, new QTableWidgetItem(temp));
            ui.info_tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
        i++;
    }
    if (i > 0) {
        QMessageBox::information(this, "提示", "查询成功!", QMessageBox::Ok);
    }
    else {
        QMessageBox::information(this, "提示", "未查询到内容!", QMessageBox::Ok);
        int temp = ui.info_tableWidget->rowCount();
        //清除表格内容
        while (temp--) {
            ui.info_tableWidget->removeRow(temp);
        }
    }
    ui.infoFind_lineEdit->clear();
}


void my_system::on_infoOut_pushButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "另存为", "库存信息表", "(*.xlsx);;(*.xls)");
    if (!filename.isEmpty()) {
        QList<QList<QString>> data;
        process_data(data, ui.info_tableWidget);
        QString title = "库存信息";
        table_control outinfo;
        outinfo.output_table(filename, title, data);
        QMessageBox::information(this, "提示", "导出成功!", QMessageBox::Ok);
    }
}


void my_system::on_information_tabWidget_currentChanged(int index)
{
    if (index == 1) {
        ui.compFind_comboBox->clear();
        QStringList find_name;
        QString find_str = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='元器件表';";
        QSqlQuery find;

        find.exec(find_str);
        find_name << "全部" << "元器件编号" << "供应商编号" << "元器件名称"<<"元器件类型";
        ui.compFind_comboBox->addItems(find_name);

        find_name.clear();
        while (find.next()) {
            find_name << find.value(0).toString();
        }
        ui.comp_tableWidget->setColumnCount(find.size());
        ui.comp_tableWidget->setHorizontalHeaderLabels(find_name);

        find_str = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='供应商表';";
        find.exec(find_str);
        find_name.clear();
        while (find.next()) {
            find_name << find.value(0).toString();
        }
        ui.supplier_tableWidget->setColumnCount(find.size());
        ui.supplier_tableWidget->setHorizontalHeaderLabels(find_name);


        //选择一行
        ui.comp_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        //无法编辑
        ui.comp_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        //标签水平拓展
        ui.comp_tableWidget->horizontalHeader()->setStretchLastSection(true);
        //列宽自动适应
        ui.comp_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        //选择一行
        ui.supplier_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        //无法编辑
        ui.supplier_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        //标签水平拓展
        ui.supplier_tableWidget->horizontalHeader()->setStretchLastSection(true);
        //列宽自动适应
        ui.supplier_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    }
}


void my_system::on_compFind_pushButton_clicked()
{
    QString find_type = ui.compFind_comboBox->currentText();
    QString find_str;
    QString input = ui.compFind_lineEdit->text();
    if (find_type == "全部") {
        find_str = "SELECT *FROM 元器件表;";
    }
    else {
        find_str = "SELECT *FROM 元器件表 WHERE " + find_type + "='" + input + "';";
    }
    QStringList ID;
    QString supplierID;
    QSqlQuery find;
    find.exec(find_str);
    ui.comp_tableWidget->setRowCount(find.numRowsAffected());
    int i = 0;
    while (find.next()) {
        //写入元器件信息
        for (int j = 0; j < ui.comp_tableWidget->columnCount(); j++) {
            QString temp = find.value(j).toString();
            ui.comp_tableWidget->setItem(i, j, new QTableWidgetItem(temp));
            ui.comp_tableWidget->item(i, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
        //写入对应的供应商编号
         supplierID = find.value("供应商编号").toString();
        if (!ID.contains(supplierID)) {//过滤重复编号
            ID << supplierID;
        }
        i++;
    }

    ui.supplier_tableWidget->setRowCount(ID.size());

    if (i > 0) {
        //写入供应商信息
        for (int k = 0; k < ID.size(); k++) {
            find_str = "SELECT *FROM 供应商表 WHERE 供应商编号='" + ID[k] + "';";
            find.exec(find_str);
            find.next();
            for (int j = 0; j < ui.supplier_tableWidget->columnCount(); j++) {
                QString temp = find.value(j).toString();
                ui.supplier_tableWidget->setItem(k, j, new QTableWidgetItem(temp));
                ui.supplier_tableWidget->item(k, j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
        }
        QMessageBox::information(this, "提示", "查询成功!", QMessageBox::Ok);
    }
    else {
        QMessageBox::information(this, "提示", "未查询到内容!", QMessageBox::Ok);
        int temp = ui.comp_tableWidget->rowCount();
        //清除表格内容
        while (temp--) {
            ui.comp_tableWidget->removeRow(temp);
        }
        temp = ui.supplier_tableWidget->rowCount();
        while (temp--) {
            ui.supplier_tableWidget->removeRow(temp);
        }
    }
    ui.compFind_lineEdit->clear();
}


void my_system::on_compOut_pushButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "另存为", "元器件信息表", "(*.xlsx);;(*.xls)");
    if (!filename.isEmpty()) {
        QList<QList<QString>> data;
        process_data(data, ui.comp_tableWidget);
        QString title = "元器件信息";
        table_control outinfo;
        outinfo.output_table(filename, title, data);
        QMessageBox::information(this, "提示", "导出成功!", QMessageBox::Ok);
    }
}


void my_system::on_supplierOut_pushButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "另存为", "供应商信息表", "(*.xlsx);;(*.xls)");
    if (!filename.isEmpty()) {
        QList<QList<QString>> data;
        process_data(data, ui.supplier_tableWidget);
        QString title = "供应商信息";
        table_control outinfo;
        outinfo.output_table(filename, title, data);
        QMessageBox::information(this, "提示", "导出成功!", QMessageBox::Ok);
    }
}


void my_system::on_pushButton_updatemin_clicked()
{
    QString minnum = ui.lineEdit_minsize->text();
    QString id = ui.outputID_lineEdit->text();
    QString place = ui.place_comboBox->currentText();
    if (minnum == "") {
        QMessageBox::information(this, "提示", "输入不能为空!", QMessageBox::Ok);
        ui.lineEdit_minsize->clear();
        return;
    }
    if (minnum.toInt() < 1 || minnum.toInt() > ui.outnum_label->text().toInt()) {
        QMessageBox::information(this, "提示", "输入不符合要求，请重新输入!", QMessageBox::Ok);
        ui.lineEdit_minsize->clear();
        return;
    }
    QString update = "UPDATE 库存表 SET 最低库存=" + minnum +
        " WHERE 元器件编号='" + id + "' AND 存储位置='" + place + "';";
    QSqlQuery query;
    query.exec(update);
    QMessageBox::information(this, "提示", "修改成功!", QMessageBox::Ok);
    ui.lineEdit_minsize->clear();
}


void my_system::on_pushButton_updatefind_clicked()
{
    //选择一行
    ui.tableWidget_update->setSelectionBehavior(QAbstractItemView::SelectRows);
    //无法编辑
    ui.tableWidget_update->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //标签水平拓展
    ui.tableWidget_update->horizontalHeader()->setStretchLastSection(true);
    //列宽自动适应
    ui.tableWidget_update->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QString input = ui.lineEdit_updatefind->text();
    if (input == "") {
        QMessageBox::information(this, "提示", "输入不能为空，请重新输入!", QMessageBox::Ok);
        ui.lineEdit_updatefind->clear();
        return;
    }
    QStringList title;
    QSqlQuery find;
    QString temp;
    QString type = ui.comboBox_update->currentText();
    QString find_str;

    find_str = "SELECT *FROM 元器件表 WHERE " + type + "='" + input + "';";
    QStringList data;
    find.exec(find_str);
    //写入元器件信息到第一列
    find.next();
    if (find.value(0) .toString() == "") {
        QMessageBox::information(this, "提示", "未查询到该元器件!", QMessageBox::Ok);
        ui.lineEdit_updatefind->clear();
        return;
    }
        temp = find.value("供应商编号").toString();
        for (int i = 0; i < 6; i++) {
            if (i != 1) {//不写入供应商编号
                data << find.value(i).toString();
            }
    }
    find_str = "SELECT *FROM 供应商表 WHERE 供应商编号='" + temp + "';";
    find.exec(find_str);
    //写入供应商信息到第一列
    find.next();
    if (find.value(0).toString() == "") {
        QMessageBox::information(this, "提示", "未查询到该元器件的供应商,请添加!", QMessageBox::Ok);
        ui.changeComp_tabWidget->setCurrentIndex(1);
        ui.lineEdit_updatefind->clear();
      return;
    }
        for (int i = 0; i < 6; i++) {
            data << find.value(i).toString();
        }

 find_str = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='元器件表';";
    find.exec(find_str);
    ui.tableWidget_update->setRowCount(find.size());
    while (find.next()) {
        if (find.value(0).toString() != "供应商编号") {
            title << find.value(0).toString();
        }
    }

    find_str = "SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='供应商表';";
    find.exec(find_str);
    //写入供应商列名
    while (find.next()) {
        title << find.value(0).toString();
    }

    ui.tableWidget_update->setRowCount(ui.tableWidget_update->rowCount() + find.size()-1);
    ui.tableWidget_update->setColumnCount(1);
    ui.tableWidget_update->setVerticalHeaderLabels(title);
    title.clear();
    title << "元器件信息";
    ui.tableWidget_update->setHorizontalHeaderLabels(title);
    title.clear();

    //写入信息到表格
        for (int i = 0; i < 11; i++) {
            if (data[0] != "") {
                ui.tableWidget_update->setItem(i, 0, new QTableWidgetItem(data[i]));
                ui.tableWidget_update->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            }
        }

    title << "元器件信息" << "供应商信息";
    ui.comboBox_updatetype->addItems(title);
    
    QMessageBox::information(this, "提示", "查询成功!", QMessageBox::Ok);
    ui.lineEdit_updatefind->clear();
}


void my_system::on_pushButton_update_clicked()
{
    QString type;
    QString find_str;
    QString id = ui.tableWidget_update->item(0, 0)->text();
    QString name;
    QSqlQuery find;
    QString input = ui.lineEdit_updateinput->text();
    //只能修改为供应商表中有的供应商，如果修改后的编号表中没有，需要添加后才能修改
    if (ui.comboBox_updatename->currentText() == "供应商编号") {
        find_str = "SELECT * FROM 供应商表 WHERE 供应商编号='" + input + "';";
        find.exec(find_str);
        find.next();
        if (find.value(0).toString() == "") {
            QMessageBox::warning(this, "提示", "未查询到该供应商，请添加!", QMessageBox::Ok);
            ui.changeComp_tabWidget->setCurrentIndex(1);
            ui.lineEdit_updateinput->clear();
            return;
        }
        find_str = "UPDATE 元器件表 SET 供应商编号='" + input + "' WHERE 元器件编号='" + id + "';";
        find.exec(find_str);
    }
    //当修改选项为元器件编号时，同时修改所有表中的元器件编号
    else if (ui.comboBox_updatename->currentText() == "元器件编号") {
        find_str = "UPDATE 元器件表 SET 元器件编号='" + input + "' WHERE 元器件编号='" + id + "';";
        find.exec(find_str);
        find_str = "UPDATE 库存表 SET 元器件编号='" + input + "' WHERE 元器件编号='" + id + "';";
        find.exec(find_str);
        find_str = "UPDATE 入库记录表 SET 元器件编号='" + input + "' WHERE 元器件编号='" + id + "';";
        find.exec(find_str);
        find_str = "UPDATE 出库记录表 SET 元器件编号='" + input + "' WHERE 元器件编号='" + id + "';";
        find.exec(find_str);
    }
    //否则只需要修改单表中的内容
    else {
        if (ui.comboBox_updatetype->currentText() == "元器件信息") {
            type = "元器件表";
            name = "元器件编号";
        }
        else {
            type = "供应商表";
            name = "供应商编号";
            id = ui.tableWidget_update->item(5, 0)->text();
        }
        find_str = "UPDATE " + type + " SET " + name + "='" + input + "' WHERE "
            + name + "='" + id + "';";
        find.exec(find_str);
 
    }
    QMessageBox::information(this, "提示", "修改成功!", QMessageBox::Ok);
    ui.lineEdit_updatefind->clear();
}



void my_system::on_comboBox_updatetype_currentTextChanged(const QString &arg1)
{
    QStringList name1;
        name1<< "元器件编号"<< "供应商编号"<<"元器件名称"<< 
            "元器件类型"<< "制造商"<< "描述";
    QStringList name2;
    name2 << "供应商编号" << "供应商名称" <<
        "联系人" << "电话" << "邮箱", "地址";
    if (arg1 == "元器件信息") {
        ui.comboBox_updatename->clear();
        ui.comboBox_updatename->addItems(name1);
    }
    else {
        ui.comboBox_updatename->clear();
        ui.comboBox_updatename->addItems(name2);
    }
}


void my_system::on_pushButton_newsupplier_clicked()
{
    QString newsupplierid = ui.newsupplierID_lineEdit->text();
    QString newsuppliername = ui.newsupplierName_lineEdit->text();
    QString newlinkman = ui.newLinkman_lineEdit->text();
    QString newphone = ui.newPhone_lineEdit->text();
    QString newmail = ui.newMail_lineEdit->text();
    QString newsite = ui.newSite_lineEdit->text();
    if (newsupplierid == "" || newsuppliername == "" || newlinkman == ""
        || newphone == "" || newmail == "" || newsite == "") {
        QMessageBox::warning(this, "提示", "请检查填写信息格式是否正确!", QMessageBox::Ok);
        return;
    }
    QString insert_supplier = "INSERT INTO 供应商表 VALUES('" + newsupplierid + "','" + newsuppliername + "','"
        + newlinkman + "','" + newphone + "','" + newmail + "','" + newsite + "');";

    QString idfind = "SELECT * FROM 供应商表 WHERE 供应商编号='" + newsupplierid + "';";

    QSqlQuery new_comp;

    new_comp.exec(idfind);
    new_comp.next();
    if (new_comp.value("供应商编号").toString() == newsupplierid) {
        QMessageBox::warning(this, "提示", "该供应商已存在!", QMessageBox::Ok);
        clear_new();
    }
    else {
        new_comp.exec(insert_supplier);
        QMessageBox::information(this, "提示", "添加成功!", QMessageBox::Ok);
        clear_new();
    }
}


void my_system::on_changeComp_tabWidget_currentChanged(int index)
{
    if (index == 2) {
        ui.comboBox_update->clear();
        QStringList title;
        title << "元器件编号" << "元器件名称";
        ui.comboBox_update->addItems(title);
    }
}

