#include "RootContactItem.h"

RootContactItem::RootContactItem(bool hasArrow, QWidget *parent)
	: QLabel(parent), m_rotation(0),m_hasArrow(hasArrow)
{
	setFixedHeight(32);//设置固定高
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);//设置大小策略

	//初始化属性动画
	m_animation = new QPropertyAnimation(this, "rotation");
	m_animation->setDuration(3000);//设置持续时间 //设置单次动画时长 50毫秒
	m_animation->setEasingCurve(QEasingCurve::InQuad);//设置动画缓和曲线类型 t^2从0加速

}

RootContactItem::~RootContactItem()
{
}

void RootContactItem::setText(const QString& title)
{
	m_titleText = title;
	update();//重新绘制
}

void RootContactItem::setExpanded(bool expand)
{
	if (expand) {
		m_animation->setEndValue(90);//设置动画结束值,箭头向下旋转90度的意思
	}else{
		m_animation->setEndValue(0);
	}
	m_animation->start();//启动

}

int RootContactItem::rotation()
{
	return m_rotation;
}

void RootContactItem::setRotation(int rotation)
{
	m_rotation = rotation;
	update();
}

void RootContactItem::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::TextAntialiasing,true);//设置呈现提升 (文本反锯齿)

	QFont font;
	font.setPointSize(10);
	painter.setFont(font);
	//绘制文本(起始X,起始Y,宽,高,对齐方式 左对齐 | 在可用空间中垂直居中,绘制的文本)
	painter.drawText(24, 0, width() - 24, height(), Qt::AlignLeft | Qt::AlignVCenter, m_titleText);
	painter.setRenderHint(QPainter::SmoothPixmapTransform,true);//渲染
	painter.save();//保存画家设置

	if (m_hasArrow) {
		QPixmap pixmap;
		pixmap.load(":/Resources/MainWindow/arrow.png");

		QPixmap tmpPixmap(pixmap.size());
		tmpPixmap.fill(Qt::transparent);//透明

		QPainter p(&tmpPixmap);
		p.setRenderHint(QPainter::SmoothPixmapTransform,true);//渲染,尽量保真

		//对坐标系操作达到旋转效果
		p.translate(pixmap.width() / 2, pixmap.height() / 2);//坐标系偏移(x方向偏移,y方向偏移)
		p.rotate(m_rotation);//旋转坐标系(顺时针)
		p.drawPixmap(0 - pixmap.width() / 2, 0 - pixmap.height() / 2, pixmap);

		painter.drawPixmap(6, (height() - pixmap.height()) / 2, tmpPixmap);
		painter.restore();//恢复画家设置
	}

	QLabel::paintEvent(event);

}
