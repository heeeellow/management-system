#pragma once

#include <QObject>
#include<qlist.h>
class table_control  : public QObject
{
	Q_OBJECT

public:
	table_control(QObject *parent=nullptr);
	~table_control();
	void output_table(const QString& filename, const QString& title,
		 const QList<QList<QString>>& data);
	void title_size(int size,QString &sizename);
};
