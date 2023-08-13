#include "msgwidget.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include "../style.h"

msgWidget::msgWidget(QWidget *parent)
    : QWidget{parent},
    inputEdit(new QLineEdit(this)),
    sendMagBtn(new QPushButton(this)),
    bottomHBL(new QHBoxLayout(this))
{
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->setAttribute(Qt::WA_TranslucentBackground);

    this->sendMagBtn->setText("发送");
    this->bottomHBL->addWidget(inputEdit);
    this->bottomHBL->addWidget(sendMagBtn);

    this->bottomHBL->setSpacing(10);
    this->bottomHBL->setContentsMargins(0,0,0,0);
    this->bottomHBL->setAlignment(Qt::AlignCenter);

    this->inputEdit->setMinimumSize(this->width() * 0.6,this->height()*0.9);
    this->sendMagBtn->resize(this->width() * 0.3,this->height() * 0.9);

    this->setLayout(this->bottomHBL);

    this->resize(parent->width() * 0.95,parent->height() * 0.2);

    this->setStyleSheet(m_msgWidget_style);
}

msgWidget &msgWidget::getInstance(QWidget *parent)
{
    static msgWidget instance(parent);
    return instance;
}
