/*myTcpServer类用于生成tcpServer并且监听并接受新的客户端请求*/


#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
#pragma once
#include <QTcpServer>
#include <QThread>
#include "mytcpsocket.h"
#include <QList> //引入QList 用来存放socket
class myTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    myTcpServer();

    static myTcpServer& getInstance(); //单例模式
    void incomingConnection(qintptr handle);
    void FResend(const char* pername,protocol::PDU *pdu);
    void MsgResend(const char* pername,protocol::PDU* pdu);
    void MsgResend(const QStringList strList,protocol::PDU* pdu);
public slots:
    void deleteSocket(myTcpSocket* mySocket);
private:
    QList<myTcpSocket*> socketList; //模板类，socketList中的每一个元素时一个myTcpSocket类的实例化对象的指针,每一个myTcpSocket对应一个客户端
};

#endif // MYTCPSERVER_H
