#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout> //水平布局
#include <QPainter> //画家
#include <QMouseEvent> //鼠标事件
#include <QFile> //文件
#include <qpainterpath.h>
//#include <QStyle> //样式表类


#define BUTTON_HEIGHT 27	//按钮高度
#define BUTTON_WIDTH 27		//按钮宽度
#define TITLE_HEIGHT 27		//标题栏高度

//按钮风格
enum class ButtonType {
	MIN_BUTTON=0, //最小化及关闭按钮
	MIN_MAX_BUTTON,//最小化,最大化及关闭按钮
	ONLY_CLOSE_BUTTON//只有关闭按钮
};

//自定义标题栏 //标题条 //示例:标签.图标_标签.标题__________最小化按钮_最大化按钮_关闭按钮   (还原按钮在界面隐藏下)
class TitleBar : public QWidget
{
	Q_OBJECT

public:
	//std::nullptr_t 是空指针字面量 nullptr 的类型。它是既非指针类型亦非指向成员指针类型的独立类型。
	//若二个重载接受不同指针类型，则需要 std::nullptr_t 的重载以接受空指针常量。
	TitleBar(QWidget *parent=nullptr);
	~TitleBar();

	//设置标题栏图标
	void setTitleIcon(const QString& filePath);
	//设置标题栏内容
	void setTitleContent(const QString& titleContent);
	//设置标题栏宽度
	void setTitleWidth(int width);
	
	//设置标题栏按钮类型
	void setButtonTybe(ButtonType buttonTybe);

	//保存窗口最大化前的位置及大小 
	void saveRestoreInfo(const QPoint& point, const QSize& size);//(标点,大小)
	//获取窗口最大化前的位置及大小 
	void getRestoreInfo(QPoint& point,QSize& size);

private:
	//绘图事件 //绘制标题栏
	void paintEvent(QPaintEvent* event);

	//鼠标双击事件 //双击标题栏进行最大化,最小化操作
	void mouseDoubleClickEvent(QMouseEvent* event);
	
	//通过鼠标按下,移动,松开,从而实现拖动标题栏移动窗口的效果
	//鼠标按下/压事件
	void mousePressEvent(QMouseEvent* event);
	//鼠标移动事件
	void mouseMoveEvent(QMouseEvent* event);
	//鼠标松开事件
	void mouseReleaseEvent(QMouseEvent* event);

	//初始化控件
	void initControl();
	//初始化信号与槽的连接
	void initConnections();
	//加载样式表(样式表名)
	void loadStyleSheet(const QString& sheetName);

signals:
	//按钮发射的信号.标题栏按钮点击时触发的信号
	//最小化按钮.信号  
	void signalButtonMinClicked();
	//最大化还原按钮.信号
	void signalButtonRestoreClicked();
	//最大化按钮.信号
	void signalButtonMaxClicked();
	//关闭按钮.信号
	void signalButtonCloseClicked();

private slots:
	//按钮响应的槽
	//最小化按钮.槽
	void onButtonMinClicked();
	//最大化还原按钮.槽
	void onButtonRestoreClicked();
	//最大化按钮.槽
	void onButtonMaxClicked();
	//关闭按钮.槽
	void onButtonCloseClicked();

private:
	QLabel* m_pIcon; //标题栏图标
	QLabel* m_pTitleContent;//标题栏内容.标签
	QPushButton* m_pButtonMin;//最小化按钮
	QPushButton* m_pButtonRestore;//最大化还原按钮
	QPushButton* m_pButtonClose;//关闭按钮
	QPushButton* m_pButtonMax;//最大化按钮

	//最大化还原按钮变量(用于保存窗体位置和大小)
	QPoint m_restorePos;
	QSize m_restoreSize;

	//移动窗口的变量
	//是否移动
	bool m_isPressed;
	//开始移动的位置
	QPoint m_startMovePos;

	//标题栏内容
	QString m_titleContent;
	//标题栏右上角按钮类型
	ButtonType m_buttonType;
	
};
