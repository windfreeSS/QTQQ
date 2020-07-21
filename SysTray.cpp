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
	setToolTip(QStringLiteral("QT-VS-QQ-仿真"));
	setIcon(QIcon(":/Resources/MainWindow/app/logo.ico"));
	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onIconActivated(QSystemTrayIcon::ActivationReason)));
}

void SysTray::addSystrayMenu()
{
	//自定义菜单
	CustomMenu* customMenu = new CustomMenu(m_parent);
	customMenu->addCustomMenu("onShow", ":/Resources/MainWindow/app/logo.ico", QStringLiteral("显示"));
	customMenu->addCustomMenu("onQuit", ":/Resources/MainWindow/app/page_close_btn_hover.png", QStringLiteral("退出"));

	//功能部件触发,就调用父窗体的基类BasicWindow::onShowNormal槽函数
	connect(customMenu->getAction("onShow"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowNormal(bool)));
	connect(customMenu->getAction("onQuit"), SIGNAL(triggered(bool)), m_parent, SLOT(onShowQuit(bool)));

	//进入事件循环,接收鼠标操作
	customMenu->exec(QCursor::pos());//QCursor::pos()是事件循环的位置所在
	delete customMenu;
	customMenu = nullptr;
}

void SysTray::onIconActivated(QSystemTrayIcon::ActivationReason reason) {
	//判断是否被点击触发
	if (reason == QSystemTrayIcon::Trigger) {
		m_parent->show();//显示父窗体
		m_parent->activateWindow();//活动窗体
	}
	else if(reason == QSystemTrayIcon::Context){//系统托盘条目的上下文菜单被请求
		addSystrayMenu();
	}
}
