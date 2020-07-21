#include "BasicWindow.h"

BasicWindow::BasicWindow(QWidget *parent)
	: QDialog(parent)
{
	//����ɫ = ��ͬ��::��ȡƤ����ɫ
	m_colorBackGround = CommonUtils::getDefaultSkinColor();

	setWindowFlags(Qt::FramelessWindowHint);//���ô�����(�ޱ߿�)
	setAttribute(Qt::WA_TranslucentBackground,true);//��������(͸���ı���)(true�ͱ�͸����)

	connect(NotifyManager::getInstance(), SIGNAL(signalSkinChanged(const QColor&)), this, SLOT(onSignalSkinChanged(const QColor&)));
}

BasicWindow::~BasicWindow()
{
}

//������ʽ��
void BasicWindow::loadStyleSheet(const QString& sheetName)
{
	m_styleName = sheetName;
	QFile file(":/Resources/QSS/" + sheetName + ".css");//��ǰĿ¼:/
	file.open(QFile::ReadOnly);

	if (file.isOpen()) {
		setStyleSheet("");
		QString qsstyleSheet = QLatin1String(file.readAll());

		//��ȡ�û���ǰ��Ƥ��RGBֵ
		QString r = QString::number(m_colorBackGround.red());
		QString g = QString::number(m_colorBackGround.green());
		QString b = QString::number(m_colorBackGround.blue());
		
		//��ʽ��:����ѡ����{background-color:rgb(%1,%2,%3);\ border-top-left-radius:4px;}
		//������ui�����е�QWidget�ı���ɫ,�ж�������ui�����е�QWidget�Ķ�̬����titleskinΪtrue
		//��ʽ������óɹ�

		//����Ƥ�� & �ײ�Ƥ��
		qsstyleSheet += QString("QWidget[titleskin=true]\
		{background-color:rgb(%1,%2,%3);\
			border-top-left-radius:4px;}\
			QWidget[buttomskin=true]\
		{border-top:1px solid rgb(%1,%2,%3,100);\
			background-color:rgb(%1,%2,%3,50);\
			border-button-left-radius:4px;\
			border-botton-right-radius:4px;}").arg(r).arg(g).arg(b);

		/*
		qsstyleSheet += QString("QWidget[titleskin=true]\ //����QWidget�����Ķ�̬����[��̬������]
		{background-color:rgb(%1,%2,%3);\ //����ɫ-��ɫ:rgb()
			border-top-left-radius:4px;}\ //top-left��ߵİ뾶
			QWidget[buttonskin=true]\ //ͬ��
		{border-top:1px solid rgb(%1,%2,%3,100);\
			background-color:rgb(%1,%2,%3,50);\
			border-button-left-radius:4px;\
			border-botton-right-radius:4px;}").arg(r).arg(g).arg(b);
		*/

		setStyleSheet(qsstyleSheet);//������ʽ��
		file.close();
	}
}

//��ȡԲͷ��(��ͷ��,Բͷ��,��С)
QPixmap BasicWindow::getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize)
{
	if (maskSize == QSize(0, 0)) {
		maskSize = mask.size();
	}
	else {
		//����(Բͷ��,�����ܴ�ľ���(�����ݺ��),ת��ģʽ::ƽ��ת��)
		mask = mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	//����ת�����ͼ��(ͼ��С,ʲô��ʽ��ȡͼ��::��ʽARGB32�����)
	QImage resultImage(maskSize, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&resultImage);//����.������Ʒģʽ(����::ԭʼ) //����ͼƬ����ģʽ
	painter.setCompositionMode(QPainter::CompositionMode_Source);//��ԭͼ����и��ƵĲ���
	painter.fillRect(resultImage.rect(), Qt::transparent);//����.������(���δ�С,͸��)
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);//����
	painter.drawPixmap(0, 0, mask);//mask���е���
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);//��...��
	painter.drawPixmap(0, 0, src.scaled(maskSize,Qt::KeepAspectRatio, Qt::SmoothTransformation));//�ٽ�src���ӽ�ȥ
	painter.end();

	return QPixmap::fromImage(resultImage);
}

//��ʼ������
void BasicWindow::initBackGroundColor()
{
	//���ѡ��
	QStyleOption opt;
	opt.init(this);//��ʼ��

	//����
	QPainter p(this);

	//��ˢ?->��ԭʼ(��)��(���::С����,)
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p,this);
}

//�����¼� //���໯����ʱ,��Ҫ��д��ͼ�¼����ñ���ͼ
void BasicWindow::paintEvent(QPaintEvent* event)
{
	initBackGroundColor();
	QDialog::paintEvent(event);
}

//��갴ѹ�¼�
void BasicWindow::mousePressEvent(QMouseEvent* event)
{
	if (event->buttons() == Qt::LeftButton) {
		m_mousePressed = true;
		//pos()������굱ǰС�������Ͻ�(0,0)λ��
		m_mousePoint = event->globalPos() - pos();
		event->accept();
	}
}

//����ƶ��¼�
void BasicWindow::mouseMoveEvent(QMouseEvent* event)
{
	//����Ƿ��� &&(��ǰ�¼���갴�� && ���)
	if (m_mousePressed && (event->buttons() && Qt::LeftButton)) {
		//�ƶ��������λ��
		move(event->globalPos() - m_mousePoint);//�ƶ�(��ǰ�¼���Ļȫ�����Ͻ�(0,0)λ��-���λ��)
		event->accept();//�¼�->����
	}
}

//����ͷ��¼�
void BasicWindow::mouseReleaseEvent(QMouseEvent* event)
{
	m_mousePressed = false;
}

//��ʾ�ر�
void BasicWindow::onShowClose(bool) {
	close();
}

//��С��
void BasicWindow::onShowMin(bool)
{
	showMinimized();
}

//����
void BasicWindow::onShowHide(bool)
{
	hide();
}

//״̬ //������ʾ
void BasicWindow::onShowNormal(bool)
{
	show();
	activateWindow();//����Ӵ���
}

//�˳�
void BasicWindow::onShowQuit(bool)
{
	QApplication::quit();//Ӧ�ó����˳�
}

//Ƥ���ı�
void BasicWindow::onSignalSkinChanged(const QColor& color)
{
	m_colorBackGround = color;
	loadStyleSheet(m_styleName);
}

//��С��
void BasicWindow::onButtonMinClicked()
{
	//(С���� == (���ڱ�־ & ���߰�ť))
	if (Qt::Tool == (windowFlags() & Qt::Tool)) {
		hide();//����С����
	}
	else {
		showMinimized();//��������С
	}
}

//��󻯼���ԭ
void BasicWindow::onButtonRestoreClicked()
{
	QPoint windowPos;
	QSize windowSize;
	_titleBar->getRestoreInfo(windowPos, windowSize);//��ȡ�������ǰ��λ�ü���С
	setGeometry(QRect(windowPos, windowSize));//���ü���(����(λ��,��С))
}

//���
void BasicWindow::onButtonMaxClicked()
{
	//���洰�����ǰ��λ�ü���С(λ��,��С(��,��))
	_titleBar->saveRestoreInfo(pos(), QSize(width(), height()));
	//���漸�� = Ӧ�ó���::����λ��->��ȡ���νṹ
	QRect desktopRect = QApplication::desktop()->availableGeometry();
	//ʵ�ʼ���=����(x,y,��,��)
	QRect fackRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(fackRect);
}

//�ر�
void BasicWindow::onButtonCloseClicked()
{
	close();
}

//���ñ�����
void BasicWindow::setTitleBar(const QString& title, const QString& icon)
{
	_titleBar->setTitleIcon(icon);
	_titleBar->setTitleContent(title);
}

//��ʼ��������
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

