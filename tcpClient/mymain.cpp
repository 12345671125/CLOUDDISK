#include "mymain.h"
#include <QGuiApplication>
#include <QScreen>
#include "MainWIndow/opewidget.h"
#include "View/m_windowflag.h"
#include <QVBoxLayout>

myMain* myMain::m_pMyMain = NULL;

myMain::myMain(QWidget *parent)
    : QWidget(parent),
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
    this->setFixedSize(initW,initH);
    this->topWidget = m_WindowFlag::getinstance();
    this->bottomWidget = OpeWidget::getinstance();
    QVBoxLayout* main = new QVBoxLayout(this);
    main->setContentsMargins(0,0,0,0);
    main->setDirection(QVBoxLayout::TopToBottom);
    main->addWidget(this->topWidget,1);
    main->addWidget(this->bottomWidget,9);

    main->setSpacing(0);
    this->setStyleSheet(".myMain{border:none;border-radius:5px;margin:0px;padding:0px;}");
    this->setWindowTitle("CloudDisk");
    this->f_Position = QPoint(this->geometry().x(),this->geometry().y());
    this->setWindowIcon(QIcon(QPixmap(":/img/logo.png")));
    this->setLayout(main);
}

myMain* myMain::getInstance(QMainWindow *parent)
{
    if(m_pMyMain == NULL)
    {
        m_pMyMain = new myMain(parent);
    }

    return m_pMyMain;
}
