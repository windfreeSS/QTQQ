#include "MsgWebView.h"

MsgHtmlObj::MsgHtmlObj(QObject* parent):QObject(parent){
	initHtmlTmpl();//��ʼ����ҳ

}

void MsgHtmlObj::initHtmlTmpl() {//��ʼ����ҳ
	m_msgLHtmlTmpl = getMsgTmplHtml("msgleftTmpl");
	m_msgRHtmlTmpl = getMsgTmplHtml("msgrightTmpl");
}

QString MsgHtmlObj::getMsgTmplHtml(const QString& code) {//��ȡ��Ϣ��ҳ
	QFile file(":/Resources/MainWindow/MsgHtml/" + code + ".html");
	file.open(QFile::ReadOnly);
	QString strData=nullptr;
	if (file.isOpen()) {//��ʧ��
		strData = QLatin1String(file.readAll());
		file.close();
	}
	else {
		QMessageBox::information(nullptr, "Tips", "Failed to init html!");//��ʾ���ļ�ʧ��
	}
	return strData;
}

/************************************************************************************************************************************************************/

MsgWebView::MsgWebView(QWidget *parent)
	: QWebEngineView(parent)
{
	MsgWebPage* page = new MsgWebPage(this);//��ҳ
	setPage(page);//���õ�ǰ��ҳ

	QWebChannel* channel = new QWebChannel(this);//ͨ��
	m_msgHtmlObj = new MsgHtmlObj(this);

	channel->registerObject("external",m_msgHtmlObj);//ע����� //external��\Resources\MainWindow\MsgHtml\msgtmpl.js�ļ���
	this->page()->setWebChannel(channel);//��������ͨ��
	this->load(QUrl("qrc:/Resources/MainWindow/MsgHtml/msgTmpl.html"));//��ʼ������Ϣҳ��
}

MsgWebView::~MsgWebView()
{
}

void MsgWebView::appendMsg(const QString& html)//׷����Ϣ
{
	QJsonObject msgObj;//���ڷ�����Ϣ��
	QString qsMsg;
	QList<QStringList> msgList = parseHtml(html);//����html
	
	for (int i = 0; i < msgList.size(); i++) {//bugͼƬ�����ֲ���һ���ͺͷ���ͼƬ�ٷ�����,���ֵ����嶪ʧ���
		if (msgList.at(i).at(0) == "img") {//ͼƬ
			QString imagePath = msgList.at(i).at(1);//ͼƬ·��
			/*·�����ܻ���������
			"qrc:/MainWindow/.../77.png"
			":/MainWindow/.../77.png"
			*/
			QPixmap pixmap;//���ر���ͼƬ
			if (imagePath.left(3) == "qrc") {//�ж�·����ߵ�N���ַ�
				//ȥ��·���е�"qrc"�ټ���ͼƬ //mid()�Ǵ��м��ȡ
				pixmap.load(imagePath.mid(3));
			}
			else {
				pixmap.load(imagePath);
			}

			//����ͼƬhtml��ʽ�ı����	//·��,���,�߶� 
			QString imgPath = QString("<img src=\"%1\" width=\"%2\" height=\"%3\"/>")
				.arg(imagePath).arg(pixmap.width()).arg(pixmap.height());

			qsMsg += imgPath;
		}
		else if (msgList.at(i).at(0) == "text") {//����
			qsMsg += msgList.at(i).at(1);
		}
	}
	msgObj.insert("MSG", qsMsg);//key,ֵ //������html�еĴ�дMSG�в���
	//ת���ĵ�
	const QString& Msg = QJsonDocument(msgObj).toJson(QJsonDocument::Compact);
	this->page()->runJavaScript(QString("appendHtml(%1)").arg(Msg));//java�ű�������
}

QList<QStringList> MsgWebView::parseHtml(const QString& html)//����html
{
	QDomDocument doc;
	doc.setContent(html);//���ýڵ�����
	const QDomElement& root = doc.documentElement();//�ڵ�Ԫ��
	const QDomNode& node = root.firstChildElement("body");//��һ����Ԫ��

	return parseDocNode(node);
}

QList<QStringList> MsgWebView::parseDocNode(const QDomNode& node)//�����ڵ�
{
	QList<QStringList> attribute;
	const QDomNodeList& list = node.childNodes();//���������ӽڵ�

	/*html��Դ�е���Ϣʾ��
	<div class="msg-wrap right">
	<img class="header" src="qrc:/Resources/MainWindow/girl.png" ondragstart="return false;" onselectstart="return false;" onselect=��document.selection.empty();��>
	<div class="msg">{{MSG}}<span class="trigon"></span></div>
</div>
	*/
	for (int i = 0; i < list.count(); i++) {
		const QDomNode& node = list.at(i);
		if (node.isElement()) {//�ж��Ƿ���Ԫ��
			//ת��Ԫ��
			const QDomElement& element = node.toElement();
			if (element.tagName() == "img") {//Ԫ�صı�ǩ���Ƿ���ͼƬ
				QStringList attributeList;
				attributeList << "img" << element.attribute("src");//��html����ȡͼƬ·��
				attribute << attributeList;
			}
			if (element.tagName() == "span") {
				QStringList attributeList;
				attributeList << "text" << element.text();
				attribute << attributeList;
			}
			if (node.hasChildNodes()) {//�����ӽڵ�
				attribute<<parseDocNode(node);
			}
		}
	}

	return attribute;
}

