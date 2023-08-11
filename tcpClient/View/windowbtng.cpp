#include "windowbtng.h"
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QIcon>
#include <QPushButton>

windowBtnG::windowBtnG(QWidget *parent)
    : QWidget{parent},
    quitBtn(new QPushButton(this)),
    fullBtn(new QPushButton(this)),
    collapseBtn(new QPushButton(this)),
    quitIcon(new QIcon(QPixmap(":/img/quit.png"))),
    fullIcon(new QIcon(QPixmap(":/img/full.png"))),
    collapseIcon(new QIcon(QPixmap(":/img/collapse.png"))),
    mainHBL(new QHBoxLayout(this))
{
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->collapseBtn->setIcon(*this->collapseIcon);
    this->fullBtn->setIcon(*this->fullIcon);
    this->quitBtn->setIcon(*this->quitIcon);
    this->mainHBL->setAlignment(Qt::AlignRight);
    this->mainHBL->setSpacing(0);
    this->mainHBL->setContentsMargins(0,0,0,0);
    this->quitBtn->setFlat(true);
    this->fullBtn->setFlat(true);
    this->collapseBtn->setFlat(true);
    this->fullBtn->setMaximumSize(32,32);
    this->collapseBtn->setMaximumSize(32,32);
    this->quitBtn->setMaximumSize(32,32);
    this->resize(96,32);
    this->mainHBL->addWidget(collapseBtn);
    this->mainHBL->addWidget(fullBtn);
    this->mainHBL->addWidget(quitBtn);
    this->setLayout(this->mainHBL);
}

windowBtnG::~windowBtnG()
{
    delete this->collapseIcon;
    delete this->fullIcon;
    delete this->quitIcon;
}

windowBtnG &windowBtnG::getInstance(QWidget *parent)
{
    static windowBtnG  mybtnG;
    return mybtnG;
}
