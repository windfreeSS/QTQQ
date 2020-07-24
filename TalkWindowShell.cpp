#include "TalkWindowShell.h"

TalkWindowShell::TalkWindowShell(BasicWindow* parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);//使Qt在小部件接受关闭事件时删除该小部件
	initControl();
}

TalkWindowShell::~TalkWindowShell()
{
	if (m_emotionWindow) {
		delete m_emotionWindow;
		m_emotionWindow = nullptr;
	}
	else {
		m_emotionWindow = nullptr;
	}
}

void TalkWindowShell::addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem,const QString& uid/*,enum class GroupType grouptype*/)
{
	ui.rightStackedWidget->addWidget(talkWindow);
	connect(m_emotionWindow, SIGNAL(signalEmotionWindowHide()), talkWindow, SLOT(onSetEmotionBtnStatus()));

	QListWidgetItem* aItem = new QListWidgetItem(ui.listWidget);
	m_talkWindowItemMap.insert(aItem, talkWindow);//左边聊天列表和聊天内容的关联

	aItem->setSelected(true);//设置被选中

	//判断是群聊还是单聊
	QSqlQueryModel sqlDepModel;
	QString strQuery = QString("select picture  from qtqq.tab_department where departmentID = %1").arg(uid);
	sqlDepModel.setQuery(strQuery);
	int row = sqlDepModel.rowCount();

	if (row == 0) {
		strQuery= QString("select picture  from qtqq.tab_employees where employeeID = %1").arg(uid);
		sqlDepModel.setQuery(strQuery);
	}

	QModelIndex index;
	index = sqlDepModel.index(0, 0);
	QImage img;
	img.load(sqlDepModel.data(index).toString());

	//设置头像
	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));

	//添加项部件
	ui.listWidget->addItem(aItem);
	ui.listWidget->setItemWidget(aItem, talkWindowItem);

	onTalkWindowItemClicked(aItem);//更新右边部件

	//联系窗口项关闭点击后 //聊天窗口主体关闭后释放资源
	connect(talkWindowItem, &TalkWindowItem::signalCloseClicked, [talkWindowItem, talkWindow, aItem, this]() {
		//处理左边
		m_talkWindowItemMap.remove(aItem);//从map中移除
		talkWindow->close();//关闭相应联系窗口
		ui.listWidget->takeItem(ui.listWidget->row(aItem));//移除左边列表的项目
		delete talkWindowItem;//释放资源
		//处理右边
		ui.rightStackedWidget->removeWidget(talkWindow);//将对应的窗口资源删除
		if (ui.rightStackedWidget->count() < 1) close();//如果右边多页面部件总数<1
		});
}

void TalkWindowShell::setCurrenWidget(QWidget* widget)
{
	if(widget)
	ui.rightStackedWidget->setCurrentWidget(widget);//设置当前部件
}

const QMap<QListWidgetItem*, QWidget*>& TalkWindowShell::gettalkWindowItemMap() const
{
	return m_talkWindowItemMap;
}

void TalkWindowShell::onEmotionBtnClicked(bool) {//表情按钮点击后执行的槽函数
	m_emotionWindow->setVisible(!m_emotionWindow->isVisible());//设置可见不可见
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));//将当前控件的相对位置转换为屏幕的绝对位置

	emotionPoint.setX(emotionPoint.x() + 170);
	emotionPoint.setY(emotionPoint.y() + 170);
	m_emotionWindow->move(emotionPoint);//表情窗口位置
}

void TalkWindowShell::onTalkWindowItemClicked(QListWidgetItem* item) {//左侧列表点击后执行的槽函数
	//更新右边的部件
	QWidget* talkwindowWidget = m_talkWindowItemMap.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talkwindowWidget);
}

void TalkWindowShell::onEmotionItemClicked(int emotionNum)
{
	//将右边多页面部件的当前显示部件转换
	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	if (curTalkWindow) {
		curTalkWindow->addEmotionImage(emotionNum);//添加图片
	}
}

void TalkWindowShell::initControl()
{
	loadStyleSheet("TalkWindow");
	setWindowTitle(QString::fromLocal8Bit("编程-聊天主窗口窗口"));
	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();//隐藏表情窗口

	QList<int> leftWidgetSize;
	leftWidgetSize << 154 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);//分裂器进行设置尺寸

	ui.listWidget->setStyle(new CustomProxyStyle(this));

	connect(ui.listWidget,&QListWidget::itemClicked,this,&TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));
}
