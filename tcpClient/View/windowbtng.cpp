#include "windowbtng.h"
#include <QApplication>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QIcon>
#include <QPushButton>
#include "../mymain.h"
#include "MainWIndow/opewidget.h"
#include "../style.h"
#include "File/filepage.h"
#include "Friend/friend.h"
#include "View/m_windowflag.h"
#include "Friend/publicchatwidget.h"
#include "File/publicFilePage.h"
#include "About/about.h"
#include "commonfunc.h"
#include "commondata.h"

windowBtnG* windowBtnG::windowbtnG = NULL;

windowBtnG::windowBtnG(QWidget *parent)
    : QWidget{parent},
    quitBtn(new QPushButton(this)),
    fullBtn(new QPushButton(this)),
    minmunBtn(new QPushButton(this)),
    switchStyleBtn(new QPushButton(this)),
    quitIcon(new QIcon(QPixmap(":/img/quit.png"))),
    fullIcon(new QIcon(QPixmap(":/img/full.png"))),
    minmunIcon(new QIcon(QPixmap(":/img/collapse.png"))),
    normalSizeIcon(new QIcon(QPixmap(":/img/normal.png"))),
    lightIcon(new QIcon(QPixmap(":/img/Light.png"))),
    nightIcon(new QIcon(QPixmap(":/img/Night.png"))),
    mainHBL(new QHBoxLayout(this))
{
    this->setAttribute(Qt::WA_StyledBackground,true); //使控件使用自定义样式
    this->setAttribute(Qt::WA_TranslucentBackground); //设置控件背景透明

    this->minmunBtn->setIcon(*this->minmunIcon);
    this->fullBtn->setIcon(*this->fullIcon);
    this->quitBtn->setIcon(*this->quitIcon);
    this->switchStyleBtn->setIcon(*this->nightIcon);

    this->mainHBL->setAlignment(Qt::AlignRight);
    this->mainHBL->setSpacing(0);
    this->mainHBL->setContentsMargins(0,0,0,0);
    this->quitBtn->setFlat(true);
    this->fullBtn->setFlat(true);
    this->minmunBtn->setFlat(true);
    this->switchStyleBtn->setFlat(true);
    this->fullBtn->setMaximumSize(32,32);
    this->minmunBtn->setMaximumSize(32,32);
    this->quitBtn->setMaximumSize(32,32);
    this->switchStyleBtn->setMaximumSize(32,32);

    this->resize(128,32);

    this->mainHBL->addWidget(switchStyleBtn);
    this->mainHBL->addWidget(minmunBtn);
    this->mainHBL->addWidget(fullBtn);
    this->mainHBL->addWidget(quitBtn);
    this->setLayout(this->mainHBL);
    QObject::connect(this->quitBtn,SIGNAL(clicked()),this,SLOT(quit()));
    QObject::connect(this->fullBtn,SIGNAL(clicked()),this,SLOT(fullScreen()));
    QObject::connect(this->minmunBtn,SIGNAL(clicked()),this,SLOT(minmun()));
    QObject::connect(this->switchStyleBtn,SIGNAL(clicked()),this,SLOT(switchStyle()));
}

windowBtnG *windowBtnG::getInstance(QWidget *parent)
{
    if(windowbtnG == NULL)
    {
        windowbtnG = new windowBtnG(parent);
    }

    return windowbtnG;
}

void windowBtnG::quit()
{
    QString strQuitMode = *reinterpret_cast<QString*>(CommonFunc::QueryData(QUITMODE));
    if(strQuitMode.compare("1") == 0)
    {
        QApplication* a;
        a->quit();
    }
    else if(strQuitMode.compare("2") == 0)
    {
        minmun();
    }
}

void windowBtnG::fullScreen()
{
    if(this->hasFull == false){
        myMain::getInstance()->f_Position = QPoint(myMain::getInstance()->geometry().x(),
                                                   myMain::getInstance()->geometry().y());
        myMain::getInstance()->showMaximized();  //最大化主窗口
        this->fullBtn->setIcon(*this->normalSizeIcon);
        this->hasFull = true;
    }else{
        myMain::getInstance()->resize(myMain::getInstance()->initW,myMain::getInstance()->initH);
//        myMain::getInstance().adjustSize();
        this->fullBtn->setIcon(*this->fullIcon);
        this->hasFull = false;

        if(!myMain::getInstance()->f_Position.isNull())
            myMain::getInstance()->move(myMain::getInstance()->f_Position);

    }

}

void windowBtnG::minmun()
{
    myMain::getInstance()->showMinimized(); //最小化主窗口
}

void windowBtnG::switchStyle()
{
    if(this->hasLight == false){
        this->switchStyleBtn->setIcon(*this->nightIcon);
        OpeWidget::getinstance()->setStyle(m_opeWidget_Light_style);
        OpeWidget::getinstance()->getfilePage()->setStyle(m_filePage_Light_style);
        OpeWidget::getinstance()->getFriend()->setStyle(m_filePage_Light_style);
        m_WindowFlag::getinstance()->setStyle(m_windowflag_Light_style);
        PublicChatWidget::getInstance().setStyle(m_publicChatWidget_Light_style);
        OpeWidget::getinstance()->getPublicFilePage()->setStyle(m_filePage_Light_style);
        About::getInstance()->setStyle(m_About_Light_style);
        this->hasLight = true;

    }else{
        this->switchStyleBtn->setIcon(*this->lightIcon);
        OpeWidget::getinstance()->setStyle(m_opeWidget_style);
        OpeWidget::getinstance()->getfilePage()->setStyle(m_filePage_style);
        OpeWidget::getinstance()->getFriend()->setStyle(m_filePage_style);
        m_WindowFlag::getinstance()->setStyle(m_windowflag_style);
        PublicChatWidget::getInstance().setStyle(m_publicChatWidget_style);
        OpeWidget::getinstance()->getPublicFilePage()->setStyle(m_filePage_style);
        About::getInstance()->setStyle(m_About_style);
        this->hasLight = false;
    }
}
