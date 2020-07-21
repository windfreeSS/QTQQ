#pragma once

#include <QObject>
#include "CommonUtils.h"

//通知管理
class NotifyManager : public QObject
{
	Q_OBJECT

public:
	NotifyManager();
	~NotifyManager();

signals:
	//信号皮肤改变
	void signalSkinChanged(const QColor&);

public:
	//获取实例
	static NotifyManager* getInstance();
	//通知管理其它窗口改变皮肤(颜色)
	void notifyOtherWindowChanageSkin(const QColor& color);

private:
	static NotifyManager* instance;//实例

};
