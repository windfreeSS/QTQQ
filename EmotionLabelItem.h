#pragma once

#include "QtClickLabel.h"
#include <qmovie.h>

//�����ǩ��Ŀ
class EmotionLabelItem : public QtClickLabel
{
	Q_OBJECT

public:
	EmotionLabelItem(QWidget *parent);
	~EmotionLabelItem();

	void setEmotionName(int emotionNum);//���ñ�������


signals:
	void emotionClicked(int emotionNum);//���鱻���

private:
	void initControl();//��ʼ���ؼ�

	int m_emotionName;//��ǰ�������
	QMovie* m_apngMovie;//��Ӱ(GIF) �Ƕȵ�Ӱ
};
