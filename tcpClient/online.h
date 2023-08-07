/*在线页面类*/

#ifndef ONLINE_H
#define ONLINE_H
#pragma once
#include <QWidget>
#include "protocol.h"

namespace Ui {
class Online;
}

class Online : public QWidget
{
    Q_OBJECT

public:
    explicit Online(QWidget *parent = nullptr);
    ~Online();
    void showUser(protocol::PDU* pdu);
    void showSearchUser(protocol::PDU* pdu);

private slots:
    void on_addFriendButton_clicked();

private:
    Ui::Online *ui;
};

#endif // ONLINE_H
