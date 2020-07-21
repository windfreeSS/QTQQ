#pragma once

#include "BasicWindow.h"
#include "ui_CCMainWindow.h"
#include "CommonUtils.h"
#include <QProxyStyle>
#include <QPainter>
#include "skinWindow.h"
#include <QTimer>
#include "SysTray.h"
#include <QEvent>
#include <QGraphicsOpacityEffect>
#include "NotifyManager.h"
#include "RootContactItem.h"
#include "ContactItem.h"
#include "WindowManager.h"
#include "TalkWindowShell.h"
#include <QMouseEvent>
#include <qapplication.h>

class QTreeWidgetItem;

/*
//自定义的风格
class CustomProxyStyle : public QProxyStyle {
public:
	//PrimitiveElement枚举描述了各种原始元素。基本元素是一种常见的GUI元素，如复选框指示符或按钮斜角。
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption* optin
		, QPainter* painter, const QWidget* widget= nullptr)const {
		if (element == PE_FrameFocusRect) {//通用的重点指标
			//不需要绘制默认的带焦点的边框
			return;
		}
		else {
			QProxyStyle::drawPrimitive(element, optin, painter,widget);
		}

	}
};*/

//QQ入口主窗口
class CCMainWindow : public BasicWindow
{
	Q_OBJECT

public:
	CCMainWindow(BasicWindow*parent = Q_NULLPTR);
	~CCMainWindow();

	void setUserName(const QString& username);//初始化用户名
	void setLevelPixmap(int level);//设置等级图像
	void setHeadPixmap(const QString& headPath);//设置表头图片
	void setStatusMenuIcom(const QString& statusPath);//设置线上状态图片

	QWidget* addOtherAppExtension(const QString& appPath,const QString& appName);//添加应用部件

	void initContactTree();//初始化联系人
private:
	void initControl();//初始化控件
	void initTimer();//初始化计时器(模拟QQ等级升级)
	void updateSeachStyle();//更新搜索样式
	void addCompanyDeps(QTreeWidgetItem* pRootGroupItem, const QString& sDeps);//添加公司分组
private:
	void resizeEvent(QResizeEvent* event);//调整大小时间
	bool eventFilter(QObject* obj, QEvent* event);//事件过滤
	void mousePressEvent(QMouseEvent* event);//鼠标按下事件
protected:
	void paintEvent(QPaintEvent* event);//为了让部件窗体不透明
private slots:
	void onAppIconClicked();

	void onItemClicked(QTreeWidgetItem* item, int column);
	void onItemExpanded(QTreeWidgetItem* item);
	void onItemCollapsed(QTreeWidgetItem* item);
	void onItemDoubleClicked(QTreeWidgetItem*, int column);
private:
	Ui::CCMainWindow ui;
	skinWindow* SkinWindow = nullptr;
	QMap<QTreeWidgetItem*,QString> m_groupMap;//所有分组的分组项
};
