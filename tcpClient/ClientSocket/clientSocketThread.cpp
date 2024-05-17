/*client类 用于处理服务器端响应*/

#include "clientSocketThread.h"
#include "ResponseHandler/responseHandler.h"
#include "filepage.h"
#include "MainWindow/opewidget.h"
#include <Friend/friend.h>
#include <QCryptographicHash> //qt内置加密库
#include <QMessageBox>
#include <QTcpSocket>
#include<QByteArray>

#include "../commonfunc.h"
#include "../commondata.h"

clientSocketThread::clientSocketThread(QObject *parent)
    : QObject(parent)
{
}

void clientSocketThread::connectToSlot()
{
    responseHandler* resHandler = reinterpret_cast<responseHandler*>(CommonFunc::QueryData(RESPONSEHANDLE));
    QObject::connect(this,SIGNAL(m_sLocalPathRes(protocol::PDU*)),resHandler,SLOT(handleLoginPathRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sRegistRes(protocol::PDU*)),resHandler,SLOT(handleRegistRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sPublicFilePathRes(protocol::PDU*)),resHandler,SLOT(handlePublieFilePathRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sLoginRes(protocol::PDU*)),resHandler,SLOT(handleLoginRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sAllOnlineRes(protocol::PDU*)),resHandler,SLOT(handleAllOnlineRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sSearchUserRes(protocol::PDU*)),resHandler,SLOT(handSearchUserRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sAddFriendReq(protocol::PDU*)),resHandler,SLOT(handleAddFriendReq(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sAddFriendRes(protocol::PDU*)),resHandler,SLOT(handAddFriendRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL( m_sFlushFriendRes(protocol::PDU*)),resHandler,SLOT(handleFlushFriendRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sPrivateChatTrans(protocol::PDU*)),resHandler,SLOT(handlePrivateChatTrans(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sPublicChatTrans(protocol::PDU*)),resHandler,SLOT(handlePublicChatTrans(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sCreateDirRes(protocol::PDU*)),resHandler,SLOT(handleCreateDirRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sFlushFileRes(protocol::PDU*)),resHandler,SLOT(handleFlushFileRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sDeleteFileRes(protocol::PDU*)),resHandler,SLOT(handleDeleteFileRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sDeleteDirRes(protocol::PDU*)),resHandler,SLOT(handleDeleteDirRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sRenameFileRes(protocol::PDU*)),resHandler,SLOT(handleRenameFileRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sUploadFileFinRes(protocol::PDU*)),resHandler,SLOT(handleUploadFileFinRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sGetFileInfoRes(protocol::PDU*)),resHandler,SLOT(handleGetFileInfoRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sErrorRes(protocol::PDU*)),resHandler,SLOT(handleErrorRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sAddFriendResRet(protocol::PDU*)),resHandler,SLOT(handleShowAddFriendRet(protocol::PDU*)));
    //QObject::connect(this,SIGNAL(m_sOpenUploadAndDownLoadPage()),resHandler,SLOT())
    QObject::connect(this,SIGNAL(m_sUploadFileInfoGetRes(protocol::PDU*)),resHandler,SLOT(handleGetUploadFileInfoRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sDownloadFileRes(protocol::PDU*)),resHandler,SLOT(handleDownloadFile(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sDownloadPublicFileRes(protocol::PDU*)),resHandler,SLOT(handleDownloadPublicFile(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sUploadPublicFileInfoGetRes(protocol::PDU*)),resHandler,SLOT(handleGetUploadPublicFileInfoRes(protocol::PDU*)));
    QObject::connect(this,SIGNAL(m_sFlushPublicFileRes(protocol::PDU*)),resHandler,SLOT(handleFlushPublicFileRes(protocol::PDU*)));
}
void clientSocketThread::initTcpSocket()
{
    this->m_TcpSocket = new QTcpSocket(this); //创建全局tcpSocket对象
    //connect(m_TcpSocket, SIGNAL(connected()), loginAndRegist::getInstance(), SLOT(socketHandle())); //socket连接成功信号
    //connect(m_TcpSocket, SIGNAL(QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error)),loginAndRegist::getInstance(), SLOT(socketError(&QAbstractSocket::error)));//socket连接错误信号

    CommonFunc::setData(PCLIENTSOCKET,m_TcpSocket);
    m_TcpSocket->setSocketOption(QTcpSocket::KeepAliveOption,true); //设置socket连接属性为保持连接
    QString ip = *reinterpret_cast<QString*>(CommonFunc::QueryData(SERVERIP));
    qint16 port = (*reinterpret_cast<QString*>(CommonFunc::QueryData(SERVERPORT))).toShort();
    m_TcpSocket->connectToHost(QHostAddress(ip),port);  //尝试连接服务器
    m_TcpSocket->waitForConnected(); //等待与服务器进行连接
    if(m_TcpSocket->state() == QTcpSocket::ConnectedState) //如果连接成功
    {
        QObject::connect(m_TcpSocket,SIGNAL(readyRead()),this,SLOT(recvMsg())); //连接接收响应信号槽
    }
    connectToSlot();
    SendRequestToGetServerLocalPath(); //在连接服务器成功后请求服务器路径
}
void clientSocketThread::recvMsg()
{
    uint PDULen = 0;
    qDebug() << "Running in thread:" << QThread::currentThreadId();
    QTcpSocket* m_TcpSocket =reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    m_TcpSocket->read((char*)&PDULen,sizeof(uint)); //para1:数据存放的地址，para2:读出的数据大小，读出uint字节的大小，这个uint为总的数据大小
    uint uiMsgLen = PDULen - sizeof(protocol::PDU); //用总的数据大小减去protocol::PDU结构体的默认，获取实际消息长度
    protocol::PDU* pdu = protocol::createPDU(uiMsgLen); //创建协议结构体，用于接收数据
    m_TcpSocket->read((char*)pdu + sizeof (uint),PDULen-sizeof (uint)); /*让指针偏移来读取剩下的数据大小,先将pdu的指针类型转换为了char类型的指针，
那么当指针指针加一的时候会向后偏移一个字节的大小*/
    switch (pdu->uiMsgType)
    {
    case protocol::ENUM_MSG_TYPE_SERVER_LOCALPATH_RESPOND:
    {
        emit this->m_sLocalPathRes(pdu);
        break;
    }
    case protocol::ENUM_MSG_TYPE_PUBLICFILEPATH_RESPOND:
    {
        emit this->m_sPublicFilePathRes(pdu);
        break;
    }
    case protocol::ENUM_MSG_TYPE_REGIST_RESPOND:
    {
        emit this->m_sRegistRes(pdu);
        break;
    }
    case protocol::ENUM_MSG_TYPE_UPLOADPUBLICFILEINFO_RESPOND:
    {
        emit this->m_sUploadPublicFileInfoGetRes(pdu);
        break;
    }

    case protocol::ENUM_MSG_TYPE_LOGIN_RESPOND:
    {
        emit this->m_sLoginRes(pdu);
        break;
    }
    case protocol::ENUM_MSG_TYPE_ALL_ONLINE_RESPOND:
    {
        emit this->m_sAllOnlineRes(pdu);
        break;
    }
    case protocol::ENUM_MSG_TYPE_SEARCHUSER_RESPOND:
    {
        emit this->m_sSearchUserRes(pdu);
        break;
    }
    case protocol::ENUM_MSG_TYPE_ADDFRIEND_REQUEST:
    {
        emit this->m_sAddFriendReq(pdu);
        break;
    }
    case protocol::ENUM_MSG_TYPE_ADDFRIEND_RESPOND:
    {
        emit this->m_sAddFriendResRet(pdu);
        break;
    }
    case protocol::ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND:
    {
        emit this->m_sFlushFriendRes(pdu);
        break;
    }
    case protocol::ENUM_MSG_TYPE_FLUSH_PUBLIC_FILE_RESPOND:
    {
        emit this->m_sFlushPublicFileRes(pdu);
        break;
    }


    case protocol::ENUM_MSG_TYPE_PRIVATE_CHAT_TRANSMIT:
    {
        emit this->m_sPrivateChatTrans(pdu);
        break;
    }

    case protocol::ENUM_MSG_TYPE_PUBLIC_CHAT_TRANSMIT:
    {
        emit this->m_sPublicChatTrans(pdu);
        break;
    }

    case protocol::ENUM_MSG_TYPE_CREATE_DIR_RESPOND:
    {
        emit this->m_sCreateDirRes(pdu);
        break;
    }

    case protocol::ENUM_MSG_TYPE_FLUSH_FILE_RESPOND:
    {
        emit this->m_sFlushFileRes(pdu);
        break;
    }

    case protocol::ENUM_MSG_TYPE_DELETE_DIR_RESPOND:
    {
        emit this->m_sDeleteDirRes(pdu);
        break;
    }

    case protocol::ENUM_MSG_TYPE_DELETE_FILE_RESPOND:
    {
        emit this->m_sDeleteFileRes(pdu);
        break;
    }
    case protocol::ENUM_MSG_TYPE_GETFILEINFO_RESPOND:
    {
        emit this->m_sGetFileInfoRes(pdu);
        break;
    }
    case protocol::ENUM_MSG_TYPE_UPLOADFILEINFO_RESPOND:
    {
        emit this->m_sUploadFileInfoGetRes(pdu);
        break;
    }

    case protocol::ENUM_MSG_TYPE_RENAME_FILE_RESPOND:
    {
        emit this->m_sRenameFileRes(pdu);
        break;
    }

    case protocol::ENUM_MSG_TYPE_UPLOADFIN_FILE_RESPOND:
    {
        emit this->m_sFlushFileRes(pdu);
        break;
    }
    case protocol::ENUM_MSG_TYPE_DOWNLOADGET_FILE_RESPOND:
    {
        emit this->m_sDownloadFileRes(pdu);
        break;
    }
    case protocol::ENUM_MSG_TYPE_DOWNLOADGET_PUBLIC_FILE_RESPOND:
    {
        emit this->m_sDownloadPublicFileRes(pdu);
        break;
    }

    case protocol::ENUM_MSG_TYPE_ERROR_RESPOND:
    {
        QMessageBox::critical(NULL,"ERROR","请求错误!,请联系管理员");
        break;
    }

    default: break;
  }
}



void clientSocketThread::sendLoginReq(QString username, QString password)
{
    QTcpSocket* m_TcpSocket =reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    //qDebug() << "Current thread:" << QThread::currentThreadId();
    QByteArray bytePwdMd5 = QCryptographicHash::hash(password.toLatin1(),QCryptographicHash::Md5); //使用MD5对密码进行加密
    password = bytePwdMd5.toHex();//将加密后的结果转换为16进制
    //        qDebug()<<password;
    protocol::PDU* pdu =  protocol::createPDU(0);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_LOGIN_REQUEST;

    int usernameSize = CommonFunc::getTcpDataSize(username);
    int passwordSize = CommonFunc::getTcpDataSize(password);

    memcpy_s(pdu->caData,64,username.toStdString().c_str(),usernameSize); //先将QString 转换成 标准C++字符串 ，再转换为C风格的字符数组
    memcpy_s(pdu->caData+64,64,password.toStdString().c_str(),passwordSize); //先将QString 转换成 标准C++字符串 ，再转换为C风格的字符数组
    m_TcpSocket->write((char*)pdu, pdu->PDULen);
    free(pdu);
    pdu = nullptr;
    CommonFunc::setData(USERNAME,&username);
}

void clientSocketThread::sendRegistReq(QString username, QString password)
{
    CommonFunc::removeNullCharacters(username); //去除末尾\0
    CommonFunc::removeNullCharacters(password); //去除末尾\0

    QByteArray bytePwdMd5 = QCryptographicHash::hash(password.toLatin1(),QCryptographicHash::Md5); //使用MD5对密码进行加密
    password = bytePwdMd5.toHex();//将加密后的结果转换为16进制

    int usernameSize = CommonFunc::getTcpDataSize(username);
    int passwordSize = CommonFunc::getTcpDataSize(password);

    protocol::PDU* pdu = protocol::createPDU(0);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_REGIST_REQUEST;
    memcpy_s(pdu->caData,64,username.toStdString().c_str(),usernameSize); //先将QString 转换成 标准C++字符串 ，再转换为C风格的字符数组
    memcpy_s(pdu->caData+64,64,password.toStdString().c_str(),passwordSize); //先将QString 转换成 标准C++字符串 ，再转换为C风格的字符数组
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu, pdu->PDULen);
    free(pdu);
    pdu = nullptr;
}

void clientSocketThread::sendAllOnlineReq()
{
    protocol::PDU pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_ALL_ONLINE_REQUEST,"在线用户请求");
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)&pdu,pdu.PDULen);
    tcpSocket->waitForBytesWritten();
}

void clientSocketThread::sendSearchUserReq(QString username)
{
    CommonFunc::removeNullCharacters(username);

    protocol::PDU  pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_SEARCHUSER_REQUEST,username.toStdString().c_str());
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)&pdu,pdu.PDULen);
    tcpSocket->waitForBytesWritten();
}

void clientSocketThread::sendAddFriAgreeReq(QString username)
{
    CommonFunc::removeNullCharacters(username);
    protocol::PDU pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_ADDFRIEND_AGREE,"0");

    int usernameSize = CommonFunc::getTcpDataSize(username);


    memcpy_s(pdu.caData,64,username.toStdString().c_str(),usernameSize);
    QString strLoginName = *reinterpret_cast<QString*>(CommonFunc::QueryData(USERNAME));

    int strLoginNameSize = CommonFunc::getTcpDataSize(strLoginName);

    memcpy_s(pdu.caData+64,64,strLoginName.toStdString().c_str(),strLoginNameSize);
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)&pdu,pdu.PDULen);
}

void clientSocketThread::sendAddFriRefuseReq(QString username)
{
    CommonFunc::removeNullCharacters(username);
    QString strLoginName = *reinterpret_cast<QString*>(CommonFunc::QueryData(USERNAME));
    protocol::PDU pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_ADDFRIEND_REFUSE,strLoginName+username);
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)&pdu,pdu.PDULen);
}

void clientSocketThread::sendFlushFileReq(QString curPath)
{
    CommonFunc::removeNullCharacters(curPath);

    int curPathSize = CommonFunc::getTcpDataSize(curPath);

    protocol::PDU* pdu = protocol::createPDU(curPathSize);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
    memcpy_s((char*)pdu->caMsg,pdu->uiMsgLen,curPath.toStdString().c_str(),curPathSize);
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu,pdu->PDULen);
    tcpSocket->waitForBytesWritten();
    tcpSocket->waitForReadyRead();
    free(pdu);
    pdu = nullptr;
}

void clientSocketThread::sendFlushPublicFileReq(QString curPath)
{
    CommonFunc::removeNullCharacters(curPath);
    int curPathSize = CommonFunc::getTcpDataSize(curPath);
    protocol::PDU* pdu = protocol::createPDU(curPathSize);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_FLUSH_PUBLIC_FILE_REQUEST;
    memcpy_s((char*)pdu->caMsg,pdu->uiMsgLen,curPath.toStdString().c_str(),curPathSize);
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu,pdu->PDULen);
    tcpSocket->waitForBytesWritten();
    tcpSocket->waitForReadyRead();
    free(pdu);
    pdu = nullptr;
}

void clientSocketThread::sendPublicChatReq(QString strUsername,QString msg)
{
    protocol::PDU*pdu = protocol::createPDU(msg.toUtf8().size());/*生成协议*/

    int strUsernameSize = CommonFunc::getTcpDataSize(strUsername);
    int msgSize = CommonFunc::getTcpDataSize(msg);
    memcpy_s(pdu->caData,128,strUsername.toStdString().c_str(),strUsernameSize);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_PUBLIC_CHAT_REQUEST;
    memcpy_s((char*)pdu->caMsg,pdu->uiMsgLen,msg.toStdString().c_str(),msgSize);
    /*通过socket发送协议*/
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu,pdu->PDULen);
    tcpSocket->waitForBytesWritten();
    free(pdu);
    pdu = nullptr;
}

void clientSocketThread::sendPrivateChatReq(QString strMyName,QString strOtherName,QString msg)
{
    CommonFunc::removeNullCharacters(strMyName);
    CommonFunc::removeNullCharacters(strOtherName);
    CommonFunc::removeNullCharacters(msg);

    int strMyNameSize = CommonFunc::getTcpDataSize(strMyName);
    int strOtherNameSize = CommonFunc::getTcpDataSize(strOtherName);
    int msgSize = CommonFunc::getTcpDataSize(msg);

    protocol::PDU *pdu = protocol::createPDU(msgSize);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST;
    memcpy_s(pdu->caData,64,strMyName.toStdString().c_str(),strMyNameSize); //将已方用户名写入协议
    memcpy_s(pdu->caData+64,64,strOtherName.toStdString().c_str(),strOtherNameSize); //将聊天对象用户名写入协议
    memcpy_s((char*)pdu->caMsg,pdu->uiMsgLen,msg.toStdString().c_str(),msgSize);//将聊天内容写入协议
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu,pdu->PDULen);//将协议写入socket后发送
    tcpSocket->waitForBytesWritten();
    free(pdu);
    pdu = nullptr;
}

void clientSocketThread::sendGoBackReq(QString strCurPath)
{
    CommonFunc::removeNullCharacters(strCurPath);

    int strCurPathSize = CommonFunc::getTcpDataSize(strCurPath);

    protocol::PDU* pdu = protocol::createPDU(strCurPathSize);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
    memcpy_s((char*)pdu->caMsg,pdu->uiMsgLen,strCurPath.toStdString().c_str(),strCurPathSize);

    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu,pdu->PDULen);
    tcpSocket->waitForBytesWritten();
    free(pdu);
    pdu = nullptr;
}

void clientSocketThread::sendSwitchReq(QString strCurPath)
{
    CommonFunc::removeNullCharacters(strCurPath);

    int strCurPathSize = CommonFunc::getTcpDataSize(strCurPath);


    protocol::PDU* pdu = protocol::createPDU(strCurPathSize);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
    memcpy_s((char*)pdu->caMsg,pdu->uiMsgLen,strCurPath.toStdString().c_str(),strCurPathSize);

    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu,pdu->PDULen);
    tcpSocket->waitForBytesWritten();
    free(pdu);
    pdu = nullptr;
}

void clientSocketThread::sendRenameFileReq(QString strNewFName,QString strCurFName)
{
    QString curPath = *reinterpret_cast<QString*>(CommonFunc::QueryData(STRCURPATH));

    CommonFunc::removeNullCharacters(curPath);
    CommonFunc::removeNullCharacters(strNewFName);
    CommonFunc::removeNullCharacters(strCurFName);

    int strNewFNameSize = CommonFunc::getTcpDataSize(strNewFName);
    int strCurFNameSize = CommonFunc::getTcpDataSize(strCurFName);
    int curPathSize = CommonFunc::getTcpDataSize(curPath);

    protocol::PDU* pdu = protocol::createPDU(curPathSize);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_RENAME_FILE_REQUEST;
    memcpy_s(pdu->caData,64,strCurFName.toStdString().c_str(),strCurFNameSize);
    memcpy_s(pdu->caData+64,64,strNewFName.toStdString().c_str(),strNewFNameSize);
    memcpy_s((char*)pdu->caMsg,pdu->uiMsgLen,curPath.toStdString().c_str(),curPathSize);
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu,pdu->PDULen);
    tcpSocket->waitForBytesWritten();
}

void clientSocketThread::sendDeleteFileReq(QString strFName, QString strCurPath)
{
    CommonFunc::removeNullCharacters(strFName);
    CommonFunc::removeNullCharacters(strCurPath);

    int strFNameSize = CommonFunc::getTcpDataSize(strFName);
    int strCurPathSize = CommonFunc::getTcpDataSize(strCurPath);

    protocol::PDU* pdu = protocol::createPDU(strCurPathSize);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_DELETE_FILE_REQUEST;
    memcpy_s(pdu->caData,128,strFName.toStdString().c_str(),strFNameSize);
    memcpy_s((char*)pdu->caMsg,pdu->uiMsgLen,strCurPath.toStdString().c_str(),strCurPathSize);

    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu,pdu->PDULen);
    tcpSocket->waitForBytesWritten();

}

void clientSocketThread::sendDeleteDirReq(QString strDirName,QString strCurPath)
{
    CommonFunc::removeNullCharacters(strDirName);
    CommonFunc::removeNullCharacters(strCurPath);

    int strDirNameSize = CommonFunc::getTcpDataSize(strDirName);
    int strCurPathSize = CommonFunc::getTcpDataSize(strCurPath);

    protocol::PDU* pdu = protocol::createPDU(strCurPathSize);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_DELETE_DIR_REQUEST;
    memcpy_s(pdu->caData,128,strDirName.toStdString().c_str(),strDirNameSize);
    memcpy_s((char*)pdu->caMsg,pdu->uiMsgLen,strCurPath.toStdString().c_str(),strCurPathSize);

    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu,pdu->PDULen);
    tcpSocket->waitForBytesWritten();
}

void clientSocketThread::sendFlushFriendReq(QString strUsername)
{
    QString strUserName = strUsername;
    protocol::PDU pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST,"");

    int strUserNameSize = CommonFunc::getTcpDataSize(strUserName);

    memcpy_s(pdu.caData,128,strUserName.toStdString().c_str(),strUserNameSize);

    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)&pdu,pdu.PDULen);
    tcpSocket->waitForBytesWritten();
}

void clientSocketThread::sendDeleteFriendReq(QString strFriendName, QString strUserName)
{
    int strFriendNameSize = CommonFunc::getTcpDataSize(strFriendName);
    int strUserNameSize = CommonFunc::getTcpDataSize(strUserName);

    protocol::PDU pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST,"");
    memcpy_s(pdu.caData,64,strFriendName.toStdString().c_str(),strFriendNameSize);
    memcpy_s(pdu.caData+64,64,strUserName.toStdString().c_str(),strUserNameSize);
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)&pdu,pdu.PDULen);
    tcpSocket->waitForBytesWritten();
}

void clientSocketThread::sendUploadFileReq(QString absoluteFilePath)
{
    CommonFunc::removeNullCharacters(absoluteFilePath);
    QFile * uploadFile = new QFile(absoluteFilePath);

    QString strUserName = *reinterpret_cast<QString*>(CommonFunc::QueryData(USERNAME));

    QString strFileName = CommonFunc::getFileNameFromAbsolutePath(absoluteFilePath);
    CommonFunc::removeNullCharacters(strFileName);
    protocol::FileInfo_s* savedFileInfo = protocol::createFileInfo_s(strFileName.toStdString().c_str(),
                                                                     uploadFile->size(),
                                                                     strUserName.toStdString().c_str());
    protocol::PDU* pdu = protocol::createPDU(sizeof(protocol::FileInfo_s));
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_UPLOADFILEINFO_REQUEST;
    memcpy_s((char*)pdu->caMsg,pdu->uiMsgLen,(char*)savedFileInfo,sizeof(protocol::FileInfo_s));
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu,pdu->PDULen);
    tcpSocket->waitForBytesWritten();
    free(pdu);
    CommonFunc::setData(UPLOADFILEABSOLUTEDNAME,&absoluteFilePath);


    delete uploadFile;
    uploadFile = nullptr;
}

void clientSocketThread::sendUploadPublicFileReq(QString absoluteFilePath)
{
    CommonFunc::removeNullCharacters(absoluteFilePath);
    QFile * uploadFile = new QFile(absoluteFilePath);

    QString strUserName = "public";

    QString strFileName = CommonFunc::getFileNameFromAbsolutePath(absoluteFilePath);
    CommonFunc::removeNullCharacters(strFileName);
    protocol::FileInfo_s* savedFileInfo = protocol::createFileInfo_s(strFileName.toStdString().c_str(),
                                                                     uploadFile->size(),
                                                                     strUserName.toStdString().c_str());
    protocol::PDU* pdu = protocol::createPDU(sizeof(protocol::FileInfo_s));
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_UPLOADPUBLICFILEINFO_REQUEST;
    memcpy_s((char*)pdu->caMsg,pdu->uiMsgLen,(char*)savedFileInfo,sizeof(protocol::FileInfo_s));
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu,pdu->PDULen);
    tcpSocket->waitForBytesWritten();
    free(pdu);
    CommonFunc::setData(UPLOADFILEABSOLUTEDNAME,&absoluteFilePath);

    delete uploadFile;
    uploadFile = nullptr;
}

void clientSocketThread::sendDownLoadFileReq(QString strTargetFileName, QString CurPath)
{
    QString strCurPath = CurPath;
    QString strServerfilePath = strCurPath + "/" + strTargetFileName;
    CommonFunc::removeNullCharacters(strServerfilePath);

    int strServerfilePathSize = CommonFunc::getTcpDataSize(strServerfilePath);

    protocol::PDU* pdu = protocol::createPDU(strServerfilePathSize);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST;
    memcpy_s(pdu->caMsg,pdu->uiMsgLen,strServerfilePath.toStdString().c_str(),strServerfilePathSize);
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu,pdu->PDULen);
    tcpSocket->waitForBytesWritten();
}

void clientSocketThread::sendGetFileInfo(QString strFileName,QString strUserName)
{

    int strFileNameSize = CommonFunc::getTcpDataSize(strFileName);
    int strUserNameSize = CommonFunc::getTcpDataSize(strUserName);

    protocol::PDU* pdu = protocol::createPDU(strFileNameSize);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_GETFILEINFO_REQUEST;
    memcpy_s(pdu->caData,128,strUserName.toStdString().c_str(),strUserNameSize);
    memcpy_s((char*)pdu->caMsg,pdu->uiMsgLen,strFileName.toStdString().c_str(),strFileNameSize);

    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu,pdu->PDULen);
    tcpSocket->waitForBytesWritten();
}

void clientSocketThread::sendCreateDirReq(QString strNewDir,QString strName, QString strCurPath)
{
    CommonFunc::removeNullCharacters(strNewDir);
    CommonFunc::removeNullCharacters(strCurPath);

    int strNewDirSize = CommonFunc::getTcpDataSize(strNewDir);
    int strNameSize = CommonFunc::getTcpDataSize(strName);
    int strCurPathSize = CommonFunc::getTcpDataSize(strCurPath);

    protocol::PDU* pdu = protocol::createPDU(strCurPath.toUtf8().size());
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_CREATE_DIR_REQUEST;
    memcpy_s(pdu->caData,64,strName.toStdString().c_str(),strNameSize);
    memcpy_s((char*)pdu->caMsg,pdu->uiMsgLen,strCurPath.toStdString().c_str(),strCurPathSize);
    memcpy_s(pdu->caData+64,64,strNewDir.toStdString().c_str(),strNewDirSize);
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu,pdu->PDULen);
    tcpSocket->waitForBytesWritten();
    free(pdu);
    pdu = nullptr;
}

clientSocketThread::~clientSocketThread()
{

}


void clientSocketThread::uploadFile(protocol::PDU pdu)
{
    this->uploadTimer->stop();

}

void clientSocketThread::SendRequestToGetServerLocalPath()
{
    QTcpSocket* m_TcpSocket =reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    protocol::PDU* pdu =  protocol::createPDU(0);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_SERVER_LOCALPATH_REQUEST;

    m_TcpSocket->write((char*)pdu,pdu->PDULen);
    m_TcpSocket->waitForBytesWritten();
    free(pdu);
    pdu = NULL;
    return;
}

void clientSocketThread::SendRequestToGetPublicFilePath()
{
    QTcpSocket* m_TcpSocket =reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    protocol::PDU* pdu =  protocol::createPDU(0);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_PUBLICFILEPATH_REQUEST;
    m_TcpSocket->write((char*)pdu,pdu->PDULen);
    m_TcpSocket->waitForBytesWritten();
    free(pdu);
    pdu = NULL;
    return;
}

void clientSocketThread::sendDownLoadPublicFileReq(QString strTargetFileName, QString CurPath)
{
    QString strCurPath = CurPath;
    QString strServerfilePath = strCurPath + "/" + strTargetFileName;
    CommonFunc::removeNullCharacters(strServerfilePath);

    int strServerfilePathSize = CommonFunc::getTcpDataSize(strServerfilePath);

    protocol::PDU* pdu = protocol::createPDU(strServerfilePathSize);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_DOWNLOAD_PUBLUC_FILE_REQUEST;
    memcpy_s(pdu->caMsg,pdu->uiMsgLen,strServerfilePath.toStdString().c_str(),strServerfilePathSize);
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)pdu,pdu->PDULen);
    tcpSocket->waitForBytesWritten();
}

void clientSocketThread::sendAddFriReq(QString strPerUserName,QString strLoginName)
{
    int strLoginNameSize = CommonFunc::getTcpDataSize(strLoginName);
    int strPerUserNameSize = CommonFunc::getTcpDataSize(strPerUserName);
    protocol::PDU pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_ADDFRIEND_REQUEST,0);
    memcpy_s(pdu.caData,64,strPerUserName.toStdString().c_str(),strPerUserNameSize);
    memcpy_s(pdu.caData+64,64,strLoginName.toStdString().c_str(),strLoginNameSize);
    QTcpSocket* tcpSocket = reinterpret_cast<QTcpSocket*>(CommonFunc::QueryData(PCLIENTSOCKET));
    tcpSocket->write((char*)&pdu,pdu.PDULen); //使用 socket发送数据到服务器
    tcpSocket->waitForBytesWritten();
}



