#pragma once

#include "CommonUtils.h"
#include <QMenu>
#include <QWidget>
#include <QMap>

//自定义菜单
class CustomMenu : public QMenu
{
	Q_OBJECT

public:
	CustomMenu(QWidget *parent=nullptr);
	~CustomMenu();

	//添加自定义菜单(映射的key,动作部件的图标路径,动作部件的对象名称)
	void addCustomMenu(const QString& text, const QString& icon,const QString& name);

	QAction* getAction(const QString& text);
private:
	QMap<QString, QAction*> m_menuActionMap;
};
