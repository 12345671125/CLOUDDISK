/*主界面类，用于初始化程序配置*/

#include "serverwin.h"
#include "ui_serverwin.h"

#include <QDir>

serverWin::serverWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::serverWin)
{
    ui->setupUi(this);
    configInit(); //初始化配置
    createServer(); //创建服务器
    createPablicFileDir(); //创建公共文件夹
}


void serverWin::configInit(){
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
           if(strList.length()>=2){
                this->ip = strList[0];//将文件中读取的ip储存到ip中
                this->port = strList[1].toUShort();//将文件中读取的port储存到port中
           }else{
                QMessageBox::critical(this,"open config","open config fault!");
           }

           file.close();
        }else{
            QMessageBox::critical(this,"open config","open config fault!");
        }

}

void serverWin::createServer()  //初始化tcp服务器
{
    if(myTcpServer::getInstance().listen(QHostAddress(this->ip),this->port))
    {

    }else
    {
        QMessageBox::critical(this,"ERROR","Listen Fault!");
    };
}

void serverWin::createPablicFileDir()
{
    // 获取当前路径
    QString currentPath = qApp->applicationDirPath();
    qDebug() << "Current Path:" << currentPath;

    // 定义要创建的文件夹名称
    QString folderName = "public";
    QString fullFolderPath = currentPath + "/" + folderName;

    // 检查文件夹是否已存在
    if (!QDir(fullFolderPath).exists()) {
        // 创建文件夹
        bool result = QDir().mkdir(fullFolderPath);
        if (result) {
            qDebug() << "Folder created successfully.";
        } else {
            qDebug() << "Failed to create folder.";
        }
    } else {
        qDebug() << "Folder already exists.";
    }

}


serverWin::~serverWin()
{
    delete ui;
}

