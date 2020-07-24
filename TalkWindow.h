#pragma once

#include <QWidget>
#include <qfile.h>
#include <QMessageBox>
#include "TalkWindowShell.h"
#include "RootContactItem.h"
#include "ContactItem.h"
#include "CommonUtils.h"
#include <QTreeWidgetItem>
#include <qtooltip.h>
#include "WindowManager.h"
#include "ui_TalkWindow.h"
#include <QSqlQueryModel>
#include <QSqlQuery>

//联系窗口
class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent ,const QString& uid/*,enum class GroupType groupType*/);
	~TalkWindow();

	void addEmotionImage(int emotionNum);//添加表情项目
	void setWindowName(const QString& name);//设置窗口名

private slots:
	void onSendBtnClicked(bool);
	void onItemDoubleClicked(QTreeWidgetItem*,int);//对双击的是哪一项进行操作,列
private:
	void initControl();//初始化控件
	void initGroupTalkStatus();//初始化群聊状态
	void initTalkWindow();//初始化群聊
	int getCompDepID();//获取公司ID

	//void initCompanyTalk();//初始化公司群聊天
	//void initPersonelTalk();//初始化人事部聊天
	//void initMarketTalk();//初始化市场部聊天
	//void initDevelopTalk();//初始化研发部聊天
	void initPtoPTalk();//初始化单聊聊天

	void addPeopInfo(QTreeWidgetItem* pRootGroupItem,int employeeID);//添加子项

	Ui::TalkWindow ui;
	QString m_talkId;//联系id
	//GroupType m_groupType;//分组的类型
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap;//所有分组联系人姓名

	bool m_isGroupTalk;//是否为群聊
};
