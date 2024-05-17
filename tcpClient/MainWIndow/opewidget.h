#ifndef OPEWIDGET_H
#define OPEWIDGET_H
#pragma once

#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <PDU/protocol.h>
class filePage;
class Friend;
class publicFilePage;
class About;

QT_BEGIN_NAMESPACE
namespace protocol { class PDU;};
QT_END_NAMESPACE

class OpeWidget : public QWidget
{
    Q_OBJECT
public:
    static OpeWidget* getinstance(QWidget *parent = nullptr);
    Friend *getFriend();
    filePage *getfilePage();
    publicFilePage* getPublicFilePage();
    About * getAbout();
    QTimer* myTimer;
    void setStyle(QString style);
public slots:
    void AllOnlineRes(protocol::PDU* pdu);
    void SearchUserRes(protocol::PDU* pdu);
    void addFriend(protocol::PDU* pdu);
    void createDir(protocol::PDU* pdu);
    void FlushFile(protocol::PDU* pdu);
    void ShowAddFriendRet(protocol::PDU* pdu);
    void FlushFriend(protocol::PDU* pdu);
    void ShowPrivateChat(protocol::PDU* pdu);
    void ShowPublicChat(protocol::PDU* pdu);
    void ShowDelDirRet(protocol::PDU* pdu);
    void ShowDelFileRet(protocol::PDU* pdu);
    void ShowRenameFileRet(protocol::PDU* pdu);
    void ShowFileInfoRet(protocol::PDU* pdu);
    void ShowUploadFileInfoRet(protocol::PDU* pdu);
    void OpenUploadAndDownLoadPage();
    void HandleDownloadFile(protocol::PDU* pdu);
    void ShowFileInfo(protocol::PDU*pdu);
    void ShowUploadPublicFileInfoRet(protocol::PDU*pdu);
    void FlushPublicFile(protocol::PDU*pdu);
    void HandleDownloadPublicFile(protocol::PDU*pdu);
signals:

private:
    OpeWidget(QWidget *parent = nullptr);
    OpeWidget(const OpeWidget&) = delete;
    OpeWidget& operator=(const OpeWidget&) = delete;
    static OpeWidget* Opewidget;
    QListWidget *m_pListW;
    Friend* m_pFriend;
    publicFilePage* m_pPublicFilePage;
    About* m_pAbout;
    QStackedWidget *m_pSW;
    filePage* m_pFilePage;
    QListWidgetItem* PublicFileItem;
    QListWidgetItem* FriendItem;
    QListWidgetItem* FileItem;
    QListWidgetItem* AboutItem;
private slots:
    void listenTransmit();
};

#endif // OPEWIDGET_H
