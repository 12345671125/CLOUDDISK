#include "task.h"

Task::Task(QObject *parent)
    : QObject{parent}
{
    /*ui初始化*/
    this->absolutedPath = QString();
    this->absolutedPath.clear();

}

Task::~Task()  //析构函数
{
    qDebug()<<QThread::currentThreadId()<<"析构";
//    delete this->waitCondition;
    delete this->mutex;
    QThread::currentThread()->quit();
//    QThread::currentThread()->wait();
}
void Task::taskThreadinit(QString curPath,QString absolutedPath,QString address,quint16 port,QWaitCondition* waitCondition)
{
    qDebug()<<"taskThreadinit";
    this->statusFlag = Task::START;
    this->mutex = new QMutex(); //锁初始化
    this->waitCondition = waitCondition;
    if(!this->absolutedPath.isEmpty()){
        return;
    }
    this->absolutedPath = this->absolutedPath;

    /*以下为成员数据初始化*/
    this->file = new QFile(absolutedPath,this);
    this->clientSocket = new QTcpSocket(this);
//    this->clientSocket->setReadBufferSize(4096);
    this->uploadEndTimer = new QTimer(this);
    this->updataTimer = new QTimer(this);
    this->clientSocket->connectToHost(QHostAddress(address),port); //使用客户端的socketDecs来构造任务类的socket,使其单独与服务器通信;
    QString loFileName = absolutedPath.mid(absolutedPath.lastIndexOf("/")+1,absolutedPath.length()-1);
    //    qDebug()<< absolutedPath;
    this->FileName = loFileName.append('\0');
    this->totalFileSize = file->size();
    this->curFileSize = 0;
    this->uploadEndTimer->setInterval(250);
    this->updataTimer->setInterval(15);
    this->curFileSize = 0;
    uploadFile(curPath,absolutedPath);
}

void Task::uploadFile(QString curPath,QString absolutedfileName)
{
    qDebug()<<"uploadFile";
    QFile file = QFile(absolutedfileName); //创建文件对象 获取文件句柄
    QString loFileName = absolutedfileName.mid(absolutedfileName.lastIndexOf("/")+1,absolutedfileName.length()-1);
    loFileName = loFileName.append('\0');
    this->serverFilePath = curPath + '/' + loFileName;
//    qDebug()<<loFileName;
    QString curpath = curPath;
    curPath.append('\0');

    /*以下为生成文件信息结构体*/

    protocol::FileInfo *fileInfo = protocol::createFileInfo(protocol::FILE_TYPE_FILE,loFileName.toStdString().c_str(),
                                                            file.size(),curpath.length());
    memcpy(fileInfo->savaPath,curPath.toStdString().c_str(),curPath.length());

    /*以下为生成协议结构体*/
    protocol::PDU* pdu = protocol::createPDU(fileInfo->totalSize);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;

    memcpy((char*)pdu->caMsg,(char*)fileInfo,fileInfo->totalSize); //将文件信息写入协议
    this->clientSocket->write((char*)pdu,pdu->PDULen);//使用socket发送协议到服务器
    free(pdu);
    pdu = nullptr;
    free(fileInfo);
    fileInfo = nullptr;
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(recvMsg()));
}
void Task::uploadFileData()
{
    qDebug()<<"uploadFileData";
    if(!this->file->open(QIODevice::ReadOnly) || this->FileName.isEmpty()){    //如果文件打开失败或者文件名为空
        QMessageBox::warning(NULL,"上传文件","打开文件失败");
        this->file = nullptr;
        return;
    }

    this->updataTimer->start();  //否则开启发送文件数据定时器,开始发送文件

}

void Task::uploadData()
{
    qDebug()<<"uploadData";
    char* pBuffer = new char[4096]; //数据缓冲区
    protocol::PDU* pdu = nullptr;
    qint64 ret = 0;
    memset(pBuffer,0,4096);
    ret = this->file->read(pBuffer,4096); //循环的中文件中读数据,一次读4096B
    //    qDebug()<<ret;
    if(ret > 0 && ret <= 4096){ //如果文件中余有字节
        pdu = protocol::createPDU(ret);
        pdu->uiMsgType = protocol::ENUM_MSG_TYPE_UPLOADBEG_FILE_REQUEST;
        memcpy(pdu->caData,this->FileName.toStdString().c_str(),this->FileName.length());
        memcpy((char*)pdu->caMsg,pBuffer,ret);
        this->clientSocket->write((char*)pdu,pdu->PDULen);
        this->clientSocket->flush();
        this->curFileSize+=ret;
        emit updatePgBGUI(int(((double)curFileSize/totalFileSize) * 100));
    }else if(ret == 0) //如果已经到达文件尾
    {
        this->updataTimer->stop(); //停止上传数据定时器
        this->file->close(); //关闭文件
        delete file;
        this->file = nullptr;
        this->uploadEndTimer->start();
    }
}

void Task::uploadFileEnd()
{
    qDebug()<<"uploadFileEnd";
    this->uploadEndTimer->stop();
    emit taskFin();
    qDebug()<<QThread::currentThreadId()<<"emit finished";
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
    qDebug()<<"recvMsg";
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
        qDebug()<<respondMsg;
        if(strcmp(respondMsg,"FILE EXIST") == 0){
//            this->uploadFileEnd();
            emit taskTerminate();
            emit taskFin();
            emit finished();
            free(pdu);
            pdu = nullptr;
//            this->~Task();
        }else{
            connect(updataTimer,SIGNAL(timeout()),this,SLOT(uploadData())); //连接定时器信号和对应的槽函数
            connect(uploadEndTimer,SIGNAL(timeout()),this,SLOT(uploadFileEnd())); //连接定时器信号和对应的槽函数
            this->taskStart();
        }
    }

        break;
    default:
        break;
    }
}
void Task::taskStart()
{
    qDebug()<<"taskStart";
    this->uploadFileData();
}
