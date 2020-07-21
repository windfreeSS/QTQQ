#pragma once
#include <QProxyStyle>
#include <QPainter>
#include <QFile>
#include <QWidget>
#include <QApplication>
#include <QSettings> //操作配置

//改变默认的部件风格
class CustomProxyStyle :public QProxyStyle {
public:
	//自定义委托风格
	CustomProxyStyle(QObject* parent=Q_NULLPTR) {
		setParent(parent);
	}

	//绘制(素元,风格选择,画家,部件)
	virtual void drawPrimitive(
	PrimitiveElement element,const QStyleOption* option, QPainter* painter, const QWidget* widget = 0)const{
		if (PE_FrameFocusRect == element) {//判断部件获取焦点时出现边框或虚线框风格
			//去掉windows中部件默认的边框或虚线框,部件获取焦点时直接返回,不进行绘制
			return;
		}
		else {
			//委托风格::
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}
	}
};

//共同的/通用的
class CommonUtils
{
public:
	CommonUtils();
	~CommonUtils();

public:
	static QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize = QSize(0,0));//获取圆头像
	static void loadStyleSheet(QWidget* widget, const QString& sheetName);//加载样式表
	static void setDefaultSkinColor(const QColor& color);//设置默认皮肤颜色
	static QColor getDefaultSkinColor();//获取默认皮肤颜色
};

