#pragma once

#include <QWidget>
#include <qevent.h>
#include "CommonUtils.h"
#include "ui_TalkWindowItem.h"

//��ϵ������Ŀ
class TalkWindowItem : public QWidget
{
	Q_OBJECT

public:
	TalkWindowItem(QWidget *parent = Q_NULLPTR);
	~TalkWindowItem();

	//����ͷ��
	void setHeadPixmap(const QPixmap& pixmap);
	//���ñ�ǩ����
	void setMsgLabelContent(const QString& msg);
	//��ȡ��˭����
	QString getMsgLabelText();

signals:
	void signalCloseClicked();

private:
	void initControl();//��ʼ���ؼ�

	void enterEvent(QEvent* event);//�����¼�
	void leaveEvent(QEvent* event);//�뿪�¼�
	void resizeEvent(QResizeEvent* event);//������С�¼�

	Ui::TalkWindowItem ui;
};
