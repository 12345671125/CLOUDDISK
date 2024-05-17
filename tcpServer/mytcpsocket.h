/*myTcpSocket类用于处理并响应客户端的请求*/

#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#pragma once
#include <QTcpSocket>
#include <QDebug>
#include "protocol.h"
#include "opedb.h"
#include <QFileInfoList>
#include <QFile>
#include <QVector>
#include <QTimer>



class myTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    myTcpSocket();
    QString getName();
    void handleGetPublicFilePath(protocol::PDU* pdu);
    void login(protocol::PDU* pdu);

    void showOnline(protocol::PDU* pdu);

    void searchUser(protocol::PDU* pdu);

    void addFriends(protocol::PDU* pdu);

    void handleFriRespond(protocol::PDU* pdu,int type);

    void handleFlushFriends(protocol::PDU* pdu);

    void handleDelFriend(protocol::PDU* pdu);

    void handlePrivateChat(protocol::PDU* pdu);

    void requestFault(protocol::PDU* pdu);

    void handlePublicChat(protocol::PDU* pdu);

    void handleCreateDir(protocol::PDU* pdu);

    void handleFlushFile(protocol::PDU* pdu);

    void handleFlushPublicFile(protocol::PDU*pdu);

    void handleDelDir(protocol::PDU* pdu);

    void handleDelFile(protocol::PDU* pdu,int mode);

    void handleReNameFile(protocol::PDU* pdu);

    void handleUploadFile(protocol::PDU* pdu);

    void handleGetUploadFileData(protocol::PDU* pdu);

    void handleUploadFileFin(protocol::PDU* pdu);

    void handleUploadFileInfo(protocol::PDU* pdu);

    void handleUploadPublicFileInfo(protocol::PDU* pdu);

    void handleGetFileInfo(protocol::PDU* pdu);

    void handleDownFile(protocol::PDU* pdu);
    void handleGetLocalPath(protocol::PDU* pdu);
    void ReadytoSendFile(protocol::PDU* pdu);
    void handleDownloadingFile(protocol::PDU* pdu);
    void handlePublicDownFile(protocol::PDU*pdu);

//    void ReadytoSendFile(protocol::PDU* pdu);


public slots:
    void recvMsg();//用于处理请求
    void regist(protocol::PDU* pdu);//用于处理注册
    void clientOffine();
    void sendFileData(int ClientCurSize);
    void sendFileEnd();
    //    void respond(PDU* pdu); //用于回复

    //    void handleGetFOnlineStatus(PDU * pdu);
    //    void logout();
signals:
    void offline(myTcpSocket* mySocket);
private:
    QString m_strName;
    QFile* file = NULL;
    QString curPath;
    QFile* downloadFile;
    //QTimer* downloadTimer;
    //QTimer* downEndTimer;
    qint64 totalSize;
    qint64 curSize;
    void removeNullCharacters(QString &str);
    int getTcpDataSize(const QString sourceData);
};

#endif // MYTCPSOCKET_H
