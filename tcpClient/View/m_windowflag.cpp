#include "m_windowflag.h"

#include <QIcon>
#include <QMouseEvent>
#include "logowidget.h"
#include "qstyle.h"
#include "windowbtng.h"
#include "mymain.h"
#include "../style.h"

m_WindowFlag::m_WindowFlag(QWidget *parent)
    : QWidget{parent}
{
     setAttribute(Qt::WA_StyledBackground,true);//让控件使用自定义样式表
    /*以下根据物理屏幕尺寸按一定比例设置程序界面大小，并打印日志输出*/
    int width = parent->width();
    int height = parent->height()*0.4;
    this->resize(width,height);
    this->myBtnG = &windowBtnG::getInstance(this);
    qDebug()<<"flag:setH"<<height;
    qDebug()<<"flag:realH"<<this->height();
    this->setWindowFlag(Qt::FramelessWindowHint);
    appIcon = new QIcon(QPixmap());
     mainLayout = new QHBoxLayout(this);
    buttonG = new QHBoxLayout(this);
     logoG = new QHBoxLayout(this);
    this->logoWidget = &logoWidget::getInstance(this);
     this->logoG->addWidget(logoWidget);
    this->buttonG->addWidget(this->myBtnG);
//     this->logoG->setAlignment(Qt::AlignCenter);
     this->logoG->setSpacing(0);
     this->logoG->setContentsMargins(0,0,0,0);
     this->buttonG->setSpacing(0);
     this->mainLayout->addLayout(logoG);
     this->mainLayout->addLayout(buttonG);
//     this->mainLayout->setSpacing(0);
     this->setLayout(mainLayout);
     this->buttonG->setContentsMargins(0,0,0,0);
     this->mainLayout->setContentsMargins(10,0,0,0);

     /*以下为样式的设置*/
     this->setStyle(m_windowflag_Light_style);

}

m_WindowFlag &m_WindowFlag::getinstance(QWidget* parent)
{
     static  m_WindowFlag WindowFlag(parent);
     return WindowFlag;
}

void m_WindowFlag::setStyle(QString style)
{
     this->style()->unpolish(this);
     this->setStyleSheet(style);
     this->style()->polish(this);
     this->update();
}
void m_WindowFlag::mousePressEvent(QMouseEvent *event)
{
     if(event->button() == Qt::LeftButton){
         this->setCursor(Qt::SizeAllCursor);
         myMain::getInstance().Dragging = true;
         myMain::getInstance().f_Position = QPoint(myMain::getInstance().geometry().x(),
                                                   myMain::getInstance().geometry().y());
         myMain::getInstance().c_Position = QCursor::pos();
     }

}

void m_WindowFlag::mouseMoveEvent(QMouseEvent *event)
{
     if(myMain::getInstance().Dragging == true){
         myMain::getInstance().move(myMain::getInstance().f_Position + (QCursor::pos() - myMain::getInstance().c_Position));
     }
}

void m_WindowFlag::mouseReleaseEvent(QMouseEvent *event)
{
     this->setCursor(Qt::ArrowCursor);
     myMain::getInstance().Dragging = false;
}
