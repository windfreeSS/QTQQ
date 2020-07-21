#include "TitleBar.h"


TitleBar::TitleBar(QWidget *parent)
	: QWidget(parent), m_isPressed(false),m_buttonType(ButtonType::MIN_MAX_BUTTON)
{
	initControl();
	initConnections();
	loadStyleSheet("Title");
}

TitleBar::~TitleBar()
{
}

//设置标题栏图标(路径)
void TitleBar::setTitleIcon(const QString& filePath)
{
	QPixmap titleIcon(filePath);//象图映射 /
	m_pIcon->setFixedSize(titleIcon.size());//设置固定大小 //图片有多大就多大
	m_pIcon->setPixmap(titleIcon);
}

//设置标题栏内容
void TitleBar::setTitleContent(const QString& titleContent)
{
	m_pTitleContent->setText(titleContent);
	m_titleContent = titleContent;
}

//设置标题栏宽度
void TitleBar::setTitleWidth(int width)
{
	setFixedWidth(width);
}

//设置标题栏按钮类型
void TitleBar::setButtonTybe(ButtonType buttonTybe)
{
	/*
	//按钮风格
enum ButtonType {
	MIN_BUTTON=0, //最小化及关闭按钮
	MIN_MAX_BUTTON,//最小化,最大化及关闭按钮
	ONLY_CLOSE_BUTTON//只有关闭按钮
};

	QPushButton* m_pButtonMin;//最小化按钮
	QPushButton* m_pButtonRestore;//最大化还原按钮
	QPushButton* m_pButtonClose;//关闭按钮
	QPushButton* m_pButtonMax;//最大化按钮
	*/
	m_buttonType = buttonTybe;
	switch (buttonTybe) {
	case ButtonType::MIN_BUTTON:
		m_pButtonRestore->setVisible(false);//设置可见()
		m_pButtonMax->setVisible(false);
		break;
	case ButtonType::MIN_MAX_BUTTON:
		m_pButtonRestore->setVisible(false);
		break;
	case ButtonType::ONLY_CLOSE_BUTTON:
		m_pButtonMax->setVisible(false);
		m_pButtonMin->setVisible(false);
		m_pButtonRestore->setVisible(false);
		break;
	default:
		break;
	}
}

//保存窗口最大化前的位置及大小
void TitleBar::saveRestoreInfo(const QPoint& point, const QSize& size)
{
	/*
	//最大化还原按钮变量(用于保存窗体位置和大小)
	QPoint m_restorePos;
	QSize m_restoreSize;
	*/
	m_restorePos = point;
	m_restoreSize = size;
}

//获取窗口最大化前的位置及大小
void TitleBar::getRestoreInfo(QPoint& point, QSize& size)
{
	point =m_restorePos;
	size = m_restoreSize;
}

//绘图事件 //绘制标题栏
void TitleBar::paintEvent(QPaintEvent* event)
{
	//画家
	QPainter painter(this);
	//画家轨道
	QPainterPath pathBack;
	//设置填充规则(环绕(核心)填充)
	pathBack.setFillRule(Qt::WindingFill);
	//添加圆角矩形(矩形(左,上,宽,高),x斜率,y斜率)
	pathBack.addRoundRect(QRect(0, 0, width(), height()),3,3);
	//设置渲染(平滑象图变换,true)
	painter.setRenderHint(QPainter::SmoothPixmapTransform,true);

	//当窗口最大化或还原后,窗口宽度改变,标题栏相应做出改变
	//TitleBar部件宽度 != 父部件宽度
	if (width() != parentWidget()->width()) {
		setFixedWidth(parentWidget()->width());
	}

	QWidget::paintEvent(event);//加上=默认部件操作
}

//鼠标双击事件 //双击标题栏进行最大化,最小化操作
void TitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
	/*
	//按钮风格
enum ButtonType {
	MIN_BUTTON=0, //最小化及关闭按钮
	MIN_MAX_BUTTON,//最小化,最大化及关闭按钮
	ONLY_CLOSE_BUTTON//只有关闭按钮
};

	QPushButton* m_pButtonMin;//最小化按钮
	QPushButton* m_pButtonRestore;//最大化还原按钮
	QPushButton* m_pButtonClose;//关闭按钮
	QPushButton* m_pButtonMax;//最大化按钮

	//标题栏右上角按钮类型
	ButtonType m_buttonType;
	*/

	//只有存在最小化,最大化按钮才有效
	if (m_buttonType == ButtonType::MIN_MAX_BUTTON) {
		//最小化按钮是否可见
		if (m_pButtonMin->isVisible()) 
			onButtonMinClicked();//最小化
		else
			onButtonMaxClicked();//最大化
	}

	//返回 部件::鼠标双击事件();
	return QWidget::mouseDoubleClickEvent(event);//默认的事件的处理
}

//鼠标按下/压事件
void TitleBar::mousePressEvent(QMouseEvent* event)
{
	if (m_buttonType == ButtonType::MIN_MAX_BUTTON) {
		//在窗口最大化时禁止拖动窗口
		if (m_pButtonMax->isVisible()) {
			m_isPressed = true;
			m_startMovePos = event->globalPos();//返回事件发生时鼠标所在的全局位置
		}
	}
	else {
		m_isPressed = true;
		m_startMovePos = event->globalPos();
	}

	return QWidget::mousePressEvent(event);
}

//鼠标移动事件
void TitleBar::mouseMoveEvent(QMouseEvent* event)
{
	if (m_isPressed) {
		QPoint movePoint = event->globalPos()- m_startMovePos;//移动多少距离=鼠标现位置-鼠标旧位置
		QPoint widgetPos = parentWidget()->pos();//返回父部件的位置
		m_startMovePos = event->globalPos();
		parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());//父部件进行移动
	}
	return QWidget::mouseMoveEvent(event);
}

//鼠标松开事件
void TitleBar::mouseReleaseEvent(QMouseEvent* event)
{
	m_isPressed = false;
	return QWidget::mouseReleaseEvent(event);
}

//初始化控件
void TitleBar::initControl()
{
	m_pIcon = new QLabel(this);
	m_pTitleContent = new QLabel(this);

	m_pButtonMin = new QPushButton(this);
	m_pButtonRestore = new QPushButton(this);
	m_pButtonMax = new QPushButton(this);
	m_pButtonClose = new QPushButton(this);

	//按钮设置固定大小
	m_pButtonMin->setFixedSize(QSize(BUTTON_HEIGHT, BUTTON_WIDTH));
	m_pButtonRestore->setFixedSize(QSize(BUTTON_HEIGHT, BUTTON_WIDTH));
	m_pButtonMax->setFixedSize(QSize(BUTTON_HEIGHT, BUTTON_WIDTH));
	m_pButtonClose->setFixedSize(QSize(BUTTON_HEIGHT, BUTTON_WIDTH));

	//设置(对象)目标名
	m_pTitleContent->setObjectName("TitleContent");//ui.TitleContent
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pButtonMax->setObjectName("ButtonMax");
	m_pButtonClose->setObjectName("ButtonClose");

	//设置布局(水平布局)
	QHBoxLayout* mylayout = new QHBoxLayout(this);
	//mylayout->setObjectName("水平布局");
	mylayout->addWidget(m_pIcon);
	mylayout->addWidget(m_pTitleContent);

	mylayout->addWidget(m_pButtonMin);
	mylayout->addWidget(m_pButtonRestore);
	mylayout->addWidget(m_pButtonMax);
	mylayout->addWidget(m_pButtonClose);

	//设置目录边缘(布局边框大小) //设置布局间隙(左,上,右,下)
	mylayout->setContentsMargins(5, 0, 0, 0);
	//设置布局里部件间的间隙
	mylayout->setSpacing(0);

	//设置尺寸策略(扩大的,固定的) //拉伸优先扩大,垂直方向固定
	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	setFixedHeight(TITLE_HEIGHT);//设置标题栏高度
	setWindowFlags(Qt::FramelessWindowHint);//设置窗体风格无边框
}

//初始化信号与槽的连接
void TitleBar::initConnections()
{
	/*
	QPushButton* m_pButtonMin;//最小化按钮
	QPushButton* m_pButtonRestore;//最大化还原按钮
	QPushButton* m_pButtonClose;//关闭按钮
	QPushButton* m_pButtonMax;//最大化按钮

	//最小化按钮.槽
	void onButtonMinClicked();
	//最大化还原按钮.槽
	void onButtonRestoreClicked();
	//最大化按钮.槽
	void onButtonMaxClicked();
	//关闭按钮.槽
	void onButtonCloseClicked();
	*/
	//按钮单击响应功能
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
}

//加载样式表(样式表名)
void TitleBar::loadStyleSheet(const QString& sheetName)
{
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);//只读打开
	if (file.isOpen()) {
		QString styleSheet = this->styleSheet();//保存当前样式表
		styleSheet += QLatin1String(file.readAll());//将文件所有内容进行读取
		setStyleSheet(styleSheet);//设置样式表
		file.close();
	}
}

//最小化按钮.槽
void TitleBar::onButtonMinClicked() {
	emit signalButtonMinClicked();
}

//最大化还原按钮.槽
void TitleBar::onButtonRestoreClicked() {
	/*
	QPushButton* m_pButtonMin;//最小化按钮
	QPushButton* m_pButtonRestore;//最大化还原按钮
	QPushButton* m_pButtonClose;//关闭按钮
	QPushButton* m_pButtonMax;//最大化按钮
	*/
	m_pButtonRestore->setVisible(false);//设置是否可见
	m_pButtonMax->setVisible(true);
	emit signalButtonRestoreClicked();
}

//最大化按钮.槽
void TitleBar::onButtonMaxClicked() {
	m_pButtonRestore->setVisible(true);
	m_pButtonMax->setVisible(false);
	emit signalButtonMaxClicked();
}

//关闭按钮.槽
void TitleBar::onButtonCloseClicked() {
	emit signalButtonCloseClicked();
}