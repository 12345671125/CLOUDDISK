#include "publicchatwidget.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include "../View/msgwidget.h"
#include "../style.h"
#include "../PDU/protocol.h"
// #include "ClientSocket/clientwin.h"
#include "qstyle.h"
#include <QTcpSocket>
#include <QMessageBox>

#include"../commonfunc.h"
#include "../commondata.h"
#include "ClientSocket/clientSocketThread.h"

PublicChatWidget::PublicChatWidget(QWidget *parent)
    : QWidget{parent},
    showMsgEdit(new QTextEdit(this)),
    mainVBL(new QVBoxLayout(this)),
    msgWidget(&msgWidget::getInstance(this))
{
    this->setAttribute(Qt::WA_StyledBackground);

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

void PublicChatWidget::showPublicChat(protocol::PDU* pdu)
{
        QString data;
        data.clear();
        char username[64] = {"\0"};
        memcpy(username,pdu->caData,64);
        data = QString::fromLocal8Bit(username,64);
        data.append(" : ").append(QString::fromLocal8Bit((char*)&pdu->caMsg,pdu->uiMsgLen)).append("\r\n");
        this->showMsgEdit->append(data);

        free(pdu);
        pdu = NULL;

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
                QString strUserName = *reinterpret_cast<QString*>(CommonFunc::QueryData(USERNAME));
                data = strUserName;
                data.append(" : ").append(msg).append("\r\n");
                this->showMsgEdit->append(data);
                this->msgWidget->inputEdit->clear();

                clientSocketThread* m_pclientSocketThread  = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
                QObject::connect(this,SIGNAL(sendPublicChatMsg(QString,QString)),m_pclientSocketThread,SLOT(sendPublicChatReq(QString,QString)));
                this->sendPublicChatMsg(strUserName,msg);
                QObject::disconnect(this,SIGNAL(sendPublicChatMsg(QString,QString)),m_pclientSocketThread,SLOT(sendPublicChatReq(QString,QString)));
            }else{
                QMessageBox::critical(this,"error","发送的内容不能为空!");
                return;
            }
}
