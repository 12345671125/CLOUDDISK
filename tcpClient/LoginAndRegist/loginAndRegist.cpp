/*主界面类，用于客户端初始化*/

#include "PDU/protocol.h"
#include "loginAndRegist.h"
#include "ui_clientwin.h"
#include "../commonfunc.h"
#include "../commondata.h"
#include <QTcpSocket>
#include "mymain.h"
#include <QThread>
#include "clientSocketThread.h"
#include "ResponseHandler/responseHandler.h"

loginAndRegist* loginAndRegist::m_pClientWin = NULL;

loginAndRegist::loginAndRegist(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::clientWin)
    , uploadTimer(new QTimer())
{
        clientSocketThread* pClientSocketThread =reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
        responseHandler* pResponseHandler = reinterpret_cast<responseHandler*>(CommonFunc::QueryData(RESPONSEHANDLE));
        connect(this,SIGNAL(initSocketThread()),pClientSocketThread,SLOT(initTcpSocket()));
        connect(this,SIGNAL(initHandleThread()),pResponseHandler,SLOT(initHandle()));
        ui->setupUi(this);
        this->uploadTimer->setInterval(500);
        setTiltleFromLang();
        this->setWindowIcon(QIcon(QPixmap(":/img/logo.png")));
}

loginAndRegist *loginAndRegist::getInstance()
{
    if(m_pClientWin == NULL)
    {
        m_pClientWin = new loginAndRegist();
    }
    return m_pClientWin;
}

loginAndRegist::~loginAndRegist()
{
    delete ui;
    delete this->uploadTimer;
}

void loginAndRegist::socketHandle()
{
    // QMessageBox::information(NULL,"连接服务器","连接服务器成功!");
}

// void loginAndRegist::socketError(QAbstractSocket::SocketError)
// {
//     QMessageBox::information(this,"连接服务器","连接服务器失败!");
//     QApplication::exit();
// }

void loginAndRegist::loginSuccess()
{
    QMessageBox::information(this,"登录","登录成功!");
    myMain::getInstance()->show();
    this->hide();
}

void loginAndRegist::loginError()
{
    QMessageBox::information(this,"登录","登录失败!用户名密码错误，或者重复登录。");
}

void loginAndRegist::showRegistRet(int ret)
{
    if(ret == 0)
    {
        QMessageBox::information(this,"注册","注册成功");
    }else
    {
        QMessageBox::information(this,"注册","注册失败，用户名重复");
    }
}

void loginAndRegist::on_login_clicked()
{
    if(!this->ui->usernameInput->text().isEmpty() && !this->ui->passwordInput->text().isEmpty()){
        QString username = this->ui->usernameInput->text();
        QString password = this->ui->passwordInput->text();
        if(username.length() > 20 || password.length()>20){
            QString strRegust = {};
            QString registErr = {};
            CommonFunc::readLangFromFile("strRegust",strRegust);
            CommonFunc::readLangFromFile("registErr",registErr);
            QMessageBox::information(this,strRegust,registErr);
        }else{
            qDebug() << "Current thread:" << QThread::currentThreadId();
            clientSocketThread* pClientSocket = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
            QObject::connect(this,SIGNAL(loginReq(QString,QString)),pClientSocket,SLOT(sendLoginReq(QString,QString)));
            emit loginReq(username,password);
            QObject::disconnect(this,SIGNAL(loginReq(QString,QString)),pClientSocket,SLOT(sendLoginReq(QString,QString)));

        }

    }else{
        QString strErr;
        strErr.clear();
        if(!CommonFunc::readErrorFromFile("passwordAndusernameErr",strErr))
        {
            strErr = "PASSWORD AND USERNAME Err!";
        }
        QMessageBox::critical(this,"ERROR",strErr);
    }
}

void loginAndRegist::on_regist_clicked()
{
    if(!this->ui->usernameInput->text().isEmpty() && !this->ui->passwordInput->text().isEmpty()){
        QString username = this->ui->usernameInput->text();
        QString password = this->ui->passwordInput->text();
        if(username.length() > 20 || password.length()>20){
            QMessageBox::information(this,"命名规则","用户名和密码的长度不能超过20个字符!");
        }else{
         clientSocketThread* pClientSocket = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
        QObject::connect(this,SIGNAL(registReq(QString,QString)),pClientSocket,SLOT(sendRegistReq(QString,QString)));
        emit registReq(username,password);
        QObject::disconnect(this,SIGNAL(registReq(QString,QString)),pClientSocket,SLOT(sendRegistReq(QString,QString)));
        }
    }else{
        QMessageBox::critical(this,"ERROR","用户名或密码不能为空!");
    }

}

void loginAndRegist::setTiltleFromLang()
{
    QString strUsername = {};
    CommonFunc::readLangFromFile("strUsername",strUsername);
    this->ui->label->setText(strUsername);
    QString strPassword = {};
    CommonFunc::readLangFromFile("strPassword",strPassword);
    this->ui->label_2->setText(strPassword);
    QString strLogin = {};
    CommonFunc::readLangFromFile("strLogin",strLogin);
    this->ui->login->setText(strLogin);
    QString strRegist = {};
    CommonFunc::readLangFromFile("strRegist",strRegist);
    this->ui->regist->setText(strRegist);
    QString strLogout = {};
    CommonFunc::readLangFromFile("strLogout",strLogout);
    this->ui->logout->setText(strLogout);
}

void loginAndRegist::on_logout_clicked()
{

}
