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
#include <QSqlQuery>

class QTreeWidgetItem;

extern QString gLoginEmployeeID;//登录者QQ号(员工号)

//QQ入口主窗口
class CCMainWindow : public BasicWindow
{
	Q_OBJECT

public:
	CCMainWindow(QString account,bool isAccountLogin,BasicWindow* parent = Q_NULLPTR);
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
	void addCompanyDeps(QTreeWidgetItem* pRootGroupItem, int DepID);//添加公司分组

	QString getHeadPicturePath();//获取登录者头像路径
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
	//QMap<QTreeWidgetItem*,QString> m_groupMap;//所有分组的分组项

	bool m_isAccountLogin;
	QString m_account;//账号或QQ号
};
