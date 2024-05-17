/*主界面类，用于初始化程序配置*/

#ifndef SERVERWIN_H
#define SERVERWIN_H
#pragma once
#include <QMainWindow>
#include <QDebug>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QFile>
#include "mytcpserver.h"

QT_BEGIN_NAMESPACE
namespace Ui { class serverWin; }
QT_END_NAMESPACE

class serverWin : public QMainWindow
{
    Q_OBJECT

public:
    serverWin(QWidget *parent = nullptr);
    ~serverWin();

private:
    Ui::serverWin *ui;
    QString ip;
    quint16 port;
    QTcpSocket serverSocket;

    void configInit(); //初始化配置文件
    void createServer(); //创建服务器
    void createPablicFileDir();
};
#endif // SERVERWIN_H
