#include "QtClickLabel.h"

QtClickLabel::QtClickLabel(QWidget *parent)
	: QLabel(parent)
{
}

QtClickLabel::~QtClickLabel()
{
}

void QtClickLabel::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		emit clicked();
	}
}
