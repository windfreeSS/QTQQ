#pragma once

#include "BasicWindow.h"
#include "CCMainWindow.h"
#include "ui_UserLogin.h"
#include <QtSql>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>


//登陆窗口
class UserLogin : public BasicWindow
{
	Q_OBJECT

public:
	UserLogin(QWidget *parent = Q_NULLPTR);
	~UserLogin();

private slots:
	void onLoginBtnClicked();

private:
	void initControl();//初始化控件
	bool connectMySql();//连接数据库
	bool veryfyAccountCode(bool& isAccountLogin, QString& strAccount);//验证账户&密码

	Ui::UserLogin ui;
};
