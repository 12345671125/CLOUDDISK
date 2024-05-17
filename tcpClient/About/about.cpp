#include "About/about.h"
#include "ui_aboutcomponent1.h"
#include "ui_aboutcomponent2.h"
#include <QStyle>
#include "style.h"
#include "commondata.h"
#include "commonfunc.h"
#include <qdesktopservices>
#include <qmessagebox.h>
#include <QEvent>
#include <QFileDialog>
About* About::m_pAboutPage = nullptr;

About *About::getInstance(QWidget* parent)
{
    if(m_pAboutPage == nullptr)
    {
        m_pAboutPage = new About(parent);
    }
    return m_pAboutPage;
}

About::About(QWidget *parent):
     quitCom(new ui_aboutComponent1("退出","退出程序","最小化托盘"))
    ,storeCom(new ui_aboutComponent2("存储","更改","打开目录"))
    ,versionCom(new ui_aboutComponent2("版本","检查更新"))
    ,webCom(new ui_aboutComponent2("开源地址","打开"))
    ,m_mainBoxLayout(new QVBoxLayout(this))

{
    QString strWebAddr = *reinterpret_cast<QString*>(CommonFunc::QueryData(WEBADDRESS));
    this->webCom->m_addressText->setText(strWebAddr);
    this->webCom->m_addressText->setAlignment(Qt::AlignCenter);
    this->m_mainBoxLayout->addWidget(this->quitCom);
    this->m_mainBoxLayout->addWidget(this->storeCom);
    this->m_mainBoxLayout->addWidget(this->versionCom);
    this->m_mainBoxLayout->addWidget(this->webCom);


    this->m_mainBoxLayout->setAlignment(Qt::AlignCenter);
    this->m_mainBoxLayout->setContentsMargins(5,5,5,5);
    this->setAttribute(Qt::WA_StyledBackground);
    this->setLayout(this->m_mainBoxLayout);
    this->setStyle(m_About_Light_style);
    connectedToSlot();

}

void About::connectedToSlot()
{
    connect(this->webCom->m_Option1,SIGNAL(clicked(bool)),this,SLOT(openWeb()));
    connect(this->storeCom->m_Option2,SIGNAL(clicked(bool)),this,SLOT(openDefaultStorePath()));
    connect(this->storeCom->m_Option1,SIGNAL(clicked(bool)),this,SLOT(modifyDefaultStorePath()));
    connect(this->versionCom->m_Option1,SIGNAL(clicked(bool)),this,SLOT(checkUpdate()));
    connect(this->quitCom->m_Option1,SIGNAL(clicked(bool)),this,SLOT(swithQuitModeToQuit()));
    connect(this->quitCom->m_Option2,SIGNAL(clicked(bool)),this,SLOT(swithQuitModeToMin()));
}

void About::openWeb()
{
    QString strWebUrl = *reinterpret_cast<QString*>(CommonFunc::QueryData(WEBADDRESS));
    if(!strWebUrl.isEmpty())
    {
        QDesktopServices::openUrl(QUrl(strWebUrl.toLatin1()));
    }else
    {
        QMessageBox::information(this,"打开网页","打开网页遇到未知错误!");
    }
}

void About::modifyDefaultStorePath()
{
    QString strFileName = QFileDialog::getExistingDirectory(NULL,"caption",".");
    if(!strFileName.isEmpty())
    {
        CommonFunc::setData(DEFAULTSTOREADDR,&strFileName);
        this->storeCom->m_addressText->setText(*reinterpret_cast<QString*>(CommonFunc::QueryData(DEFAULTSTOREADDR)));
    }
}

void About::openDefaultStorePath()
{
    QString folderPath = *reinterpret_cast<QString*>(CommonFunc::QueryData(DEFAULTSTOREADDR));
    QString finalPath = "file:///" + folderPath;
    QDesktopServices::openUrl(QUrl(finalPath));
}

void About::checkUpdate()
{
    QMessageBox::information(nullptr,"检查更新","未发现新版本!");
}

void About::swithQuitModeToQuit()
{
    QString mode = "1";
    CommonFunc::setData(QUITMODE,&mode);
}

void About::swithQuitModeToMin()
{
    QString mode = "2";
    CommonFunc::setData(QUITMODE,&mode);
}
void About::setStyle(QString style)
{
    this->style()->unpolish(this);
    this->setStyleSheet(style);
    this->style()->polish(this);
    this->update();
}

bool About::event(QEvent *event)
{
    if(event->type() == QEvent::Show)
    {
        QString strQuitMode = *reinterpret_cast<QString*>(CommonFunc::QueryData(QUITMODE));
        this->storeCom->m_addressText->setText(*reinterpret_cast<QString*>(CommonFunc::QueryData(DEFAULTSTOREADDR)));
        this->versionCom->m_addressText->setText(*reinterpret_cast<QString*>(CommonFunc::QueryData(VERSION)));
        if(strQuitMode.compare("1") == 0)
        {
            this->quitCom->m_Option1->setChecked(true);
        }else if(strQuitMode.compare("2") == 0)
        {
            this->quitCom->m_Option2->setChecked(true);
        }
    }
}
