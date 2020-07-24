#include "TalkWindowShell.h"

TalkWindowShell::TalkWindowShell(BasicWindow* parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);//ʹQt��С�������ܹر��¼�ʱɾ����С����
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
	m_talkWindowItemMap.insert(aItem, talkWindow);//��������б���������ݵĹ���

	aItem->setSelected(true);//���ñ�ѡ��

	//�ж���Ⱥ�Ļ��ǵ���
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

	//����ͷ��
	talkWindowItem->setHeadPixmap(QPixmap::fromImage(img));

	//������
	ui.listWidget->addItem(aItem);
	ui.listWidget->setItemWidget(aItem, talkWindowItem);

	onTalkWindowItemClicked(aItem);//�����ұ߲���

	//��ϵ������رյ���� //���촰������رպ��ͷ���Դ
	connect(talkWindowItem, &TalkWindowItem::signalCloseClicked, [talkWindowItem, talkWindow, aItem, this]() {
		//�������
		m_talkWindowItemMap.remove(aItem);//��map���Ƴ�
		talkWindow->close();//�ر���Ӧ��ϵ����
		ui.listWidget->takeItem(ui.listWidget->row(aItem));//�Ƴ�����б����Ŀ
		delete talkWindowItem;//�ͷ���Դ
		//�����ұ�
		ui.rightStackedWidget->removeWidget(talkWindow);//����Ӧ�Ĵ�����Դɾ��
		if (ui.rightStackedWidget->count() < 1) close();//����ұ߶�ҳ�沿������<1
		});
}

void TalkWindowShell::setCurrenWidget(QWidget* widget)
{
	if(widget)
	ui.rightStackedWidget->setCurrentWidget(widget);//���õ�ǰ����
}

const QMap<QListWidgetItem*, QWidget*>& TalkWindowShell::gettalkWindowItemMap() const
{
	return m_talkWindowItemMap;
}

void TalkWindowShell::onEmotionBtnClicked(bool) {//���鰴ť�����ִ�еĲۺ���
	m_emotionWindow->setVisible(!m_emotionWindow->isVisible());//���ÿɼ����ɼ�
	QPoint emotionPoint = this->mapToGlobal(QPoint(0, 0));//����ǰ�ؼ������λ��ת��Ϊ��Ļ�ľ���λ��

	emotionPoint.setX(emotionPoint.x() + 170);
	emotionPoint.setY(emotionPoint.y() + 170);
	m_emotionWindow->move(emotionPoint);//���鴰��λ��
}

void TalkWindowShell::onTalkWindowItemClicked(QListWidgetItem* item) {//����б�����ִ�еĲۺ���
	//�����ұߵĲ���
	QWidget* talkwindowWidget = m_talkWindowItemMap.find(item).value();
	ui.rightStackedWidget->setCurrentWidget(talkwindowWidget);
}

void TalkWindowShell::onEmotionItemClicked(int emotionNum)
{
	//���ұ߶�ҳ�沿���ĵ�ǰ��ʾ����ת��
	TalkWindow* curTalkWindow = dynamic_cast<TalkWindow*>(ui.rightStackedWidget->currentWidget());
	if (curTalkWindow) {
		curTalkWindow->addEmotionImage(emotionNum);//���ͼƬ
	}
}

void TalkWindowShell::initControl()
{
	loadStyleSheet("TalkWindow");
	setWindowTitle(QString::fromLocal8Bit("���-���������ڴ���"));
	m_emotionWindow = new EmotionWindow;
	m_emotionWindow->hide();//���ر��鴰��

	QList<int> leftWidgetSize;
	leftWidgetSize << 154 << width() - 154;
	ui.splitter->setSizes(leftWidgetSize);//�������������óߴ�

	ui.listWidget->setStyle(new CustomProxyStyle(this));

	connect(ui.listWidget,&QListWidget::itemClicked,this,&TalkWindowShell::onTalkWindowItemClicked);
	connect(m_emotionWindow, SIGNAL(signalEmotionItemClicked(int)), this, SLOT(onEmotionItemClicked(int)));
}
