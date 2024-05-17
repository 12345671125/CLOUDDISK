/*protocol类为协议类，规定cs端之间的通信细节，用于cs端传输数据*/

#include "protocol.h"
#include <QDebug>

protocol::PDU *protocol::createPDU(uint uiMsgLen)
{
    uint PDULen = sizeof(protocol::PDU)+uiMsgLen; //协议总长(数据长度加原始协议长)
    protocol::PDU*  pdu = (protocol::PDU*)malloc(PDULen); //生成pdu协议对象
    if(pdu == nullptr)  //防御性编程 如果内存空间不足，退出程序
        exit(EXIT_FAILURE);
    else
        memset(pdu,0,PDULen); //初始化协议

    pdu->PDULen = PDULen; //将协议总长写入协议
    pdu->uiMsgLen = uiMsgLen;//将数据总长写入协议
    return pdu;
}
protocol::PDU protocol::PDU::default_request(uint Type,QString requestStr,uint MsgSize) //一个默认的请求模板，用于发送文字提示类请求
{
    protocol::PDU instance;
    memset(instance.caData,0,128);
    instance.PDULen = sizeof(protocol::PDU);
    instance.uiMsgLen = MsgSize;
    instance.uiMsgType = Type;

    int resquestStrSize = requestStr.toUtf8().size();

    memcpy(instance.caData,requestStr.toStdString().c_str(),resquestStrSize);
    return instance;
}

protocol::PDU protocol::PDU::default_respond(uint Type,QString respondStr,uint MsgSize) //一个默认的回复模板，用于发送文字提示类回复
{
    protocol::PDU instance;
    memset(instance.caData,0,128);
    instance.PDULen = sizeof(protocol::PDU);
    instance.uiMsgLen = MsgSize;
    instance.uiMsgType = Type;
    respondStr.append('\0');
    memcpy(instance.caData,respondStr.toStdString().c_str(),respondStr.toUtf8().size());
    return instance;
}



protocol::FileInfo* protocol::createFileInfo(const int iFileType, const char *caFileName, const qint64 FileSize,int pathLen)
{
    int totalSize = sizeof(protocol::FileInfo) + pathLen;
    QString strCaFileName = QString::fromUtf8(caFileName);
    if(strCaFileName.indexOf(QChar::Null) != -1)//如果末尾有 \0 去除末尾 \0
    {
        strCaFileName.truncate(strCaFileName.indexOf(QChar::Null));
    }
    int strCaFileNameSize =strCaFileName.toUtf8().size();
    protocol::FileInfo* fileInfo = (FileInfo*)malloc(totalSize);
    memset(fileInfo,0,totalSize);
    fileInfo->iFileType = iFileType;
    memcpy_s(fileInfo->caFileName,MAXFILENAME,strCaFileName.toStdString().c_str(),strCaFileNameSize);
    fileInfo->FileSize = FileSize;
    fileInfo->totalSize = totalSize;
    fileInfo->pathLen = pathLen;
    return fileInfo;
}
protocol::FileInfo_s *protocol::createFileInfo_s(const char* filename, const qint64 filesize, const char* uploadname)
{
    protocol::FileInfo_s* fileinfo = (protocol::FileInfo_s*)malloc(sizeof(protocol::FileInfo_s));
    memset(fileinfo->filename,0,MAXFILENAME);
    QString strFileName = QString::fromUtf8(filename);
    QString strUploadName = QString::fromUtf8(uploadname);
    if(strFileName.indexOf(QChar::Null) != -1)//如果末尾有 \0 去除末尾 \0
    {
        strFileName.truncate(strFileName.indexOf(QChar::Null));
    }
    memcpy_s(fileinfo->filename,MAXFILENAME,strFileName.toStdString().c_str(),strFileName.toUtf8().size());
    fileinfo->filesize = filesize;
    fileinfo->uploadTime = QDateTime();
    fileinfo->uploadTime = QDateTime::currentDateTime();
    memcpy_s(fileinfo->uploadname,64,strUploadName.toStdString().c_str(),strUploadName.toUtf8().size());
    return fileinfo;
}

protocol::FileInfoList* protocol::createFileInfoList(int length)
{
    protocol::FileInfoList*fileInfoList = (protocol::FileInfoList*)malloc(sizeof(protocol::FileInfoList));
    fileInfoList->FileListLength = length;
    fileInfoList->FileListSize = length * sizeof(protocol::FileInfo);
    fileInfoList->structSize = sizeof(protocol::FileInfoList) + fileInfoList->FileListSize;
    return fileInfoList;
}
