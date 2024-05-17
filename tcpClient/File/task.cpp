#include "task.h"
#include "PDU/protocol.h"
#include "commondata.h"
#include "commonfunc.h"

Task::Task(QObject *parent)
    : QObject{parent}
{

}

Task::~Task()  //析构函数
{
    qDebug()<<QThread::currentThreadId()<<"析构";

    delete this->mutex;
    QThread::currentThread()->quit();

}
void Task::taskThreadinit(QString strFilePath,QString strCurPath,QWaitCondition* waitCondition)
{
    qDebug()<<"taskThreadinit";

    this->statusFlag = Task::START;

    this->mutex = new QMutex(); //锁初始化

    this->strFilePath = {};

    this->waitCondition = waitCondition;

    if(strFilePath.isEmpty()){  //
        return;
    }

    this->strFilePath = strFilePath; //将本地文件绝对路径保存
    this->strCurPath = strCurPath; //读取当前路径并保存

    /*以下为成员数据初始化*/
    this->objFile = new QFile(this->strFilePath,this); //初始化文件类

    if(!this->objFile->exists())
    {
        return;
    }



    this->uploadEndTimer = new QTimer(this); //初始化timer
    this->updataTimer = new QTimer(this); //初始化timer

    //初始化socket和网络连接
    this->clientSocket = new QTcpSocket(this);//初始化socket
    this->clientSocket->setReadBufferSize(4096); //设置发送缓冲区大小为4096kb
    QString ip = *reinterpret_cast<QString*>(CommonFunc::QueryData(SERVERIP));
    qint16 port = (*reinterpret_cast<QString*>(CommonFunc::QueryData(SERVERPORT))).toShort();
    this->clientSocket->connectToHost(QHostAddress(ip),port);  //尝试连接服务器
    this->clientSocket->waitForConnected(); //等待与服务器进行连接

    //截取文件名
    QString strFileName = CommonFunc::getFileNameFromAbsolutePath(this->strFilePath);
    if(strFileName.isEmpty())
    {
        return;  //防御性编程
    }

    this->strFileName = strFileName; //保存文件名

    this->totalFileSize = this->objFile->size(); //读取文件总大小

    this->uploadedFileSize = 0; //初始化已上传大小

    this->uploadEndTimer->setInterval(250); //此定时器用来监听上传已结束消息

    this->updataTimer->setInterval(20); //此定时器用来发送上传消息

    preUploadFile();

}

void Task::preUploadFile()
{
    if(this->strFileName.isEmpty() || this->strCurPath.isEmpty())
    {
        return;
    }

    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(recvMsg())); //连接读取服务器响应信号槽

    /*以下为生成文件信息结构体*/

    int strCurPathSize = CommonFunc::getTcpDataSize(strCurPath);

    protocol::FileInfo *fileInfo = protocol::createFileInfo(protocol::FILE_TYPE_FILE,this->strFileName.toStdString().c_str(),
                                                            this->totalFileSize,strCurPathSize);
    memcpy_s(fileInfo->savaPath,fileInfo->pathLen,this->strCurPath.toStdString().c_str(),strCurPathSize);

    /*以下为生成协议结构体*/
    protocol::PDU* pdu = protocol::createPDU(fileInfo->totalSize);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;

    memcpy_s((char*)pdu->caMsg,pdu->uiMsgLen,(char*)fileInfo,fileInfo->totalSize); //将文件信息写入协议

    this->clientSocket->write((char*)pdu,pdu->PDULen);//使用socket发送协议到服务器
    this->clientSocket->waitForBytesWritten();

    free(pdu);
    pdu = nullptr;
    free(fileInfo);
    fileInfo = nullptr;
}
void Task::uploadFileData()
{
    if( this->strFileName.isEmpty() || !this->objFile->open(QIODevice::ReadOnly)){    //如果文件打开失败或者文件名为空
        // QMessageBox::warning(NULL,"上传文件","打开文件失败");

        this->objFile = NULL;

        return;
    }
    Sleep(100);

    this->updataTimer->start();  //开启发送文件数据定时器,开始发送文件数据

}

void Task::fileDataScheduledSender()
{
    // qDebug()<<"uploadData";
    char buffer[4096] = {}; //缓冲区
    qint64 ret = 0;
    protocol::PDU* pdu = nullptr;

    ret = this->objFile->read(buffer,4096); //循环的中文件中读数据,一次读4096KB
    if(ret > 0 && ret <= 4096){ //如果文件中余有字节
        pdu = protocol::createPDU(ret);
        pdu->uiMsgType = protocol::ENUM_MSG_TYPE_UPLOADBEG_FILE_REQUEST;
        memcpy(pdu->caData,this->strFileName.toStdString().c_str(),this->strFileName.length());
        memcpy((char*)pdu->caMsg,buffer,ret);
        this->clientSocket->write((char*)pdu,pdu->PDULen);
        this->clientSocket->waitForBytesWritten();

        this->uploadedFileSize += ret;

        emit updatePgBGUI(int(((double)this->uploadedFileSize/totalFileSize) * 100));

    }else if(ret == 0 && this->uploadedFileSize == this->totalFileSize) //如果已经到达文件尾
    {
        this->updataTimer->stop(); //停止上传数据定时器
        this->objFile->close(); //关闭文件
        delete this->objFile;
        this->objFile = nullptr;
        this->uploadEndTimer->start();
    }
}

void Task::uploadFileEnd()
{
    // qDebug()<<"uploadFileEnd";

    this->uploadEndTimer->stop();

    emit taskFin();
    //qDebug()<<QThread::currentThreadId()<<"emit finished";
    emit finished();
//    this->~Task();

    //    qDebug()<<"Task::uploadFileEnd";
}

void Task::pauseTask()  //暂停任务
{
     qDebug()<<"pauseTask";
//    qDebug()<<"1pauseTask";
    if(this->statusFlag == Task::START){  //如果文件状态为开始
        this->statusFlag = Task::PAUSE; //将文件状态改为暂停
        while (this->statusFlag == Task::PAUSE) {  //进入循环

            this->mutex->lock();   //上锁
            this->updataTimer->stop(); //停止上传数据
            this->waitCondition->wait(this->mutex);//阻塞线程
            this->mutex->unlock();//解锁

            break;
        }
    }else if(this->statusFlag == Task::PAUSE){
        this->statusFlag = Task::START;
        this->updataTimer->start();
    }
}

void Task::cancelTask()
{
    QString cancelFilePath = this->serverFilePath;
    emit delUnaccomplishedFile(cancelFilePath);
    this->~Task();
}

void Task::recvMsg()
{
    //qDebug()<<"recvMsg";
    uint PDULen = 0;
    this->clientSocket->read((char*)&PDULen,sizeof(uint)); //para1:数据存放的地址，para2:读出的数据大小，读出uint字节的大小，这个uint为总的数据大小
    uint uiMsgLen = PDULen - sizeof(protocol::PDU); //用总的数据大小减去protocol::PDU结构体的默认，获取实际消息长度
    protocol::PDU* pdu = protocol::createPDU(uiMsgLen); //创建协议结构体，用于接收数据
    this->clientSocket->read((char*)pdu + sizeof (uint),PDULen-sizeof (uint)); /*让指针偏移来读取剩下的数据大小,先将pdu的指针类型转换为了char类型的指针*/
    switch (pdu->uiMsgType) {
    case protocol::ENUM_MSG_TYPE_UPLOADGET_FILE_RESPOND:
    {
        char respondMsg[64] = {"\0"};
        memcpy(respondMsg,pdu->caData,64);
        //qDebug()<<respondMsg;
        if(strcmp(respondMsg,"FILE EXIST") == 0){
//            this->uploadFileEnd();
            emit taskTerminate();
            emit taskFin();
            emit finished();
            free(pdu);
            pdu = nullptr;
//            this->~Task();
        }else{
            connect(updataTimer,SIGNAL(timeout()),this,SLOT(fileDataScheduledSender())); //连接定时器信号和对应的槽函数
            connect(uploadEndTimer,SIGNAL(timeout()),this,SLOT(uploadFileEnd())); //连接定时器信号和对应的槽函数
            this->taskStart();
        }
        break;
    }
    default:
        break;
    }
}
void Task::taskStart()
{
    //qDebug()<<"taskStart";
    this->uploadFileData();
}
