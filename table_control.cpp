
#include "table_control.h"
#include<qaxobject.h>
#include<qdir.h>
#include<qdebug.h>
#include<qprogressdialog.h>
table_control::table_control(QObject *parent)
	: QObject(parent)
{
	
}

table_control::~table_control()
{}

void table_control::output_table(const QString &filename, const QString& title, 
	 const QList<QList<QString>> &data)
{
	QProgressDialog* progressDialog = new QProgressDialog;
	progressDialog->setMinimumWidth(300);               // 设置最小宽度
	progressDialog->setWindowModality(Qt::ApplicationModal);    // 模态 Qt::WindowModal 模态
	progressDialog->setMinimumDuration(0);              // 等待0秒后显示
	progressDialog->setWindowTitle(tr("保存文件"));      // 标题名
	progressDialog->setLabelText(tr("正在导出"));        // 标签的
	progressDialog->setCancelButtonText(tr("取消"));   // 取消按钮
	progressDialog->setRange(0, 100);
	progressDialog->setAttribute(Qt::WA_DeleteOnClose);
	progressDialog->setValue(30);
	QAxObject* excel = new QAxObject(this);
	if(excel->setControl("ket.Application")){}
	else {
		excel->setControl("Excel.Application");
	}
	QAxObject* workbooks = excel->querySubObject("Workbooks");
	excel->setProperty("DisplayAlerts", false);
	workbooks->dynamicCall("Add");
	QAxObject* workbook = excel->querySubObject("ActiveWorkBook");
	QAxObject* worksheets = workbook->querySubObject("Sheets");
	QAxObject* worksheet = worksheets->querySubObject("Item(int)", 1);
	QString sizename;
	title_size(data[0].size(), sizename);
	//设置标题居中
	QAxObject* Range = worksheet->querySubObject("Range(const QString&)", sizename);
	Range->setProperty("MergeCells", true);//合并单元格
	Range->setProperty("Value2", title);
	progressDialog->setValue(50);
	//写入数据
	for (int i = 1; i < data.size()+1; i++) {
		for (int j = 1; j < data[i - 1].size() + 1; j++) {
			 Range = worksheet->querySubObject("Cells(int,int)", i + 1, j);
			 Range->setProperty("NumberFormat", "@");//数据为文本格式
			Range->dynamicCall("SetValue(const QString &)", data[i - 1][j - 1]);
		}
	}
	progressDialog->setValue(70);
	//选择所有已使用的表格，列宽自动，居中对齐
	Range = worksheet->querySubObject("UsedRange");
	QAxObject* cells = Range->querySubObject("Columns");
	cells->dynamicCall("AutoFit");
	Range->setProperty("HorizontalAlignment", -4108);
	Range->setProperty("VerticalAlignment", -4108);
	workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(filename));
	workbook->dynamicCall("Close()"); //关闭工作簿 
	excel->dynamicCall("Quit()"); //退出进程
	progressDialog->setValue(100);
	delete progressDialog;
	delete excel;
}

void table_control::title_size(int size,QString &sizename)
{
	QChar temp;
	if (size > 9) {
		temp = (QChar)('A' + size - 9);
	}
	else {
		temp = (QChar)(size +'0'+ 16);
	}
	sizename = "A1:" + (QString)temp + "1";
}
