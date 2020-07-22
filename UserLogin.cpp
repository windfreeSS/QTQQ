#include "UserLogin.h"

UserLogin::UserLogin(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);//�������� 
	initTitleBar();//��ʼ������
	setTitleBar(" ",":/Resources/MainWindow/qqlogoclassic.png");//���ñ�����(�����ı�,ͼ��)
	loadStyleSheet("UserLogin");//������ʽ��
	initControl();//��ʼ���ؼ�
}

UserLogin::~UserLogin()
{
}

//��ʼ���ؼ�
void UserLogin::initControl()
{
	QLabel* headlabel = new QLabel(this);
	headlabel->setFixedSize(68, 68);//���ù̶���С
	QPixmap pix(":/Resources/MainWindow/head_mask.png");
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/girl.png"), pix, headlabel->size()));
	headlabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);//�ƶ����м�
	
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked);//�����½,��֤���ݿ��˺�����

	//�������ݿ�ʧ��
	if (!connectMySql()) {
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),QString::fromLocal8Bit("����ʧ��!") );
		close();
	}
}

bool UserLogin::connectMySql()//�������ݿ�
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");//myysql���ݿ�
	db.setHostName("127.0.0.1");//����������
	//db.setHostName("localhost");//����������
	db.setPort(3306);//���ö˿�
	db.setUserName("root");//���õ�¼��
	db.setPassword("zhou1zhou2");//��������
	db.setDatabaseName("qtqq");//�������ݿ���

	//QSqlError e = db.lastError();//�����ݿ���ֵ����һ������
	//QMessageBox::information(NULL, QString::fromLocal8Bit("�򲻿�MySQL����:"), e.databaseText()+"--"+e.driverText());

	if (db.open()) {
		//QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("���ӳɹ�!"));
		return true;//���ݴ򿪳ɹ�
	}else{
		return false;//���ݿ��ʧ��
	}
}

bool UserLogin::veryfyAccountCode()//��֤�˻�&����
{//���Ż�
	
	QString strAccountInput = ui.editUserAccount->text();//�˻�
	QString strCodeInput = ui.editPassword->text();//����

	//����Ա���Ż���QQ�� 
	QString strSqlCode = QString("select code from tab_accounts where employeeID = %1;").arg(strAccountInput);
	QSqlQuery queryEmployeeID(strSqlCode);//��ѯ
	queryEmployeeID.exec();//ִ��

	if (queryEmployeeID.first()) {//ָ��������һ��
		QString strCode = queryEmployeeID.value(0).toString();//���ݿ���QQ�����ݶ�Ӧ������
		if (strCode == strCodeInput) {//�ж���������ݿ�������Ƿ�һ��
			return true;
		}
		else {
			return false;
		}
	}

	//�˺ŵ�¼
	strSqlCode = QString("select code,employeeID from tab_accounts where account  = '%1';").arg(strAccountInput);
	QSqlQuery queryAccount(strSqlCode);
	queryAccount.exec();

	if (queryAccount.first()) {
		QString strCode = queryAccount.value(0).toString();
		if (strCode == strCodeInput)
			return true;
		else
			return false;
	}

	return false;
}

void UserLogin::onLoginBtnClicked() {
	if (!veryfyAccountCode()) {
		QMessageBox::warning(NULL, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("��������˺Ż���������������!"));
		//ui.editUserAccount->setText("");
		//ui.editPassword->setText("");
		return;
	}
	close();
	CCMainWindow* mainwindow = new CCMainWindow;
	mainwindow->show();
}
