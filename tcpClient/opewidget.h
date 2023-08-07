#ifndef OPEWIDGET_H
#define OPEWIDGET_H
#pragma once
#include <QWidget>
#include <QListWidget>
#include "friend.h"
#include <QStackedWidget>
#include "filepage.h"
#include <QTimer>

class OpeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OpeWidget(QWidget *parent = nullptr);
    static OpeWidget& getinstance();
    Friend *getFriend();
    filePage *getfilePage();
    QTimer* myTimer;
signals:

private:
    QListWidget *m_pListW;
    Friend* m_pFriend;
    QStackedWidget *m_pSW;
    filePage* m_pFilePage;
private slots:
    void listenTransmit();
};

#endif // OPEWIDGET_H
