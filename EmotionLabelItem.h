#pragma once

#include "QtClickLabel.h"
#include <qmovie.h>

//表情标签项目
class EmotionLabelItem : public QtClickLabel
{
	Q_OBJECT

public:
	EmotionLabelItem(QWidget *parent);
	~EmotionLabelItem();

	void setEmotionName(int emotionNum);//设置表情名称


signals:
	void emotionClicked(int emotionNum);//表情被点击

private:
	void initControl();//初始化控件

	int m_emotionName;//当前表情序号
	QMovie* m_apngMovie;//电影(GIF) 角度电影
};
