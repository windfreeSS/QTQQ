#include "EmotionLabelItem.h"

EmotionLabelItem::EmotionLabelItem(QWidget *parent)
	: QtClickLabel(parent)
{
	initControl();
	//当前表情标签被点击
	connect(this,&QtClickLabel::clicked, [this] {emit emotionClicked(m_emotionName);});

}

EmotionLabelItem::~EmotionLabelItem()
{
}

void EmotionLabelItem::setEmotionName(int emotionNum)
{
	m_emotionName = emotionNum;
	QString imageName = QString(":/Resources/MainWindow/emotion/%1.png").arg(emotionNum);

	m_apngMovie = new QMovie(imageName,"apng",this);//路径,格式,父类
	m_apngMovie->start();//启动
	m_apngMovie->stop(); //停止
	setMovie(m_apngMovie);//表情标签项目设置电影
}

void EmotionLabelItem::initControl() {
	//设置标签每一个都居中对齐
	setAlignment(Qt::AlignCenter);
	//设置当前标签对象名
	setObjectName("emotionLabelItem");
	//设置当前标签大小
	setFixedSize(32,32);
}