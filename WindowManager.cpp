#include "WindowManager.h"

//����ģʽ,����ȫ�־�̬����
Q_GLOBAL_STATIC(WindowManager,theInstance)

WindowManager::WindowManager():QObject(nullptr),m_talkwindowshell(nullptr)
{
}

WindowManager::~WindowManager()
{
}

QWidget* WindowManager::findWindowName(const QString& qsWindowName)
{
	//ͨ��������key�鿴���ڹ���Ĵ���ӳ�����Ƿ��д˴���
	if (m_windowMap.contains(qsWindowName)) {//�Ƿ����
		return m_windowMap.value(qsWindowName);//�����ڷ���
	}
	else {
		return nullptr;
	}
}

void WindowManager::deleteWindowName(const QString& qsWindowName)
{
	m_windowMap.remove(qsWindowName);//ɾ��
}

void WindowManager::addWindowName(const QString& qsWindowName, QWidget* qWidget)
{
	if (!m_windowMap.contains(qsWindowName)) {//����ӳ����û�д˴�����Ϣ��
		m_windowMap.insert(qsWindowName, qWidget);
	}
}

WindowManager* WindowManager::getInstance()
{
	return theInstance();
}

void WindowManager::addNewTalkWindow(const QString& uid, enum class GroupType groupType, const QString& strPeople)
{
	if (m_talkwindowshell==nullptr) {//�жϴ��������Ƿ�Ϊ��
		m_talkwindowshell = new TalkWindowShell;
		//���촰�����巢��������ٵ��ź�,�ͽ�m_talkwindowshellָ���ÿ�
		connect(m_talkwindowshell, &TalkWindowShell::destroyed, [this](QObject* obj) {m_talkwindowshell = nullptr;});
	}//�������������

	//ͨ��CCMainWWindow��ui.treeWidget����keyȥ���ڹ����в����Ƿ��д�key�Ĵ���,�оͷ���,û�о�nullptr
	QWidget* widget = findWindowName(uid);
	if (widget == nullptr) {//���������û��TalkWindow,TalkWindowItem
		TalkWindow* talkwindow = new TalkWindow(m_talkwindowshell,uid, groupType);//��ϵ����
		TalkWindowItem* talkwindowItem = new TalkWindowItem(talkwindow);//��ϵ������Ŀ

		//�Դ�����������TalkWindow,TalkWindowItem����(����)
		switch (groupType)
		{
		case GroupType::COMPANY:
			//��������ui.rightStackedWidget�ı���λ��0,0��ʼ
			talkwindow->setWindowName(QStringLiteral("�������-ԽдԽͺͷ"));
			//��������ui.listWidget����ĿTalkWindowItem,��0,0��ʼ
			talkwindowItem->setMsgLabelContent(QStringLiteral("��˾Ⱥ"));
			break;
		case GroupType::PERSONELGROUP:
			talkwindow->setWindowName(QStringLiteral("����,����,����,��������"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("���²�"));
			break;
		case GroupType::DEVELOPMENTGROUP:
			talkwindow->setWindowName(QStringLiteral("ֻ�����ֱ������:һ�������찤���,��һ����û���õ�."));
			talkwindowItem->setMsgLabelContent(QStringLiteral("�з���"));
			break;
		case GroupType::MARKETGROUP:
			talkwindow->setWindowName(QStringLiteral("���칤����Ŭ��,����Ŭ���ҹ���"));
			talkwindowItem->setMsgLabelContent(QStringLiteral("�г���"));
			break;
		case GroupType::PTOP:
			talkwindow->setWindowName(QStringLiteral("��,�зɻ�"));
			talkwindowItem->setMsgLabelContent(strPeople);
			break;
		default:
			break;
		}

		m_talkwindowshell->addTalkWindow(talkwindow, talkwindowItem, groupType);//���촰�����彫�µ����촰����ӽ�ȥ
	}
	else {
		//��������б���Ϊѡ��
		QListWidgetItem* item = m_talkwindowshell->gettalkWindowItemMap().key(widget);//��ȡ��ǰ��
		item->setSelected(true);
		//�����Ҳ൱ǰ���촰��
		m_talkwindowshell->setCurrenWidget(widget);//���õ�ǰ�Ĵ�����ұ�
	}
	m_talkwindowshell->show();//���µ����촰��������ʾ
	m_talkwindowshell->activateWindow();//������Ϊ��Ĵ���
}
