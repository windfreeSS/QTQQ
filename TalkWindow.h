#pragma once

#include <QWidget>
#include <qfile.h>
#include <QMessageBox>
#include "TalkWindowShell.h"
#include "RootContactItem.h"
#include "ContactItem.h"
#include "CommonUtils.h"
#include <QTreeWidgetItem>
#include <qtooltip.h>
#include "WindowManager.h"
#include "ui_TalkWindow.h"
#include <QSqlQueryModel>
#include <QSqlQuery>

//��ϵ����
class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent ,const QString& uid/*,enum class GroupType groupType*/);
	~TalkWindow();

	void addEmotionImage(int emotionNum);//��ӱ�����Ŀ
	void setWindowName(const QString& name);//���ô�����

private slots:
	void onSendBtnClicked(bool);
	void onItemDoubleClicked(QTreeWidgetItem*,int);//��˫��������һ����в���,��
private:
	void initControl();//��ʼ���ؼ�
	void initGroupTalkStatus();//��ʼ��Ⱥ��״̬
	void initTalkWindow();//��ʼ��Ⱥ��
	int getCompDepID();//��ȡ��˾ID

	//void initCompanyTalk();//��ʼ����˾Ⱥ����
	//void initPersonelTalk();//��ʼ�����²�����
	//void initMarketTalk();//��ʼ���г�������
	//void initDevelopTalk();//��ʼ���з�������
	void initPtoPTalk();//��ʼ����������

	void addPeopInfo(QTreeWidgetItem* pRootGroupItem,int employeeID);//�������

	Ui::TalkWindow ui;
	QString m_talkId;//��ϵid
	//GroupType m_groupType;//���������
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap;//���з�����ϵ������

	bool m_isGroupTalk;//�Ƿ�ΪȺ��
};
