#include "downloadtask.h"
#include "PDU/protocol.h"

#include "commonfunc.h"
#include "commondata.h"

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
    delete this->objDownloadFile;
    //    delete this->waitCondition;
    delete this->mutex;
    QThread::currentThread()->quit();
    //    QThread::currentThread()->wait();
}
void downLoadTask::taskThreadinit(QString ServerfilePath,QString absolutedFileName,QString address,quint16 port,QWaitCondition* waitCondition)
{
    if(!this->absolutedFileName.isEmpty()){
        return;
    }
    this->statusFlag = downLoadTask::START; //初始化下载任务线程为开始
    this->mutex = new QMutex(); //锁初始化
    this->waitCondition = waitCondition;
    this->totalFileSize = 0;
    this->curFileSize = 0;
    this->absolutedFileName = absolutedFileName;

    /*以下为成员数据初始化*/
    this->objDownloadFile = new QFile(absolutedFileName,this); //创建对应文件
    //与服务器建立连接
    this->clientSocket = new QTcpSocket(this);
    this->clientSocket ->setSocketOption(QTcpSocket::KeepAliveOption,true); //设置socket连接属性为保持连接
    this->clientSocket->setReadBufferSize(8192);
    this->clientSocket->connectToHost(QHostAddress(address),port); //使用客户端的socketDecs来构造任务类的socket,使其单独与服务器通信;
    this->clientSocket->waitForConnected();
    QObject::connect(this->clientSocket,SIGNAL(readyRead()),this,SLOT(getFileData()));
    this->strFileName = CommonFunc::getFileNameFromAbsolutePath(absolutedFileName);

    this->objDownloadFile->open(QIODevice::Append);

    QString strServerfilePath =  ServerfilePath;
    CommonFunc::removeNullCharacters(strServerfilePath);

    int strServerFilePathSize = CommonFunc::getTcpDataSize(strServerfilePath);

    protocol::PDU* pdu = protocol::createPDU(strServerFilePathSize);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_DOWNLOADREADY_FILE_REQUEST;
    memcpy_s(pdu->caMsg,pdu->uiMsgLen,strServerfilePath.toStdString().c_str(),strServerFilePathSize);

    this->clientSocket->write((char*)pdu,pdu->PDULen);
    this->clientSocket->waitForBytesWritten();
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
        qDebug()<<strPercent.toDouble() * 100;
        emit updatePgBGUI(strPercent.toDouble());
        int ret = this->objDownloadFile->write((char*)pdu->caMsg,pdu->uiMsgLen);
        this->curFileSize += ret;
        this->objDownloadFile->waitForBytesWritten(3000);
        this->sendDownLoadTaskReq(curFileSize);
        // this->clientSocket->flush();
        qDebug()<<curFileSize;
    }else if(pdu->uiMsgType == protocol::ENUM_MSG_TYPE_DOWNLOADFIN_FILE_RESPOND){
            objDownloadFile->close();
            free(pdu);
            pdu = nullptr;
            emit taskFin();
            emit finished();
            return;
    }
    free(pdu);
    pdu = nullptr;
}

void downLoadTask::sendDownLoadTaskReq(int curFileSize)
{
    QString strCurFileSize = QString("%1").arg(curFileSize);
    CommonFunc::removeNullCharacters(strCurFileSize);
    protocol::PDU pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_UPLOADBEGING_FILE_REQUEST,strCurFileSize);
    this->clientSocket->write((char*)&pdu,pdu.PDULen);
    this->clientSocket->waitForBytesWritten();
}
