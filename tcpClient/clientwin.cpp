/*主界面类，用于客户端初始化*/

#include "clientwin.h"
#include "ui_clientwin.h"
#include "privatechat.h"
#include "filepage.h"

clientWin::clientWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::clientWin)
{
    ui->setupUi(this);
    initconfig(); //初始化配置
    connectToServer();
    this->uploadTimer = new QTimer(this);
    this->uploadTimer->setInterval(500);
    this->clientSocket.setSocketOption(QTcpSocket::KeepAliveOption,true);
    QObject::connect(&this->clientSocket,SIGNAL(connected()),this,SLOT(showConnected()));
    QObject::connect(&this->clientSocket,SIGNAL(readyRead()),this,SLOT(recvMsg()));
//    QObject::connect(this->uploadTimer,SIGNAL(timeout()),this,SLOT(uploadFile()));
}

clientWin &clientWin::getInstance()
{
    static clientWin instance;
    return instance;
}

QTcpSocket &clientWin::getTcpSocket()
{
    return this->clientSocket;
}

QString clientWin::getLoginName()
{
    return this->m_strLoginName;
}

//从配置文件中获取服务器ip和port
void clientWin::initconfig()
{
    QFile file(":/config");  //定义File对象 用来读取文件
    if(file.open(QIODevice::ReadOnly))//以只读方式打开文件
    {
       QByteArray byteArr =  file.readAll();//将配置文件中的内容读出，以二进制的形式储存在QByteArray中
       QString configData = QString::fromStdString(byteArr.toStdString());//将二进制数据转换为QString的形式
//       qDebug()<<configData;
       configData.replace("\n",";");
       QStringList strList = configData.split(";");
       for(int i = 0;i<strList.length();i++){ //遍历strList,截取ip和port
            strList[i]   = strList[i].mid(strList[i].indexOf("=")+1,-1);
//            qDebug()<<strList[i];
       }
       this->ip = strList[0];//将文件中读取的ip储存到ip中
       this->port = strList[1].toUShort();//将文件中读取的port储存到port中
       file.close();
    }else{
        QMessageBox::critical(this,"open config","open config fault!");
    }
}


//连接服务器
void clientWin::connectToServer(){
    this->clientSocket.connectToHost(QHostAddress(this->ip),this->port);  //连接服务器
}



void clientWin::showConnected(){
    QMessageBox::information(this,"connectToSercer","连接服务器成功!");
}

void clientWin::recvMsg()
{
    uint PDULen = 0;
    this->clientSocket.read((char*)&PDULen,sizeof(uint)); //para1:数据存放的地址，para2:读出的数据大小，读出uint字节的大小，这个uint为总的数据大小
    uint uiMsgLen = PDULen - sizeof(protocol::PDU); //用总的数据大小减去protocol::PDU结构体的默认，获取实际消息长度
    protocol::PDU* pdu = protocol::createPDU(uiMsgLen); //创建协议结构体，用于接收数据
    this->clientSocket.read((char*)pdu + sizeof (uint),PDULen-sizeof (uint)); /*让指针偏移来读取剩下的数据大小,先将pdu的指针类型转换为了char类型的指针，
那么当指针指针加一的时候会向后偏移一个字节的大小*/
    switch (pdu->uiMsgType)
    {
    case protocol::ENUM_MSG_TYPE_REGIST_RESPOND:
    {
        qDebug() << pdu->caData;
        if(strcmp(pdu->caData,REGIST_OK) == 0){
            QMessageBox::information(this,"注册","注册成功");
        }else if(strcmp(pdu->caData,REGIST_FAILED) == 0){
            qDebug() << 1;
            QMessageBox::warning(this,"注册","注册失败，用户名重复");
        }
        break;
    }
    case protocol::ENUM_MSG_TYPE_LOGIN_RESPOND:
    {
        qDebug() << pdu->caData;
        if(strcmp(pdu->caData,LOGIN_OK) == 0){
            m_strCurPath = QString("./%1").arg(m_strLoginName);
            QMessageBox::information(this,"登录","LOGIN_OK");
            this->setWindowTitle("当前用户:"+this->m_strLoginName);
            OpeWidget::getinstance().show();
            OpeWidget::getinstance().setWindowTitle("当前用户:"+this->m_strLoginName);
            this->hide();
        }else if(strcmp(pdu->caData,LOGIN_FAILED) == 0){
            qDebug() << 1;
            QMessageBox::warning(this,"登录","用户名或密码错误");
        }
        break;
    }
    case protocol::ENUM_MSG_TYPE_ALL_ONLINE_RESPOND:
    {

        OpeWidget::getinstance().getFriend()->showAllOnlineUser(pdu);
        break;
    }
    case protocol::ENUM_MSG_TYPE_SEARCHUSER_RESPOND:
    {
        OpeWidget::getinstance().getFriend()->showSearchUser(pdu);
        break;
    }
    case protocol::ENUM_MSG_TYPE_ADDFRIEND_REQUEST:
    {
        char username[64] = {"\n"}; //创建username数组用于存放用户名
        memset(username,0,sizeof(username));
        strncpy(username,pdu->caData,64);//从pdu.cadata中读取username
        int ret = QMessageBox::information(this,"好友请求",QString("%1 想要添加你为好友").arg(username),QMessageBox::Yes,QMessageBox::No);
        if(ret == QMessageBox::Yes)
        {
            protocol::PDU pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_ADDFRIEND_AGREE,"0");
            memcpy(pdu.caData,username,64);
            memcpy(pdu.caData+64,this->m_strLoginName.toStdString().c_str(),64);
            this->clientSocket.write((char*)&pdu,pdu.PDULen);
        }else
        {
            protocol::PDU pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_ADDFRIEND_REFUSE,this->getLoginName()+username);
            this->clientSocket.write((char*)&pdu,pdu.PDULen);
        }
        break;
    }
    case protocol::ENUM_MSG_TYPE_ADDFRIEND_RESPOND:
    {
        QMessageBox::information(this,"添加好友",pdu->caData);
        break;
    }
    case protocol::ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND:
    {
        OpeWidget::getinstance().getFriend()->updateFriend(pdu);
        break;
    }


    case protocol::ENUM_MSG_TYPE_PRIVATE_CHAT_TRANSMIT:
    {
        this->showPrivateMsg(pdu);
        break;
    }

    case protocol::ENUM_MSG_TYPE_PUBLIC_CHAT_TRANSMIT:
    {
        this->showPublicMsg(pdu);
        break;
    }

    case protocol::ENUM_MSG_TYPE_CREATE_DIR_RESPOND:
    {
        this->showCreateDir(pdu);
        break;
    }

    case protocol::ENUM_MSG_TYPE_FLUSH_FILE_RESPOND:
    {
        this->flushFile(pdu);
        break;
    }

    case protocol::ENUM_MSG_TYPE_DELETE_DIR_RESPOND:
    {
        this->delDir(pdu);
        break;
    }

    case protocol::ENUM_MSG_TYPE_DELETE_FILE_RESPOND:
    {
        this->delFile(pdu);
        break;
    }

    case protocol::ENUM_MSG_TYPE_RENAME_FILE_RESPOND:
    {
        this->renameFile(pdu);
        break;
    }
//    case protocol::ENUM_MSG_TYPE_UPLOADGET_FILE_RESPOND:
//    {
//        OpeWidget::getinstance().getfilePage()->emitSignal();
//        break;
//    }

    case protocol::ENUM_MSG_TYPE_UPLOADFIN_FILE_RESPOND:
    {
        OpeWidget::getinstance().getfilePage()->flushFile();
        break;
    }

    case protocol::ENUM_MSG_TYPE_GETFILEINFO_RESPOND:
    {
        qDebug()<<"showFileInfo(pdu)";
        OpeWidget::getinstance().getfilePage()->showFileInfo(pdu);
        break;
    }

//    case protocol::ENUM_MSG_TYPE_DOWNLOADGET_FILE_RESPOND:
//    {
//        OpeWidget::getinstance().getfilePage()->emitDownLoadSignal(pdu);
//        break;
//    }

    case protocol::ENUM_MSG_TYPE_ERROR_RESPOND:
    {
        QMessageBox::critical(this,"ERROR","请求错误!,请联系管理员");
        break;
    }

    default: break;
  }

  free(pdu);
  pdu = nullptr;
}

clientWin::~clientWin()
{
    delete ui;
    delete this->uploadTimer;
}

//#if 0
//void clientWin::on_sendButton_clicked()
//{
//    QString strMsg = this->ui->msgInput->text();
//    if(!strMsg.isEmpty()){
//            protocol::PDU *pdu = createprotocol::PDU(strMsg.size()+1);
//            pdu->uiMsgType = _msgText_;
//            memcpy(pdu->caMsg,strMsg.toStdString().c_str(),strMsg.size());
//            this->clientSocket.write((char*) pdu, pdu->uiprotocol::PDULen);
//            free(pdu);
//            pdu = nullptr;
//    }else{
//        QMessageBox::warning(this,"send Message","发送的信息不能为空!");
//    }
//}
//#endif

void clientWin::on_login_clicked()
{
    if(!this->ui->usernameInput->text().isEmpty() && !this->ui->passwordInput->text().isEmpty()){
        QString username = this->ui->usernameInput->text();
        QString password = this->ui->passwordInput->text();
        if(username.length() > 20 || password.length()>20){
            QMessageBox::information(this,"命名规则","用户名和密码的长度不能超过20个字符!");
        }else{
            QByteArray bytePwdMd5 = QCryptographicHash::hash(password.toLatin1(),QCryptographicHash::Md5); //使用MD5对密码进行加密
            password = bytePwdMd5.toHex();//将加密后的结果转换为16进制
            //        qDebug()<<password;
            protocol::PDU* pdu =  protocol::createPDU(0);
            pdu->uiMsgType = protocol::ENUM_MSG_TYPE_LOGIN_REQUEST;
            strncpy(pdu->caData,username.toStdString().c_str(),64); //先将QString 转换成 标准C++字符串 ，再转换为C风格的字符数组
            strncpy(pdu->caData+64,password.toStdString().c_str(),64); //先将QString 转换成 标准C++字符串 ，再转换为C风格的字符数组
            this->clientSocket.write((char*)pdu, pdu->PDULen);
            this->m_strLoginName = username; //将用户名保存
            free(pdu);
            pdu = nullptr;
        }

    }else{
        QMessageBox::critical(this,"ERROR","用户名或密码不能为空!");
    }
}

void clientWin::on_regist_clicked()
{
    if(!this->ui->usernameInput->text().isEmpty() && !this->ui->passwordInput->text().isEmpty()){
        QString username = this->ui->usernameInput->text();
        QString password = this->ui->passwordInput->text();
        if(username.length() > 20 || password.length()>20){
            QMessageBox::information(this,"命名规则","用户名和密码的长度不能超过20个字符!");
        }else{
        QByteArray bytePwdMd5 = QCryptographicHash::hash(password.toLatin1(),QCryptographicHash::Md5); //使用MD5对密码进行加密
        password = bytePwdMd5.toHex();//将加密后的结果转换为16进制
//        qDebug()<<password;
        protocol::PDU* pdu = protocol::createPDU(0);
        pdu->uiMsgType = protocol::ENUM_MSG_TYPE_REGIST_REQUEST;
        strncpy(pdu->caData,username.toStdString().c_str(),64); //先将QString 转换成 标准C++字符串 ，再转换为C风格的字符数组
        strncpy(pdu->caData+64,password.toStdString().c_str(),64); //先将QString 转换成 标准C++字符串 ，再转换为C风格的字符数组
        this->clientSocket.write((char*)pdu, pdu->PDULen);
        free(pdu);
        pdu = nullptr;
        }
    }else{
        QMessageBox::critical(this,"ERROR","用户名或密码不能为空!");
    }

}

void clientWin::showPrivateMsg(protocol::PDU *pdu)
{
    qDebug()<<"showPrivateMsg";
    char username[64] = {"\0"};
    char pername[64] = {"\0"};
    memcpy(username,pdu->caData,64);
    memcpy(pername,pdu->caData+64,64); //解析协议中的私聊对象名
    QString uname = username;
    if(!PrivateChat::getInstance().isHidden()||!PrivateChat::getInstance().isActiveWindow() || !PrivateChat::getInstance().isVisible()){
        int result =  QMessageBox::information(this,"新消息","你有一条来自"+uname+"的新消息,是否打开聊天框？",QMessageBox::Yes,QMessageBox::No);
        if(result == QMessageBox::Yes){
            PrivateChat::getInstance().setChatName(uname.append("     "));//向后填充5个空格
            qDebug()<<uname;
            PrivateChat::getInstance().show();
            PrivateChat::getInstance().showMsg(pdu);

        }
    }else{
        PrivateChat::getInstance().showMsg(pdu);
    }

}

void clientWin::showPublicMsg(protocol::PDU *pdu)
{
    qDebug()<<"showPublicMsg";
    OpeWidget::getinstance().getFriend()->showPublicChat(pdu);
}

void clientWin::showCreateDir(protocol::PDU *pdu)
{
    QMessageBox::information(this,"创建文件夹",pdu->caData);
    OpeWidget::getinstance().getfilePage()->flushFile();
}

void clientWin::flushFile(protocol::PDU *pdu)
{
    if(pdu == NULL) return;
    filePage::getInstance().updateFileList(pdu);
}

void clientWin::delDir(protocol::PDU *pdu)
{
    if(pdu == NULL) return;
    char result[64] = {'\0'};
    memcpy(result,pdu->caData,64);
    if(strcmp(result,"success") == 0)
    {
        QMessageBox::information(this,"删除文件夹","删除成功");
    }
    else if(strcmp(result,"dir not exist") == 0)
    {
        QMessageBox::information(this,"删除文件夹","文件夹不存在，删除失败");
    }else if(strcmp(result,"remove fault") == 0)
    {
        QMessageBox::information(this,"删除文件夹","删除失败");
    }
    OpeWidget::getinstance().getfilePage()->flushFile();
}

void clientWin::delFile(protocol::PDU *pdu)
{
    if(pdu == NULL) return;
    char result[64] = {'\0'};
    memcpy(result,pdu->caData,64);
    if(strcmp(result,"success") == 0)
    {
        QMessageBox::information(this,"删除文件","删除成功");
    }
    else if(strcmp(result,"file not exist") == 0)
    {
        QMessageBox::information(this,"删除文件","文件不存在，删除失败");
    }else if(strcmp(result,"remove fault") == 0)
    {
        QMessageBox::information(this,"删除文件","删除失败");
    }
    OpeWidget::getinstance().getfilePage()->flushFile();
}

void clientWin::renameFile(protocol::PDU *pdu)
{
    if(pdu == NULL) return;
    char result[64] = {'\0'};
    memcpy(result,pdu->caData,64);
    if(strcmp(result,"success") == 0)
    {
        QMessageBox::information(this,"重命名文件","重命名成功");
    }
    else if(strcmp(result,"file not exist") == 0)
    {
        QMessageBox::information(this,"重命名文件","文件不存在，重命名失败");
    }else if(strcmp(result,"rename fault") == 0)
    {
        QMessageBox::information(this,"重命名文件","重命名失败");
    }
    OpeWidget::getinstance().getfilePage()->flushFile();
}

void clientWin::uploadFile(protocol::PDU *pdu)
{
    this->uploadTimer->stop();

}

void clientWin::on_logout_clicked()
{

}

QString clientWin::curPath()
{
    return this->m_strCurPath;
}

QString clientWin::getServerIp()
{
    return this->ip;
}

quint16 clientWin::getServerPort()
{
    return this->port;
}

void clientWin::modifyCurPath(QString str)
{
    this->m_strCurPath = str;
}

