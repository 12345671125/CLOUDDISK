#include "ui_aboutcomponent1.h"

ui_aboutComponent1::ui_aboutComponent1(QString titleName, QString Option1, QString Option2, QString Option3, QWidget *parent)
    :ui_aboutComponent(parent)
    ,m_Option1(new QRadioButton())
    ,m_Option2(new QRadioButton())
    ,m_Option3(new QRadioButton())
    ,m_btnGroup(new QHBoxLayout())
{
    this->setAttribute(Qt::WA_StyledBackground);
    if(Option1.isEmpty())
    {
        this->m_Option1->setVisible(false);
    }else
    {
        this->m_Option1->setText(Option1);
        m_btnGroup->addWidget(this->m_Option1,1);
    }
    if(Option2.isEmpty())
    {
        this->m_Option2->setVisible(false);
    }else
    {
        this->m_Option2->setText(Option2);
        m_btnGroup->addWidget(this->m_Option2,1);
    }
    if(Option3.isEmpty())
    {
        this->m_Option3->setVisible(false);
    }else
    {
        this->m_Option3->setText(Option3);
        m_btnGroup->addWidget(this->m_Option3,1);
    }

    m_btnGroup->setContentsMargins(5,5,5,5);
    m_btnGroup->setAlignment(Qt::AlignCenter);
    this->m_title->setText(titleName);
    m_mainHLayout->addWidget(m_title,1);
    this->m_mainHLayout->addLayout(this->m_btnGroup,4);
    this->m_title->setAlignment(Qt::AlignCenter);
    this->m_mainHLayout->setAlignment(Qt::AlignCenter);
    this->setLayout(this->m_mainHLayout);
    this->setFixedHeight(80);

}
