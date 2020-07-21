#include "BasicWindow.h"

BasicWindow::BasicWindow(QWidget *parent)
	: QDialog(parent)
{
	//背景色 = 共同的::获取皮肤颜色
	m_colorBackGround = CommonUtils::getDefaultSkinColor();

	setWindowFlags(Qt::FramelessWindowHint);//设置窗体风格(无边框)
	setAttribute(Qt::WA_TranslucentBackground,true);//设置属性(透明的背景)(true就变透明了)

	connect(NotifyManager::getInstance(), SIGNAL(signalSkinChanged(const QColor&)), this, SLOT(onSignalSkinChanged(const QColor&)));
}

BasicWindow::~BasicWindow()
{
}

//加载样式表
void BasicWindow::loadStyleSheet(const QString& sheetName)
{
	m_styleName = sheetName;
	QFile file(":/Resources/QSS/" + sheetName + ".css");//当前目录:/
	file.open(QFile::ReadOnly);

	if (file.isOpen()) {
		setStyleSheet("");
		QString qsstyleSheet = QLatin1String(file.readAll());

		//获取用户当前的皮肤RGB值
		QString r = QString::number(m_colorBackGround.red());
		QString g = QString::number(m_colorBackGround.green());
		QString b = QString::number(m_colorBackGround.blue());
		
		//样式表:属性选择器{background-color:rgb(%1,%2,%3);\ border-top-left-radius:4px;}
		//设置了ui界面中的QWidget的背景色,判断条件需ui界面中的QWidget的动态属性titleskin为true
		//样式表才设置成功

		//标题皮肤 & 底部皮肤
		qsstyleSheet += QString("QWidget[titleskin=true]\
		{background-color:rgb(%1,%2,%3);\
			border-top-left-radius:4px;}\
			QWidget[buttomskin=true]\
		{border-top:1px solid rgb(%1,%2,%3,100);\
			background-color:rgb(%1,%2,%3,50);\
			border-button-left-radius:4px;\
			border-botton-right-radius:4px;}").arg(r).arg(g).arg(b);

		/*
		qsstyleSheet += QString("QWidget[titleskin=true]\ //界面QWidget部件的动态属性[动态属性名]
		{background-color:rgb(%1,%2,%3);\ //背景色-颜色:rgb()
			border-top-left-radius:4px;}\ //top-left框边的半径
			QWidget[buttonskin=true]\ //同上
		{border-top:1px solid rgb(%1,%2,%3,100);\
			background-color:rgb(%1,%2,%3,50);\
			border-button-left-radius:4px;\
			border-botton-right-radius:4px;}").arg(r).arg(g).arg(b);
		*/

		setStyleSheet(qsstyleSheet);//设置样式表
		file.close();
	}
}

//获取圆头像(方头像,圆头像,大小)
QPixmap BasicWindow::getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize)
{
	if (maskSize == QSize(0, 0)) {
		maskSize = mask.size();
	}
	else {
		//缩放(圆头像,尽可能大的矩形(保持纵横比),转换模式::平滑转换)
		mask = mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	//保存转换后的图像(图大小,什么形式获取图像::格式ARGB32自左乘)
	QImage resultImage(maskSize, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&resultImage);//画家.设置作品模式(画家::原始) //设置图片叠加模式
	painter.setCompositionMode(QPainter::CompositionMode_Source);//将原图像进行复制的操作
	painter.fillRect(resultImage.rect(), Qt::transparent);//画家.填充矩形(矩形大小,透明)
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);//覆盖
	painter.drawPixmap(0, 0, mask);//mask进行叠加
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);//在...里
	painter.drawPixmap(0, 0, src.scaled(maskSize,Qt::KeepAspectRatio, Qt::SmoothTransformation));//再将src叠加进去
	painter.end();

	return QPixmap::fromImage(resultImage);
}

//初始化背景
void BasicWindow::initBackGroundColor()
{
	//风格选项
	QStyleOption opt;
	opt.init(this);//初始化

	//画家
	QPainter p(this);

	//画刷?->画原始(简单)的(风格::小部件,)
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p,this);
}

//绘制事件 //子类化部件时,需要重写绘图事件设置背景图
void BasicWindow::paintEvent(QPaintEvent* event)
{
	initBackGroundColor();
	QDialog::paintEvent(event);
}

//鼠标按压事件
void BasicWindow::mousePressEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton) {
		m_mousePressed = true;
		//pos()返回鼠标当前小部件左上角(0,0)位置
		m_mousePoint = event->globalPos() - pos();
		event->accept();
	}
}

//鼠标移动事件
void BasicWindow::mouseMoveEvent(QMouseEvent* event)
{
	//鼠标是否按下 &&(当前事件鼠标按下 && 左键)
	if (m_mousePressed && (event->buttons() && Qt::LeftButton)) {
		//移动到现鼠标位置
		move(event->globalPos() - m_mousePoint);//移动(当前事件屏幕全局左上角(0,0)位置-鼠标位置)
		event->accept();//事件->接受
	}
}

//鼠标释放事件
void BasicWindow::mouseReleaseEvent(QMouseEvent* event)
{
	m_mousePressed = false;
}

//显示关闭
void BasicWindow::onShowClose(bool) {
	close();
}

//最小化
void BasicWindow::onShowMin(bool)
{
	showMinimized();
}

//隐藏
void BasicWindow::onShowHide(bool)
{
	hide();
}

//状态 //正常显示
void BasicWindow::onShowNormal(bool)
{
	show();
	activateWindow();//活动的子窗口
}

//退出
void BasicWindow::onShowQuit(bool)
{
	QApplication::quit();//应用程序退出
}

//皮肤改变
void BasicWindow::onSignalSkinChanged(const QColor& color)
{
	m_colorBackGround = color;
	loadStyleSheet(m_styleName);
}

//最小化
void BasicWindow::onButtonMinClicked()
{
	//(小部件 == (窗口标志 & 工具按钮))
	if (Qt::Tool == (windowFlags() & Qt::Tool)) {
		hide();//隐藏小部件
	}
	else {
		showMinimized();//缩放至最小
	}
}

//最大化及还原
void BasicWindow::onButtonRestoreClicked()
{
	QPoint windowPos;
	QSize windowSize;
	_titleBar->getRestoreInfo(windowPos, windowSize);//获取窗口最大化前的位置及大小
	setGeometry(QRect(windowPos, windowSize));//设置几何(矩形(位置,大小))
}

//最大化
void BasicWindow::onButtonMaxClicked()
{
	//保存窗口最大化前的位置及大小(位置,大小(宽,高))
	_titleBar->saveRestoreInfo(pos(), QSize(width(), height()));
	//桌面几何 = 应用程序::桌面位置->获取几何结构
	QRect desktopRect = QApplication::desktop()->availableGeometry();
	//实际几何=几何(x,y,宽,高)
	QRect fackRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(fackRect);
}

//关闭
void BasicWindow::onButtonCloseClicked()
{
	close();
}

//设置标题栏
void BasicWindow::setTitleBar(const QString& title, const QString& icon)
{
	_titleBar->setTitleIcon(icon);
	_titleBar->setTitleContent(title);
}

//初始化标题栏
void BasicWindow::initTitleBar(ButtonType ButtonType) {
	_titleBar = new TitleBar(this);
	_titleBar->setButtonTybe(ButtonType);
	_titleBar->move(0, 0);

	//connect(_titleBar, &TitleBar::signalButtonMinClicked, this, &BasicWindow::onButtonMinClicked);
	connect(_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}

