#include "CCMainWindow.h"

CCMainWindow::CCMainWindow(QString account, bool isAccountLogin, BasicWindow* parent)
	: BasicWindow(parent), SkinWindow(nullptr),m_isAccountLogin(isAccountLogin),m_account(account)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::Tool);//设置窗口风格(当前窗口风格 | 工具窗口风格)
	loadStyleSheet("CCMainWindow");//加载样式表

	setHeadPixmap(getHeadPicturePath());
	initControl();//初始化控件
	initTimer();
}

CCMainWindow::~CCMainWindow()
{
}

void CCMainWindow::initControl()
{
	//树获取焦点时不绘制边框
	ui.treeWidget->setStyle(new CustomProxyStyle);
	setLevelPixmap(0);
	//setHeadPixmap(":/Resources/MainWindow/girl.png");
	setStatusMenuIcom(":/Resources/MainWindow/StatusSucceeded.png");

	QHBoxLayout* appupLayout = new QHBoxLayout;
	appupLayout->setContentsMargins(0, 0, 0, 0);//设置边缘间隙
	
	//添加应用部件(app图片路径,app部件对象名)
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_7.png", "app_7"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_2.png", "app_2"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_3.png", "app_3"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_4.png", "app_4"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_5.png", "app_5"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_6.png", "app_6"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/skin.png", "app_skin"));
	appupLayout->addStretch();//弹簧
	appupLayout->setSpacing(2);//间距

	ui.appWidget->setLayout(appupLayout);

	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_10.png", "app_10"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_8.png", "app_8"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_11.png", "app_11"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_9.png", "app_9"));
	ui.bottomLayout_up->addStretch();//添加弹簧

	//个性签名 好友搜索
	ui.lineEdit->installEventFilter(this);//安装事件过滤器(此窗体监视)
	ui.searchLineEdit->installEventFilter(this);

	initContactTree();//初始化联系人

	//最小化&关闭
	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowHide(bool)));
	connect(ui.sysclose , SIGNAL(clicked(bool)), this, SLOT(onShowClose(bool)));

	//皮肤改变就更新搜索好友框样式
	connect(NotifyManager::getInstance(), &NotifyManager::signalSkinChanged, [this] {updateSeachStyle(); });

	SysTray* sysTray = new SysTray(this);//系统托盘
}

void CCMainWindow::setUserName(const QString& username)
{
	ui.nameLabel->adjustSize();//根据内容调整自己尺寸
	//文本过长则进行省略
	//fontMetrics()返回的是QFontMetrics类对象
	//(文本,省略模式(方向),在哪像素省略)
	QString name = ui.nameLabel->fontMetrics().elidedText(username,Qt::ElideRight, ui.nameLabel->width());//省略文本

	ui.nameLabel->setText(name);
	ui.nameLabel->setToolTip(username);
}

void CCMainWindow::setLevelPixmap(int level)
{
	QPixmap levelPximap(ui.levelBtn->size());
	levelPximap.fill(Qt::transparent);//设置为透明

	QPainter painter(&levelPximap);
	painter.drawPixmap(0, 4, QPixmap(":/Resources/MainWindow/lv.png"));

	int unitNum = level % 10;//个数
	int tenNum = level / 10;//十位数

	//十位(在指定位置,将图片的指定部分画出)
	painter.drawPixmap(10, 4, QPixmap(":/Resources/MainWindow/levelvalue.png")
		,tenNum * 6,0,6,7);

	//个数(绘制点x,绘制点y,加载图片,截取位置x,截取位置y,截取多宽,截取多高)
	painter.drawPixmap(16, 4, QPixmap(":/Resources/MainWindow/levelvalue.png")
		, unitNum * 6, 0, 6, 7);

	ui.levelBtn->setIcon(levelPximap);
	ui.levelBtn->setIconSize(ui.levelBtn->size());
}

void CCMainWindow::setHeadPixmap(const QString& headPath)
{
	QPixmap pix;
	pix.load(":/Resources/MainWindow/head_mask.png");
	ui.headLabel->setPixmap(getRoundImage(QPixmap(headPath), pix, ui.headLabel->size()));
}

void CCMainWindow::setStatusMenuIcom(const QString& statusPath)
{
	QPixmap statusBtnPixmap(ui.stausBtn->size());
	statusBtnPixmap.fill(Qt::transparent);

	QPainter painter(&statusBtnPixmap);
	painter.drawPixmap(4, 4, QPixmap(statusPath));

	ui.stausBtn->setIcon(statusBtnPixmap);
	ui.stausBtn->setIconSize(ui.stausBtn->size());
}

QWidget* CCMainWindow::addOtherAppExtension(const QString& appPath, const QString& appName)
{
	QPushButton* btn = new QPushButton(this);
	btn->setFixedSize(20, 20);//设置固定大小

	QPixmap pixmap(btn->size());
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	QPixmap appPixmap(appPath);

	//((btn->width() - appPixmap.width()) / 2 )是为了居中显示
	painter.drawPixmap((btn->width() - appPixmap.width()) / 2,
		(btn->height() - appPixmap.height()) / 2 , appPixmap);

	btn->setIcon(pixmap);
	btn->setIconSize(btn->size());

	btn->setObjectName(appName);//设置对象名 

	//背景为透明
	btn->setProperty("hasborder",true);//设置属性(初始化属性名,初始化bool)
	
	connect(btn, &QPushButton::clicked, this, &CCMainWindow::onAppIconClicked);

	return btn;
}

void CCMainWindow::initContactTree()
{
	//点击,展开与收缩,双击时的信号,点击的哪一项,哪一列
	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onItemClicked(QTreeWidgetItem*, int)));
	connect(ui.treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(onItemExpanded(QTreeWidgetItem*)));
	connect(ui.treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(onItemCollapsed(QTreeWidgetItem*)));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));

	//根节点
	QTreeWidgetItem* pRootGroupItem = new QTreeWidgetItem;
	//设置子项指标政策
	pRootGroupItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//即使没有子项，也会显示用于展开和折叠的控件。
	pRootGroupItem->setData(0, Qt::UserRole, 0);//根项数据为0

	//添加子项
	RootContactItem* pItemName = new RootContactItem(true, ui.treeWidget);

	//获取公司部门ID
	QSqlQuery queryCompDepID(QString("select departmentID from qtqq.tab_department where department_name = '%1'").arg(QString::fromLocal8Bit("公司群")));
	queryCompDepID.exec();//执行
	queryCompDepID.first();//指向结果集的第一条
	int ComDepID = queryCompDepID.value(0).toInt();//获取公司ID

	//QMessageBox::information(nullptr, QString::fromLocal8Bit("公司ID:"), queryCompDepID.value(0).toString());//2000

	//获取登录者所在部门ID(部门群号)
	QSqlQuery querySelfDepID(QString("select departmentID from qtqq.tab_employees where employeeID = %1 ").arg(gLoginEmployeeID));
	querySelfDepID.exec();//执行
	querySelfDepID.next();//指向结果集的第一条
	int SelfDepID = querySelfDepID.value(0).toInt();//获取部门ID

	//QMessageBox::information(nullptr, QString::fromLocal8Bit("部门ID:"), querySelfDepID.value(0).toString());//2001

	//初始化公司群及登录者所在部门群
	addCompanyDeps(pRootGroupItem, ComDepID);
	addCompanyDeps(pRootGroupItem, SelfDepID);

	QString strGroupName = QString::fromLocal8Bit("编程世界");
	pItemName->setText(strGroupName);

	//插入分组节点
	ui.treeWidget->addTopLevelItem(pRootGroupItem);//添加顶级项目
	ui.treeWidget->setItemWidget(pRootGroupItem,0, pItemName);//设置项目部件

	//QStringList sCompDeps;//公司部门
	//sCompDeps << QString::fromLocal8Bit("公司群");
	//sCompDeps << QString::fromLocal8Bit("人事部");
	//sCompDeps << QString::fromLocal8Bit("研发部");
	//sCompDeps << QString::fromLocal8Bit("市场部");

	//for (int nIndex = 0; nIndex < sCompDeps.length(); nIndex++) {
	//	addCompanyDeps(pRootGroupItem, sCompDeps.at(nIndex));//添加编程世界分组
	//}
}

void CCMainWindow::initTimer()
{
	QTimer* timer = new QTimer(this);
	timer->setInterval(500);//设置间隔
	connect(timer, &QTimer::timeout, [this]() {
		static int level = 0;
		if (level == 99)level = 0;
		level++;
		setLevelPixmap(level);
		});
	timer->start();//启动计时器
}

void CCMainWindow::updateSeachStyle()
{
	ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background_color:rgba(%1,%2,%3,50)};border-bottom:1px solid rgba(%1,%2,%3,30);}\
											QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/search_icon.png);};")
											.arg(m_colorBackGround.red()).arg(m_colorBackGround.green()).arg(m_colorBackGround.blue()));
}

void CCMainWindow::addCompanyDeps(QTreeWidgetItem* pRootGroupItem, int DepID)//添加公司分组
{
	QTreeWidgetItem* pChild = new QTreeWidgetItem;
	//添加子节点 //子项数据为1
	pChild->setData(0, Qt::UserRole, 1);//可用于特定于应用程序目的的第一个角色
	pChild->setData(0, Qt::UserRole + 1, DepID);//将DepID它的地址做为它的唯一的身份证

	//联系人项插入到this->ui.treeWidget中,展开即见
	ContactItem* pContactItem = new ContactItem(ui.treeWidget);//树为主窗体

	//获取公司,部门头像
	QSqlQuery queryPicture(QString("select picture from qtqq.tab_department where departmentID = %1").arg(DepID));
	queryPicture.exec();
	queryPicture.next();
	QPixmap groupPix;
	groupPix.load(queryPicture.value(0).toString());

	//设置联系人项的资料
	QPixmap pix;
	pix.load(":/Resources/MainWindow/head_mask.png");
	pContactItem->setHeadPixmap(getRoundImage(groupPix,pix, pContactItem->getHeadLabelSize()));//头像

	//获取部门名称
	QSqlQuery queryDepName(QString("select department_name from qtqq.tab_department where departmentID = %1").arg(DepID));
	queryDepName.exec();
	queryDepName.next();
	QString strDepName = QString(queryDepName.value(0).toString());

	pContactItem->setUserName(strDepName);//联系人用户名
	pRootGroupItem->addChild(pChild);//根项添加子项
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);//设置项目部件

	//将当前新子项映射便于管理,索引等 //不然的话就需要根据this->ui.treeWidget来进行了
	//m_groupMap.insert(pChild, sDeps);
}

QString CCMainWindow::getHeadPicturePath()
{
	//需优化,优化的是去掉m_account和m_isAccountLogin,使用查询判断或就能减少代码,减少运算,减少内存
	QString strPicturePath;

	if (!m_isAccountLogin) {//QQ号登录
		QSqlQuery queryPicture(QString("select picture from qtqq.tab_employees where employeeID = %1").arg(gLoginEmployeeID));
		queryPicture.exec();
		queryPicture.next();
		strPicturePath = queryPicture.value(0).toString();
	}
	else {//员工号登录
		QSqlQuery queryEmployeeID(QString("select employeeID from qtqq.tab_accounts where account = '%1'").arg(m_account));
		queryEmployeeID.exec();
		queryEmployeeID.next();
		int employeeID = queryEmployeeID.value(0).toInt();
		QSqlQuery queryPicture(QString("select picture from qtqq.tab_employees where employeeID = %1").arg(employeeID));
		queryPicture.exec();
		queryPicture.next();
		strPicturePath = queryPicture.value(0).toString();
	}

	return strPicturePath;
}

void CCMainWindow::resizeEvent(QResizeEvent* event)
{
	setUserName(QString::fromLocal8Bit("仿QQ的奇妙之旅,C+经验++,手机号覅是根据"));//从本地的编码转换成Unicode编码生成QString对象
	BasicWindow::resizeEvent(event);
}

bool CCMainWindow::eventFilter(QObject* obj, QEvent* event)
{
	if (ui.searchLineEdit == obj) {
		if (event->type() == QEvent::FocusIn) {//获得键盘焦点事件
			//设置样式表 
			ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background-color:rgb(255,255,255);border-bottom:1px solid rgba(%1,%2,%3,100);}\
			QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/main_search_deldown.png)}\
			QPushButton#searchBtn:hover{border-image:url(:/Resources/MainWindow/search/main_search_delhighlight.png)}\
			QPushButton#searchBtn:Pressed{border-image:url(:/Resources/MainWindow/search/main_search_delhighdown.png)}")
			.arg(m_colorBackGround.red()).arg(m_colorBackGround.green()).arg(m_colorBackGround.blue()));
		}
		else if (event->type() == QEvent::FocusOut) {//焦点离开
			updateSeachStyle();

		}
	}
	return false;
}

void CCMainWindow::mousePressEvent(QMouseEvent* event)
{
	//程序的部件不是搜索部件而且还有焦点
	if (qApp->widgetAt(event->pos()) != ui.searchLineEdit && ui.searchLineEdit->hasFocus()) {
		ui.searchLineEdit->clearFocus();//清除焦点
	}
	else if (qApp->widgetAt(event->pos()) != ui.lineEdit && ui.lineEdit->hasFocus()) {
		ui.lineEdit->clearFocus();
	}

	BasicWindow::mousePressEvent(event);
}

void CCMainWindow::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	//填充矩形(当前几何,色值)
	painter.fillRect(rect(), QColor(255, 255, 255, 255));
}

void CCMainWindow::onItemClicked(QTreeWidgetItem* item, int column)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();//判断项目的数据
	if (!bIsChild) {//根项
		item->setExpanded(!item->isExpanded());//未展开则展开子项
	}
}

void CCMainWindow::onItemExpanded(QTreeWidgetItem* item)
{//展开树项操作
	bool bIsChild = item->data(0, Qt::UserRole).toBool();//判断项目的数据0或1;//0是根项,1是子项
	if (!bIsChild) {//根项
		//dynamic_cast 将基类对象指针(或引用)转换到继承类
		RootContactItem* prootItem = dynamic_cast<RootContactItem*>(ui.treeWidget->itemWidget(item, 0));//转换类型
		if (prootItem) {
			prootItem->setExpanded(true);//设置展开角度
		}
	}
}

void CCMainWindow::onItemCollapsed(QTreeWidgetItem* item)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();//判断项目的数据0或1;//0是根项,1是子项
	if (!bIsChild) {//根项
		//dynamic_cast 将基类对象指针(或引用)转换到继承类
		RootContactItem* prootItem = dynamic_cast<RootContactItem*>(ui.treeWidget->itemWidget(item, 0));//转换类型
		if (prootItem) {
			prootItem->setExpanded(false);//设置展开角度
		}
	}
}

void CCMainWindow::onItemDoubleClicked(QTreeWidgetItem* item, int column)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (bIsChild) {//如果是子项

		WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString());

		//QString strGroup = m_groupMap.value(item);//双击的是哪个联系项
		//if (strGroup == QString::fromLocal8Bit("公司群")) {
		//	//窗体管理::获取窗体管理实例->添加新联系窗口(树的哪一项,窗口类型)
		//	WindowManager::getInstance()->addNewTalkWindow(item->data(0,Qt::UserRole+1).toString(), GroupType::COMPANY);
		//}
		//else if (strGroup == QString::fromLocal8Bit("人事部")) {
		//	WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), GroupType::PERSONELGROUP);
		//}
		//else if (strGroup == QString::fromLocal8Bit("市场部")) {
		//	WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), GroupType::MARKETGROUP);
		//}
		//else if (strGroup == QString::fromLocal8Bit("研发部")) {
		//	WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), GroupType::DEVELOPMENTGROUP);
		//}
	}
}

void CCMainWindow::onAppIconClicked() {
	//sender()是OBJECT返回信号的函数
	
	if (sender()->objectName() == "app_skin") {//判断信号发送者对象名是否是
		if (SkinWindow == nullptr) {
			SkinWindow = new skinWindow;
			SkinWindow->show();
		}
		else {
			delete SkinWindow;
			SkinWindow = new skinWindow;
			SkinWindow->show();
		}	
	}
}