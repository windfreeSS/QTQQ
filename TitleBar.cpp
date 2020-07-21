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

//���ñ�����ͼ��(·��)
void TitleBar::setTitleIcon(const QString& filePath)
{
	QPixmap titleIcon(filePath);//��ͼӳ�� /
	m_pIcon->setFixedSize(titleIcon.size());//���ù̶���С //ͼƬ�ж��Ͷ��
	m_pIcon->setPixmap(titleIcon);
}

//���ñ���������
void TitleBar::setTitleContent(const QString& titleContent)
{
	m_pTitleContent->setText(titleContent);
	m_titleContent = titleContent;
}

//���ñ��������
void TitleBar::setTitleWidth(int width)
{
	setFixedWidth(width);
}

//���ñ�������ť����
void TitleBar::setButtonTybe(ButtonType buttonTybe)
{
	/*
	//��ť���
enum ButtonType {
	MIN_BUTTON=0, //��С�����رհ�ť
	MIN_MAX_BUTTON,//��С��,��󻯼��رհ�ť
	ONLY_CLOSE_BUTTON//ֻ�йرհ�ť
};

	QPushButton* m_pButtonMin;//��С����ť
	QPushButton* m_pButtonRestore;//��󻯻�ԭ��ť
	QPushButton* m_pButtonClose;//�رհ�ť
	QPushButton* m_pButtonMax;//��󻯰�ť
	*/
	m_buttonType = buttonTybe;
	switch (buttonTybe) {
	case ButtonType::MIN_BUTTON:
		m_pButtonRestore->setVisible(false);//���ÿɼ�()
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

//���洰�����ǰ��λ�ü���С
void TitleBar::saveRestoreInfo(const QPoint& point, const QSize& size)
{
	/*
	//��󻯻�ԭ��ť����(���ڱ��洰��λ�úʹ�С)
	QPoint m_restorePos;
	QSize m_restoreSize;
	*/
	m_restorePos = point;
	m_restoreSize = size;
}

//��ȡ�������ǰ��λ�ü���С
void TitleBar::getRestoreInfo(QPoint& point, QSize& size)
{
	point =m_restorePos;
	size = m_restoreSize;
}

//��ͼ�¼� //���Ʊ�����
void TitleBar::paintEvent(QPaintEvent* event)
{
	//����
	QPainter painter(this);
	//���ҹ��
	QPainterPath pathBack;
	//����������(����(����)���)
	pathBack.setFillRule(Qt::WindingFill);
	//���Բ�Ǿ���(����(��,��,��,��),xб��,yб��)
	pathBack.addRoundRect(QRect(0, 0, width(), height()),3,3);
	//������Ⱦ(ƽ����ͼ�任,true)
	painter.setRenderHint(QPainter::SmoothPixmapTransform,true);

	//��������󻯻�ԭ��,���ڿ�ȸı�,��������Ӧ�����ı�
	//TitleBar������� != ���������
	if (width() != parentWidget()->width()) {
		setFixedWidth(parentWidget()->width());
	}

	QWidget::paintEvent(event);//����=Ĭ�ϲ�������
}

//���˫���¼� //˫���������������,��С������
void TitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
	/*
	//��ť���
enum ButtonType {
	MIN_BUTTON=0, //��С�����رհ�ť
	MIN_MAX_BUTTON,//��С��,��󻯼��رհ�ť
	ONLY_CLOSE_BUTTON//ֻ�йرհ�ť
};

	QPushButton* m_pButtonMin;//��С����ť
	QPushButton* m_pButtonRestore;//��󻯻�ԭ��ť
	QPushButton* m_pButtonClose;//�رհ�ť
	QPushButton* m_pButtonMax;//��󻯰�ť

	//���������Ͻǰ�ť����
	ButtonType m_buttonType;
	*/

	//ֻ�д�����С��,��󻯰�ť����Ч
	if (m_buttonType == ButtonType::MIN_MAX_BUTTON) {
		//��С����ť�Ƿ�ɼ�
		if (m_pButtonMin->isVisible()) 
			onButtonMinClicked();//��С��
		else
			onButtonMaxClicked();//���
	}

	//���� ����::���˫���¼�();
	return QWidget::mouseDoubleClickEvent(event);//Ĭ�ϵ��¼��Ĵ���
}

//��갴��/ѹ�¼�
void TitleBar::mousePressEvent(QMouseEvent* event)
{
	if (m_buttonType == ButtonType::MIN_MAX_BUTTON) {
		//�ڴ������ʱ��ֹ�϶�����
		if (m_pButtonMax->isVisible()) {
			m_isPressed = true;
			m_startMovePos = event->globalPos();//�����¼�����ʱ������ڵ�ȫ��λ��
		}
	}
	else {
		m_isPressed = true;
		m_startMovePos = event->globalPos();
	}

	return QWidget::mousePressEvent(event);
}

//����ƶ��¼�
void TitleBar::mouseMoveEvent(QMouseEvent* event)
{
	if (m_isPressed) {
		QPoint movePoint = event->globalPos()- m_startMovePos;//�ƶ����پ���=�����λ��-����λ��
		QPoint widgetPos = parentWidget()->pos();//���ظ�������λ��
		m_startMovePos = event->globalPos();
		parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());//�����������ƶ�
	}
	return QWidget::mouseMoveEvent(event);
}

//����ɿ��¼�
void TitleBar::mouseReleaseEvent(QMouseEvent* event)
{
	m_isPressed = false;
	return QWidget::mouseReleaseEvent(event);
}

//��ʼ���ؼ�
void TitleBar::initControl()
{
	m_pIcon = new QLabel(this);
	m_pTitleContent = new QLabel(this);

	m_pButtonMin = new QPushButton(this);
	m_pButtonRestore = new QPushButton(this);
	m_pButtonMax = new QPushButton(this);
	m_pButtonClose = new QPushButton(this);

	//��ť���ù̶���С
	m_pButtonMin->setFixedSize(QSize(BUTTON_HEIGHT, BUTTON_WIDTH));
	m_pButtonRestore->setFixedSize(QSize(BUTTON_HEIGHT, BUTTON_WIDTH));
	m_pButtonMax->setFixedSize(QSize(BUTTON_HEIGHT, BUTTON_WIDTH));
	m_pButtonClose->setFixedSize(QSize(BUTTON_HEIGHT, BUTTON_WIDTH));

	//����(����)Ŀ����
	m_pTitleContent->setObjectName("TitleContent");//ui.TitleContent
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pButtonMax->setObjectName("ButtonMax");
	m_pButtonClose->setObjectName("ButtonClose");

	//���ò���(ˮƽ����)
	QHBoxLayout* mylayout = new QHBoxLayout(this);
	//mylayout->setObjectName("ˮƽ����");
	mylayout->addWidget(m_pIcon);
	mylayout->addWidget(m_pTitleContent);

	mylayout->addWidget(m_pButtonMin);
	mylayout->addWidget(m_pButtonRestore);
	mylayout->addWidget(m_pButtonMax);
	mylayout->addWidget(m_pButtonClose);

	//����Ŀ¼��Ե(���ֱ߿��С) //���ò��ּ�϶(��,��,��,��)
	mylayout->setContentsMargins(5, 0, 0, 0);
	//���ò����ﲿ����ļ�϶
	mylayout->setSpacing(0);

	//���óߴ����(�����,�̶���) //������������,��ֱ����̶�
	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	setFixedHeight(TITLE_HEIGHT);//���ñ������߶�
	setWindowFlags(Qt::FramelessWindowHint);//���ô������ޱ߿�
}

//��ʼ���ź���۵�����
void TitleBar::initConnections()
{
	/*
	QPushButton* m_pButtonMin;//��С����ť
	QPushButton* m_pButtonRestore;//��󻯻�ԭ��ť
	QPushButton* m_pButtonClose;//�رհ�ť
	QPushButton* m_pButtonMax;//��󻯰�ť

	//��С����ť.��
	void onButtonMinClicked();
	//��󻯻�ԭ��ť.��
	void onButtonRestoreClicked();
	//��󻯰�ť.��
	void onButtonMaxClicked();
	//�رհ�ť.��
	void onButtonCloseClicked();
	*/
	//��ť������Ӧ����
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked()));
	connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
}

//������ʽ��(��ʽ����)
void TitleBar::loadStyleSheet(const QString& sheetName)
{
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);//ֻ����
	if (file.isOpen()) {
		QString styleSheet = this->styleSheet();//���浱ǰ��ʽ��
		styleSheet += QLatin1String(file.readAll());//���ļ��������ݽ��ж�ȡ
		setStyleSheet(styleSheet);//������ʽ��
		file.close();
	}
}

//��С����ť.��
void TitleBar::onButtonMinClicked() {
	emit signalButtonMinClicked();
}

//��󻯻�ԭ��ť.��
void TitleBar::onButtonRestoreClicked() {
	/*
	QPushButton* m_pButtonMin;//��С����ť
	QPushButton* m_pButtonRestore;//��󻯻�ԭ��ť
	QPushButton* m_pButtonClose;//�رհ�ť
	QPushButton* m_pButtonMax;//��󻯰�ť
	*/
	m_pButtonRestore->setVisible(false);//�����Ƿ�ɼ�
	m_pButtonMax->setVisible(true);
	emit signalButtonRestoreClicked();
}

//��󻯰�ť.��
void TitleBar::onButtonMaxClicked() {
	m_pButtonRestore->setVisible(true);
	m_pButtonMax->setVisible(false);
	emit signalButtonMaxClicked();
}

//�رհ�ť.��
void TitleBar::onButtonCloseClicked() {
	emit signalButtonCloseClicked();
}