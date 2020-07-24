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
#include <QSqlQuery>

class QTreeWidgetItem;

extern QString gLoginEmployeeID;//��¼��QQ��(Ա����)

//QQ���������
class CCMainWindow : public BasicWindow
{
	Q_OBJECT

public:
	CCMainWindow(QString account,bool isAccountLogin,BasicWindow* parent = Q_NULLPTR);
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
	void addCompanyDeps(QTreeWidgetItem* pRootGroupItem, int DepID);//��ӹ�˾����

	QString getHeadPicturePath();//��ȡ��¼��ͷ��·��
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
	//QMap<QTreeWidgetItem*,QString> m_groupMap;//���з���ķ�����

	bool m_isAccountLogin;
	QString m_account;//�˺Ż�QQ��
};
