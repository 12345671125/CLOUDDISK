/*好友界面类*/
#pragma execution_character_set("utf-8")
#include "friend.h"
#include "protocol.h"
#include "clientwin.h"
#include "privatechat.h"

Friend::Friend(QWidget *parent) : QWidget(parent)
{
    this->m_pShowMsgTE = new QTextEdit(this);
    this->m_pShowMsgTE->setReadOnly(true);
    this->m_pFriendListWidget = new QListWidget;
    this->m_pInputMsgLE = new QLineEdit;

    this->m_pDelFriendPB = new QPushButton("删除");
    this->m_pFlushFriendPB = new QPushButton("刷新");
    this->m_pShowOnlineUserPB = new QPushButton("显示在线用户");
    this->m_pSearchUserPB = new QPushButton("查询用户");
    this->m_pMsgSendPB = new QPushButton("信息发送");
    this->m_pPrivateChatPB = new QPushButton("私聊");
    this->m_pSearchUserPB->setDisabled(true);
    QVBoxLayout *pLeftBL = new QVBoxLayout;
    pLeftBL->addWidget(m_pDelFriendPB);
    pLeftBL->addWidget(m_pFlushFriendPB);
    pLeftBL->addWidget(m_pShowOnlineUserPB);
    pLeftBL->addWidget(m_pSearchUserPB);
    pLeftBL->addWidget(m_pPrivateChatPB);

    QHBoxLayout* pTopHBL = new QHBoxLayout;
    pTopHBL->addWidget(m_pShowMsgTE);
    pTopHBL->addWidget(m_pFriendListWidget);
    pTopHBL->addLayout(pLeftBL);

    QHBoxLayout* pMsgHBL = new QHBoxLayout;
    pMsgHBL->addWidget(m_pInputMsgLE);
    pMsgHBL->addWidget(m_pMsgSendPB);

    QVBoxLayout* pMain = new QVBoxLayout;
    pMain->addLayout(pTopHBL);
    pMain->addLayout(pMsgHBL);

    online = new Online;
    pMain->addWidget(online);
    online->hide();
    setLayout(pMain);

    this->flushFriends();
//    this->m_pPrivateChatPB->setDisabled(true);
//    if(this->m_pFriendListWidget->count()>0){
//        this->m_pFriendListWidget->setCurrentItem(0);
//    }

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

    QObject::connect(m_pMsgSendPB,SIGNAL(clicked(bool)),this,SLOT(publicChat())); //群聊
    QObject::connect(m_pFriendListWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(widgetListRequested(QPoint)));


    this->m_pFriendListWidget->setContextMenuPolicy(Qt::CustomContextMenu); //添加菜单策略
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


void Friend::showPublicChat(protocol::PDU *pdu)
{
    qDebug()<<"showPublicChat";
    if(pdu == nullptr) return;
    QString data;
    data.clear();
    char username[64] = {"\0"};
    memcpy(username,pdu->caData,64);
    data = QString::fromLocal8Bit(username,64);
    data.append(" : ").append(QString::fromLocal8Bit((char*)pdu->caMsg,pdu->uiMsgLen)).append("\r\n");
    this->m_pShowMsgTE->append(data);

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

void Friend::publicChat()
{
    QString msg = this->m_pInputMsgLE->text();//获取文本框输入内容
    if(!msg.isEmpty()){
        QString data;
        data.clear();
        data = clientWin::getInstance().getLoginName();
        data.append(" : ").append(msg).append("\r\n");
        this->m_pShowMsgTE->append(data);
        this->m_pInputMsgLE->clear();
        protocol::PDU*pdu = protocol::createPDU(msg.length());/*生成协议*/
        memcpy(pdu->caData,clientWin::getInstance().getLoginName().toStdString().c_str(),64);
        pdu->uiMsgType = protocol::ENUM_MSG_TYPE_PUBLIC_CHAT_REQUEST;
//        QByteArray Bdata = data.toUtf8();
        memcpy((char*)pdu->caMsg,msg.toStdString().c_str(),msg.length());
        /*通过socket发送协议*/
        clientWin::getInstance().getTcpSocket().write((char*)pdu,pdu->PDULen);
        free(pdu);
        pdu = nullptr;
    }else{
        QMessageBox::critical(this,"error","发送的内容不能为空!");
        return;
    }



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
