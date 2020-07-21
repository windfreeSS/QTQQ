#pragma once

#include <QObject>
#include "CommonUtils.h"

//֪ͨ����
class NotifyManager : public QObject
{
	Q_OBJECT

public:
	NotifyManager();
	~NotifyManager();

signals:
	//�ź�Ƥ���ı�
	void signalSkinChanged(const QColor&);

public:
	//��ȡʵ��
	static NotifyManager* getInstance();
	//֪ͨ�����������ڸı�Ƥ��(��ɫ)
	void notifyOtherWindowChanageSkin(const QColor& color);

private:
	static NotifyManager* instance;//ʵ��

};
