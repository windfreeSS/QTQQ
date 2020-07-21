#pragma once

#include <QObject>
#include "TalkWindow.h"
#include "TalkWindowItem.h"
#include "TalkWindowShell.h"

class TalkWindowShell;
enum class GroupType;
//窗体管理
class WindowManager : public QObject
{
	Q_OBJECT

public:
	WindowManager();
	~WindowManager();

	QWidget* findWindowName(const QString& qsWindowName);//查找窗口名称
	void deleteWindowName(const QString& qsWindowName);//删除窗口
	void addWindowName(const QString& qsWindowName,QWidget* qWidget);//添加窗口

	static WindowManager* getInstance();//获取实例
	void addNewTalkWindow(const QString& uid,enum class GroupType groupType, const QString& strPeople = "");//添加新聊天窗口

private:
	TalkWindowShell* m_talkwindowshell;
	QMap<QString, QWidget*> m_windowMap;
};
