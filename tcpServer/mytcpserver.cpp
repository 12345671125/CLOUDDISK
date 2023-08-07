/*myTcpServer类用于生成tcpServer并且监听并接受新的客户端请求*/

#include "mytcpserver.h"


myTcpServer::myTcpServer()
{
    this->socketList.clear();
}

myTcpServer &myTcpServer::getInstance()
{
    static myTcpServer instance;     //创建静态myTcpServer对象 instance
    return instance;  //返回 静态对象 instance , 因为为静态的所以不管调用多少次都只产生一个
}

void myTcpServer::incomingConnection(qintptr handle)  /*当服务器端监听到客户端的请求时，会触发incomingConnection函数，
产生一个连接socket，服务器端通过这个socket与客户端进行通行*/
{
    qDebug()<<"new connect!";
    myTcpSocket* socket = new myTcpSocket();
    socket->setSocketDescriptor(handle); //qt产生的socket描述符放入自己的socket中
    this->socketList.push_back(socket); //将socket推入List

    QObject:: connect(socket,SIGNAL(offline(myTcpSocket*)),this,SLOT(deleteSocket(myTcpSocket*)));
}

void myTcpServer::FResend(const char *pername, protocol::PDU *pdu)
{
    qDebug()<<"FResend";
    if(pername == NULL || pdu == NULL){
        return;
    }
    QString strName = pername;
    qDebug()<<strName;
    for(int i = 0;i<this->socketList.length();i++){
        if(this->socketList.at(i)->getName() == strName){
            this->socketList.at(i)->write((char*)pdu,pdu->PDULen); //通过登陆时传过来的username,将好友请求转发给对应用户
            break;
        }
    }
}

void myTcpServer::MsgResend(const char *pername, protocol::PDU *pdu)
{
    if(pername == NULL || pdu == NULL)
        return;
    QString strName = pername;
    //    qDebug()<<strName;
    for(int i = 0;i<this->socketList.length();i++){
        //        qDebug()<<"MSGRESEND_for";
        if(this->socketList.at(i)->getName() == strName){ //如果socket列表中有对应的用户名
            //            qDebug()<<"MSGRESEND_if";
            pdu->uiMsgType = protocol::ENUM_MSG_TYPE_PRIVATE_CHAT_TRANSMIT; //将协议中的请求类型改为消息转发
            this->socketList.at(i)->write((char*)pdu,pdu->PDULen); //转发消息
            return;
        }
    }
}

void myTcpServer::MsgResend(const QStringList strList, protocol::PDU *pdu)
{
    qDebug()<<"MsgResend";
    if(strList.isEmpty()) return;
    if(pdu == nullptr) return;
    for(int i = 0;i<this->socketList.length();i++){
        for(int j = 0;j<strList.length();j++){
            if(this->socketList.at(i)->getName() == strList[j]){
                pdu->uiMsgType = protocol::ENUM_MSG_TYPE_PUBLIC_CHAT_TRANSMIT; //将协议中的请求类型改为消息转发
                this->socketList.at(i)->write((char*)pdu,pdu->PDULen);     //进行消息转发
            }
        }

    }
}

void myTcpServer::deleteSocket(myTcpSocket *mySocket)
{
    QList<myTcpSocket*>::iterator iter = socketList.begin(); //创建迭代器
    for(;iter != socketList.end();iter++){
        if(mySocket == *iter){
            (*iter)->deleteLater();//释放空间
            *iter = NULL;
            socketList.erase(iter);
            break;
        }
    }
    for(int i = 0;i<this->socketList.length();i++){
        qDebug() << socketList.at(i)->getName();
    }
}
