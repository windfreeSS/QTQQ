#include "NotifyManager.h"

NotifyManager* NotifyManager::instance = nullptr;

NotifyManager::NotifyManager():QObject(nullptr)
{
}

NotifyManager::~NotifyManager()
{
}

//获取实例
NotifyManager* NotifyManager::getInstance()
{
	if (instance==nullptr) {
		instance = new NotifyManager();
	}

	return instance;
}

//通知管理其它窗口改变皮肤(颜色)
void NotifyManager::notifyOtherWindowChanageSkin(const QColor& color)
{
	emit signalSkinChanged(color);
	CommonUtils::setDefaultSkinColor(color);
}
