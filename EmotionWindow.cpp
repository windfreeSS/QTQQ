#include "EmotionWindow.h"

EmotionWindow::EmotionWindow(QWidget *parent)
	: QWidget(parent)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);//设置窗体风格为无边框和子窗口类型
	setAttribute(Qt::WA_TranslucentBackground);//设置属性为透明
	setAttribute(Qt::WA_DeleteOnClose);//回收属性
	ui.setupUi(this);
	
	initControl();//初始化控件
}

EmotionWindow::~EmotionWindow()
{
}


const int emotionColumn = 14;//表情列
const int emotionRow = 12;//表情行

void EmotionWindow::initControl()
{
	CommonUtils::loadStyleSheet(this,"EmotionWindow");//加载样式表
	//加载图片
	for (int row = 0; row < emotionRow;row++) {
		for (int column = 0; column < emotionColumn; column++) {
			EmotionLabelItem* label = new EmotionLabelItem(this);
			label->setEmotionName(row * emotionColumn + column);//将序号传进去
			  
			//connect(label, &EmotionLabelItem::emotionClicked(int), this, &EmotionWindow::addEmotion(int));
			connect(label,SIGNAL(emotionClicked(int)), this,SLOT(addEmotion(int)));
			ui.gridLayout->addWidget(label, row, column);//在表情窗体布局管理中添加部件
		}
	}
}

void EmotionWindow::paintEvent(QPaintEvent* event)
{
	QStyleOption opt;//风格选择器
	opt.init(this);//初始化

	QPainter painter(this);

	style()->drawPrimitive(QStyle::PE_Widget,&opt,&painter,this);

	__super::paintEvent(event);//其它的绘图操作
}

void EmotionWindow::addEmotion(int emotionNum) {
	hide();//隐藏
	emit signalEmotionWindowHide();
	emit signalEmotionItemClicked(emotionNum);
}