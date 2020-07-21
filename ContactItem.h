#pragma once

#include <QWidget>
#include <QPixmap>
#include "CommonUtils.h"
#include "ui_ContactItem.h"

//��ϵ����Ŀ
class ContactItem : public QWidget
{
	Q_OBJECT

public:
	ContactItem(QWidget *parent = Q_NULLPTR);
	~ContactItem();

	//��ȡ,����һЩ����
	void setUserName(const QString& userName);
	void setSignName(const QString& signName);
	void setHeadPixmap(const QPixmap& headPath);
	QString getUserName()const;
	QSize getHeadLabelSize()const;


private:
	Ui::ContactItem ui;

	void initControl();
};
