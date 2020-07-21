#pragma once

#include <QWidget>
#include "ui_EmotionWindow.h"
#include "CommonUtils.h"
#include "EmotionLabelItem.h"
#include <qstyleoption.h>
#include <qpainter.h>

//表情窗口
class EmotionWindow : public QWidget
{
	Q_OBJECT

public:
	EmotionWindow(QWidget *parent = Q_NULLPTR);
	~EmotionWindow();

private slots:
	void addEmotion(int emotionNum);//添加表情

signals:
	void signalEmotionWindowHide();//信号表情窗口隐藏
	void signalEmotionItemClicked(int emotionNum);//信号表情包项目点击
private:
	void initControl();//初始化控件
	void paintEvent(QPaintEvent* event)override;//重些绘图事件

	Ui::EmotionWindow ui;
};
