#pragma once

#include <QLabel>
#include <QPropertyAnimation>
#include <QPainter>

//根联系项
class RootContactItem : public QLabel
{
	Q_OBJECT

		//箭头旋转角度变化
		Q_PROPERTY(int rotation READ rotation WRITE setRotation)
public:
	RootContactItem(bool hasArrow = true, QWidget* parent =nullptr);//箭头,父项
	~RootContactItem();

	void setText(const QString& title);//设置文本
	void setExpanded(bool expand);//设置展开

private:
	int rotation();//旋转
	void setRotation(int rotation);//设置旋转

	QString m_titleText;//显示的文本
	int m_rotation;//箭头的角度
	bool m_hasArrow;//是否有箭头

	QPropertyAnimation* m_animation;//动画
protected:
	void paintEvent(QPaintEvent* event);//重写绘图事件

};
