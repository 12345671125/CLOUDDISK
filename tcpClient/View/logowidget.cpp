#include "logowidget.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPushButton>
#include "style.h"

logoWidget::logoWidget(QWidget *parent)
    : QWidget{parent},
    appIcon(new QIcon(QPixmap(":/img/logo.png"))),
    mainHBL(new QHBoxLayout(this)),
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
    this->setStyleSheet(m_LogoWidget_style);
    this->apptitle->setText("CloudDisk");
    this->Icon->setMaximumSize(32,32);
    this->apptitle->resize(50,50);
    this->resize(90,52);
    this->mainHBL->addWidget(this->Icon);
    this->mainHBL->addWidget(this->apptitle);
    this->setLayout(mainHBL);

}

logoWidget::~logoWidget()
{
    delete this->appIcon;
}

logoWidget &logoWidget::getInstance(QWidget *parent)
{
    static logoWidget instance(parent);
    return instance;
}
