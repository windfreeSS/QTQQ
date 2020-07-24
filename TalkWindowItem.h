#pragma once

#include <QWidget>
#include <qevent.h>
#include "CommonUtils.h"
#include "ui_TalkWindowItem.h"

//联系窗口项目
class TalkWindowItem : public QWidget
{
	Q_OBJECT

public:
	TalkWindowItem(QWidget *parent = Q_NULLPTR);
	~TalkWindowItem();

	//设置头像
	void setHeadPixmap(const QPixmap& pixmap);
	//设置标签内容
	void setMsgLabelContent(const QString& msg);
	//获取跟谁聊天
	QString getMsgLabelText();

signals:
	void signalCloseClicked();

private:
	void initControl();//初始化控件

	void enterEvent(QEvent* event);//进入事件
	void leaveEvent(QEvent* event);//离开事件
	void resizeEvent(QResizeEvent* event);//调整大小事件

	Ui::TalkWindowItem ui;
};
