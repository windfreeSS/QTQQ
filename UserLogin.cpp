#include "UserLogin.h"

QString gLoginEmployeeID;//登录者QQ号(员工号)

UserLogin::UserLogin(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);//设置属性 
	initTitleBar();//初始化标题
	setTitleBar(" ",":/Resources/MainWindow/qqlogoclassic.png");//设置标题栏(标题文本,图标)
	loadStyleSheet("UserLogin");//加载样式表
	initControl();//初始化控件
}

UserLogin::~UserLogin()
{
}

//初始化控件
void UserLogin::initControl()
{
	QLabel* headlabel = new QLabel(this);
	headlabel->setFixedSize(68, 68);//设置固定大小
	QPixmap pix(":/Resources/MainWindow/head_mask.png");
	headlabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/app/logo.ico"), pix, headlabel->size()));
	headlabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);//移动到中间
	
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked);//点击登陆,验证数据库账号密码

	//连接数据库失败
	if (!connectMySql()) {
		QMessageBox::information(NULL, QString("提示"), QString("连接失败!"));
		//QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("连接失败!") );
		close();
	}
}

bool UserLogin::connectMySql()//连接数据库
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");//myysql数据库
	db.setHostName("127.0.0.1");//设置主机名
	//db.setHostName("localhost");//设置主机名
	db.setPort(3306);//设置端口
	db.setUserName("root");//设置登录名
	db.setPassword("zhou1zhou2");//设置密码
	db.setDatabaseName("qtqq");//设置数据库名

	//QSqlError e = db.lastError();//打开数据库出现的最后一个问题
	//QMessageBox::information(NULL, QString::fromLocal8Bit("打不开MySQL问题:"), e.databaseText()+"--"+e.driverText());

	if (db.open()) {
		//QMessageBox::information(nullptr, QString("提示"), QString("连接成功!"));
		//QMessageBox::information(nullptr, QStringLiteral("提示"), QStringLiteral("连接成功!"));
		//QMessageBox::information(NULL, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("连接成功!"));
		return true;//数据打开成功
	}else{
		return false;//数据库打开失败
	}
}

bool UserLogin::veryfyAccountCode(bool &isAccountLogin, QString& strAccount)//验证账户&密码
{//需优化
	
	QString strAccountInput = ui.editUserAccount->text();//账户
	QString strCodeInput = ui.editPassword->text();//密码

	//输入员工号或者QQ号 
	QString strSqlCode = QString("select code from tab_accounts where employeeID = %1").arg(strAccountInput);
	QSqlQuery queryEmployeeID(strSqlCode);//查询
	queryEmployeeID.exec();//执行

	if (queryEmployeeID.first()) {//指向结果集第一条
		QString strCode = queryEmployeeID.value(0).toString();//数据库中QQ号数据对应的密码
		if (strCode == strCodeInput) {//判断密码和数据库的密码是否一致
			gLoginEmployeeID = strAccountInput;//登录者QQ号(员工号)
			//QMessageBox::information(nullptr, QString("员工号:"), gLoginEmployeeID);
			isAccountLogin = false;
			strAccount = strAccountInput;
			return true;
		}
		else {
			return false;
		}
	}

	//账号登录
	strSqlCode = QString("select code,employeeID from tab_accounts where account  = '%1'").arg(strAccountInput);
	QSqlQuery queryAccount(strSqlCode);
	queryAccount.exec();

	if (queryAccount.first()) {
		QString strCode = queryAccount.value(0).toString();
		if (strCode == strCodeInput){
			gLoginEmployeeID= queryAccount.value(1).toString();
			//QMessageBox::information(nullptr, QString("账号:"), gLoginEmployeeID);
			isAccountLogin = true;
			strAccount = strAccountInput;
			return true;
		}
		else {
			return false;
		}
	}

	return false;
}

void UserLogin::onLoginBtnClicked() {
	bool isAccountLogin;
	QString strAccount;//账号或QQ号
	if (!veryfyAccountCode(isAccountLogin, strAccount)) {
		QMessageBox::warning(Q_NULLPTR, QString("提示"), QString("您输入的账号或密码请重新输入!"));
		//ui.editUserAccount->setText("");
		//ui.editPassword->setText("");
		return;
	}
	close();
	CCMainWindow* mainwindow = new CCMainWindow(strAccount, isAccountLogin);
	mainwindow->show();
}
