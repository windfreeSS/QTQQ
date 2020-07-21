#pragma once

#include <QDialog>
#include "TitleBar.h"
#include <QStyleOption>
#include <QApplication>
#include <QDeskTopWidget>
#include <QLabel>
#include "CommonUtils.h"
#include "NotifyManager.h"

//实现窗体的样式表(风格),头像,事件的处理,标题栏,位置,窗体移动

//基类窗口 //用于其它窗口的基类
class BasicWindow : public QDialog
{
	Q_OBJECT

public:
	BasicWindow(QWidget *parent = nullptr);
	virtual ~BasicWindow();

public:
	//加载样式表
	void loadStyleSheet(const QString& sheetName);
	//获取圆头像(来源,圆头像,大小)
	QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize = QSize(0, 0));

private:
	//初始化背景
	void initBackGroundColor();

protected:
	//绘制事件 //子类化部件时,需要重写绘图事件设置背景图
	void paintEvent(QPaintEvent* event);
	//鼠标按压事件
	void mousePressEvent(QMouseEvent* event);
	//鼠标移动事件
	void mouseMoveEvent(QMouseEvent* event);
	//鼠标释放事件
	void mouseReleaseEvent(QMouseEvent* event);

protected:
	//初始化标题栏
	void initTitleBar(ButtonType buttontype = ButtonType::MIN_BUTTON);
	//设置标题栏
	void setTitleBar(const QString& title, const QString& icon = "");

private slots:
	//窗口显示操作信号:
	void onShowClose(bool);//显示关闭
	void onShowMin(bool);//最小化
	void onShowHide(bool);//隐藏
	void onShowNormal(bool);//状态 //正常显示
	void onShowQuit(bool);//退出
	void onSignalSkinChanged(const QColor& color);//皮肤改变

	//按钮点击:
	void onButtonMinClicked();//最小化
	void onButtonRestoreClicked();//最大化及还原
	void onButtonMaxClicked();//最大化
	void onButtonCloseClicked();//关闭

protected:
	QPoint m_mousePoint;//鼠标位置
	bool m_mousePressed;//鼠标是否按下
	QColor m_colorBackGround;//背景色
	QString m_styleName;//样式名称
	TitleBar* _titleBar;//标题栏
};
