#include "mymain.h"
#include <QGuiApplication>
#include <QScreen>
#include "MainWIndow/opewidget.h"
#include "View/m_windowflag.h"
#include <QVBoxLayout>
myMain::myMain(QWidget *parent)
    : QWidget{parent}
{
    /*以下为设置界面属性和样式*/
    this->setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_StyledBackground,true);
    setAttribute(Qt::WA_TranslucentBackground);
    QScreen * screen = QGuiApplication::primaryScreen();//获取主屏幕
    QRect screenRect = screen->availableVirtualGeometry();  //获取屏幕尺寸，位置
    int screenW = screenRect.width(); //获取物理屏幕的宽度
    int screenH = screenRect.height(); //获取物理屏幕的长度

    /*以下根据物理屏幕尺寸按一定比例设置程序界面大小，并打印日志输出*/
    int initW = screenW * 1080 / 1920;
    int initH = screenH * 680 / 1080;
    qDebug()<<initH;
    this->resize(initW,initH);
    this->topWidget = &m_WindowFlag::getinstance(this);
    this->bottomWidget = &OpeWidget::getinstance(this);
    QVBoxLayout* main = new QVBoxLayout(this);
    main->setContentsMargins(0,0,0,0);
    main->addWidget(this->topWidget);
    main->addWidget(this->bottomWidget);
    main->setSpacing(0);
    this->setLayout(main);
    this->setStyleSheet(".myMain{border:none;border-radius:5px;margin:0px;padding:0px;}");
}

myMain& myMain::getInstance(QWidget *parent)
{
    static myMain mainWidget(parent);
    return mainWidget;
}
