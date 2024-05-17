#ifndef PUBLICCHATWIDGET_H
#define PUBLICCHATWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTextEdit;
class QVBoxLayout;
class msgWidget;
namespace protocol {class PDU;};
QT_END_NAMESPACE
class PublicChatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PublicChatWidget(QWidget *parent = nullptr);
    static PublicChatWidget& getInstance(QWidget *parent = nullptr);
    void showPublicChat(protocol::PDU* pdu);
    void setStyle(QString style);

public slots:
    void publicChat();

private:
    QTextEdit* showMsgEdit;
    QVBoxLayout* mainVBL;
    msgWidget* msgWidget;
    int width;
    int height;

signals:
    void sendPublicChatMsg(QString strUsername,QString msg);
};

#endif // PUBLICCHATWIDGET_H
