#include "ui_aboutcomponent2.h"

ui_aboutComponent2::ui_aboutComponent2(QString titleName, QString Option1, QString Option2, QString Option3, QWidget* parent) :
    ui_aboutComponent(parent)
    ,m_addressText(new QTextEdit())
    ,m_Option1(new QPushButton())
    ,m_Option2(new QPushButton())
    ,m_Option3(new QPushButton())
    ,m_HLayout(new QHBoxLayout())
{
    this->setAttribute(Qt::WA_StyledBackground);
    this->m_HLayout->addWidget(m_addressText,3);
    // this->m_addressText->setAlignment(Qt::AlignCenter);
    this->m_addressText->setWordWrapMode (QTextOption::NoWrap);
    m_addressText->setFixedHeight(30);
    m_addressText->setStyleSheet("border-radius: 15px;");
    m_addressText->setReadOnly(true);
    if(Option1.isEmpty())
    {
        this->m_Option1->setVisible(false);
    }else
    {
        this->m_Option1->setText(Option1);
        m_HLayout->addWidget(this->m_Option1,1);
    }
    if(Option2.isEmpty())
    {
        this->m_Option2->setVisible(false);
    }else
    {
        this->m_Option2->setText(Option2);
        m_HLayout->addWidget(this->m_Option2,1);
    }
    if(Option3.isEmpty())
    {
        this->m_Option3->setVisible(false);
    }else
    {
        this->m_Option3->setText(Option3);
        m_HLayout->addWidget(this->m_Option3,1);
    }

    m_HLayout->setContentsMargins(5,5,5,5);
    m_HLayout->setAlignment(Qt::AlignCenter);
    this->m_title->setText(titleName);
    this->m_mainHLayout->addWidget(this->m_title,1);
    this->m_mainHLayout->addLayout(this->m_HLayout,4);
    this->m_title->setAlignment(Qt::AlignCenter);
    this->m_mainHLayout->setAlignment(Qt::AlignCenter);
    this->setLayout(this->m_mainHLayout);
    this->setFixedHeight(80);
}
