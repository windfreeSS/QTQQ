#include "NotifyManager.h"

NotifyManager* NotifyManager::instance = nullptr;

NotifyManager::NotifyManager():QObject(nullptr)
{
}

NotifyManager::~NotifyManager()
{
}

//��ȡʵ��
NotifyManager* NotifyManager::getInstance()
{
	if (instance==nullptr) {
		instance = new NotifyManager();
	}

	return instance;
}

//֪ͨ�����������ڸı�Ƥ��(��ɫ)
void NotifyManager::notifyOtherWindowChanageSkin(const QColor& color)
{
	emit signalSkinChanged(color);
	CommonUtils::setDefaultSkinColor(color);
}
