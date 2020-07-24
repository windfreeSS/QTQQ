#include "TalkWindow.h"

TalkWindow::TalkWindow(QWidget* parent, const QString& uid/*, enum class GroupType groupType*/)
	: QWidget(parent),m_talkId(uid)
	//,m_groupType(groupType)
{
	ui.setupUi(this);
	WindowManager::getInstance()->addWindowName(m_talkId,this);//���������ʵ�����ӳ��
	setAttribute(Qt::WA_DeleteOnClose);//���ô�������
	initGroupTalkStatus();
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
		WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole+1).toString()/*, GroupType::PTOP, strPeopleName*/);
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

	if (m_isGroupTalk) {
		initTalkWindow();//��ʼ��Ⱥ��
	}
	else {
		initPtoPTalk();//��ʼ������
	}

	/*switch (m_groupType)
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
	}*/
}

void TalkWindow::initGroupTalkStatus()
{
	QSqlQueryModel sqlDepModel;
	QString strSql = QString("select * from qtqq.tab_department where departmentID = %1").arg(m_talkId);
	sqlDepModel.setQuery(strSql);
	int row = sqlDepModel.rowCount();
	if (row == 0) {//����
		m_isGroupTalk = false;
	}
	else {//Ⱥ��
		m_isGroupTalk = true;
	}
}

void TalkWindow::initTalkWindow()
{
	QTreeWidgetItem* prootitem = new QTreeWidgetItem;//����
	prootitem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//��������Ĳ���(û�����������)

	prootitem->setData(0, Qt::UserRole, 0);//����������ʾ���ָ�����
	RootContactItem* pitemname = new RootContactItem(false, ui.treeWidget);//����ϵ��(�޼�ͷ,����)
	
	ui.treeWidget->setFixedHeight(646);//shell�߶�-shellͷ��(talkwindow::ui.titlewidget)

	//��ǰ�����Ⱥ������������
	QString strGroupName;
	QSqlQuery queryGroupName(QString("select department_name from qtqq.tab_department where departmentID = %1").arg(m_talkId));
	queryGroupName.exec();
	if (queryGroupName.next()) {
		strGroupName= queryGroupName.value(0).toString();
	}

	//��˾Ⱥ����Ⱥ
	QSqlQueryModel queryEmployeeModel;
	if (getCompDepID() == m_talkId.toInt()) {//��˾Ⱥ
		queryEmployeeModel.setQuery("select employeeID from qtqq.tab_employees where status = 1");
	}
	else {
		queryEmployeeModel.setQuery(QString("select employeeID from qtqq.tab_employees where status = 1 and departmentID = %1").arg(m_talkId.toInt()));
	}

	int nemployeenum = queryEmployeeModel.rowCount();//Ⱥ��ȺԱ����

	//Ⱥ����ϵ����ı�
	QString qsGroupName = QString("%1 %2/%3").arg(strGroupName).arg(0).arg(nemployeenum);//����/������
	pitemname->setText(qsGroupName);

	//�������ڵ�
	ui.treeWidget->addTopLevelItem(prootitem);
	ui.treeWidget->setItemWidget(prootitem, 0, pitemname);

	//չ��
	prootitem->setExpanded(true);

	//�������
	for (int i = 0; i < nemployeenum; i++) {
		QModelIndex modelIndex = queryEmployeeModel.index(i, 0);
		int employeeID = queryEmployeeModel.data(modelIndex).toInt();//Ա��QQ��
		addPeopInfo(prootitem,employeeID);
	}
}

int TalkWindow::getCompDepID()
{
	QSqlQuery queryDepID(QString("select departmentID from qtqq.tab_department where department_name = '%1'").arg(QString::fromLocal8Bit("��˾Ⱥ")));
	queryDepID.exec();
	queryDepID.next();
	return queryDepID.value(0).toInt();
}

//void talkwindow::initcompanytalk()
//{
//	qtreewidgetitem* prootitem = new qtreewidgetitem;//����
//	prootitem->setchildindicatorpolicy(qtreewidgetitem::showindicator);//��������Ĳ���(û�����������)
//
//	prootitem->setdata(0, qt::userrole, 0);//����������ʾ���ָ�����
//	rootcontactitem* pitemname = new rootcontactitem(false,ui.treewidget);//����ϵ��(�޼�ͷ,����)
//
//	ui.treewidget->setfixedheight(646);//shell�߶�-shellͷ��(talkwindow::ui.titlewidget)
//	
//	int nemployeenum = 50;//ȺԱ����
//	qstring qsgroupname = qstring::fromlocal8bit("��˾Ⱥ %1/%2").arg(0).arg(nemployeenum);//����/������
//	pitemname->settext(qsgroupname);
//	 
//	//�������ڵ�
//	ui.treewidget->addtoplevelitem(prootitem);
//	ui.treewidget->setitemwidget(prootitem,0,pitemname);
//
//	//չ��
//	prootitem->setexpanded(true);
//
//	//�������
//	for (int i = 0; i < nemployeenum; i++) {
//		addpeopinfo(prootitem);
//	}
//}
//
//void talkwindow::initpersoneltalk()
//{
//	qtreewidgetitem* prootitem = new qtreewidgetitem;//����
//	prootitem->setchildindicatorpolicy(qtreewidgetitem::showindicator);//��������Ĳ���(û�����������)
//
//	prootitem->setdata(0, qt::userrole, 0);//����������ʾ���ָ�����
//	rootcontactitem* pitemname = new rootcontactitem(false, ui.treewidget);//����ϵ��(�޼�ͷ,����)
//
//	ui.treewidget->setfixedheight(646);//shell�߶�-shellͷ��(talkwindow::ui.titlewidget)
//
//	int nemployeenum = 5;//ȺԱ����
//	qstring qsgroupname = qstring::fromlocal8bit("���²� %1/%2").arg(0).arg(nemployeenum);//����/������
//	pitemname->settext(qsgroupname);
//
//	//�������ڵ�
//	ui.treewidget->addtoplevelitem(prootitem);
//	ui.treewidget->setitemwidget(prootitem, 0, pitemname);
//
//	//չ��
//	prootitem->setexpanded(true);
//
//	//�������
//	for (int i = 0; i < nemployeenum; i++) {
//		addpeopinfo(prootitem);
//	}
//}
//
//void talkwindow::initmarkettalk()
//{
//	qtreewidgetitem* prootitem = new qtreewidgetitem;//����
//	prootitem->setchildindicatorpolicy(qtreewidgetitem::showindicator);//��������Ĳ���(û�����������)
//
//	prootitem->setdata(0, qt::userrole, 0);//����������ʾ���ָ�����
//	rootcontactitem* pitemname = new rootcontactitem(false, ui.treewidget);//����ϵ��(�޼�ͷ,����)
//
//	ui.treewidget->setfixedheight(646);//shell�߶�-shellͷ��(talkwindow::ui.titlewidget)
//
//	int nemployeenum = 8;//ȺԱ����
//	qstring qsgroupname = qstring::fromlocal8bit("�г��� %1/%2").arg(0).arg(nemployeenum);//����/������
//	pitemname->settext(qsgroupname);
//
//	//�������ڵ�
//	ui.treewidget->addtoplevelitem(prootitem);
//	ui.treewidget->setitemwidget(prootitem, 0, pitemname);
//
//	//չ��
//	prootitem->setexpanded(true);
//
//	//�������
//	for (int i = 0; i < nemployeenum; i++) {
//		addpeopinfo(prootitem);
//	}
//}
//
//void talkwindow::initdeveloptalk()
//{
//	qtreewidgetitem* prootitem = new qtreewidgetitem;//����
//	prootitem->setchildindicatorpolicy(qtreewidgetitem::showindicator);//��������Ĳ���(û�����������)
//
//	prootitem->setdata(0, qt::userrole, 0);//����������ʾ���ָ�����
//	rootcontactitem* pitemname = new rootcontactitem(false, ui.treewidget);//����ϵ��(�޼�ͷ,����)
//
//	ui.treewidget->setfixedheight(646);//shell�߶�-shellͷ��(talkwindow::ui.titlewidget)
//
//	int nemployeenum = 32;//ȺԱ����
//	qstring qsgroupname = qstring::fromlocal8bit("�з��� %1/%2").arg(0).arg(nemployeenum);//����/������
//	pitemname->settext(qsgroupname);
//
//	//�������ڵ�
//	ui.treewidget->addtoplevelitem(prootitem);
//	ui.treewidget->setitemwidget(prootitem, 0, pitemname);
//
//	//չ��
//	prootitem->setexpanded(true);
//
//	//�������
//	for (int i = 0; i < nemployeenum; i++) {
//		addpeopinfo(prootitem);
//	}
//}

void TalkWindow::initPtoPTalk()
{
	QPixmap pixSkin;
	pixSkin.load(":/Resources/MainWindow/skin.png");
	ui.widget->setFixedSize(pixSkin.size());

	QLabel* skinLabel = new QLabel(ui.widget);
	skinLabel->setPixmap(pixSkin);
	skinLabel->setFixedSize(ui.widget->size());
}

void TalkWindow::addPeopInfo(QTreeWidgetItem* pRootGroupItem,int employeeID)
{
	QTreeWidgetItem* pChild = new QTreeWidgetItem();

	//����ӽڵ�
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, employeeID);//���������ַ��Ϊ������Ψһ���֤��

	ContactItem* pContactItem = new ContactItem(ui.treeWidget);//��ϵ��

	//��ȡ��,ǩ��,ͷ��
	QString strName, strSign, strPicturePath;
	QSqlQueryModel queryInfoModel;
	queryInfoModel.setQuery(QString("select employee_name,employee_sign,picture from qtqq.tab_employees where employeeID = %1 ").arg(employeeID));

	QModelIndex nameIndex, signIndex, pictureIndex;
	nameIndex = queryInfoModel.index(0, 0);//��,��
	signIndex = queryInfoModel.index(0, 1);
	pictureIndex = queryInfoModel.index(0, 2);

	strName = queryInfoModel.data(nameIndex).toString();
	strSign = queryInfoModel.data(signIndex).toString();
	strPicturePath = queryInfoModel.data(pictureIndex).toString();

	QPixmap pix1;
	pix1.load(":/Resources/MainWindow/head_mask.png");
	QImage image1(strPicturePath);

	//static int ii = 0;
	pContactItem->setHeadPixmap(CommonUtils::getRoundImage(QPixmap::fromImage(image1), pix1, pContactItem->getHeadLabelSize())); 
	pContactItem->setUserName(strName);//�û���
	pContactItem->setSignName(strSign);//ǩ��

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

	QString str = pContactItem->getUserName();
	m_groupPeopleMap.insert(pChild, str);
}
