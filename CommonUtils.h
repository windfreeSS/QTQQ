#pragma once
#include <QProxyStyle>
#include <QPainter>
#include <QFile>
#include <QWidget>
#include <QApplication>
#include <QSettings> //��������

//�ı�Ĭ�ϵĲ������
class CustomProxyStyle :public QProxyStyle {
public:
	//�Զ���ί�з��
	CustomProxyStyle(QObject* parent=Q_NULLPTR) {
		setParent(parent);
	}

	//����(��Ԫ,���ѡ��,����,����)
	virtual void drawPrimitive(
	PrimitiveElement element,const QStyleOption* option, QPainter* painter, const QWidget* widget = 0)const{
		if (PE_FrameFocusRect == element) {//�жϲ�����ȡ����ʱ���ֱ߿�����߿���
			//ȥ��windows�в���Ĭ�ϵı߿�����߿�,������ȡ����ʱֱ�ӷ���,�����л���
			return;
		}
		else {
			//ί�з��::
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}
	}
};

//��ͬ��/ͨ�õ�
class CommonUtils
{
public:
	CommonUtils();
	~CommonUtils();

public:
	static QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize = QSize(0,0));//��ȡԲͷ��
	static void loadStyleSheet(QWidget* widget, const QString& sheetName);//������ʽ��
	static void setDefaultSkinColor(const QColor& color);//����Ĭ��Ƥ����ɫ
	static QColor getDefaultSkinColor();//��ȡĬ��Ƥ����ɫ
};

