#pragma once

#include "CustomMenu.h"
#include <QSystemTrayIcon>
#include <QWidget>

//系统托盘 (为解决CCMainWindow窗体缩小后不知所向的问题)
class SysTray : public QSystemTrayIcon
{
	Q_OBJECT

public:
	SysTray(QWidget* parent);
	~SysTray();

public slots:
	//系统托盘图标被激活
	void onIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
	void initSystemTray();
	void addSystrayMenu();

	QWidget* m_parent;
};
