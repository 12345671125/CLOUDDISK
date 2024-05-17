/*主界面类，用于客户端初始化*/

#ifndef RESHANDER
#define RESHANDER
#pragma once
#include <QThread>
#include <PDU/protocol.h>

QT_BEGIN_NAMESPACE
namespace protocol { class PDU;};
QT_END_NAMESPACE


class OpeWidget;

class responseHandler : public QObject
{
    Q_OBJECT

public:
    static responseHandler &getInstance(QObject *parent = nullptr);
    ~responseHandler();
signals:
    void m_sRegistRet(int ret);
    void m_sLoginSuccess();
    void m_sLoginError();
    void m_sAllOnline(protocol::PDU* pdu);
    void m_sSearchUser(protocol::PDU* pdu);
    void m_sAddFriend(protocol::PDU* pdu);
    void m_sCreateDir(protocol::PDU* pdu);
    void m_sFlushFile(protocol::PDU* pdu);
    void m_sShowAddFriendRet(protocol::PDU* pdu);
    void m_sFlushFriend(protocol::PDU* pdu);
    void m_sPrivateChatTrans(protocol::PDU* pdu);
    void m_sPublicChatTrans(protocol::PDU* pdu);
    void m_sDelDir(protocol::PDU* pdu);
    void m_sDelFile(protocol::PDU* pdu);
    void m_sRenameFile(protocol::PDU* pdu);
    void m_sShowFileInfo(protocol::PDU* pdu);
    void m_sShowUploadFileInfoRes(protocol::PDU* pdu);
    void m_sOpenUploadAndDownLoadPage();
    void m_sHandleDownloadFile(protocol::PDU* pdu);
    void m_sShowUploadPublicFileInfoRes(protocol::PDU* pdu);
    void m_sFlushPublicFile(protocol::PDU* pdu);
    void m_sHandleDownloadPublicFile(protocol::PDU* pdu);
public slots:
    void initHandle();
    void handleShowAddFriendRet(protocol::PDU* resPdu);
    void handleLoginPathRes(protocol::PDU* resPdu);
    void handlePublieFilePathRes(protocol::PDU* resPdu);
    void handleRegistRes(protocol::PDU* resPdu);
    void handleLoginRes(protocol::PDU* resPdu);
    void handleAllOnlineRes(protocol::PDU* resPdu);
    void handSearchUserRes(protocol::PDU* resPdu);
    void handleAddFriendReq(protocol::PDU* resPdu);
    void handAddFriendRes(protocol::PDU* resPdu);
    void handleFlushFriendRes(protocol::PDU* resPdu);
    void handlePrivateChatTrans(protocol::PDU* resPdu);
    void handlePublicChatTrans(protocol::PDU* resPdu);
    void handleCreateDirRes(protocol::PDU* resPdu);
    void handleFlushFileRes(protocol::PDU* resPdu);
    void handleDeleteFileRes(protocol::PDU* resPdu);
    void handleDeleteDirRes(protocol::PDU* resPdu);
    void handleRenameFileRes(protocol::PDU* resPdu);
    void handleUploadFileFinRes(protocol::PDU* resPdu);
    void handleGetFileInfoRes(protocol::PDU* resPdu);
    void handleErrorRes(protocol::PDU* resPdu);
    void handleGetUploadFileInfoRes(protocol::PDU* resPdu);
    void handleOpenUploadAndDownLoadPage();
    void handleDownloadFile(protocol::PDU* resPdu);
    void handleGetUploadPublicFileInfoRes(protocol::PDU* resPdu);
    void handleFlushPublicFileRes(protocol::PDU* resPdu);
    void handleDownloadPublicFile(protocol::PDU* resPdu);

private:
        responseHandler(QObject *parent = nullptr);
        responseHandler(const responseHandler&) = delete;
        responseHandler& operator=(const responseHandler&) = delete;
};
#endif // RESHANDER
