#include "TalkWindow.h"

TalkWindow::TalkWindow(QWidget* parent, const QString& uid, enum class GroupType groupType)
	: QWidget(parent),m_talkId(uid),m_groupType(groupType)
{
	ui.setupUi(this);
	WindowManager::getInstance()->addWindowName(m_talkId,this);//���������ʵ�����ӳ��
	setAttribute(Qt::WA_DeleteOnClose);
	initControl();
}

TalkWindow::~TalkWindow()
{
	//�ر���ϵ���ڽ�������������map����Ӧ����ɾ��,���������ӵ������ɴ���ָ�������ٵ��ڴ�ռ�
	WindowManager::getInstance()->deleteWindowName(m_talkId);
}

void TalkWindow::addEmotionImage(int emotionNum)//��ӱ���ͼƬ
{
	ui.textEdit->setFocus();//��ȡ����
	ui.textEdit->addEmotionUrl(emotionNum);
}

void TalkWindow::setWindowName(const QString& name)
{
	ui.nameLabel->setText(name);
}

void TalkWindow::onSendBtnClicked(bool) {
	if (ui.textEdit->toPlainText().isEmpty()) {//ת��Ϊ���ı�,�ж��Ƿ�Ϊ��
		//��ʾ��ʾ��Ϣ(����,��ʾ��Ϣ,������,��ʾ���С,��ʾʱ��)
		QToolTip::showText(this->mapToGlobal(QPoint(630, 660)),QString::fromLocal8Bit("���͵���Ϣ����Ϊ��!"),this,QRect(0,0,120,100),2000);
		return;
	}

	QString& html= ui.textEdit->document()->toHtml();//��ȡ��ǰ�ĵ�ת��Ϊhtml

	//�ı�htmlt���û��������������� 
	if ((!html.contains(".png")&& !html.contains("</span>")) || (html.contains(".png") && html.contains("</span>"))) {//html������ͼƬͬʱ�������ı�
		QString fontHtml;
		QString text = ui.textEdit->toPlainText();//�����û�������ı�
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly)) {
			fontHtml = file.readAll();
			fontHtml.replace("%1", text);
			file.close();
		}
		else {
			QMessageBox::information(this,QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("msgFont.txt�ļ�������"));//��ʾ
			return;
		}
		if (!html.contains(fontHtml)) {//���û�а����ͽ����ı��滻
			html.replace(text, fontHtml);
		}
	}
	//bug ��ͼ+����,ֻ��ʾ����û����ʾ����
	
	ui.textEdit->clear();//���
	ui.textEdit->deleteAllEmotionImage();//ɾ�����б���
	ui.msgWidget->appendMsg(html);//����Ϣ���������ҳ��Ϣ
}

void TalkWindow::onItemDoubleClicked(QTreeWidgetItem* item, int column) {//��˫��������һ����в���,��
	//�Ƿ�������
	if (item->data(0, Qt::UserRole).toBool()) {//����
		QString strPeopleName = m_groupPeopleMap.value(item);//��ȡ����
		WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole+1).toString(), GroupType::PTOP, strPeopleName);
	}
}

void TalkWindow::initControl()
{
	QList<int> rightWidgetSize;//�ұ߲����ߴ��С
	rightWidgetSize << 600 << 138;
	ui.bodySplitter->setSizes(rightWidgetSize);//���÷�������С
	ui.textEdit->setFontPointSize(10);//���ñ༭�������С
	ui.textEdit->setFocus();//���ý���

	connect(ui.sysmin, SIGNAL(clicked(bool)), parent(), SLOT(onShowMin(bool)));//���촰����С����ť�����,��������֮��Ӧ
	connect(ui.sysclose, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));
	connect(ui.closeBtn, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));

	connect(ui.faceBtn, SIGNAL(clicked(bool)), parent(), SLOT(onEmotionBtnClicked(bool)));
	connect(ui.sendBtn, SIGNAL(clicked(bool)), this, SLOT(onSendBtnClicked(bool)));

	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));

	switch (m_groupType)
	{
	case GroupType::COMPANY:
		initCompanyTalk();
		break;
	case GroupType::PERSONELGROUP:
		initPersonelTalk();
		break;
	case GroupType::DEVELOPMENTGROUP:
		initDevelopTalk();
		break;
	case GroupType::MARKETGROUP:
		initMarketTalk();
		break;
	case GroupType::PTOP:
		initPtoPTalk();
		break;
	default:
		break;
	}
}

void TalkWindow::initCompanyTalk()
{
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem;//����
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//��������Ĳ���(û�����������)

	pRootItem->setData(0, Qt::UserRole, 0);//����������ʾ���ָ�����
	RootContactItem* pItemName = new RootContactItem(false,ui.treeWidget);//����ϵ��(�޼�ͷ,����)

	ui.treeWidget->setFixedHeight(646);//shell�߶�-shellͷ��(TalkWindow::ui.titleWidget)
	
	int nEmployeeNum = 50;//ȺԱ����
	QString qsGroupName = QString::fromLocal8Bit("��˾Ⱥ %1/%2").arg(0).arg(nEmployeeNum);//����/������
	pItemName->setText(qsGroupName);
	 
	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem,0,pItemName);

	//չ��
	pRootItem->setExpanded(true);

	//�������
	for (int i = 0; i < nEmployeeNum; i++) {
		addPeopInfo(pRootItem);
	}
}

void TalkWindow::initPersonelTalk()
{
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem;//����
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//��������Ĳ���(û�����������)

	pRootItem->setData(0, Qt::UserRole, 0);//����������ʾ���ָ�����
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);//����ϵ��(�޼�ͷ,����)

	ui.treeWidget->setFixedHeight(646);//shell�߶�-shellͷ��(TalkWindow::ui.titleWidget)

	int nEmployeeNum = 5;//ȺԱ����
	QString qsGroupName = QString::fromLocal8Bit("���²� %1/%2").arg(0).arg(nEmployeeNum);//����/������
	pItemName->setText(qsGroupName);

	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//չ��
	pRootItem->setExpanded(true);

	//�������
	for (int i = 0; i < nEmployeeNum; i++) {
		addPeopInfo(pRootItem);
	}
}

void TalkWindow::initMarketTalk()
{
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem;//����
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//��������Ĳ���(û�����������)

	pRootItem->setData(0, Qt::UserRole, 0);//����������ʾ���ָ�����
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);//����ϵ��(�޼�ͷ,����)

	ui.treeWidget->setFixedHeight(646);//shell�߶�-shellͷ��(TalkWindow::ui.titleWidget)

	int nEmployeeNum = 8;//ȺԱ����
	QString qsGroupName = QString::fromLocal8Bit("�г��� %1/%2").arg(0).arg(nEmployeeNum);//����/������
	pItemName->setText(qsGroupName);

	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//չ��
	pRootItem->setExpanded(true);

	//�������
	for (int i = 0; i < nEmployeeNum; i++) {
		addPeopInfo(pRootItem);
	}
}

void TalkWindow::initDevelopTalk()
{
	QTreeWidgetItem* pRootItem = new QTreeWidgetItem;//����
	pRootItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//��������Ĳ���(û�����������)

	pRootItem->setData(0, Qt::UserRole, 0);//����������ʾ���ָ�����
	RootContactItem* pItemName = new RootContactItem(false, ui.treeWidget);//����ϵ��(�޼�ͷ,����)

	ui.treeWidget->setFixedHeight(646);//shell�߶�-shellͷ��(TalkWindow::ui.titleWidget)

	int nEmployeeNum = 32;//ȺԱ����
	QString qsGroupName = QString::fromLocal8Bit("�з��� %1/%2").arg(0).arg(nEmployeeNum);//����/������
	pItemName->setText(qsGroupName);

	//�������ڵ�
	ui.treeWidget->addTopLevelItem(pRootItem);
	ui.treeWidget->setItemWidget(pRootItem, 0, pItemName);

	//չ��
	pRootItem->setExpanded(true);

	//�������
	for (int i = 0; i < nEmployeeNum; i++) {
		addPeopInfo(pRootItem);
	}
}

void TalkWindow::initPtoPTalk()
{
	QPixmap pixSkin;
	pixSkin.load(":/Resources/MainWindow/skin.png");
	ui.widget->setFixedSize(pixSkin.size());

	QLabel* skinLabel = new QLabel(ui.widget);
	skinLabel->setPixmap(pixSkin);
	skinLabel->setFixedSize(ui.widget->size());
}

void TalkWindow::addPeopInfo(QTreeWidgetItem* pRootGroupItem)
{
	QTreeWidgetItem* pChild = new QTreeWidgetItem();

	QPixmap pix1;
	pix1.load(":/Resources/MainWindow/head_mask.png");
	const QPixmap image1(":/Resources/MainWindow/girl.png");

	//����ӽڵ�
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, QString::number((int)pChild));//���������ַ��Ϊ������Ψһ���֤��
	
	ContactItem* pContactItem = new ContactItem(ui.treeWidget);//��ϵ��

	static int ii = 0;
	pContactItem->setHeadPixmap(CommonUtils::getRoundImage(image1, pix1, pContactItem->getHeadLabelSize()));
	pContactItem->setUserName(QString::fromLocal8Bit("��ţ����%1").arg(ii));//�û���
	pContactItem->setSignName(QString::fromLocal8Bit("����%1").arg(ii++));//ǩ��

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

	QString str = pContactItem->getUserName();
	m_groupPeopleMap.insert(pChild, str);
}
