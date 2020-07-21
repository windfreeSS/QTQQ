#include "skinWindow.h"

skinWindow::skinWindow(BasicWindow* parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	loadStyleSheet("SkinWindow");
	setAttribute(Qt::WA_DeleteOnClose);//ʹQt��С�������ܹر��¼�ʱɾ����С����

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
			label->setCursor(Qt::PointingHandCursor);//���ù����ʽ

			//C++11����֧��lambda���ʽ
			//ִ�е���[row, column, colorList]() {}���������
			//����˴˱�ǩ,��ȡ��ǰ������ʵ�����øı��䴰�ڴ�����ɫ
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

	//Ƥ��������С��&�ر�
	connect(ui.sysmin, SIGNAL(clicked(bool)), this, SLOT(onShowMin(bool)));
	connect(ui.sysclose, SIGNAL(clicked()), this, SLOT(onShowClose()));
}

void skinWindow::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	//������(��ǰ����,ɫֵ)
	painter.fillRect(rect(), QColor(255, 255, 255, 255));
}

void skinWindow::onShowClose() {
	close();
}