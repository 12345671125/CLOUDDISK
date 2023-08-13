#include "mymain.h"
#include <QGuiApplication>
#include <QScreen>
#include "MainWIndow/opewidget.h"
#include "View/m_windowflag.h"
#include <QVBoxLayout>
myMain::myMain(QWidget *parent)
    : QWidget{parent},
    screen(QGuiApplication::primaryScreen()),
    realW(screen->availableVirtualGeometry().width()),
    realH(screen->availableVirtualGeometry().height()),
    initW(realW * 1080 / 1920),
    initH(realH * 680 / 1080),
    c_Position(QCursor::pos()),
    Dragging(false)
{
    /*以下为设置界面属性和样式*/
    this->setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground,true);
    setAttribute(Qt::WA_TranslucentBackground);
    this->resize(this->initW,this->initH);
    this->topWidget = &m_WindowFlag::getinstance(this);
    this->bottomWidget = &OpeWidget::getinstance(this);
    QVBoxLayout* main = new QVBoxLayout(this);
    main->setContentsMargins(0,0,0,0);
    main->addWidget(this->topWidget);
    main->addWidget(this->bottomWidget);
    main->setSpacing(0);
    this->setLayout(main);
    this->setStyleSheet(".myMain{border:none;border-radius:5px;margin:0px;padding:0px;}");
    this->f_Position = QPoint(this->geometry().x(),this->geometry().y());

}

myMain& myMain::getInstance(QWidget *parent)
{
    static myMain mainWidget(parent);
    return mainWidget;
}
