#include "downloadtask.h"

downLoadTask::downLoadTask(QObject *parent)
    : QObject{parent}
{
    /*ui初始化*/
    this->absolutedFileName = QString();
    this->absolutedFileName.clear();

}

downLoadTask::~downLoadTask()  //析构函数
{
    qDebug()<<QThread::currentThreadId()<<"析构";
    delete this->clientSocket;
    delete this->file;
    //    delete this->waitCondition;
    delete this->mutex;
    QThread::currentThread()->quit();
    //    QThread::currentThread()->wait();
}
void downLoadTask::taskThreadinit(QString ServerfilePath,QString absolutedFileName,QString address,quint16 port,QWaitCondition* waitCondition)
{
    this->statusFlag = downLoadTask::START;
    this->mutex = new QMutex(); //锁初始化
    this->waitCondition = waitCondition;
    if(!this->absolutedFileName.isEmpty()){
        return;
    }
    this->absolutedFileName = this->absolutedFileName;

    /*以下为成员数据初始化*/
    this->file = new QFile(absolutedFileName,this);
    this->clientSocket = new QTcpSocket(this);
    this->clientSocket->connectToHost(QHostAddress(address),port); //使用客户端的socketDecs来构造任务类的socket,使其单独与服务器通信;
//    this->clientSocket->setReadBufferSize(4096);
    QString loFileName = absolutedFileName.mid(absolutedFileName.lastIndexOf("/")+1,absolutedFileName.length()-1); //文件名
    //    qDebug()<< absolutedPath;
    QObject::connect(this->clientSocket,SIGNAL(readyRead()),this,SLOT(getFileData()));
    this->FileName = loFileName.append('\0');
    this->totalFileSize = 0;
    this->curFileSize = 0;
    this->curFileSize = 0;
    QString fileName = absolutedFileName.mid(absolutedFileName.lastIndexOf('/')+1,absolutedFileName.length()-1);

    protocol::PDU* pdu = protocol::createPDU(ServerfilePath.length()+1);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
    memcpy(pdu->caMsg,ServerfilePath.toStdString().c_str(),pdu->uiMsgLen);
    this->clientSocket->write((char*)pdu,pdu->PDULen);
//    downloadFile(absolutedFileName);

//    QString loFileName = absolutedFileName.mid(absolutedFileName.lastIndexOf("/")+1,absolutedFileName.length()-1);
    loFileName = loFileName.append('\0');
    QFile* downloadFile = new QFile(absolutedFileName);
    this->file = downloadFile;
    this->file->open(QIODevice::WriteOnly);

//    qDebug()<<"taskThreadinit";
}

//void downLoadTask::downloadFile(QString absolutedfileName)
//{
//    qDebug()<<"void downLoadTask::downloadFile(QString absolutedfileName)";
//    QString loFileName = absolutedfileName.mid(absolutedfileName.lastIndexOf("/")+1,absolutedfileName.length()-1);
//    loFileName = loFileName.append('\0');

//    protocol::PDU* pdu = protocol::createPDU(0);
//    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_DOWNLOADREADY_FILE_REQUEST;
//    this->clientSocket->write((char*)pdu,pdu->PDULen);
//    QFile* downloadFile = new QFile(absolutedfileName);
//    this->file = downloadFile;
//}
void downLoadTask::pauseTask()  //暂停任务
{
    //    qDebug()<<"1pauseTask";
    if(this->statusFlag == downLoadTask::START){  //如果文件状态为开始
        this->statusFlag = downLoadTask::PAUSE; //将文件状态改为暂停
        while (this->statusFlag == downLoadTask::PAUSE) {  //进入循环

            this->mutex->lock();   //上锁
            this->waitCondition->wait(this->mutex);//阻塞线程
            this->mutex->unlock();//解锁
            break;
        }
    }else if(this->statusFlag == downLoadTask::PAUSE){
        this->statusFlag = downLoadTask::START;
    }
}

void downLoadTask::cancelTask()
{
    this->~downLoadTask();
}

void downLoadTask::getFileData()
{
    uint PDULen = 0;
    this->clientSocket->read((char*)&PDULen,sizeof(uint)); //para1:数据存放的地址，para2:读出的数据大小，读出uint字节的大小，这个uint为总的数据大小
    uint uiMsgLen = PDULen - sizeof(protocol::PDU); //用总的数据大小减去protocol::PDU结构体的默认，获取实际消息长度
    protocol::PDU* pdu = protocol::createPDU(uiMsgLen); //创建协议结构体，用于接收数据
    this->clientSocket->read((char*)pdu + sizeof (uint),PDULen-sizeof (uint)); //*让指针偏移来读取剩下的数据大小,先将pdu的指针类型转换为了char类型的指针，
    if(pdu->uiMsgType == protocol::ENUM_MSG_TYPE_DOWNLOADBEG_FILE_RESPOND){
        char str[64] = {'\0'};
        memcpy(str,pdu->caData,64);
        QString strPercent = str;
        emit updatePgBGUI(strPercent.toDouble());
        file->write((char*)pdu->caMsg,pdu->uiMsgLen);
        this->clientSocket->flush();
        qDebug()<<pdu->uiMsgLen;
    }else if(pdu->uiMsgType == protocol::ENUM_MSG_TYPE_DOWNLOADFIN_FILE_RESPOND){
            qDebug()<<"ENUM_MSG_TYPE_DOWNLOADFIN_FILE_RESPOND";
            file->close();
    }
    free(pdu);
    pdu = nullptr;
}
