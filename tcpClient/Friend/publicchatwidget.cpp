#include "publicchatwidget.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include "../View/msgwidget.h"
#include "../style.h"
#include "../PDU/protocol.h"
#include "ClientSocket/clientwin.h"
#include "qstyle.h"

PublicChatWidget::PublicChatWidget(QWidget *parent)
    : QWidget{parent},
    showMsgEdit(new QTextEdit(this)),
    mainVBL(new QVBoxLayout(this)),
    msgWidget(&msgWidget::getInstance(this))
{
    this->setAttribute(Qt::WA_StyledBackground);
    this->width = parent->width() * 0.6;
    this->height = parent->height();
    this->resize(this->width,this->height);

    this->showMsgEdit->setMinimumSize(this->width*0.9,this->height * 0.8);
    this->showMsgEdit->setReadOnly(true);

    this->mainVBL->addWidget(this->showMsgEdit);
    this->mainVBL->addWidget(this->msgWidget);

    this->mainVBL->setContentsMargins(0,0,0,0);
    this->mainVBL->setSpacing(10);

    this->setLayout(this->mainVBL);

    connect(this->msgWidget->sendMagBtn,SIGNAL(clicked()),this,SLOT(publicChat()));

    this->setStyle(m_publicChatWidget_Light_style);
}

PublicChatWidget &PublicChatWidget::getInstance(QWidget *parent)
{
    static PublicChatWidget instance(parent);
    return instance;
}

void PublicChatWidget::showPublicChat(protocol::PDU *pdu)
{
        if(pdu == nullptr) return;
        QString data;
        data.clear();
        char username[64] = {"\0"};
        memcpy(username,pdu->caData,64);
        data = QString::fromLocal8Bit(username,64);
        data.append(" : ").append(QString::fromLocal8Bit((char*)pdu->caMsg,pdu->uiMsgLen)).append("\r\n");
        this->showMsgEdit->append(data);

}

void PublicChatWidget::setStyle(QString style)
{
        this->style()->unpolish(this);
        this->setStyleSheet(style);
        this->style()->polish(this);
        this->update();
}
void PublicChatWidget::publicChat()
{
        QString msg = this->msgWidget->inputEdit->text();//获取文本框输入内容
            if(!msg.isEmpty()){
                QString data;
                data.clear();
                data = clientWin::getInstance().getLoginName();
                data.append(" : ").append(msg).append("\r\n");
                this->showMsgEdit->append(data);
                this->msgWidget->inputEdit->clear();
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
