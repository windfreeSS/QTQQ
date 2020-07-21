#pragma once

#include <QTextEdit>
#include "qmovie.h"
#include <qurl.h>

//信息文本编辑框
class QMsgTextEdit : public QTextEdit
{
	Q_OBJECT

public:
	QMsgTextEdit(QWidget *parent);
	~QMsgTextEdit();

	//添加表情(表情号)
	void addEmotionUrl(int emotionNum);
	//删除所有表情图片 
	void deleteAllEmotionImage();
private slots:
	//表情图片帧改变(帧)
	void onEmotionImageFrameChange(int frame);
	

private:
	QList<QString> m_listEmotionUrl;//所有表情
	QMap<QMovie*, QString> m_emotionMap;//表情映射
};
