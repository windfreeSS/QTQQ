#include <QGuiApplication> //Gui
#include <QApplication> //����QWidget�� Gui
#include <QQmlApplicationEngine>
#include "UserLogin.h"
//#include "CCMainWindow.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    //if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)) QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    //qDebug() << QSqlDatabase::drivers();//������ݿ��Ƿ��������

    a.setQuitOnLastWindowClosed(false);

    UserLogin* userlogin = new UserLogin;
    userlogin->show();

    return a.exec();
}
