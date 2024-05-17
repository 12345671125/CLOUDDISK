#include "logowidget.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include "style.h"
#include <qevent.h>
logoWidget* logoWidget::logowidget = NULL;

logoWidget::logoWidget(QWidget *parent)
    : QWidget{parent},
    mainHBL(new QHBoxLayout(this)),
    appIcon(new QIcon(QPixmap(":/img/logo.png"))),
    Icon(new QPushButton(this)),
    apptitle(new QLabel(this))
{
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->mainHBL->setContentsMargins(0,0,0,0);
    //    this->mainHBL->setAlignment(Qt::AlignCenter);
    this->mainHBL->setSpacing(0);
    this->Icon->setIcon(*(this->appIcon));
    this->Icon->setFlat(true);
    this->Icon->setCursor(Qt::PointingHandCursor);
    this->apptitle->setText("CloudDisk");
    this->Icon->setMaximumSize(32,32);
    this->apptitle->resize(50,50);
    this->resize(90,52);
    this->mainHBL->addWidget(this->Icon);
    this->mainHBL->addWidget(this->apptitle);
    this->setLayout(mainHBL);

    /*以下为样式的设置*/

    this->setStyleSheet(m_LogoWidget_style);

}

logoWidget *logoWidget::getInstance(QWidget *parent)
{
    if(logowidget == NULL)
    {
        logowidget = new logoWidget(parent);
    }

    return logowidget;
}
