#ifndef DOWNLOADTASK_H
#define DOWNLOADTASK_H

#include <QObject>
#include <QMessageBox>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>
#include <QThread>
#include <QHostAddress>
#include <QWaitCondition>
#include <QMutex>
#include <QWaitCondition>

QT_BEGIN_INCLUDE_NAMESPACE
namespace protocol {class PDU;};
QT_END_INCLUDE_NAMESPACE

class downLoadTask : public QObject
{
    Q_OBJECT
public:
    explicit downLoadTask(QObject *parent = NULL);
    ~downLoadTask();
    void uploadFileData();
    enum statusFlag {  //任务状态
        START,
        PAUSE,
        STOP,
        };

public slots:
//    void taskStart();     //开始任务
    void taskThreadinit(QString ServerfilePath,QString absolutedFileName, QString address, quint16 port,QWaitCondition* waitCondition);//对象初始化
//    void downloadFile(QString absolutedFileName);// 发送上传文件请求
    void pauseTask(); //暂停任务
    void cancelTask(); //取消任务
    void getFileData(); //
    void sendDownLoadTaskReq(int curFileSize);
private:
    QString strFileName; //文件名
    qint64 totalFileSize; //文件大小
    qint64 curFileSize; //已上传文件大小
    QTcpSocket* clientSocket; //存放客户端socket
    QFile* objDownloadFile;  //文件描述符
    QString absolutedFileName; //保存文件按绝对路径
    QMutex* mutex; //锁  -- 用于线程挂起，实现任务暂停
    QWaitCondition* waitCondition; //信号量 -- 用于线程挂起 实现任务暂停
    int statusFlag;  //控制当前任务状态
signals:
    void updatePgBGUI(int percent);  //更新任务进度条
    void taskFin(); //任务结束信号1
    void finished(); //任务结束信号2
};

#endif // DOWNLOADTASK_H
