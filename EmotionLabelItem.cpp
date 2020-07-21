#include "EmotionLabelItem.h"

EmotionLabelItem::EmotionLabelItem(QWidget *parent)
	: QtClickLabel(parent)
{
	initControl();
	//��ǰ�����ǩ�����
	connect(this,&QtClickLabel::clicked, [this] {emit emotionClicked(m_emotionName);});

}

EmotionLabelItem::~EmotionLabelItem()
{
}

void EmotionLabelItem::setEmotionName(int emotionNum)
{
	m_emotionName = emotionNum;
	QString imageName = QString(":/Resources/MainWindow/emotion/%1.png").arg(emotionNum);

	m_apngMovie = new QMovie(imageName,"apng",this);//·��,��ʽ,����
	m_apngMovie->start();//����
	m_apngMovie->stop(); //ֹͣ
	setMovie(m_apngMovie);//�����ǩ��Ŀ���õ�Ӱ
}

void EmotionLabelItem::initControl() {
	//���ñ�ǩÿһ�������ж���
	setAlignment(Qt::AlignCenter);
	//���õ�ǰ��ǩ������
	setObjectName("emotionLabelItem");
	//���õ�ǰ��ǩ��С
	setFixedSize(32,32);
}