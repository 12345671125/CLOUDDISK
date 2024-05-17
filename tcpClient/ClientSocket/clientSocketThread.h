/*主界面类，用于客户端初始化*/

#ifndef CLIENTWIN_H
#define CLIENTWIN_H
#pragma once
#include <QMainWindow>
#include <QFile> //文件类用于静态读取文件
#include <QMessageBox>
#include <QHostAddress>
#include <QTimer>
#include <QThread>
#include "PDU/protocol.h"

 QT_BEGIN_NAMESPACE
 namespace protocol { struct PDU;};
 QT_END_NAMESPACE


class OpeWidget;
class QTcpSocket;
\

class clientSocketThread : public QObject
{
    Q_OBJECT

public:
    clientSocketThread(QObject *parent = nullptr);
    ~clientSocketThread();
public slots:
    void recvMsg();
    void initTcpSocket();
    void sendLoginReq(QString username,QString password);
    void sendRegistReq(QString username,QString password);
    void sendAllOnlineReq();
    void sendSearchUserReq(QString username);
    void sendAddFriAgreeReq(QString username);
    void sendAddFriRefuseReq(QString username);
    void sendFlushFileReq(QString curPath);
    void sendFlushPublicFileReq(QString curPath);
    void sendPublicChatReq(QString strUsername,QString msg);
    void sendPrivateChatReq(QString strMyName,QString strOtherName,QString msg);
    void sendCreateDirReq(QString strNewDir,QString strName,QString strCurPath);
    void sendGoBackReq(QString strCurPath);
    void sendSwitchReq(QString strCurPath);
    void sendRenameFileReq(QString strNewFName,QString strCurFName);
    void sendDeleteFileReq(QString strFName,QString strCurPath);
    void sendDeleteDirReq(QString strDirName,QString strCurPath);
    void sendFlushFriendReq(QString strUsername);
    void sendDeleteFriendReq(QString strFriendName,QString strUserName);
    void sendUploadFileReq(QString absoluteFilePath);
    void sendUploadPublicFileReq(QString absoluteFilePath);
    void sendDownLoadFileReq(QString strTargetFileName,QString CurPath);
    void sendGetFileInfo(QString strFileName,QString strUserName);
    void SendRequestToGetPublicFilePath(); //获取公共文件路径
    void sendDownLoadPublicFileReq(QString strTargetFileName,QString CurPath);
    void sendAddFriReq(QString strPerUserName ,QString strLoginName);
signals:
    void m_addFriendRes(protocol::PDU* resPdu);
    void m_sLocalPathRes(protocol::PDU* resPdu);
    void m_sPublicFilePathRes(protocol::PDU* resPdu);
    void m_sRegistRes(protocol::PDU* resPdu);
    void m_sLoginRes(protocol::PDU* resPdu);
    void m_sAllOnlineRes(protocol::PDU* resPdu);
    void m_sSearchUserRes(protocol::PDU* resPdu);
    void m_sAddFriendReq(protocol::PDU* resPdu);
    void m_sAddFriendRes(protocol::PDU* resPdu);
    void m_sAddFriendResRet(protocol::PDU* resPdu);
    void m_sFlushFriendRes(protocol::PDU* resPdu);
    void m_sPrivateChatTrans(protocol::PDU* resPdu);
    void m_sPublicChatTrans(protocol::PDU* resPdu);
    void m_sCreateDirRes(protocol::PDU* resPdu);
    void m_sFlushFileRes(protocol::PDU* resPdu);
    void m_sDeleteFileRes(protocol::PDU* resPdu);
    void m_sDeleteDirRes(protocol::PDU* resPdu);
    void m_sRenameFileRes(protocol::PDU* resPdu);
    void m_sUploadFileFinRes(protocol::PDU* resPdu);
    void m_sGetFileInfoRes(protocol::PDU* resPdu);
    void m_sErrorRes(protocol::PDU* resPdu);
    void m_sOpenUploadAndDownLoadPage();
    void m_sUploadFileInfoGetRes(protocol::PDU* resPdu);
    void m_sDownloadFileRes(protocol::PDU*resPdu);
    void m_sUploadPublicFileInfoGetRes(protocol::PDU*resPdu);
    void m_sFlushPublicFileRes(protocol::PDU*resPdu);
    void m_sDownloadPublicFileRes(protocol::PDU*resPdu);

public slots:


private:
    void connectToSlot();
    QTimer* uploadTimer;
    QTcpSocket* m_TcpSocket;
    void delFile(protocol::PDU pdu);
    void uploadFile(protocol::PDU pdu);
    void SendRequestToGetServerLocalPath(); //获取服务器路径
};
#endif // CLIENTWIN_H
