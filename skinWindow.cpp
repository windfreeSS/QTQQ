#include "skinWindow.h"

skinWindow::skinWindow(BasicWindow* parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	loadStyleSheet("SkinWindow");
	setAttribute(Qt::WA_DeleteOnClose);//使Qt在小部件接受关闭事件时删除该小部件

	initControl();
}

skinWindow::~skinWindow()
{
}

void skinWindow::initControl()
{
	QList<QColor> colorList = {
		QColor(22,154,218),QColor(40,138,221),QColor(49,166,107),QColor(218,67,68),
		QColor(177,99,158),QColor(107,81,92),QColor(89,92,160),QColor(0,0,0),
		QColor(115,22,225),QColor(122,221,35),QColor(12,210,100),QColor(78,67,53)
	};

	for (int row = 0; row < 3; row++) {
		for (int column = 0; column < 4;column++) {
			QtClickLabel* label = new QtClickLabel(this);
			label->setCursor(Qt::PointingHandCursor);//设置光标样式

			//C++11的新支持lambda表达式
			//执行的是[row, column, colorList]() {}这里的内容
			//点击了此标签,获取当前主窗口实例调用改变其窗口窗体颜色
			connect(label, &QtClickLabel::clicked, [row, column, colorList](){
				NotifyManager::getInstance()->notifyOtherWindowChanageSkin(colorList.at(row * 4 + column));
				});

			label->setFixedSize(84, 84);
			
			QPalette palette;
			palette.setColor(QPalette::Background, colorList.at(row * 4 + column));

			label->setAutoFillBackground(true);
			label->setPalette(palette);
			
			ui.gridLayout->addWidget(label, row, column);
		}
	}

	//皮肤窗口最小化&关闭
	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowMin(bool)));
	connect(ui.sysclose, SIGNAL(clicked()), this, SLOT(onShowClose()));
}

void skinWindow::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	//填充矩形(当前几何,色值)
	painter.fillRect(rect(), QColor(255, 255, 255, 255));
}

void skinWindow::onShowClose() {
	close();
}