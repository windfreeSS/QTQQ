#include "SysTray.h"

SysTray::SysTray(QWidget* parent)
	:m_parent(parent),
	QSystemTrayIcon(parent)
{
	initSystemTray();
	show();
}

SysTray::~SysTray()
{
}

void SysTray::initSystemTray()
{
	setToolTip(QStringLiteral("QT-VS-QQ-����"));
	setIcon(QIcon(":/Resources/MainWindow/app/logo.ico"));
	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onIconActivated(QSystemTrayIcon::ActivationReason)));
}

void SysTray::addSystrayMenu()
{
	//�Զ���˵�
	CustomMenu* customMenu = new CustomMenu(m_parent);
	customMenu->addCustomMenu("onShow", ":/Resources/MainWindow/app/logo.ico", QStringLiteral("��ʾ"));
	customMenu->addCustomMenu("onQuit", ":/Resources/MainWindow/app/page_close_btn_hover.png", QStringLiteral("�˳�"));

	//���ܲ�������,�͵��ø�����Ļ���BasicWindow::onShowNormal�ۺ���
	connect(customMenu->getAction("onShow"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowNormal(bool)));
	connect(customMenu->getAction("onQuit"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowQuit(bool)));

	//�����¼�ѭ��,����������
	customMenu->exec(QCursor::pos());//QCursor::pos()���¼�ѭ����λ������
	delete customMenu;
	customMenu = nullptr;
}

void SysTray::onIconActivated(QSystemTrayIcon::ActivationReason reason) {
	//�ж��Ƿ񱻵������
	if (reason == QSystemTrayIcon::Trigger) {
		m_parent->show();//��ʾ������
		m_parent->activateWindow();//�����
	}
	else if(reason == QSystemTrayIcon::Context){//ϵͳ������Ŀ�������Ĳ˵�������
		addSystrayMenu();
	}
}
