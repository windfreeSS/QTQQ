#pragma once

#include <QWidget>
#include "ui_EmotionWindow.h"
#include "CommonUtils.h"
#include "EmotionLabelItem.h"
#include <qstyleoption.h>
#include <qpainter.h>

//���鴰��
class EmotionWindow : public QWidget
{
	Q_OBJECT

public:
	EmotionWindow(QWidget *parent = Q_NULLPTR);
	~EmotionWindow();

private slots:
	void addEmotion(int emotionNum);//��ӱ���

signals:
	void signalEmotionWindowHide();//�źű��鴰������
	void signalEmotionItemClicked(int emotionNum);//�źű������Ŀ���
private:
	void initControl();//��ʼ���ؼ�
	void paintEvent(QPaintEvent* event)override;//��Щ��ͼ�¼�

	Ui::EmotionWindow ui;
};
