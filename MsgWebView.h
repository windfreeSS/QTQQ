#pragma once

#include <QWebEngineView> //������ʾ��ҳ
#include <QDomNode> //Qt������DOM�ڵ���˵��,����,�ı��ȵȶ������ô˽��в���||��ʾ
#include <qfile.h>
#include <qpixmap.h>
#include <qmessagebox.h>
#include <qstringlist.h>
#include <QDomNodeList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebChannel>

//��Ϣ��ҳ����
class MsgHtmlObj :public QObject {//���ڷ�����Ϣ�Ķ���,����ҳ�����ı����Ա
	Q_OBJECT
	
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)   //��̬���� //���˷�����Ϣ
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)   //�Ҷ�̬���� //�Լ�������Ϣ
public:
	MsgHtmlObj(QObject* parent=nullptr);

signals:
	void signalMsgHtml(const QString& html);
private:
	void initHtmlTmpl();//��ʼ��������ҳ
	QString getMsgTmplHtml(const QString& code);//��ȡ��Ϣ��ҳ

	QString m_msgLHtmlTmpl;//���˷�����Ϣ
	QString m_msgRHtmlTmpl;//�Լ�������Ϣ
};

//��Ϣ��ҳ��¼
class MsgWebPage :public QWebEnginePage {
	Q_OBJECT
public:
	MsgWebPage::MsgWebPage(QObject* parent = nullptr) :QWebEnginePage(parent) {}
protected:
	//��д��Ϊ�˶������������
	bool acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame) {
		//������qrc:/*.html
		if (url.scheme() == QString("qrc"))//�ж�url����
			return true;
		return false;
	}
};

//��Ϣ��ҳ��ͼ
class MsgWebView : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent = nullptr);
	~MsgWebView();

	void appendMsg(const QString& html);//׷����Ϣ

private:
	//����,������ȡ�û������ı������Ϣ
	QList<QStringList> parseHtml(const QString& html);//����html
	//�����ڵ�
	QList<QStringList> parseDocNode(const QDomNode& node);

	MsgHtmlObj* m_msgHtmlObj;//��Ϣ��ҳ����
};
