#include "MsgWebView.h"

MsgHtmlObj::MsgHtmlObj(QObject* parent):QObject(parent){
	initHtmlTmpl();//初始化网页

}

void MsgHtmlObj::initHtmlTmpl() {//初始化网页
	m_msgLHtmlTmpl = getMsgTmplHtml("msgleftTmpl");
	m_msgRHtmlTmpl = getMsgTmplHtml("msgrightTmpl");
}

QString MsgHtmlObj::getMsgTmplHtml(const QString& code) {//获取信息网页
	QFile file(":/Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strData=nullptr;
	if (file.isOpen()) {//打开失败
		strData = QLatin1String(file.readAll());
		file.close();
	}
	else {
		QMessageBox::information(nullptr, "Tips", "Failed to init html!");//提示打开文件失败
	}
	return strData;
}

/************************************************************************************************************************************************************/

MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent)
{
	MsgWebPage* page = new MsgWebPage(this);//网页
	setPage(page);//设置当前网页

	QWebChannel* channel = new QWebChannel(this);//通道
	m_msgHtmlObj = new MsgHtmlObj(this);

	channel->registerObject("external",m_msgHtmlObj);//注册对象 //external在\Resources\MainWindow\MsgHtml\msgtmpl.js文件里
	this->page()->setWebChannel(channel);//设置网络通道
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));//初始化收信息页面
}

MsgWebView::~MsgWebView()
{
}

void MsgWebView::appendMsg(const QString& html)//追加信息
{
	QJsonObject msgObj;//用于发送信息的
	QString qsMsg;
	QList<QStringList> msgList = parseHtml(html);//解析html
	
	for (int i = 0; i < msgList.size(); i++) {//bug图片和文字不能一起发送和发完图片再发文字,文字的字体丢失情况
		if (msgList.at(i).at(0) == "img") {//图片
			QString imagePath = msgList.at(i).at(1);//图片路径
			/*路径可能会以下这样
			"qrc:/MainWindow/.../77.png"
			":/MainWindow/.../77.png"
			*/
			QPixmap pixmap;//加载表情图片
			if (imagePath.left(3) == "qrc") {//判断路径左边的N个字符
				//去掉路径中的"qrc"再加载图片 //mid()是从中间截取
				pixmap.load(imagePath.mid(3));
			}
			else {
				pixmap.load(imagePath);
			}

			//表情图片html格式文本组合	//路径,宽度,高度 
			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\"/>")
				.arg(imagePath).arg(pixmap.width()).arg(pixmap.height());

			qsMsg += imgPath;
		}
		else if (msgList.at(i).at(0) == "text") {//文字
			qsMsg += msgList.at(i).at(1);
		}
	}
	msgObj.insert("MSG", qsMsg);//key,值 //体现在html中的大写MSG中插入
	//转换文档
	const QString& Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);
	this->page()->runJavaScript(QString("appendHtml(%1)").arg(Msg));//java脚本的运行
}

QList<QStringList> MsgWebView::parseHtml(const QString& html)//解析html
{
	QDomDocument doc;
	doc.setContent(html);//设置节点内容
	const QDomElement& root = doc.documentElement();//节点元素
	const QDomNode& node = root.firstChildElement("body");//第一个子元素

	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode& node)//解析节点
{
	QList<QStringList> attribute;
	const QDomNodeList& list = node.childNodes();//返回所有子节点

	/*html资源中的信息示例
	<div class="msg-wrap right">
	<img class="header" src="qrc:/Resources/MainWindow/girl.png" ondragstart="return false;" onselectstart="return false;" onselect=”document.selection.empty();”>
	<div class="msg">{{MSG}}<span class="trigon"></span></div>
</div>
	*/
	for (int i = 0; i < list.count(); i++) {
		const QDomNode& node = list.at(i);
		if (node.isElement()) {//判断是否是元素
			//转换元素
			const QDomElement& element = node.toElement();
			if (element.tagName() == "img") {//元素的标签名是否是图片
				QStringList attributeList;
				attributeList << "img" << element.attribute("src");//从html中提取图片路径
				attribute << attributeList;
			}
			if (element.tagName() == "span") {
				QStringList attributeList;
				attributeList << "text" << element.text();
				attribute << attributeList;
			}
			if (node.hasChildNodes()) {//还有子节点
				attribute<<parseDocNode(node);
			}
		}
	}

	return attribute;
}

