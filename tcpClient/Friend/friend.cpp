/*好友界面类*/
#pragma execution_character_set("utf-8")
#include "friend.h"
#include "PDU/protocol.h"
#include "privatechat.h"
#include "Friend/online.h"
#include "MainWIndow/opewidget.h"
#include <QMenu>
#include <QDebug>
#include "../style.h"
#include "publicchatwidget.h"

#include "../commonfunc.h"
#include "../commondata.h"
#include<QTcpSocket>
#include <QMessageBox>
#include "ClientSocket/clientSocketThread.h"
#include <QEvent>

Friend* Friend::m_pFriendPage = nullptr;

Friend::Friend(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground);


    this->m_pFriendListWidget = new QListWidget;

    this->m_PublicChatWidget = &PublicChatWidget::getInstance();

    this->m_pDelFriendPB = new QPushButton("删除");
    this->m_pFlushFriendPB = new QPushButton("刷新");
    this->m_pShowOnlineUserPB = new QPushButton("在线用户");
    this->m_pSearchUserPB = new QPushButton("查询用户");

    this->m_pPrivateChatPB = new QPushButton("私聊");
    this->m_pSearchUserPB->setDisabled(true);

    QVBoxLayout *pLeftBL = new QVBoxLayout;
    pLeftBL->addWidget(m_pDelFriendPB);
    pLeftBL->addWidget(m_pFlushFriendPB);
    pLeftBL->addWidget(m_pShowOnlineUserPB);
    pLeftBL->addWidget(m_pSearchUserPB);
    pLeftBL->addWidget(m_pPrivateChatPB);

    pLeftBL->setContentsMargins(0,0,5,0);
    pLeftBL->setSpacing(0);
    QHBoxLayout* pTopHBL = new QHBoxLayout;
    pTopHBL->addWidget(this->m_PublicChatWidget);
    pTopHBL->addWidget(m_pFriendListWidget);
    pTopHBL->addLayout(pLeftBL);

    pTopHBL->setContentsMargins(0,10,0,20);


    QVBoxLayout* pMain = new QVBoxLayout;
    pMain->addLayout(pTopHBL);

    pMain->setContentsMargins(0,0,0,0);
    online = new Online;
    pMain->addWidget(online);
    online->hide();
    setLayout(pMain);



/*设置刷新好友定时器*/
    this->m_Timer = new QTimer(this);
    m_Timer->setInterval(10000); //设置刷新定时器的时间间隔为10秒
//    m_Timer->start();

    QObject::connect(m_pShowOnlineUserPB,SIGNAL(clicked(bool)),this,SLOT(showOnline()));
    QObject::connect(m_pSearchUserPB,SIGNAL(clicked(bool)),this,SLOT(searchUser()));

    QObject::connect(m_Timer,SIGNAL(timeout()),this,SLOT(flushFriends()));
    QObject::connect(m_pFlushFriendPB,SIGNAL(clicked(bool)),this,SLOT(flushFriends()));

    QObject::connect(m_pDelFriendPB,SIGNAL(clicked(bool)),this,SLOT(deleteFriend()));
    QObject::connect(m_pPrivateChatPB,SIGNAL(clicked(bool)),this,SLOT(privateChat()));

    QObject::connect(m_pFriendListWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(widgetListRequested(QPoint)));


    this->m_pFriendListWidget->setContextMenuPolicy(Qt::CustomContextMenu); //添加菜单策略

    this->setStyle(m_friendPage_Light_style);
}

Friend *Friend::getInsance(QWidget *parent)
{
    if(m_pFriendPage == nullptr)
    {
        m_pFriendPage = new Friend(parent);
    }
    return m_pFriendPage;
}

Friend::~Friend()
{
    this->m_Timer->stop();
    delete this->m_Timer;
}

void Friend::showAllOnlineUser(protocol::PDU* pdu)
{
    this->online->showUser(pdu);

}

void Friend::showSearchUser(protocol::PDU* pdu)
{
    this->online->showSearchUser(pdu);
}

void Friend::updateFriend(protocol::PDU* pdu)
{
    this->m_pFriendListWidget->clear(); //清空现有好友队列

    uint uiSize = pdu->uiMsgLen / 64;
    char caName[64] = {'\0'};
    for(uint i = 0;i<uiSize;i++){
        memcpy(caName,((char*)pdu->caMsg)+(i*64),64);
        QString str = QString::fromUtf8(caName,64);
        CommonFunc::removeNullCharacters(str);
        qDebug()<<str;
        QChar status = str.at(str.length()-1);
        str  = str.mid(0,str.length()-2);
        if(status == '1'){
            str.append(" : 在线");
            qDebug()<<str;
            this->m_pFriendListWidget->addItem(str);
        }else if(status == '0'){
            str.append(" : 离线");
             qDebug()<<str;
            this->m_pFriendListWidget->addItem(str);
        }
//        qDebug()<<caName;
    }
    free(pdu);
    pdu = NULL;
}

void Friend::addFriend(protocol::PDU* pdu)
{
    char username[64] = { }; //创建username数组用于存放用户名
    memset(username,0,sizeof(username));
    memcpy_s(username,64,pdu->caData+64,64);//从pdu.cadata中读取username
    int ret = QMessageBox::information(NULL,"好友请求",QString("%1 想要添加你为好友").arg(username),QMessageBox::Yes,QMessageBox::No);
    QString strUsername = QString::fromUtf8(username);
    if(ret == QMessageBox::Yes)
    {
        clientSocketThread* m_clientSocketThread = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
        QObject::connect(this,SIGNAL(sendAddFriAgree(QString)),m_clientSocketThread,SLOT(sendAddFriAgreeReq(QString)));
        emit sendAddFriAgree(strUsername);
        QObject::disconnect(this,SIGNAL(sendAddFriAgree(QString)),m_clientSocketThread,SLOT(sendAddFriAgreeReq(QString)));
    }else
    {
        clientSocketThread* m_clientSocketThread = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
        QObject::connect(this,SIGNAL(sendAddFriRefuse(QString)),m_clientSocketThread,SLOT(sendAddFriRefuseReq(QString)));
        emit sendAddFriRefuse(strUsername);
        QObject::disconnect(this,SIGNAL(sendAddFriRefuse(QString)),m_clientSocketThread,SLOT(sendAddFriRefuseReq(QString)));
    }
    free(pdu);
    pdu = NULL;
}

void Friend::showAddFriendRet(protocol::PDU* pdu)
{
    QMessageBox::information(NULL,"添加好友",pdu->caData);
    free(pdu);
    pdu = NULL;
}

void Friend::showPrivateChat(protocol::PDU* pdu)
{
        qDebug()<<"showPrivateMsg";
        char username[64] = {"\0"};
        char pername[64] = {"\0"};
        memcpy(username,pdu->caData,64);
        memcpy(pername,pdu->caData+64,64); //解析协议中的私聊对象名
        QString uname = username;
        if(PrivateChat::getInstance().isHidden()||!PrivateChat::getInstance().isActiveWindow() || !PrivateChat::getInstance().isVisible()){
            int result =  QMessageBox::information(NULL,"新消息","你有一条来自"+uname+"的新消息,是否打开聊天框？",QMessageBox::Yes,QMessageBox::No);
            if(result == QMessageBox::Yes){
                PrivateChat::getInstance().setChatName(uname.append("     "));//向后填充5个空格
                qDebug()<<uname;
                PrivateChat::getInstance().show();
                PrivateChat::getInstance().showMsg(pdu);

            }
        }else{
            PrivateChat::getInstance().showMsg(pdu);
        }
}

void Friend::showPublicChat(protocol::PDU* pdu)
{
    this->m_PublicChatWidget->showPublicChat(pdu);
}

bool Friend::event(QEvent *event)
{
    if(event->type() == QEvent::Show)
    {
        this->flushFriends();
    }
    return QWidget::event(event);
}

void Friend::showOnline()
{
    this->m_pSearchUserPB->setDisabled(false);
    if(online -> isHidden()){
         online->show();
         clientSocketThread* m_clientSocketThread = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
         QObject::connect(this,SIGNAL(sendAllOnline()),m_clientSocketThread,SLOT(sendAllOnlineReq()));
         emit sendAllOnline();
         QObject::disconnect(this,SIGNAL(sendAllOnline()),m_clientSocketThread,SLOT(sendAllOnlineReq()));
    }
    else
         online->hide();

}

void Friend::searchUser()
{
    QString userName  = QInputDialog::getText(this,"搜索","用户名:");
    if(!userName.isEmpty())
    {
        clientSocketThread* m_clientSocketThread = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
        QObject::connect(this,SIGNAL(sendSearchUser(QString)),m_clientSocketThread,SLOT(sendSearchUserReq(QString)));
        emit sendSearchUser(userName); //发送搜索用户信号
        QObject::disconnect(this,SIGNAL(sendSearchUser(QString)),m_clientSocketThread,SLOT(sendSearchUserReq(QString)));
    }
}

void Friend::flushFriends()
{
    QString strUserName = *reinterpret_cast<QString*>(CommonFunc::QueryData(USERNAME));

    clientSocketThread* m_clientSocketThread = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
    QObject::connect(this,SIGNAL(sendFlushFriend(QString)),m_clientSocketThread,SLOT(sendFlushFriendReq(QString)));
    emit sendFlushFriend(strUserName);
    QObject::disconnect(this,SIGNAL(sendFlushFriend(QString)),m_clientSocketThread,SLOT(sendFlushFriendReq(QString)));
}

void Friend::deleteFriend()
{
    QString strFriendName = OpeWidget::getinstance()->getFriend()->m_pFriendListWidget->currentItem()->text(); //获取当前选中的好友用户名
    QString strUserName = *reinterpret_cast<QString*>(CommonFunc::QueryData(USERNAME));
    if(QMessageBox::information(this,"删除好友","确定要删除好友"+strFriendName+"?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes){
    /*向服务器发送删除好友请求*/
        clientSocketThread* m_clientSocketThread = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
        QObject::connect(this,SIGNAL(sendDeleteFriend(QString,QString)),m_clientSocketThread,SLOT(sendDeleteFriendReq(QString, QString)));
        emit sendDeleteFriend(strFriendName,strUserName);
        QObject::disconnect(this,SIGNAL(sendDeleteFriend(QString,QString)),m_clientSocketThread,SLOT(sendDeleteFriendReq(QString, QString)));
    }



}

void Friend::privateChat()
{
    if(!this->m_pFriendListWidget->currentItem() || this->m_pFriendListWidget->count() == 0){
        return;
    }
    PrivateChat::getInstance().setChatName(this->m_pFriendListWidget->currentItem()->text());

    PrivateChat::getInstance().show();
}

void Friend::setStyle(QString style)
{
    this->style()->unpolish(this);
    this->setStyleSheet(style);
    this->style()->polish(this);
    this->update();
}



void Friend::widgetListRequested(const QPoint &pos)
{
    QMenu defaultMenuList =  QMenu(this);
    QAction flushFriend = QAction(QIcon(QPixmap(":/img/flush.png")),"刷新",this);
    QObject::connect(&flushFriend,SIGNAL(triggered()),this,SLOT(flushFriends()));
    defaultMenuList.addAction(&flushFriend);
    qDebug()<<m_pFriendListWidget->itemAt(mapToGlobal(QCursor::pos()));
    defaultMenuList.exec(QCursor::pos());

}

//void Friend::getFOnlineStatus()
//{
//    protocol::PDU pdu = protocol::PDU::default_request(ENUM_MSG_TYPE_FONLINE_STATUS_REQUEST,"");
//    memcpy(pdu.caData,clientWin::getInstance().getLoginName().toStdString().c_str(),64);
//    clientWin::getInstance().getTcpSocket().write((char*)&pdu,pdu.uiprotocol::PDULen);
//}
