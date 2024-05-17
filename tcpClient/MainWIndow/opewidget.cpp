#include "opewidget.h"
#include "Friend/friend.h"
#include "File/filepage.h"
#include <QGuiApplication>
#include "View/m_windowflag.h"
#include <QScreen>
#include <style.h>
#include "publicchatwidget.h"
#include "publicFilePage.h"
#include "About/about.h"

OpeWidget* OpeWidget::Opewidget = nullptr;

OpeWidget::OpeWidget(QWidget *parent) : QWidget(parent)
{

    this->setAttribute(Qt::WA_StyledBackground,true); //让控件使用自定义样式表



    this->m_pListW = new QListWidget(this);
    QListWidgetItem* publicFileItem = new QListWidgetItem("文件广场");
    QListWidgetItem* friendItem = new QListWidgetItem("好友界面");
    QListWidgetItem* FileItem = new QListWidgetItem("文件界面");
    QListWidgetItem* AboutItem = new QListWidgetItem("关于软件");
    this->PublicFileItem = publicFileItem;
    this->FriendItem = friendItem;
    this->FileItem = FileItem;
    this->AboutItem = AboutItem;
    publicFileItem->setTextAlignment(Qt::AlignCenter);
    FriendItem->setTextAlignment(Qt::AlignCenter);
    FileItem->setTextAlignment(Qt::AlignCenter);
    AboutItem->setTextAlignment(Qt::AlignCenter);
    this->m_pListW->addItem(publicFileItem);
    this->m_pListW->addItem(friendItem);
    this->m_pListW->addItem(FileItem);
    this->m_pListW->addItem(AboutItem);

    this->m_pPublicFilePage = publicFilePage::getInstance(this);
    this->m_pFriend = Friend::getInsance(this);
    this->m_pFilePage = filePage::getInstance(this);
    this->m_pAbout = About::getInstance(this);

    m_pSW  = new QStackedWidget;
    m_pSW->addWidget(m_pPublicFilePage);
    m_pSW->addWidget(m_pFriend);
    m_pSW->addWidget(m_pFilePage);
    m_pSW->addWidget(m_pAbout);
    this->m_pSW->setMinimumWidth(this->width()*0.8);
    QHBoxLayout* Main = new QHBoxLayout;
    Main->addWidget(m_pListW);
    Main->addWidget(m_pSW);
    this->myTimer = new QTimer(this);
    this->myTimer->setInterval(100);
    this->myTimer->start();
    Main->setContentsMargins(0,0,0,0);
    setLayout(Main);

    /*以下为信号槽的连接*/
    QObject::connect(m_pListW,SIGNAL(currentRowChanged(int)),m_pSW,SLOT(setCurrentIndex(int)));
    QObject::connect(myTimer,SIGNAL(timeout()),this,SLOT(listenTransmit()));
    /*以下为样式设置*/
    this->setStyle(m_opeWidget_Light_style);
}

OpeWidget* OpeWidget::getinstance(QWidget* parent)
{
    if(OpeWidget::Opewidget == nullptr)
    {
        Opewidget = new OpeWidget(parent);
    }
    return Opewidget;
}

Friend *OpeWidget::getFriend()
{
    return m_pFriend;
}

filePage *OpeWidget::getfilePage()
{
    return m_pFilePage;
}

publicFilePage *OpeWidget::getPublicFilePage()
{
    return m_pPublicFilePage;
}

void OpeWidget::setStyle(QString style)
{
    this->style()->unpolish(this);
    this->setStyleSheet(style);
    this->style()->polish(this);
    this->update();
}

void OpeWidget::AllOnlineRes(protocol::PDU* pdu)
{
    this->m_pFriend->showAllOnlineUser(pdu);
}

void OpeWidget::SearchUserRes(protocol::PDU* pdu)
{
    this->m_pFriend->showSearchUser(pdu);
}

void OpeWidget::addFriend(protocol::PDU* pdu)
{
    this->m_pFriend->addFriend(pdu);
}

void OpeWidget::createDir(protocol::PDU* pdu)
{
    this->m_pFilePage->flushFile();
    free(pdu);
    pdu = NULL;
}

void OpeWidget::FlushFile(protocol::PDU* pdu)
{
    this->m_pFilePage->updateFileList(pdu);

}

void OpeWidget::ShowAddFriendRet(protocol::PDU* pdu)
{
    this->m_pFriend->showAddFriendRet(pdu);
}

void OpeWidget::FlushFriend(protocol::PDU* pdu)
{
    this->m_pFriend->updateFriend(pdu);
}

void OpeWidget::ShowPrivateChat(protocol::PDU* pdu)
{
    this->m_pFriend->showPrivateChat(pdu);
}

void OpeWidget::ShowPublicChat(protocol::PDU* pdu)
{
    this->m_pFriend->showPublicChat(pdu);
}

void OpeWidget::ShowDelDirRet(protocol::PDU* pdu)
{
    this->m_pFilePage->delDir(pdu);
}

void OpeWidget::ShowDelFileRet(protocol::PDU* pdu)
{
    this->m_pFilePage->delFile(pdu);
}

void OpeWidget::ShowRenameFileRet(protocol::PDU* pdu)
{
    this->m_pFilePage->renameFile(pdu);
}

void OpeWidget::ShowFileInfoRet(protocol::PDU* pdu)
{
    this->m_pFilePage->showFileInfo(pdu);
}

void OpeWidget::ShowUploadFileInfoRet(protocol::PDU *pdu)
{
    this->m_pFilePage->getFileInfoRes(pdu);
}

void OpeWidget::OpenUploadAndDownLoadPage()
{

}

void OpeWidget::HandleDownloadFile(protocol::PDU* pdu)
{
    this->m_pFilePage->handleDownloadFile(pdu);
}

void OpeWidget::ShowFileInfo(protocol::PDU *pdu)
{
    this->m_pFilePage->showFileInfo(pdu);
}

void OpeWidget::ShowUploadPublicFileInfoRet(protocol::PDU *pdu)
{
    this->m_pPublicFilePage->getFileInfoRes(pdu);
}

void OpeWidget::FlushPublicFile(protocol::PDU *pdu)
{
    this->m_pPublicFilePage->updateFileList(pdu);
}

void OpeWidget::HandleDownloadPublicFile(protocol::PDU *pdu)
{
    this->m_pPublicFilePage->handleDownloadFile(pdu);
}

void OpeWidget::listenTransmit()
{
    if(this->m_pSW->currentWidget() == this->m_pFilePage){
        // this->getinstance().getfilePage()->flushFile();
        this->myTimer->stop();
    }

}
