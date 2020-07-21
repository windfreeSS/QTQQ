#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout> //ˮƽ����
#include <QPainter> //����
#include <QMouseEvent> //����¼�
#include <QFile> //�ļ�
#include <qpainterpath.h>
//#include <QStyle> //��ʽ����


#define BUTTON_HEIGHT 27	//��ť�߶�
#define BUTTON_WIDTH 27		//��ť���
#define TITLE_HEIGHT 27		//�������߶�

//��ť���
enum class ButtonType {
	MIN_BUTTON=0, //��С�����رհ�ť
	MIN_MAX_BUTTON,//��С��,��󻯼��رհ�ť
	ONLY_CLOSE_BUTTON//ֻ�йرհ�ť
};

//�Զ�������� //������ //ʾ��:��ǩ.ͼ��_��ǩ.����__________��С����ť_��󻯰�ť_�رհ�ť   (��ԭ��ť�ڽ���������)
class TitleBar : public QWidget
{
	Q_OBJECT

public:
	//std::nullptr_t �ǿ�ָ�������� nullptr �����͡����Ǽȷ�ָ���������ָ���Աָ�����͵Ķ������͡�
	//���������ؽ��ܲ�ָͬ�����ͣ�����Ҫ std::nullptr_t �������Խ��ܿ�ָ�볣����
	TitleBar(QWidget *parent=nullptr);
	~TitleBar();

	//���ñ�����ͼ��
	void setTitleIcon(const QString& filePath);
	//���ñ���������
	void setTitleContent(const QString& titleContent);
	//���ñ��������
	void setTitleWidth(int width);
	
	//���ñ�������ť����
	void setButtonTybe(ButtonType buttonTybe);

	//���洰�����ǰ��λ�ü���С 
	void saveRestoreInfo(const QPoint& point, const QSize& size);//(���,��С)
	//��ȡ�������ǰ��λ�ü���С 
	void getRestoreInfo(QPoint& point,QSize& size);

private:
	//��ͼ�¼� //���Ʊ�����
	void paintEvent(QPaintEvent* event);

	//���˫���¼� //˫���������������,��С������
	void mouseDoubleClickEvent(QMouseEvent* event);
	
	//ͨ����갴��,�ƶ�,�ɿ�,�Ӷ�ʵ���϶��������ƶ����ڵ�Ч��
	//��갴��/ѹ�¼�
	void mousePressEvent(QMouseEvent* event);
	//����ƶ��¼�
	void mouseMoveEvent(QMouseEvent* event);
	//����ɿ��¼�
	void mouseReleaseEvent(QMouseEvent* event);

	//��ʼ���ؼ�
	void initControl();
	//��ʼ���ź���۵�����
	void initConnections();
	//������ʽ��(��ʽ����)
	void loadStyleSheet(const QString& sheetName);

signals:
	//��ť������ź�.��������ť���ʱ�������ź�
	//��С����ť.�ź�  
	void signalButtonMinClicked();
	//��󻯻�ԭ��ť.�ź�
	void signalButtonRestoreClicked();
	//��󻯰�ť.�ź�
	void signalButtonMaxClicked();
	//�رհ�ť.�ź�
	void signalButtonCloseClicked();

private slots:
	//��ť��Ӧ�Ĳ�
	//��С����ť.��
	void onButtonMinClicked();
	//��󻯻�ԭ��ť.��
	void onButtonRestoreClicked();
	//��󻯰�ť.��
	void onButtonMaxClicked();
	//�رհ�ť.��
	void onButtonCloseClicked();

private:
	QLabel* m_pIcon; //������ͼ��
	QLabel* m_pTitleContent;//����������.��ǩ
	QPushButton* m_pButtonMin;//��С����ť
	QPushButton* m_pButtonRestore;//��󻯻�ԭ��ť
	QPushButton* m_pButtonClose;//�رհ�ť
	QPushButton* m_pButtonMax;//��󻯰�ť

	//��󻯻�ԭ��ť����(���ڱ��洰��λ�úʹ�С)
	QPoint m_restorePos;
	QSize m_restoreSize;

	//�ƶ����ڵı���
	//�Ƿ��ƶ�
	bool m_isPressed;
	//��ʼ�ƶ���λ��
	QPoint m_startMovePos;

	//����������
	QString m_titleContent;
	//���������Ͻǰ�ť����
	ButtonType m_buttonType;
	
};
