#include "ui_aboutcomponent.h"

ui_aboutComponent::ui_aboutComponent(QWidget *parent)
    : QWidget{parent}
    ,m_mainHLayout(new QHBoxLayout(this))
    ,m_title(new QTextEdit())
{
    m_mainHLayout->setContentsMargins(0,0,0,0);
    m_title->setAttribute(Qt::WA_TranslucentBackground);
    // m_title->setStyleSheet("text-align:center;");
    this->m_title->setAlignment(Qt::AlignCenter);
    m_title->setReadOnly(true);
    m_title->setFixedSize(80,30);
}
