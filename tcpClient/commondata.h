#ifndef COMMONDATA_H
#define COMMONDATA_H
#pragma once

#include<QString>
#include <QRect>

class QTcpSocket;
class responseHandler;
class clientSocketThread;

#define INIFILEEXTENSION ".ini"

enum dataCode
{
    PCLIENTSOCKET,
    LOCALLANGUAGE,
    SCREENRECT,
    SERVERPORT,
    SERVERIP,
    USERNAME,
    SERVERPATH,
    STRCURPATH,
    STRROOTPATH,
    RESPONSEHANDLE,
    CLIENTSOCKETTHREAD,
    UPLOADFILEABSOLUTEDNAME,
    PUBLICFILEPATH,
    WEBADDRESS,
    QUITMODE,
    DEFAULTSTOREADDR,
    VERSION,
    STYLEMODE
};

class CommonData
{
public :
    long long QueryOrSetData(int code,int mode = 0,void* data = nullptr); // 1 -- set  0 -- get
    static CommonData& getInstance(); //单例模式构建 全局唯一
private:
    CommonData();
    QTcpSocket* pClientSocket;
    QString strLocalLanguage = "";
    QRect objScreenRect = QRect();
    QString strServerIP = "";
    QString strServerPort = "";
    QString strUserName = "";
    QString strServerPath = "";
    QString strCurPath = "";
    QString strRootPath = "";
    responseHandler* pResponseHandler;
    clientSocketThread* pClientSocketThread;
    QString strUploadFileAbsolutedName = "";
    QString strPublicFIlePath = "";
    QString strWebAddress="https://github.com/12345671125/CLOUDDISK";
    QString strQuitMode = ""; //1为直接关闭程序，0为最小化
    QString strDefaultStoreAddr = ""; //储存地址
    QString strVersion="";
    QString strStyleMode=""; //主题

};

#endif // COMMONDATA_H
