#include "m_windowflag.h"

#include <QIcon>
#include "logowidget.h"
#include "windowbtng.h"

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
     setStyleSheet(".m_WindowFlag{border:none;background:#1e1c1b;border-top-left-radius:5px;border-top-right-radius:5px;} \
                    QLabel{color:white;}"
                    );

}

m_WindowFlag &m_WindowFlag::getinstance(QWidget* parent)
{
     static  m_WindowFlag WindowFlag(parent);
     return WindowFlag;
}
