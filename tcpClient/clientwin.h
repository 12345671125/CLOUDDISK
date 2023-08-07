/*主界面类，用于客户端初始化*/


#ifndef CLIENTWIN_H
#define CLIENTWIN_H
#pragma once
#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QFile> //文件类用于静态读取文件
#include <QMessageBox>
#include <QHostAddress>
#include "protocol.h"
#include <QCryptographicHash> //qt内置加密库
#include "opewidget.h"
#include <QTimer>

//#define _file_ 1
//#define _msgText_ 0
//#define _other_ 2

QT_BEGIN_NAMESPACE
namespace Ui { class clientWin; }
QT_END_NAMESPACE

class clientWin : public QMainWindow
{
    Q_OBJECT

public:
    clientWin(QWidget *parent = nullptr);
    static clientWin& getInstance();
    QTcpSocket& getTcpSocket();
    QString getLoginName();
    QString curPath();
    QString getServerIp();
    quint16 getServerPort();
    void modifyCurPath(QString str);
    ~clientWin();
public slots:
    void showConnected();
    void recvMsg();
private slots:
//    void on_sendButton_clicked();

    void on_login_clicked();

    void on_regist_clicked();

    void on_logout_clicked();

private:
    Ui::clientWin *ui;
    QString ip;//用于存放服务器ip
    qint16 port; //用于存放服务器端口
    QTcpSocket clientSocket;//客户端socket对象
    QString m_strLoginName;
    QString m_strCurPath;
    QTimer* uploadTimer;

    void initconfig(); //初始化服务器配置
    void connectToServer();
    void showPrivateMsg(protocol::PDU* pdu);
    void showPublicMsg(protocol::PDU* pdu);
    void showCreateDir(protocol::PDU* pdu);
    void flushFile(protocol::PDU* pdu);
    void delDir(protocol::PDU* pdu);
    void delFile(protocol::PDU* pdu);
    void renameFile(protocol::PDU* pdu);
    void uploadFile(protocol::PDU* pdu);
};
#endif // CLIENTWIN_H
