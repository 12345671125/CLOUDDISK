/*主界面类，用于客户端初始化*/

#ifndef LOGINANDREGIST_H
#define LOGINANDREGIST_H
#pragma once
#include <QMainWindow>
#include <QFile> //文件类用于静态读取文件
#include <QMessageBox>

#include <QTimer>
#include <QAbstractSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class clientWin; };
namespace protocol { class PDU;};
QT_END_NAMESPACE


class OpeWidget;


class loginAndRegist : public QMainWindow
{
    Q_OBJECT

public:
    static loginAndRegist *getInstance();
    void connectToSlot();
    ~loginAndRegist();
signals:
    void loginReq(QString username,QString password);
    void registReq(QString username,QString password);
    void initSocketThread();
    void initHandleThread();
public slots:
    void socketHandle();
    // void socketError(QAbstractSocket::SocketError);
    void loginSuccess();
    void loginError();
    void showRegistRet(int ret);

private slots:

    void on_login_clicked();

    void on_regist_clicked();

    void on_logout_clicked();

private:
    loginAndRegist(QWidget *parent = nullptr);//定义私有构造函数，只在类内部访问
    loginAndRegist(const loginAndRegist&) = delete; //删除拷贝构造
    loginAndRegist& operator=(const loginAndRegist&) = delete; //删除=号赋值语句
    static loginAndRegist *m_pClientWin;//静态全局变量
    Ui::clientWin *ui;
    QTimer* uploadTimer;
    void setTiltleFromLang();
};
#endif // CLIENTWIN_H
