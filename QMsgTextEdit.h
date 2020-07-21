#pragma once

#include <QTextEdit>
#include "qmovie.h"
#include <qurl.h>

//��Ϣ�ı��༭��
class QMsgTextEdit : public QTextEdit
{
	Q_OBJECT

public:
	QMsgTextEdit(QWidget *parent);
	~QMsgTextEdit();

	//��ӱ���(�����)
	void addEmotionUrl(int emotionNum);
	//ɾ�����б���ͼƬ 
	void deleteAllEmotionImage();
private slots:
	//����ͼƬ֡�ı�(֡)
	void onEmotionImageFrameChange(int frame);
	

private:
	QList<QString> m_listEmotionUrl;//���б���
	QMap<QMovie*, QString> m_emotionMap;//����ӳ��
};
