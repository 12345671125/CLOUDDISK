#include "privatechat.h"
#include "ui_privatechat.h"
#include "protocol.h"
#include <QMessageBox>
#include "clientwin.h"

PrivateChat::PrivateChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrivateChat)
{
    ui->setupUi(this);
    this->setWindowTitle("私聊对话框");
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
        msg.append("  ");
        msg.append(QString::fromLocal8Bit((char*)pdu->caMsg,pdu->uiMsgLen));
        msg.append("\r\n\r\n");
        ui->textEdit->append(msg);
}

void PrivateChat::on_sendPtn_clicked()
{
//    qDebug()<<"on_sendPtn_clicked";
    QString strMsg = ui->inputEdit->text();
    ui->inputEdit->clear();
//    qDebug()<<strMsg;
    if(!strMsg.isEmpty()){
        protocol::PDU *pdu = protocol::createPDU(strMsg.size()+1);
        pdu->uiMsgType = protocol::ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST;
        memcpy(pdu->caData,clientWin::getInstance().getLoginName().toStdString().c_str(),64); //将已方用户名写入协议
        memcpy(pdu->caData+64,this->m_strChatName.toStdString().c_str(),64); //将聊天对象用户名写入协议
        memcpy((char*)pdu->caMsg,strMsg.toStdString().c_str(),strMsg.length());//将聊天内容写入协议
        clientWin::getInstance().getTcpSocket().write((char*)pdu,pdu->PDULen);//将协议写入socket后发送
        free(pdu);
        pdu = nullptr;

    }else{
        QMessageBox::warning(this,"私聊","发送的聊天信息不能为空");
    }
    QString msg;
    msg.clear();
    msg.append(clientWin::getInstance().getLoginName());
    msg.append(" : ");
    msg.append("\n");
    msg.append("  ");
    msg.append(strMsg);
    msg.append("\r\n\r\n");
    ui->textEdit->append(msg);
}

