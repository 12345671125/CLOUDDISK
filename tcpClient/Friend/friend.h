/*好友界面类*/


#ifndef FRIEND_H
#define FRIEND_H
#pragma once
#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace protocol { class PDU;};
class PublicChatWidget;
QT_END_NAMESPACE

class Online;


class Friend : public QWidget
{
    Q_OBJECT
public:
    static Friend* getInsance(QWidget *parent);
    ~Friend();
    void showAllOnlineUser(protocol::PDU* pdu);
    void showSearchUser(protocol::PDU* pdu);
    void updateFriend(protocol::PDU* pdu);
    void addFriend(protocol::PDU* pdu);
    void showAddFriendRet(protocol::PDU* pdu);
    void showPrivateChat(protocol::PDU* pdu);
    void showPublicChat(protocol::PDU* pdu);


signals:
    void sendAllOnline();
    void sendSearchUser(QString username);
    void sendAddFriAgree(QString username); //同意添加好友信号
    void sendAddFriRefuse(QString username);//拒绝添加好友信号
    void sendFlushFriend(QString strUsername);
    void sendDeleteFriend(QString strFriendName,QString strUserName);
protected:
    bool event(QEvent *event) override;
public slots:
    void showOnline();//显示在线用户槽函数
    void searchUser(); //查找用户槽函数
    void flushFriends(); //刷新好友列表
    void deleteFriend(); //删除好友
    void privateChat(); //私聊
    void setStyle(QString style);
private:
    Friend(QWidget *parent = nullptr);
    Friend(const Friend&) = delete;
    Friend& operator=(const Friend&) = delete;
    static Friend *m_pFriendPage;
//    QTextEdit* m_pShowMsgTE;
    QListWidget* m_pFriendListWidget;
//    QLineEdit* m_pInputMsgLE;
    QPushButton* m_pDelFriendPB;
    QPushButton* m_pFlushFriendPB;
    QPushButton* m_pShowOnlineUserPB;
    QPushButton* m_pSearchUserPB;
//    QPushButton* m_pMsgSendPB;
    QPushButton* m_pPrivateChatPB;
    PublicChatWidget* m_PublicChatWidget;


    QTimer* m_Timer; //这个定时器用与刷新好友

    Online* online;
private slots:
        void widgetListRequested(const QPoint &pos);
//    void getFOnlineStatus(); //获取好友在线状态
};

#endif // FRIEND_H
