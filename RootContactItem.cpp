#include "RootContactItem.h"

RootContactItem::RootContactItem(bool hasArrow, QWidget *parent)
	: QLabel(parent), m_rotation(0),m_hasArrow(hasArrow)
{
	setFixedHeight(32);//���ù̶���
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);//���ô�С����

	//��ʼ�����Զ���
	m_animation = new QPropertyAnimation(this, "rotation");
	m_animation->setDuration(3000);//���ó���ʱ�� //���õ��ζ���ʱ�� 50����
	m_animation->setEasingCurve(QEasingCurve::InQuad);//���ö��������������� t^2��0����

}

RootContactItem::~RootContactItem()
{
}

void RootContactItem::setText(const QString& title)
{
	m_titleText = title;
	update();//���»���
}

void RootContactItem::setExpanded(bool expand)
{
	if (expand) {
		m_animation->setEndValue(90);//���ö�������ֵ,��ͷ������ת90�ȵ���˼
	}else{
		m_animation->setEndValue(0);
	}
	m_animation->start();//����

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
	painter.setRenderHint(QPainter::TextAntialiasing,true);//���ó������� (�ı������)

	QFont font;
	font.setPointSize(10);
	painter.setFont(font);
	//�����ı�(��ʼX,��ʼY,��,��,���뷽ʽ ����� | �ڿ��ÿռ��д�ֱ����,���Ƶ��ı�)
	painter.drawText(24, 0, width() - 24, height(), Qt::AlignLeft | Qt::AlignVCenter, m_titleText);
	painter.setRenderHint(QPainter::SmoothPixmapTransform,true);//��Ⱦ
	painter.save();//���滭������

	if (m_hasArrow) {
		QPixmap pixmap;
		pixmap.load(":/Resources/MainWindow/arrow.png");

		QPixmap tmpPixmap(pixmap.size());
		tmpPixmap.fill(Qt::transparent);//͸��

		QPainter p(&tmpPixmap);
		p.setRenderHint(QPainter::SmoothPixmapTransform,true);//��Ⱦ,��������

		//������ϵ�����ﵽ��תЧ��
		p.translate(pixmap.width() / 2, pixmap.height() / 2);//����ϵƫ��(x����ƫ��,y����ƫ��)
		p.rotate(m_rotation);//��ת����ϵ(˳ʱ��)
		p.drawPixmap(0 - pixmap.width() / 2, 0 - pixmap.height() / 2, pixmap);

		painter.drawPixmap(6, (height() - pixmap.height()) / 2, tmpPixmap);
		painter.restore();//�ָ���������
	}

	QLabel::paintEvent(event);

}
