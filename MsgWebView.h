#pragma once

#include <QWebEngineView> //用于显示网页
#include <QDomNode> //Qt中所有DOM节点如说明,属性,文本等等都可以用此进行操作||表示
#include <qfile.h>
#include <qpixmap.h>
#include <qmessagebox.h>
#include <qstringlist.h>
#include <QDomNodeList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebChannel>

//信息网页对象
class MsgHtmlObj :public QObject {//用于发送信息的对象,和网页交互的必须成员
	Q_OBJECT
	
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml)   //左动态属性 //别人发的信息
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml)   //右动态属性 //自己发的信息
public:
	MsgHtmlObj(QObject* parent=nullptr);

signals:
	void signalMsgHtml(const QString& html);
private:
	void initHtmlTmpl();//初始化聊天网页
	QString getMsgTmplHtml(const QString& code);//获取信息网页

	QString m_msgLHtmlTmpl;//别人发的信息
	QString m_msgRHtmlTmpl;//自己发的信息
};

//信息网页记录
class MsgWebPage :public QWebEnginePage {
	Q_OBJECT
public:
	MsgWebPage::MsgWebPage(QObject* parent = nullptr) :QWebEnginePage(parent) {}
protected:
	//重写是为了对于请求的限制
	bool acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame) {
		//仅接受qrc:/*.html
		if (url.scheme() == QString("qrc"))//判断url类型
			return true;
		return false;
	}
};

//信息网页视图
class MsgWebView : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent = nullptr);
	~MsgWebView();

	void appendMsg(const QString& html);//追加信息

private:
	//解析,用于提取用户发来的表情和信息
	QList<QStringList> parseHtml(const QString& html);//解析html
	//解析节点
	QList<QStringList> parseDocNode(const QDomNode& node);

	MsgHtmlObj* m_msgHtmlObj;//信息网页对象
};
