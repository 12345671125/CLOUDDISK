#include "itemui.h"
#include "opewidget.h"
#include "downloadtask.h"
#include "task.h"
#include "PDU/protocol.h"
#include <MainWIndow/opewidget.h>
#include <File/filepage.h>
#include "File/publicFilePage.h"

#include"../commonfunc.h"
#include"../commondata.h"

ItemUI::ItemUI(QString FileName,int width,int height,QWidget *parent)
    : QWidget{parent}
{

    /*以下为ui初始化*/
    this->item =new QListWidgetItem();
    item->setSizeHint(QSize(width-20,40));
    this->itemWidget = new QWidget(this);
    this->progressBar =new QProgressBar(this);
    this->stopBtn =new QPushButton("停止",this);
    this->cancelBtn =new QPushButton("取消",this);
    this-> btnLayout =new QHBoxLayout();
    this-> mainVLayout = new QVBoxLayout();
    this->WaitCondition = new QWaitCondition();
    btnLayout->addWidget(stopBtn);
    btnLayout->addWidget(cancelBtn);
    this->fileNameLab =new QLabel(this);
    fileNameLab->setText(FileName);
    this->itemMainLay =new QHBoxLayout();
    this->progressBar->setTextVisible(true);
    itemMainLay->addWidget(fileNameLab);
    itemMainLay->addWidget(progressBar);
    itemMainLay->addLayout(btnLayout);
    mainVLayout->addLayout(itemMainLay);
    itemWidget->setLayout(mainVLayout);
}

ItemUI::~ItemUI()
{
    delete this->item;
    //delete this->btnLayout;
    //delete this-> mainVLayout;
    delete this->WaitCondition;
    //delete this->itemMainLay;
}
QWidget *ItemUI::getWidget()  //返回类中的Widget对象指针
{
    return this->itemWidget;
}
QListWidgetItem *ItemUI::getListWidgetItem()  //返回类中的Item对象指针
{
    return this->item;
}

void ItemUI::createTask(QString curPath,QString absolutedPath)   //创建任务函数
{
    qDebug()<<"main ThreadID:"<<QThread::currentThreadId();
    Task* task = new Task(NULL); //创建任务对象
    this->task = task;
    QThread* workThread = new QThread(this); //创建工作线程  --使用线程池可以减少创建销毁线程的开销
    this->workThread = workThread;
    /*以下为信号和槽函数的连接*/

//    QObject::connect(this,SIGNAL(uploadTaskStart()),task,SLOT(taskStart()));
    QObject::connect(this,SIGNAL(uploadTaskThreadinit(QString,QString,QWaitCondition*)),task,SLOT(taskThreadinit(QString,QString,QWaitCondition*)));
//    QObject::connect(this,SIGNAL(uploadTaskStart()),task,SLOT(taskStart()));
    QObject::connect(this->task,SIGNAL(updatePgBGUI(int)),this,SLOT(updatePgBGUI(int))); //单例模式在成员函数中重复信号槽的连接会导致信号的重复发送
    QObject::connect(this->task,SIGNAL(taskFin()),OpeWidget::getinstance()->getfilePage(),SLOT(flushFile()));
    QObject::connect(this->task,SIGNAL(taskFin()),OpeWidget::getinstance()->getPublicFilePage(),SLOT(flushFile()));
    QObject::connect(this->task,SIGNAL(finished()),workThread,SLOT(quit()));
    QObject::connect(this->task,SIGNAL(taskFin()),this,SLOT(taskFin()));
    QObject::connect(this->task,SIGNAL(taskTerminate()),this,SLOT(TaskTerminate()));
    QObject::connect(stopBtn,SIGNAL(clicked()),this->task,SLOT(pauseTask()));
    QObject::connect(cancelBtn,SIGNAL(clicked()),this->task,SLOT(cancelTask()));
    QObject::connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancelTask()));
    QObject::connect(stopBtn,SIGNAL(clicked()),this,SLOT(switchBtnText()));
    QObject::connect(task,SIGNAL(delUnaccomplishedFile(QString)),this,SLOT(delUnaccomplishedFIle(QString)));
    task->moveToThread(workThread);  //将task对象移入workThread线程中
    QString address = *reinterpret_cast<QString*>(CommonFunc::QueryData(SERVERIP));  //获取服务器地址
    qint16 port = (*reinterpret_cast<QString*>(CommonFunc::QueryData(SERVERPORT))).toShort();  //获取服务器ip
    workThread->start(); //启动线程
    emit uploadTaskThreadinit(absolutedPath,curPath,this->WaitCondition); //发送任务初始化信号,使任务对象初始化
//    emit uploadTaskStart(); //发送开始任务信号

}

void ItemUI::updatePgBGUI(int percent)   //更新任务进度条
{
    this->curFileSize = curFileSize;
    this->progressBar->setValue(percent);
}

void ItemUI::cancelTask()   //取消任务
{
    emit cancelTask(this->item);
}

void ItemUI::switchBtnText(void)  //切换按钮文本
{
    if(this->stopBtn->text() == "停止")
        this->stopBtn->setText("开始");
    else{
        this->stopBtn->setText("停止");
        WaitCondition->wakeAll();
    }

}

void ItemUI::taskFin()
{
    this->stopBtn->setEnabled(false);
    this->cancelBtn->setText("关闭");
}

void ItemUI::createDownloadTask(QString ServerfilePath,QString absolutedFileName)
{
    //qDebug()<<"main ThreadID:"<<QThread::currentThreadId();
    downLoadTask *task = new downLoadTask(NULL); //创建任务对象
    this->downloadTask = task;
    QThread* workThread = new QThread(NULL); //创建工作线程  --使用线程池可以减少创建销毁线程的开销
    this->workThread = workThread;
    /*以下为信号和槽函数的连接*/

//    QObject::connect(this,SIGNAL(downloadTaskStart()),task,SLOT(taskStart()));
    QObject::connect(this,SIGNAL(downloadTaskThreadinit(QString,QString,QString,quint16,QWaitCondition*)),task,SLOT(taskThreadinit(QString,QString,QString,quint16,QWaitCondition*)));
    QObject::connect(this->downloadTask,SIGNAL(updatePgBGUI(int)),this,SLOT(updatePgBGUI(int))); //单例模式在成员函数中重复信号槽的连接会导致信号的重复发送
    //QObject::connect(this->downloadTask,SIGNAL(taskFin()),OpeWidget::getinstance()->getfilePage(),SLOT(uploadFileEnd()));
    QObject::connect(this->downloadTask,SIGNAL(finished()),workThread,SLOT(quit()));
    QObject::connect(this->downloadTask,SIGNAL(taskFin()),this,SLOT(taskFin()));
    QObject::connect(stopBtn,SIGNAL(clicked()),this->downloadTask,SLOT(pauseTask()));
    QObject::connect(cancelBtn,SIGNAL(clicked()),this->downloadTask,SLOT(cancelTask()));
    QObject::connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancelTask()));
    QObject::connect(stopBtn,SIGNAL(clicked()),this,SLOT(switchBtnText()));

    task->moveToThread(workThread);  //将task对象移入workThread线程中
    QString address = *reinterpret_cast<QString*>(CommonFunc::QueryData(SERVERIP));  //获取服务器地址
    qint16 port = (*reinterpret_cast<QString*>(CommonFunc::QueryData(SERVERPORT))).toShort();  //获取服务器ip
    workThread->start(); //启动线程
    //qDebug()<<"void ItemUI::createDownloadTask(QString absolutedFileName)";
    emit downloadTaskThreadinit(ServerfilePath,absolutedFileName,address,port,this->WaitCondition); //发送任务初始化信号,使任务对象初始化
    //    emit downloadTaskStart(); //发送开始任务信号
}

void ItemUI::TaskTerminate()
{
    QMessageBox::information(nullptr,"文件下载","文件已存在，上传终止");
    emit taskFin();
}

void ItemUI::delUnaccomplishedFIle(QString serFilePath)
{
    protocol::PDU* pdu = protocol::createPDU(serFilePath.length());
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_CANCEL_UPLOAD_FILE_REQUEST;
    memcpy(pdu->caMsg,serFilePath.toStdString().c_str(),serFilePath.length());
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu,pdu->PDULen);
    tcpSocket->waitForBytesWritten();
    free(pdu);
    pdu = nullptr;
}


