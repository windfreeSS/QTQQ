#include "QMsgTextEdit.h"

QMsgTextEdit::QMsgTextEdit(QWidget *parent)
	: QTextEdit(parent)
{
}

QMsgTextEdit::~QMsgTextEdit()
{
	deleteAllEmotionImage();
}

void QMsgTextEdit::addEmotionUrl(int emotionNum)//添加表情
{
	const QString& imageName = QString("qrc:/Resources/MainWindow/emotion/%1.png").arg(emotionNum);
	const QString& flagName = QString("%1").arg(imageName);
	insertHtml(QString("<img src='%1' />").arg(flagName));//插入网页 //bug
	if (m_listEmotionUrl.contains(imageName))return;
	else m_listEmotionUrl.append(imageName);//链表中没有就添加进去

	QMovie* apngMovie = new QMovie(imageName,"apng",this);
	m_emotionMap.insert(apngMovie, flagName);
	
	//数据帧改变时发射的信号
	connect(apngMovie,SIGNAL(frameChanged(int)),this,SLOT(onEmotionImageFrameChange(int)));
	apngMovie->start();//启动
	updateGeometry();//更新几何
}

void QMsgTextEdit::deleteAllEmotionImage()//删除所有表情图片
{
	for (auto itor = m_emotionMap.constBegin(); itor != m_emotionMap.constEnd();++itor) {
		delete itor.key();
	}
	m_emotionMap.clear();
}

void QMsgTextEdit::onEmotionImageFrameChange(int frame) {
	QMovie* movie = qobject_cast<QMovie*>(sender());//获取信号发送者
	//当前文档->添加资源(图片资源,图片资源的url,模式)
	document()->addResource(QTextDocument::ImageResource, QUrl(m_emotionMap.value(movie)), movie->currentPixmap());
}