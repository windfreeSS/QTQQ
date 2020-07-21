#pragma once

#include <QLabel>
#include <QPropertyAnimation>
#include <QPainter>

//����ϵ��
class RootContactItem : public QLabel
{
	Q_OBJECT

		//��ͷ��ת�Ƕȱ仯
		Q_PROPERTY(int rotation READ rotation WRITE setRotation)
public:
	RootContactItem(bool hasArrow = true, QWidget* parent =nullptr);//��ͷ,����
	~RootContactItem();

	void setText(const QString& title);//�����ı�
	void setExpanded(bool expand);//����չ��

private:
	int rotation();//��ת
	void setRotation(int rotation);//������ת

	QString m_titleText;//��ʾ���ı�
	int m_rotation;//��ͷ�ĽǶ�
	bool m_hasArrow;//�Ƿ��м�ͷ

	QPropertyAnimation* m_animation;//����
protected:
	void paintEvent(QPaintEvent* event);//��д��ͼ�¼�

};
