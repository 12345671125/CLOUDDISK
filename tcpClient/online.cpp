/*在线页面类*/

#include "online.h"
#include "ui_online.h"
#include "clientwin.h"
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

void Online::showUser(protocol::PDU *pdu)
{
    if(pdu == NULL) return;
    uint uiSize = pdu->uiMsgLen/32;
    char caTmp[32];
    this->ui->online_lw->clear();
    for(uint i = 0;i<uiSize;i++){
        memcpy(caTmp,(char*)pdu->caMsg+i*32,32);
        this->ui->online_lw->addItem(caTmp);
    }

}

void Online::showSearchUser(protocol::PDU *pdu)
{
    if(pdu == NULL) return;
    uint uiSize = pdu->uiMsgLen/32;
    char caTmp[32];
    this->ui->online_lw->clear();
    for(uint i = 0;i<uiSize;i++){
        memcpy(caTmp,(char*)pdu->caMsg+i*32,32);
        this->ui->online_lw->addItem(caTmp);
    }
}

void Online::on_addFriendButton_clicked()
{
    QListWidgetItem *item = this->ui->online_lw->currentItem();
    QString strPerUserName = item->text();
    QString strLoginName = clientWin::getInstance().getLoginName();
    protocol::PDU pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_ADDFRIEND_REQUEST,0);
    memcpy(pdu.caData,strLoginName.toStdString().c_str(),64);
    memcpy(pdu.caData+64,strPerUserName.toStdString().c_str(),64);
    clientWin::getInstance().getTcpSocket().write((char*)&pdu,pdu.PDULen); //使用 socket发送数据到服务器

}

