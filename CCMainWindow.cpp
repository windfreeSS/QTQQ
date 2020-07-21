#include "CCMainWindow.h"

CCMainWindow::CCMainWindow(BasicWindow* parent)
	: BasicWindow(parent), SkinWindow(nullptr)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() | Qt::Tool);//���ô��ڷ��(��ǰ���ڷ�� | ���ߴ��ڷ��)
	loadStyleSheet("CCMainWindow");//������ʽ��

	initControl();//��ʼ���ؼ�
	initTimer();
}

CCMainWindow::~CCMainWindow()
{
}

void CCMainWindow::initControl()
{
	//����ȡ����ʱ�����Ʊ߿�
	ui.treeWidget->setStyle(new CustomProxyStyle);
	setLevelPixmap(0);
	setHeadPixmap(":/Resources/MainWindow/girl.png");
	setStatusMenuIcom(":/Resources/MainWindow/StatusSucceeded.png");

	QHBoxLayout* appupLayout = new QHBoxLayout;
	appupLayout->setContentsMargins(0, 0, 0, 0);//���ñ�Ե��϶
	
	//���Ӧ�ò���(appͼƬ·��,app����������)
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_7.png", "app_7"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_2.png", "app_2"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_3.png", "app_3"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_4.png", "app_4"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_5.png", "app_5"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_6.png", "app_6"));
	appupLayout->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/skin.png", "app_skin"));
	appupLayout->addStretch();//����
	appupLayout->setSpacing(2);//���

	ui.appWidget->setLayout(appupLayout);

	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_10.png", "app_10"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_8.png", "app_8"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_11.png", "app_11"));
	ui.bottomLayout_up->addWidget(addOtherAppExtension(":/Resources/MainWindow/app/app_9.png", "app_9"));
	ui.bottomLayout_up->addStretch();//��ӵ���

	//����ǩ�� ��������
	ui.lineEdit->installEventFilter(this);//��װ�¼�������(�˴������)
	ui.searchLineEdit->installEventFilter(this);

	initContactTree();//��ʼ����ϵ��

	//��С��&�ر�
	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowHide(bool)));
	connect(ui.sysclose , SIGNAL(clicked(bool)), this, SLOT(onShowClose(bool)));

	//Ƥ���ı�͸����������ѿ���ʽ
	connect(NotifyManager::getInstance(), &NotifyManager::signalSkinChanged, [this] {updateSeachStyle(); });

	SysTray* sysTray = new SysTray(this);//ϵͳ����
}

void CCMainWindow::setUserName(const QString& username)
{
	ui.nameLabel->adjustSize();//�������ݵ����Լ��ߴ�
	//�ı����������ʡ��
	//fontMetrics()���ص���QFontMetrics�����
	//(�ı�,ʡ��ģʽ(����),��������ʡ��)
	QString name = ui.nameLabel->fontMetrics().elidedText(username,Qt::ElideRight, ui.nameLabel->width());//ʡ���ı�

	ui.nameLabel->setText(name);
	ui.nameLabel->setToolTip(username);
}

void CCMainWindow::setLevelPixmap(int level)
{
	QPixmap levelPximap(ui.levelBtn->size());
	levelPximap.fill(Qt::transparent);//����Ϊ͸��

	QPainter painter(&levelPximap);
	painter.drawPixmap(0, 4, QPixmap(":/Resources/MainWindow/lv.png"));

	int unitNum = level % 10;//����
	int tenNum = level / 10;//ʮλ��

	//ʮλ(��ָ��λ��,��ͼƬ��ָ�����ֻ���)
	painter.drawPixmap(10, 4, QPixmap(":/Resources/MainWindow/levelvalue.png")
		,tenNum * 6,0,6,7);

	//����(���Ƶ�x,���Ƶ�y,����ͼƬ,��ȡλ��x,��ȡλ��y,��ȡ���,��ȡ���)
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
	btn->setFixedSize(20, 20);//���ù̶���С

	QPixmap pixmap(btn->size());
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	QPixmap appPixmap(appPath);

	//((btn->width() - appPixmap.width()) / 2 )��Ϊ�˾�����ʾ
	painter.drawPixmap((btn->width() - appPixmap.width()) / 2,
		(btn->height() - appPixmap.height()) / 2 , appPixmap);

	btn->setIcon(pixmap);
	btn->setIconSize(btn->size());

	btn->setObjectName(appName);//���ö����� 

	//����Ϊ͸��
	btn->setProperty("hasborder",true);//��������(��ʼ��������,��ʼ��bool)
	
	connect(btn, &QPushButton::clicked, this, &CCMainWindow::onAppIconClicked);

	return btn;
}

void CCMainWindow::initContactTree()
{
	//���,չ��������,˫��ʱ���ź�,�������һ��,��һ��
	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onItemClicked(QTreeWidgetItem*, int)));
	connect(ui.treeWidget, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(onItemExpanded(QTreeWidgetItem*)));
	connect(ui.treeWidget, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(onItemCollapsed(QTreeWidgetItem*)));
	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));

	//���ڵ�
	QTreeWidgetItem* pRootGroupItem = new QTreeWidgetItem;
	//��������ָ������
	pRootGroupItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//��ʹû�����Ҳ����ʾ����չ�����۵��Ŀؼ���
	pRootGroupItem->setData(0, Qt::UserRole, 0);//��������Ϊ0

	//�������
	RootContactItem* pItemName = new RootContactItem(true, ui.treeWidget);

	QString strGroupName = QString::fromLocal8Bit("�������");
	pItemName->setText(strGroupName);

	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootGroupItem);//��Ӷ�����
	ui.treeWidget->setItemWidget(pRootGroupItem,0, pItemName);//������Ŀ����

	QStringList sCompDeps;//��˾����
	sCompDeps << QString::fromLocal8Bit("��˾Ⱥ");
	sCompDeps << QString::fromLocal8Bit("���²�");
	sCompDeps << QString::fromLocal8Bit("�з���");
	sCompDeps << QString::fromLocal8Bit("�г���");
	
	for (int nIndex = 0; nIndex < sCompDeps.length(); nIndex++) {
		addCompanyDeps(pRootGroupItem, sCompDeps.at(nIndex));//��ӱ���������
	}
}

void CCMainWindow::initTimer()
{
	QTimer* timer = new QTimer(this);
	timer->setInterval(500);//���ü��
	connect(timer, &QTimer::timeout, [this]() {
		static int level = 0;
		if (level == 99)level = 0;
		level++;
		setLevelPixmap(level);
		});
	timer->start();//������ʱ��
}

void CCMainWindow::updateSeachStyle()
{
	ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background_color:rgba(%1,%2,%3,50)};border-bottom:1px solid rgba(%1,%2,%3,30);}\
											QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/search_icon.png);};")
											.arg(m_colorBackGround.red()).arg(m_colorBackGround.green()).arg(m_colorBackGround.blue()));
}

void CCMainWindow::addCompanyDeps(QTreeWidgetItem* pRootGroupItem, const QString& sDeps)//��ӹ�˾����
{
	QTreeWidgetItem* pChild = new QTreeWidgetItem;
	//����ӽڵ� //��������Ϊ1
	pChild->setData(0, Qt::UserRole, 1);//�������ض���Ӧ�ó���Ŀ�ĵĵ�һ����ɫ
	pChild->setData(0, Qt::UserRole + 1, QString::number((int)pChild));//��pChild���ĵ�ַ��Ϊ����Ψһ�����֤

	//��ϵ������뵽this->ui.treeWidget��,չ������
	ContactItem* pContactItem = new ContactItem(ui.treeWidget);//��Ϊ������

	//������ϵ���������
	QPixmap pix;
	pix.load(":/Resources/MainWindow/head_mask.png");
	pContactItem->setHeadPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/girl.png"),pix, pContactItem->getHeadLabelSize()));//ͷ��
	pContactItem->setUserName(sDeps);//��ϵ���û���
	pRootGroupItem->addChild(pChild);//�����������
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);//������Ŀ����

	//����ǰ������ӳ����ڹ���,������ //��Ȼ�Ļ�����Ҫ����this->ui.treeWidget��������
	m_groupMap.insert(pChild, sDeps);
}

void CCMainWindow::resizeEvent(QResizeEvent* event)
{
	setUserName(QString::fromLocal8Bit("��QQ������֮��,C+����++,�ֻ���҅�Ǹ���"));//�ӱ��صı���ת����Unicode��������QString����
	BasicWindow::resizeEvent(event);
}

bool CCMainWindow::eventFilter(QObject* obj, QEvent* event)
{
	if (ui.searchLineEdit == obj) {
		if (event->type() == QEvent::FocusIn) {//��ü��̽����¼�
			//������ʽ�� 
			ui.searchWidget->setStyleSheet(QString("QWidget#searchWidget{background-color:rgb(255,255,255);border-bottom:1px solid rgba(%1,%2,%3,100);}\
			QPushButton#searchBtn{border-image:url(:/Resources/MainWindow/search/main_search_deldown.png)}\
			QPushButton#searchBtn:hover{border-image:url(:/Resources/MainWindow/search/main_search_delhighlight.png)}\
			QPushButton#searchBtn:Pressed{border-image:url(:/Resources/MainWindow/search/main_search_delhighdown.png)}")
			.arg(m_colorBackGround.red()).arg(m_colorBackGround.green()).arg(m_colorBackGround.blue()));
		}
		else if (event->type() == QEvent::FocusOut) {//�����뿪
			updateSeachStyle();

		}
	}
	return false;
}

void CCMainWindow::mousePressEvent(QMouseEvent* event)
{
	//����Ĳ������������������һ��н���
	if (qApp->widgetAt(event->pos()) != ui.searchLineEdit && ui.searchLineEdit->hasFocus()) {
		ui.searchLineEdit->clearFocus();//�������
	}
	else if (qApp->widgetAt(event->pos()) != ui.lineEdit && ui.lineEdit->hasFocus()) {
		ui.lineEdit->clearFocus();
	}

	BasicWindow::mousePressEvent(event);
}

void CCMainWindow::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	//������(��ǰ����,ɫֵ)
	painter.fillRect(rect(), QColor(255, 255, 255, 255));
}

void CCMainWindow::onItemClicked(QTreeWidgetItem* item, int column)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();//�ж���Ŀ������
	if (!bIsChild) {//����
		item->setExpanded(!item->isExpanded());//δչ����չ������
	}
}

void CCMainWindow::onItemExpanded(QTreeWidgetItem* item)
{//չ���������
	bool bIsChild = item->data(0, Qt::UserRole).toBool();//�ж���Ŀ������0��1;//0�Ǹ���,1������
	if (!bIsChild) {//����
		//dynamic_cast ���������ָ��(������)ת�����̳���
		RootContactItem* prootItem = dynamic_cast<RootContactItem*>(ui.treeWidget->itemWidget(item, 0));//ת������
		if (prootItem) {
			prootItem->setExpanded(true);//����չ���Ƕ�
		}
	}
}

void CCMainWindow::onItemCollapsed(QTreeWidgetItem* item)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();//�ж���Ŀ������0��1;//0�Ǹ���,1������
	if (!bIsChild) {//����
		//dynamic_cast ���������ָ��(������)ת�����̳���
		RootContactItem* prootItem = dynamic_cast<RootContactItem*>(ui.treeWidget->itemWidget(item, 0));//ת������
		if (prootItem) {
			prootItem->setExpanded(false);//����չ���Ƕ�
		}
	}
}

void CCMainWindow::onItemDoubleClicked(QTreeWidgetItem* item, int column)
{
	bool bIsChild = item->data(0, Qt::UserRole).toBool();
	if (bIsChild) {//���������
		QString strGroup = m_groupMap.value(item);//˫�������ĸ���ϵ��
		if (strGroup == QString::fromLocal8Bit("��˾Ⱥ")) {
			//�������::��ȡ�������ʵ��->�������ϵ����(������һ��,��������)
			WindowManager::getInstance()->addNewTalkWindow(item->data(0,Qt::UserRole+1).toString(), GroupType::COMPANY);
		}
		else if (strGroup == QString::fromLocal8Bit("���²�")) {
			WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), GroupType::PERSONELGROUP);
		}
		else if (strGroup == QString::fromLocal8Bit("�г���")) {
			WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), GroupType::MARKETGROUP);
		}
		else if (strGroup == QString::fromLocal8Bit("�з���")) {
			WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole + 1).toString(), GroupType::DEVELOPMENTGROUP);
		}
	}
}

void CCMainWindow::onAppIconClicked() {
	//sender()��OBJECT�����źŵĺ���
	
	if (sender()->objectName() == "app_skin") {//�ж��źŷ����߶������Ƿ���
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