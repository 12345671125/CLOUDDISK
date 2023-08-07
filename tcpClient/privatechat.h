#ifndef PRIVATECHAT_H
#define PRIVATECHAT_H

#include "protocol.h"
#include <QWidget>

namespace Ui {
class PrivateChat;
}

class PrivateChat : public QWidget
{
    Q_OBJECT

public:
    explicit PrivateChat(QWidget *parent = nullptr);
    ~PrivateChat();
    void setChatName(QString strName);
    static PrivateChat &getInstance();
    void showMsg(protocol::PDU* pdu = NULL); //显示消息

private slots:
    void on_sendPtn_clicked();

private:
    Ui::PrivateChat *ui;
    QString m_strChatName;
};

#endif // PRIVATECHAT_H
