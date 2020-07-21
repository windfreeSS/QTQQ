#include "WindowManager.h"

//单例模式,创建全局静态对象
Q_GLOBAL_STATIC(WindowManager,theInstance)

WindowManager::WindowManager():QObject(nullptr),m_talkwindowshell(nullptr)
{
}

WindowManager::~WindowManager()
{
}

QWidget* WindowManager::findWindowName(const QString& qsWindowName)
{
	//通过传来的key查看窗口管理的窗口映射中是否有此窗口
	if (m_windowMap.contains(qsWindowName)) {//是否包含
		return m_windowMap.value(qsWindowName);//将窗口返回
	}
	else {
		return nullptr;
	}
}

void WindowManager::deleteWindowName(const QString& qsWindowName)
{
	m_windowMap.remove(qsWindowName);//删除
}

void WindowManager::addWindowName(const QString& qsWindowName, QWidget* qWidget)
{
	if (!m_windowMap.contains(qsWindowName)) {//窗口映射中没有此窗口信息就
		m_windowMap.insert(qsWindowName, qWidget);
	}
}

WindowManager* WindowManager::getInstance()
{
	return theInstance();
}

void WindowManager::addNewTalkWindow(const QString& uid, enum class GroupType groupType, const QString& strPeople)
{
	if (m_talkwindowshell==nullptr) {//判断窗口主体是否为空
		m_talkwindowshell = new TalkWindowShell;
		//聊天窗口主体发射的是销毁的信号,就将m_talkwindowshell指针置空
		connect(m_talkwindowshell, &TalkWindowShell::destroyed, [this](QObject* obj) {m_talkwindowshell = nullptr;});
	}//窗口主体必须有

	//通过CCMainWWindow的ui.treeWidget传来key去窗口管理中查找是否有此key的窗口,有就返回,没有就nullptr
	QWidget* widget = findWindowName(uid);
	if (widget == nullptr) {//窗体管理中没有TalkWindow,TalkWindowItem
		TalkWindow* talkwindow = new TalkWindow(m_talkwindowshell,uid, groupType);//联系窗口
		TalkWindowItem* talkwindowItem = new TalkWindowItem(talkwindow);//联系窗口项目

		//以窗口类型设置TalkWindow,TalkWindowItem数据(界面)
		switch (groupType)
		{
		case GroupType::COMPANY:
			//窗体主体ui.rightStackedWidget的标题位置0,0开始
			talkwindow->setWindowName(QStringLiteral("编程世界-越写越秃头"));
			//窗体主体ui.listWidget的项目TalkWindowItem,从0,0开始
			talkwindowItem->setMsgLabelContent(QStringLiteral("公司群"));
			break;
		case GroupType::PERSONELGROUP:
			talkwindow->setWindowName(QStringLiteral("公心,民心,爱心,事事在心"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("人事部"));
			break;
		case GroupType::DEVELOPMENTGROUP:
			talkwindow->setWindowName(QStringLiteral("只有两种编程语言:一种是天天挨骂的,另一种是没人用的."));
			talkwindowItem->setMsgLabelContent(QStringLiteral("研发部"));
			break;
		case GroupType::MARKETGROUP:
			talkwindow->setWindowName(QStringLiteral("今天工作不努力,明天努力找工作"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("市场部"));
			break;
		case GroupType::PTOP:
			talkwindow->setWindowName(QStringLiteral("看,有飞机"));
			talkwindowItem->setMsgLabelContent(strPeople);
			break;
		default:
			break;
		}

		m_talkwindowshell->addTalkWindow(talkwindow, talkwindowItem, groupType);//聊天窗体主体将新的聊天窗口添加进去
	}
	else {
		//左侧聊天列表设为选择
		QListWidgetItem* item = m_talkwindowshell->gettalkWindowItemMap().key(widget);//获取当前项
		item->setSelected(true);
		//设置右侧当前聊天窗口
		m_talkwindowshell->setCurrenWidget(widget);//设置当前的窗体的右边
	}
	m_talkwindowshell->show();//更新的聊天窗口主体显示
	m_talkwindowshell->activateWindow();//并设置为活动的窗体
}
