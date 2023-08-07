#include "opewidget.h"

OpeWidget::OpeWidget(QWidget *parent) : QWidget(parent)
{
    this->m_pListW = new QListWidget(this);
    this->m_pListW->addItem("好友界面");
    this->m_pListW->addItem("文件界面");
    this->m_pFriend = new Friend;
    this->m_pFilePage = new filePage;
    m_pSW  = new QStackedWidget;
    m_pSW->addWidget(m_pFriend);
    m_pSW->addWidget(m_pFilePage);
    QHBoxLayout* pMain = new QHBoxLayout;
    pMain->addWidget(m_pListW);
    pMain->addWidget(m_pSW);
    this->myTimer = new QTimer(this);
    this->myTimer->setInterval(100);
    this->myTimer->start();

    setLayout(pMain);
    QObject::connect(m_pListW,SIGNAL(currentRowChanged(int)),m_pSW,SLOT(setCurrentIndex(int)));
    QObject::connect(myTimer,SIGNAL(timeout()),this,SLOT(listenTransmit()));
}

OpeWidget& OpeWidget::getinstance()
{
    static OpeWidget instance; //一个静态的OpeWidget 对象
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

void OpeWidget::listenTransmit()
{
    if(this->m_pSW->currentWidget() == this->m_pFilePage){
        this->getinstance().getfilePage()->flushFile();
        this->myTimer->stop();
    }

}
