#include "EmotionWindow.h"

EmotionWindow::EmotionWindow(QWidget *parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);//���ô�����Ϊ�ޱ߿���Ӵ�������
	setAttribute(Qt::WA_TranslucentBackground);//��������Ϊ͸��
	setAttribute(Qt::WA_DeleteOnClose);//��������
	ui.setupUi(this);
	
	initControl();//��ʼ���ؼ�
}

EmotionWindow::~EmotionWindow()
{
}


const int emotionColumn = 14;//������
const int emotionRow = 12;//������

void EmotionWindow::initControl()
{
	CommonUtils::loadStyleSheet(this,"EmotionWindow");//������ʽ��
	//����ͼƬ
	for (int row = 0; row < emotionRow;row++) {
		for (int column = 0; column < emotionColumn; column++) {
			EmotionLabelItem* label = new EmotionLabelItem(this);
			label->setEmotionName(row * emotionColumn + column);//����Ŵ���ȥ
			  
			//connect(label, &EmotionLabelItem::emotionClicked(int), this, &EmotionWindow::addEmotion(int));
			connect(label,SIGNAL(emotionClicked(int)), this,SLOT(addEmotion(int)));
			ui.gridLayout->addWidget(label, row, column);//�ڱ��鴰�岼�ֹ�������Ӳ���
		}
	}
}

void EmotionWindow::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;//���ѡ����
	opt.init(this);//��ʼ��

	QPainter painter(this);

	style()->drawPrimitive(QStyle::PE_Widget,&opt,&painter,this);

	__super::paintEvent(event);//�����Ļ�ͼ����
}

void EmotionWindow::addEmotion(int emotionNum) {
	hide();//����
	emit signalEmotionWindowHide();
	emit signalEmotionItemClicked(emotionNum);
}