#include "Login.h"
#include "ui_Login.h"
#include<qsqldatabase.h>
#include<qmessagebox.h>
#include<qsqlquery.h>
#include<qsqlerror.h>
#include<qsettings.h>
#include<qnetworkinterface.h>
QString LoginD::user = NULL;
bool LoginD::admin = false;
bool LoginD::database_state = false;

LoginD::LoginD(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginD)
{
    ui->setupUi(this);
   this->setWindowTitle("电子元器件仓库管理系统");
	if (!database_state) {
		database_init();
		database_state = link_database();
	}
}

LoginD::~LoginD()
{
    delete ui;
}

bool LoginD::check_login(QString id)
{
	QString idfind = "SELECT * FROM 用户表 WHERE 用户编号='" + id + "';";
	QSqlQuery log;
	log.exec(idfind);
	log.next();
	if (log.value("登录状态").toString() == "在线"
		&&log.value("物理地址").toString()!=getMac()) {
		return false;
	}
	return true;
}

bool LoginD::exit_user()
{
	QMessageBox maybe;
	maybe.setWindowTitle("警告");
	maybe.setIcon(QMessageBox::Warning);
	maybe.setText("该用户已在线，是否强制下线？");
	QPushButton* yes = maybe.addButton("是", QMessageBox::YesRole);
	maybe.addButton("否", QMessageBox::NoRole);
	maybe.exec();
	if (maybe.clickedButton() == (QAbstractButton*)yes) {
		//设置用户为离线状态
		update_login(ui->lineEdit_id->text(), "离线",getMac());
		return true;
	}
	else {
		return false;
	}
}

void LoginD::update_login(QString id, QString state,QString mac)
{
	QString idfind = "UPDATE 用户表 SET 登录状态='"+state+"',物理地址='"+mac+"' WHERE 用户编号='" + id + "';";
	QSqlQuery log;
	log.exec(idfind);
}

bool LoginD::link_database()
{
	QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL");
	database.setHostName(ui->lineEdit_hostName->text());
	database.setPort(ui->lineEdit_port->text().toInt());
	database.setUserName(ui->lineEdit_datauser->text());
	database.setPassword(ui->lineEdit_datapwd->text());
	database.setDatabaseName(ui->lineEdit_databasename->text());
	if (database.open()) {
		return true;
	}
	else {
		QString erro = "数据库连接失败,请检查配置：" + database.lastError().text();
		QMessageBox::warning(this, tr("警告"), erro, QMessageBox::Ok);
		return false;
	}
}

void LoginD::logins()
{
	if (database_state) {
		QSqlQuery log;
		QString idline_in = ui->lineEdit_id->text();
		if (!check_login(idline_in)) {
			if (!exit_user()) {
				return;
			}
		}
		QString idfind = "SELECT * FROM 用户表 WHERE 用户编号='" + idline_in + "';";
		log.exec(idfind);
		bool flag = true;//判断是否在数据库中查询到工号
		while (log.next()) {
			if (log.value("用户编号") == idline_in) {
				flag = false;
				if (ui->lineEdit_name->text().trimmed() == log.value("用户名")
					&& ui->lineEdit_pwd->text() == log.value("密码")) {
					if (log.value("用户编号") == "000") {
						admin = true;//开启超管状态
						QMessageBox::information(this, "提示", "超管账号已登录", QMessageBox::Ok);
					}
					update_login(idline_in, "在线",getMac());
					user = idline_in;
					accept();
					break;
				}
				else {
					QMessageBox::warning(this, tr("警告"), tr("用户名或者密码错误"), QMessageBox::Ok);
					ui->lineEdit_name->setFocus();
					break;
				}
			}
		}
		if (flag) {
			QMessageBox::warning(this, tr("警告"), tr("工号不存在"), QMessageBox::Ok);
			ui->lineEdit_id->setFocus();
		}
		ui->lineEdit_id->clear();
		ui->lineEdit_name->clear();
		ui->lineEdit_pwd->clear();
	}
	else {
		QMessageBox::warning(this, tr("警告"), tr("数据库连接失败!"), QMessageBox::Ok);
		return;
	}
}

void LoginD::database_init()
{
	QSettings save("config.ini", QSettings::IniFormat);
	ui->lineEdit_hostName->setText(save.value("hostname").toString());
	 ui->lineEdit_port->setText(save.value("port","3306").toString());
	 ui->lineEdit_databasename->setText(save.value("databasename","management").toString());
	 ui->lineEdit_datauser->setText(save.value("username","root").toString());
	ui->lineEdit_datapwd->setText(save.value("pwd").toString());
}

QString LoginD::getMac()
{
	QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
	int nCnt = nets.count();
	QString strMacAddr = "";
	for (int i = 0; i < nCnt; i++)
	{
		// 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
		if (nets[i].flags().testFlag(QNetworkInterface::IsUp) 
			&& nets[i].flags().testFlag(QNetworkInterface::IsRunning) 
			&& !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
		{
			strMacAddr = nets[i].hardwareAddress();
			break;
		}
	}
	return strMacAddr;
}

void LoginD::on_pushButton_dataopen_clicked()
{
	database_state = link_database();
	if (database_state) {
		QMessageBox::information(this, "提示", "数据库连接成功", QMessageBox::Ok);
	}
}


void LoginD::on_pushButton_savedata_clicked()
{
	QSettings save("config.ini", QSettings::IniFormat);
	save.setValue("hostname", ui->lineEdit_hostName->text());
	save.setValue("port", ui->lineEdit_port->text());
	save.setValue("databasename", ui->lineEdit_databasename->text());
	save.setValue("username", ui->lineEdit_datauser->text());
	save.setValue("pwd", ui->lineEdit_datapwd->text());
	QMessageBox::information(this, "提示", "保存成功", QMessageBox::Ok);
}


void LoginD::on_pushButton_login_clicked()
{
	logins();
}


void LoginD::on_pushButton_exit_clicked()
{
	this->close();
}

