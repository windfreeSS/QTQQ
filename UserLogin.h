#pragma once

#include "BasicWindow.h"
#include "CCMainWindow.h"
#include "ui_UserLogin.h"
#include <QtSql>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>


//��½����
class UserLogin : public BasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = Q_NULLPTR);
	~UserLogin();

private slots:
	void onLoginBtnClicked();

private:
	void initControl();//��ʼ���ؼ�
	bool connectMySql();//�������ݿ�
	bool veryfyAccountCode(bool& isAccountLogin, QString& strAccount);//��֤�˻�&����

	Ui::UserLogin ui;
};
