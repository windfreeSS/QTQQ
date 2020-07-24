#pragma once

#include "BasicWindow.h"
#include "CommonUtils.h"
#include <qmap.h>
#include <qlistwidget.h>
#include "EmotionWindow.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"
#include "ui_TalkWindowShell.h"
#include <QSqlQueryModel>
//#include <QListWidgetItem>

//聊天窗口类型
enum class GroupType{
	COMPANY,//公司群
	PERSONELGROUP,//人事部
	DEVELOPMENTGROUP,//研发部
	MARKETGROUP,//市场部
	PTOP	//同事之间单独聊天
};

class TalkWindow;//联系窗口

//聊天窗口主体
class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(BasicWindow* parent = Q_NULLPTR);
	~TalkWindowShell();

	//添加新的聊天窗口(聊天窗口,聊天窗口项目,聊天窗口类型)
	void addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem, const QString& uid/*, enum class GroupType grouptype*/);
	void setCurrenWidget(QWidget* widget);//设置当前聊天窗口

	const QMap<QListWidgetItem*, QWidget*>& gettalkWindowItemMap()const;
public slots:
	void onEmotionBtnClicked(bool);//表情按钮点击后执行的槽函数
private slots:
	void onTalkWindowItemClicked(QListWidgetItem* item);//左侧列表点击后执行的槽函数
	void onEmotionItemClicked(int emotionNum);//表情被选中执行的槽函数

private:
	void initControl();//初始化空件

	Ui::TalkWindowShellClass ui;
	QMap<QListWidgetItem*, QWidget*> m_talkWindowItemMap;//打开的聊天窗口
	EmotionWindow* m_emotionWindow;//表情窗口
};
