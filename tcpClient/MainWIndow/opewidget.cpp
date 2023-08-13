#include "opewidget.h"
#include "Friend/friend.h"
#include "File/filepage.h"
#include <QGuiApplication>
#include "View/m_windowflag.h"
#include <QScreen>
#include <style.h>


OpeWidget::OpeWidget(QWidget *parent) : QWidget(parent)
{

    this->setAttribute(Qt::WA_StyledBackground,true); //让控件使用自定义样式表
    int width = parent->width();
    int height = parent->height()*0.6;
    this->resize(width,height);
    qDebug()<<"Ope:setH"<<height;
    qDebug()<<"Ope:realH"<<this->height();
    this->m_pListW = new QListWidget(this);
    QListWidgetItem* friendItem = new QListWidgetItem("好友界面");
    QListWidgetItem* FileItem = new QListWidgetItem("文件界面");
    this->FriendItem = friendItem;
    this->FileItem = FileItem;
    FriendItem->setTextAlignment(Qt::AlignCenter);
    FileItem->setTextAlignment(Qt::AlignCenter);
    this->m_pListW->addItem(friendItem);
    this->m_pListW->addItem(FileItem);

    this->m_pListW->setCurrentItem(friendItem);

    this->m_pFriend = new Friend;
    this->m_pFilePage = new filePage;
    m_pSW  = new QStackedWidget;
    m_pSW->addWidget(m_pFriend);
    m_pSW->addWidget(m_pFilePage);
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

OpeWidget::~OpeWidget()
{
    delete this->FriendItem;
    delete this->FileItem;
    this->FileItem = nullptr;
    this->FriendItem = nullptr;
}

OpeWidget& OpeWidget::getinstance(QWidget* parent)
{
    static OpeWidget instance(parent); //一个静态的OpeWidget 对象
    return instance;//返回这个对象的引用
}

Friend *OpeWidget::getFriend()
{
    return m_pFriend;
}

filePage *OpeWidget::getfilePage()
{
    return m_pFilePage;
}

void OpeWidget::setStyle(QString style)
{
    this->style()->unpolish(this);
    this->setStyleSheet(style);
    this->style()->polish(this);
    this->update();
}

void OpeWidget::listenTransmit()
{
    if(this->m_pSW->currentWidget() == this->m_pFilePage){
        this->getinstance().getfilePage()->flushFile();
        this->myTimer->stop();
    }

}
