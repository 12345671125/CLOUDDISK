/*在线页面类*/

#include "online.h"
#include "ui_online.h"
#include "PDU/protocol.h"
#include <QTcpSocket>
#include "../commonfunc.h"
#include "../commondata.h"
#include "ClientSocket/clientSocketThread.h"

Online::Online(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Online)
{
    ui->setupUi(this);
}

Online::~Online()
{
    delete ui;
}

void Online::showUser(protocol::PDU* pdu)
{
    uint uiSize = pdu->uiMsgLen/32;
    char caTmp[32];
    QString strUserName = *reinterpret_cast<QString*>(CommonFunc::QueryData(USERNAME));
    CommonFunc::removeNullCharacters(strUserName);
    this->ui->online_lw->clear();
    for(uint i = 0;i<uiSize;i++){
        memcpy_s(caTmp,32,(char*)pdu->caMsg+i*32,32);
        QString strName = QString::fromUtf8(caTmp);
        CommonFunc::removeNullCharacters(strName);
        if(strName.compare(strUserName) == 0)
        {
            continue;
        }
        this->ui->online_lw->addItem(caTmp);
    }
    free(pdu);
    pdu = NULL;

}

void Online::showSearchUser(protocol::PDU* pdu)
{
    uint uiSize = pdu->uiMsgLen/32;
    char caTmp[32];
    this->ui->online_lw->clear();
    for(uint i = 0;i<uiSize;i++){
        memcpy(caTmp,(char*)pdu->caMsg+i*32,32);
        this->ui->online_lw->addItem(caTmp);
    }
    free(pdu);
    pdu = NULL;
}

void Online::on_addFriendButton_clicked()
{
    QListWidgetItem *item = this->ui->online_lw->currentItem();
    QString strPerUserName = item->text();
    QString strUserName = *reinterpret_cast<QString*>(CommonFunc::QueryData(USERNAME));
    QString strLoginName = strUserName;

    clientSocketThread* m_clientSocketThread = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
    QObject::connect(this,SIGNAL(sendAddFriReq(QString,QString)),m_clientSocketThread,SLOT(sendAddFriReq(QString,QString)));
    emit sendAddFriReq(strPerUserName,strLoginName);
    QObject::connect(this,SIGNAL(sendAddFriReq(QString,QString)),m_clientSocketThread,SLOT(sendAddFriReq(QString,QString)));

}

