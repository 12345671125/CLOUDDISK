#ifndef TASK_H
#define TASK_H
#pragma once
#include <QObject>
#include "protocol.h"
#include <QMessageBox>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>
#include <QThread>
#include <QHostAddress>
#include <QWaitCondition>
#include <QMutex>
#include <QWaitCondition>

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(QObject *parent = NULL);
    ~Task();
    void uploadFileData();
    enum statusFlag {  //任务状态
        START,
        PAUSE,
        STOP,
    };

public slots:
    void uploadData();   //上传文件数据
    void uploadFileEnd(); //结束上传文件
    void taskStart();     //开始任务
    void taskThreadinit(QString curPath,QString absolutedPath, QString address, quint16 port,QWaitCondition* waitCondition);//对象初始化
    void uploadFile(QString curPath,QString absolutedFileName);// 发送上传文件请求
    void pauseTask(); //暂停任务
    void cancelTask(); //取消任务
    void recvMsg(); //接受服务器消息
private:

    QString FileName; //文件名
    qint64 totalFileSize; //文件大小
    qint64 curFileSize; //已上传文件大小
    QTcpSocket* clientSocket; //存放客户端socket
    QString serverFilePath;
    QFile* file;  //文件描述符
    QTimer* updataTimer = nullptr; //用于间隔时间发送数据
    QTimer* uploadEndTimer = nullptr; //用于发送上传文件结束请求
    QString absolutedPath; //保存文件按绝对路径
    QMutex* mutex; //锁  -- 用于线程挂起，实现任务暂停
    QWaitCondition* waitCondition; //信号量 -- 用于线程挂起 实现任务暂停
    int statusFlag;  //控制当前任务状态
signals:
    void updatePgBGUI(int percent);  //更新任务进度条
    void taskFin(); //任务结束信号1
    void finished(); //任务结束信号2
    void taskTerminate();
    void delUnaccomplishedFile(QString serFilePath);

};

#endif // TASK_H
