#pragma once

#include <QWidget>
#include "ui_skinWindow.h"
#include "BasicWindow.h"
#include "QtClickLabel.h"
#include "NotifyManager.h"

//�ı�CCMainWindowƤ����
class skinWindow : public BasicWindow
{
	Q_OBJECT

public:
	skinWindow(BasicWindow *parent = Q_NULLPTR);
	~skinWindow();

public:
	void initControl();

public slots:
	void onShowClose();

private:
	void paintEvent(QPaintEvent* event);//Ϊ���ò������岻͸��

	Ui::skinWindow ui;
};
