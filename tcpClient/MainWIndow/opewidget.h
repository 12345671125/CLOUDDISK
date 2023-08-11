#ifndef OPEWIDGET_H
#define OPEWIDGET_H
#pragma once

#include <QWidget>
#include <QListWidget>
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout>
class filePage;
class Friend;

class OpeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OpeWidget(QWidget *parent = nullptr);
    ~OpeWidget();
    static OpeWidget& getinstance(QWidget *parent = nullptr);
    Friend *getFriend();
    filePage *getfilePage();
    QTimer* myTimer;
signals:

private:
    QListWidget *m_pListW;
    Friend* m_pFriend;
    QStackedWidget *m_pSW;
    filePage* m_pFilePage;
    QListWidgetItem* FriendItem;
    QListWidgetItem* FileItem;
private slots:
    void listenTransmit();
};

#endif // OPEWIDGET_H
