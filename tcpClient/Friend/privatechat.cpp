#include "privatechat.h"
#include "ui_privatechat.h"
#include "PDU/protocol.h"
#include <QMessageBox>
#include<QTcpSocket>
#include "../commonfunc.h"
#include "../commondata.h"
#include "ClientSocket/clientSocketThread.h"

PrivateChat::PrivateChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrivateChat)
{
    ui->setupUi(this);
    this->setWindowTitle("私聊对话框");
    this->setWindowIcon(QIcon(QPixmap(":/img/logo.png")));
}

PrivateChat::~PrivateChat()
{
    delete ui;
}

void PrivateChat::setChatName(QString strName)
{
    this->m_strChatName = strName.mid(0,strName.length()-5); //截取用户名
    ui->nameLabel->setText(this->m_strChatName);
    if(strName.mid(strName.length()-2,strName.length()-1) == "离线")
        this->ui->sendPtn->setDisabled(true);
    else
        this->ui->sendPtn->setDisabled(false);
    if(this->m_strChatName.isEmpty()){
        QMessageBox::critical(this,"error","遇到未知错误!");
        this->setAttribute(Qt::WA_DeleteOnClose,true);
        this->close();
    }
//    qDebug()<<strName;
}

PrivateChat &PrivateChat::getInstance()
{
    static PrivateChat instance;
    return instance;
}

void PrivateChat::showMsg(protocol::PDU* pdu)
{
        char pername[64] = {"\0"};
        QString msg;
        msg.clear();
        memcpy(pername,pdu->caData,64);
        msg.append(pername);
        msg.append(" : ");
        msg.append("\n");
        // msg.append("  ");
        msg.append(QString::fromLocal8Bit((char*)pdu->caMsg,pdu->uiMsgLen));
        // msg.append("\r\n\r\n");
        ui->textEdit->append(msg);
        free(pdu);
        pdu = NULL;
}

void PrivateChat::on_sendPtn_clicked()
{
//    qDebug()<<"on_sendPtn_clicked";
    QString strMsg = ui->inputEdit->text();
    ui->inputEdit->clear();
//    qDebug()<<strMsg;
    if(!strMsg.isEmpty()){
        QString strUserName = *reinterpret_cast<QString*>(CommonFunc::QueryData(USERNAME));
        clientSocketThread* m_pclientSocketThread  = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
        QObject::connect(this,SIGNAL(sendPrivateMsgReq(QString,QString,QString)),m_pclientSocketThread,SLOT(sendPrivateChatReq(QString,QString,QString)));
        emit this->sendPrivateMsgReq(strUserName,this->m_strChatName,strMsg);
        QObject::disconnect(this,SIGNAL(sendPrivateMsgReq(QString,QString,QString)),m_pclientSocketThread,SLOT(sendPrivateChatReq(QString,QString,QString)));
    }else{
        QMessageBox::warning(this,"私聊","发送的聊天信息不能为空");
    }
    QString msg;
    msg.clear();
    QString strUserName = *reinterpret_cast<QString*>(CommonFunc::QueryData(USERNAME));
    msg.append(strUserName);
    msg.append(" : ");
    msg.append("\n");
    msg.append("  ");
    msg.append(strMsg);
    msg.append("\r\n\r\n");
    ui->textEdit->append(msg);
}

