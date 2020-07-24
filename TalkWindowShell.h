#pragma once

#include "BasicWindow.h"
#include "CommonUtils.h"
#include <qmap.h>
#include <qlistwidget.h>
#include "EmotionWindow.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"
#include "ui_TalkWindowShell.h"
#include <QSqlQueryModel>
//#include <QListWidgetItem>

//���촰������
enum class GroupType{
	COMPANY,//��˾Ⱥ
	PERSONELGROUP,//���²�
	DEVELOPMENTGROUP,//�з���
	MARKETGROUP,//�г���
	PTOP	//ͬ��֮�䵥������
};

class TalkWindow;//��ϵ����

//���촰������
class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(BasicWindow* parent = Q_NULLPTR);
	~TalkWindowShell();

	//����µ����촰��(���촰��,���촰����Ŀ,���촰������)
	void addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem, const QString& uid/*, enum class GroupType grouptype*/);
	void setCurrenWidget(QWidget* widget);//���õ�ǰ���촰��

	const QMap<QListWidgetItem*, QWidget*>& gettalkWindowItemMap()const;
public slots:
	void onEmotionBtnClicked(bool);//���鰴ť�����ִ�еĲۺ���
private slots:
	void onTalkWindowItemClicked(QListWidgetItem* item);//����б�����ִ�еĲۺ���
	void onEmotionItemClicked(int emotionNum);//���鱻ѡ��ִ�еĲۺ���

private:
	void initControl();//��ʼ���ռ�

	Ui::TalkWindowShellClass ui;
	QMap<QListWidgetItem*, QWidget*> m_talkWindowItemMap;//�򿪵����촰��
	EmotionWindow* m_emotionWindow;//���鴰��
};
