#pragma once

#include "BasicWindow.h"
#include "ui_CCMainWindow.h"
#include "CommonUtils.h"
#include <QProxyStyle>
#include <QPainter>
#include "skinWindow.h"
#include <QTimer>
#include "SysTray.h"
#include <QEvent>
#include <QGraphicsOpacityEffect>
#include "NotifyManager.h"
#include "RootContactItem.h"
#include "ContactItem.h"
#include "WindowManager.h"
#include "TalkWindowShell.h"
#include <QMouseEvent>
#include <qapplication.h>

class QTreeWidgetItem;

/*
//�Զ���ķ��
class CustomProxyStyle : public QProxyStyle {
public:
	//PrimitiveElementö�������˸���ԭʼԪ�ء�����Ԫ����һ�ֳ�����GUIԪ�أ��縴ѡ��ָʾ����ťб�ǡ�
	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption* optin
		, QPainter* painter, const QWidget* widget= nullptr)const {
		if (element == PE_FrameFocusRect) {//ͨ�õ��ص�ָ��
			//����Ҫ����Ĭ�ϵĴ�����ı߿�
			return;
		}
		else {
			QProxyStyle::drawPrimitive(element, optin, painter,widget);
		}

	}
};*/

//QQ���������
class CCMainWindow : public BasicWindow
{
	Q_OBJECT

public:
	CCMainWindow(BasicWindow*parent = Q_NULLPTR);
	~CCMainWindow();

	void setUserName(const QString& username);//��ʼ���û���
	void setLevelPixmap(int level);//���õȼ�ͼ��
	void setHeadPixmap(const QString& headPath);//���ñ�ͷͼƬ
	void setStatusMenuIcom(const QString& statusPath);//��������״̬ͼƬ

	QWidget* addOtherAppExtension(const QString& appPath,const QString& appName);//���Ӧ�ò���

	void initContactTree();//��ʼ����ϵ��
private:
	void initControl();//��ʼ���ؼ�
	void initTimer();//��ʼ����ʱ��(ģ��QQ�ȼ�����)
	void updateSeachStyle();//����������ʽ
	void addCompanyDeps(QTreeWidgetItem* pRootGroupItem, const QString& sDeps);//��ӹ�˾����
private:
	void resizeEvent(QResizeEvent* event);//������Сʱ��
	bool eventFilter(QObject* obj, QEvent* event);//�¼�����
	void mousePressEvent(QMouseEvent* event);//��갴���¼�
protected:
	void paintEvent(QPaintEvent* event);//Ϊ���ò������岻͸��
private slots:
	void onAppIconClicked();

	void onItemClicked(QTreeWidgetItem* item, int column);
	void onItemExpanded(QTreeWidgetItem* item);
	void onItemCollapsed(QTreeWidgetItem* item);
	void onItemDoubleClicked(QTreeWidgetItem*, int column);
private:
	Ui::CCMainWindow ui;
	skinWindow* SkinWindow = nullptr;
	QMap<QTreeWidgetItem*,QString> m_groupMap;//���з���ķ�����
};
