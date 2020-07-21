#pragma once

#include "CustomMenu.h"
#include <QSystemTrayIcon>
#include <QWidget>

//ϵͳ���� (Ϊ���CCMainWindow������С��֪���������)
class SysTray : public QSystemTrayIcon
{
	Q_OBJECT

public:
	SysTray(QWidget* parent);
	~SysTray();

public slots:
	//ϵͳ����ͼ�걻����
	void onIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
	void initSystemTray();
	void addSystrayMenu();

	QWidget* m_parent;
};
