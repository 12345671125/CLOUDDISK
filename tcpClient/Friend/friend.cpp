/*好友界面类*/
#pragma execution_character_set("utf-8")
#include "friend.h"
#include "PDU/protocol.h"
#include "clientwin.h"
#include "privatechat.h"
#include "Friend/online.h"
#include "MainWIndow/opewidget.h"
#include <QMenu>
#include <QDebug>
#include "../style.h"
#include "publicchatwidget.h"

Friend::Friend(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground);


    this->m_pFriendListWidget = new QListWidget;

    this->m_PublicChatWidget = &PublicChatWidget::getInstance(this);

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
    this->m_pFriendListWidget->setMaximumSize(this->width()*0.35,this->height());
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

    this->flushFriends();


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

Friend::~Friend()
{
    this->m_Timer->stop();
    delete this->m_Timer;
}

void Friend::showAllOnlineUser(protocol::PDU* pdu)
{
    if(pdu == NULL) return;
    this->online->showUser(pdu);

}

void Friend::showSearchUser(protocol::PDU *pdu)
{
    if(pdu == NULL) return;
    this->online->showSearchUser(pdu);
}

void Friend::updateFriend(protocol::PDU *pdu)
{
    this->m_pFriendListWidget->clear(); //清空现有好友队列
    if(NULL == pdu){
        return;
    }
    uint uiSize = pdu->uiMsgLen / 64;
    char caName[64] = {'\0'};
    for(uint i = 0;i<uiSize;i++){
        memcpy(caName,((char*)pdu->caMsg)+(i*64),64);
        QString str = QString::fromLocal8Bit(caName,64);
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
}

void Friend::showOnline()
{
    this->m_pSearchUserPB->setDisabled(false);
    if(online -> isHidden()){
         online->show();
        protocol::PDU pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_ALL_ONLINE_REQUEST,"在线用户请求");
         clientWin::getInstance().getTcpSocket().write((char*)&pdu,pdu.PDULen);
    }
    else
         online->hide();
}

void Friend::searchUser()
{
    QString userName  = QInputDialog::getText(this,"搜索","用户名:");
    if(!userName.isEmpty())
    {
//         qDebug() << userName;
         protocol::PDU  pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_SEARCHUSER_REQUEST,userName.toStdString().c_str());
         clientWin::getInstance().getTcpSocket().write((char*)&pdu,pdu.PDULen);
    }
}

void Friend::flushFriends()
{
    protocol::PDU pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST,"");
    memcpy(pdu.caData,clientWin::getInstance().getLoginName().toStdString().c_str(),64);
    clientWin::getInstance().getTcpSocket().write((char*)&pdu,pdu.PDULen);
}

void Friend::deleteFriend()
{
    QString curName = OpeWidget::getinstance().getFriend()->m_pFriendListWidget->currentItem()->text(); //获取当前选中的好友用户名
    curName = curName.mid(0,curName.length()-5);
    if(QMessageBox::information(this,"删除好友","确定要删除好友"+curName+"?",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes){
    /*向服务器发送删除好友请求*/
         protocol::PDU pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST,"");
    memcpy(pdu.caData,curName.toStdString().c_str(),64);
    memcpy(pdu.caData+64,clientWin::getInstance().getLoginName().toStdString().c_str(),64);
    clientWin::getInstance().getTcpSocket().write((char*)&pdu,pdu.PDULen);
    this->flushFriends();
}



}

void Friend::privateChat()
{
    if(!this->m_pFriendListWidget->currentItem() || this->m_pFriendListWidget->count() == 0){
        return;
    }
    PrivateChat::getInstance().setChatName(this->m_pFriendListWidget->currentItem()->text());
//    qDebug()<<this->m_pFriendListWidget->currentItem()->text();
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
