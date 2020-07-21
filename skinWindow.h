#pragma once

#include <QWidget>
#include "ui_skinWindow.h"
#include "BasicWindow.h"
#include "QtClickLabel.h"
#include "NotifyManager.h"

//改变CCMainWindow皮肤类
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
	void paintEvent(QPaintEvent* event);//为了让部件窗体不透明

	Ui::skinWindow ui;
};
