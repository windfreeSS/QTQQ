#pragma once

#include <QLabel>
#include <QMouseEvent>

class QtClickLabel : public QLabel
{
	Q_OBJECT

public:
	QtClickLabel(QWidget *parent);
	~QtClickLabel();

protected:
	void mousePressEvent(QMouseEvent* event);
signals:
	void clicked();
};
