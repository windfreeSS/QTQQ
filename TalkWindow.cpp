#include "TalkWindow.h"

TalkWindow::TalkWindow(QWidget* parent, const QString& uid/*, enum class GroupType groupType*/)
	: QWidget(parent),m_talkId(uid)
	//,m_groupType(groupType)
{
	ui.setupUi(this);
	WindowManager::getInstance()->addWindowName(m_talkId,this);//往窗体管理实例添加映射
	setAttribute(Qt::WA_DeleteOnClose);//设置窗体属性
	initGroupTalkStatus();
	initControl();
}

TalkWindow::~TalkWindow()
{
	//关闭联系窗口进而将窗体管理的map所对应的项删除,以免二次添加导致生成窗口指向以销毁的内存空间
	WindowManager::getInstance()->deleteWindowName(m_talkId);
}

void TalkWindow::addEmotionImage(int emotionNum)//添加表情图片
{
	ui.textEdit->setFocus();//获取焦点
	ui.textEdit->addEmotionUrl(emotionNum);
}

void TalkWindow::setWindowName(const QString& name)
{
	ui.nameLabel->setText(name);
}

void TalkWindow::onSendBtnClicked(bool) {
	if (ui.textEdit->toPlainText().isEmpty()) {//转换为纯文本,判断是否为空
		//显示提示信息(坐标,提示信息,父窗体,提示框大小,显示时间)
		QToolTip::showText(this->mapToGlobal(QPoint(630, 660)),QString::fromLocal8Bit("发送的信息不能为空!"),this,QRect(0,0,120,100),2000);
		return;
	}

	QString& html= ui.textEdit->document()->toHtml();//获取当前文档转换为html

	//文本htmlt如果没有字体则添加字体 
	if ((!html.contains(".png")&& !html.contains("</span>")) || (html.contains(".png") && html.contains("</span>"))) {//html不包含图片同时不包含文本
		QString fontHtml;
		QString text = ui.textEdit->toPlainText();//保存用户输入的文本
		QFile file(":/Resources/MainWindow/MsgHtml/msgFont.txt");
		if (file.open(QIODevice::ReadOnly)) {
			fontHtml = file.readAll();
			fontHtml.replace("%1", text);
			file.close();
		}
		else {
			QMessageBox::information(this,QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("msgFont.txt文件不存在"));//提示
			return;
		}
		if (!html.contains(fontHtml)) {//如果没有包含就进行文本替换
			html.replace(text, fontHtml);
		}
	}
	//bug 发图+文字,只显示表情没有显示文字
	
	ui.textEdit->clear();//清空
	ui.textEdit->deleteAllEmotionImage();//删除所有表情
	ui.msgWidget->appendMsg(html);//收信息窗口添加网页信息
}

void TalkWindow::onItemDoubleClicked(QTreeWidgetItem* item, int column) {//对双击的是哪一项进行操作,列
	//是否是子项
	if (item->data(0, Qt::UserRole).toBool()) {//子项
		QString strPeopleName = m_groupPeopleMap.value(item);//获取人名
		WindowManager::getInstance()->addNewTalkWindow(item->data(0, Qt::UserRole+1).toString()/*, GroupType::PTOP, strPeopleName*/);
	}
}

void TalkWindow::initControl()
{
	QList<int> rightWidgetSize;//右边部件尺寸大小
	rightWidgetSize << 600 << 138;
	ui.bodySplitter->setSizes(rightWidgetSize);//设置分裂器大小
	ui.textEdit->setFontPointSize(10);//设置编辑区字体大小
	ui.textEdit->setFocus();//设置焦点

	connect(ui.sysmin, SIGNAL(clicked(bool)), parent(), SLOT(onShowMin(bool)));//聊天窗口最小化按钮被点击,父窗体随之响应
	connect(ui.sysclose, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));
	connect(ui.closeBtn, SIGNAL(clicked(bool)), parent(), SLOT(onShowClose(bool)));

	connect(ui.faceBtn, SIGNAL(clicked(bool)), parent(), SLOT(onEmotionBtnClicked(bool)));
	connect(ui.sendBtn, SIGNAL(clicked(bool)), this, SLOT(onSendBtnClicked(bool)));

	connect(ui.treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));

	if (m_isGroupTalk) {
		initTalkWindow();//初始化群聊
	}
	else {
		initPtoPTalk();//初始化单聊
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
	if (row == 0) {//单聊
		m_isGroupTalk = false;
	}
	else {//群聊
		m_isGroupTalk = true;
	}
}

void TalkWindow::initTalkWindow()
{
	QTreeWidgetItem* prootitem = new QTreeWidgetItem;//根项
	prootitem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);//设置子项的策略(没有子项都能收缩)

	prootitem->setData(0, Qt::UserRole, 0);//设置数据以示区分父和子
	RootContactItem* pitemname = new RootContactItem(false, ui.treeWidget);//根联系项(无箭头,父项)
	
	ui.treeWidget->setFixedHeight(646);//shell高度-shell头高(talkwindow::ui.titlewidget)

	//当前聊天的群组名或人名称
	QString strGroupName;
	QSqlQuery queryGroupName(QString("select department_name from qtqq.tab_department where departmentID = %1").arg(m_talkId));
	queryGroupName.exec();
	if (queryGroupName.next()) {
		strGroupName= queryGroupName.value(0).toString();
	}

	//公司群或部门群
	QSqlQueryModel queryEmployeeModel;
	if (getCompDepID() == m_talkId.toInt()) {//公司群
		queryEmployeeModel.setQuery("select employeeID from qtqq.tab_employees where status = 1");
	}
	else {
		queryEmployeeModel.setQuery(QString("select employeeID from qtqq.tab_employees where status = 1 and departmentID = %1").arg(m_talkId.toInt()));
	}

	int nemployeenum = queryEmployeeModel.rowCount();//群组群员总数

	//群组联系项的文本
	QString qsGroupName = QString("%1 %2/%3").arg(strGroupName).arg(0).arg(nemployeenum);//在线/总人数
	pitemname->setText(qsGroupName);

	//插入分组节点
	ui.treeWidget->addTopLevelItem(prootitem);
	ui.treeWidget->setItemWidget(prootitem, 0, pitemname);

	//展开
	prootitem->setExpanded(true);

	//添加子项
	for (int i = 0; i < nemployeenum; i++) {
		QModelIndex modelIndex = queryEmployeeModel.index(i, 0);
		int employeeID = queryEmployeeModel.data(modelIndex).toInt();//员工QQ号
		addPeopInfo(prootitem,employeeID);
	}
}

int TalkWindow::getCompDepID()
{
	QSqlQuery queryDepID(QString("select departmentID from qtqq.tab_department where department_name = '%1'").arg(QString::fromLocal8Bit("公司群")));
	queryDepID.exec();
	queryDepID.next();
	return queryDepID.value(0).toInt();
}

//void talkwindow::initcompanytalk()
//{
//	qtreewidgetitem* prootitem = new qtreewidgetitem;//根项
//	prootitem->setchildindicatorpolicy(qtreewidgetitem::showindicator);//设置子项的策略(没有子项都能收缩)
//
//	prootitem->setdata(0, qt::userrole, 0);//设置数据以示区分父和子
//	rootcontactitem* pitemname = new rootcontactitem(false,ui.treewidget);//根联系项(无箭头,父项)
//
//	ui.treewidget->setfixedheight(646);//shell高度-shell头高(talkwindow::ui.titlewidget)
//	
//	int nemployeenum = 50;//群员总数
//	qstring qsgroupname = qstring::fromlocal8bit("公司群 %1/%2").arg(0).arg(nemployeenum);//在线/总人数
//	pitemname->settext(qsgroupname);
//	 
//	//插入分组节点
//	ui.treewidget->addtoplevelitem(prootitem);
//	ui.treewidget->setitemwidget(prootitem,0,pitemname);
//
//	//展开
//	prootitem->setexpanded(true);
//
//	//添加子项
//	for (int i = 0; i < nemployeenum; i++) {
//		addpeopinfo(prootitem);
//	}
//}
//
//void talkwindow::initpersoneltalk()
//{
//	qtreewidgetitem* prootitem = new qtreewidgetitem;//根项
//	prootitem->setchildindicatorpolicy(qtreewidgetitem::showindicator);//设置子项的策略(没有子项都能收缩)
//
//	prootitem->setdata(0, qt::userrole, 0);//设置数据以示区分父和子
//	rootcontactitem* pitemname = new rootcontactitem(false, ui.treewidget);//根联系项(无箭头,父项)
//
//	ui.treewidget->setfixedheight(646);//shell高度-shell头高(talkwindow::ui.titlewidget)
//
//	int nemployeenum = 5;//群员总数
//	qstring qsgroupname = qstring::fromlocal8bit("人事部 %1/%2").arg(0).arg(nemployeenum);//在线/总人数
//	pitemname->settext(qsgroupname);
//
//	//插入分组节点
//	ui.treewidget->addtoplevelitem(prootitem);
//	ui.treewidget->setitemwidget(prootitem, 0, pitemname);
//
//	//展开
//	prootitem->setexpanded(true);
//
//	//添加子项
//	for (int i = 0; i < nemployeenum; i++) {
//		addpeopinfo(prootitem);
//	}
//}
//
//void talkwindow::initmarkettalk()
//{
//	qtreewidgetitem* prootitem = new qtreewidgetitem;//根项
//	prootitem->setchildindicatorpolicy(qtreewidgetitem::showindicator);//设置子项的策略(没有子项都能收缩)
//
//	prootitem->setdata(0, qt::userrole, 0);//设置数据以示区分父和子
//	rootcontactitem* pitemname = new rootcontactitem(false, ui.treewidget);//根联系项(无箭头,父项)
//
//	ui.treewidget->setfixedheight(646);//shell高度-shell头高(talkwindow::ui.titlewidget)
//
//	int nemployeenum = 8;//群员总数
//	qstring qsgroupname = qstring::fromlocal8bit("市场部 %1/%2").arg(0).arg(nemployeenum);//在线/总人数
//	pitemname->settext(qsgroupname);
//
//	//插入分组节点
//	ui.treewidget->addtoplevelitem(prootitem);
//	ui.treewidget->setitemwidget(prootitem, 0, pitemname);
//
//	//展开
//	prootitem->setexpanded(true);
//
//	//添加子项
//	for (int i = 0; i < nemployeenum; i++) {
//		addpeopinfo(prootitem);
//	}
//}
//
//void talkwindow::initdeveloptalk()
//{
//	qtreewidgetitem* prootitem = new qtreewidgetitem;//根项
//	prootitem->setchildindicatorpolicy(qtreewidgetitem::showindicator);//设置子项的策略(没有子项都能收缩)
//
//	prootitem->setdata(0, qt::userrole, 0);//设置数据以示区分父和子
//	rootcontactitem* pitemname = new rootcontactitem(false, ui.treewidget);//根联系项(无箭头,父项)
//
//	ui.treewidget->setfixedheight(646);//shell高度-shell头高(talkwindow::ui.titlewidget)
//
//	int nemployeenum = 32;//群员总数
//	qstring qsgroupname = qstring::fromlocal8bit("研发部 %1/%2").arg(0).arg(nemployeenum);//在线/总人数
//	pitemname->settext(qsgroupname);
//
//	//插入分组节点
//	ui.treewidget->addtoplevelitem(prootitem);
//	ui.treewidget->setitemwidget(prootitem, 0, pitemname);
//
//	//展开
//	prootitem->setexpanded(true);
//
//	//添加子项
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

	//添加子节点
	pChild->setData(0, Qt::UserRole, 1);
	pChild->setData(0, Qt::UserRole + 1, employeeID);//将此子项地址作为此子项唯一身份证号

	ContactItem* pContactItem = new ContactItem(ui.treeWidget);//联系项

	//获取名,签名,头像
	QString strName, strSign, strPicturePath;
	QSqlQueryModel queryInfoModel;
	queryInfoModel.setQuery(QString("select employee_name,employee_sign,picture from qtqq.tab_employees where employeeID = %1 ").arg(employeeID));

	QModelIndex nameIndex, signIndex, pictureIndex;
	nameIndex = queryInfoModel.index(0, 0);//行,列
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
	pContactItem->setUserName(strName);//用户名
	pContactItem->setSignName(strSign);//签名

	pRootGroupItem->addChild(pChild);
	ui.treeWidget->setItemWidget(pChild, 0, pContactItem);

	QString str = pContactItem->getUserName();
	m_groupPeopleMap.insert(pChild, str);
}
