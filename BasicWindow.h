#pragma once

#include <QDialog>
#include "TitleBar.h"
#include <QStyleOption>
#include <QApplication>
#include <QDeskTopWidget>
#include <QLabel>
#include "CommonUtils.h"
#include "NotifyManager.h"

//ʵ�ִ������ʽ��(���),ͷ��,�¼��Ĵ���,������,λ��,�����ƶ�

//���ര�� //�����������ڵĻ���
class BasicWindow : public QDialog
{
	Q_OBJECT

public:
	BasicWindow(QWidget *parent = nullptr);
	virtual ~BasicWindow();

public:
	//������ʽ��
	void loadStyleSheet(const QString& sheetName);
	//��ȡԲͷ��(��Դ,Բͷ��,��С)
	QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize = QSize(0, 0));

private:
	//��ʼ������
	void initBackGroundColor();

protected:
	//�����¼� //���໯����ʱ,��Ҫ��д��ͼ�¼����ñ���ͼ
	void paintEvent(QPaintEvent* event);
	//��갴ѹ�¼�
	void mousePressEvent(QMouseEvent* event);
	//����ƶ��¼�
	void mouseMoveEvent(QMouseEvent* event);
	//����ͷ��¼�
	void mouseReleaseEvent(QMouseEvent* event);

protected:
	//��ʼ��������
	void initTitleBar(ButtonType buttontype = ButtonType::MIN_BUTTON);
	//���ñ�����
	void setTitleBar(const QString& title, const QString& icon = "");

private slots:
	//������ʾ�����ź�:
	void onShowClose(bool);//��ʾ�ر�
	void onShowMin(bool);//��С��
	void onShowHide(bool);//����
	void onShowNormal(bool);//״̬ //������ʾ
	void onShowQuit(bool);//�˳�
	void onSignalSkinChanged(const QColor& color);//Ƥ���ı�

	//��ť���:
	void onButtonMinClicked();//��С��
	void onButtonRestoreClicked();//��󻯼���ԭ
	void onButtonMaxClicked();//���
	void onButtonCloseClicked();//�ر�

protected:
	QPoint m_mousePoint;//���λ��
	bool m_mousePressed;//����Ƿ���
	QColor m_colorBackGround;//����ɫ
	QString m_styleName;//��ʽ����
	TitleBar* _titleBar;//������
};
