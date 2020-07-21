#pragma once

#include <QObject>
#include "TalkWindow.h"
#include "TalkWindowItem.h"
#include "TalkWindowShell.h"

class TalkWindowShell;
enum class GroupType;
//�������
class WindowManager : public QObject
{
	Q_OBJECT

public:
	WindowManager();
	~WindowManager();

	QWidget* findWindowName(const QString& qsWindowName);//���Ҵ�������
	void deleteWindowName(const QString& qsWindowName);//ɾ������
	void addWindowName(const QString& qsWindowName,QWidget* qWidget);//��Ӵ���

	static WindowManager* getInstance();//��ȡʵ��
	void addNewTalkWindow(const QString& uid,enum class GroupType groupType, const QString& strPeople = "");//��������촰��

private:
	TalkWindowShell* m_talkwindowshell;
	QMap<QString, QWidget*> m_windowMap;
};
