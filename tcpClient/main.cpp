#include <QApplication>
#include <QIcon>
#include <QtCore>
#include <Windows.h>

#include "LoginAndRegist/loginAndRegist.h"
#include "mymain.h"

#include "commondata.h"
#include "commonfunc.h"
#include "ClientSocket/clientSocketThread.h"
#include "ResponseHandler/responseHandler.h"


bool initServerconfig(); //初始化服务器配置信息
bool init();
int main(int argc, char *argv[])
{
    /*防止二次启动*/
    HANDLE hMutex = CreateMutex(nullptr, TRUE, (LPCWSTR)qApp->applicationName().toStdWString().c_str());
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        CloseHandle(hMutex);
        hMutex = NULL;
    }

    QApplication a(argc, argv);
    //生成页面实例
     myMain* mainWindow = myMain::getInstance();

    if(!init())  //如果初始化失败结束进程
    {
        a.exit(-1);
    }

    loginAndRegist* objLoginPage = loginAndRegist::getInstance();

    QThread socketThread = QThread();
    QThread resHandleThread = QThread();
    clientSocketThread* pClientSocket = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
    responseHandler* pResponseHandler = reinterpret_cast<responseHandler*>(CommonFunc::QueryData(RESPONSEHANDLE));

    pClientSocket->moveToThread(&socketThread);
    pResponseHandler->moveToThread(&resHandleThread);
    socketThread.start();
    resHandleThread.start();

    emit objLoginPage->initSocketThread();
    emit objLoginPage->initHandleThread();


    objLoginPage->show();

    a.exec();

    //当程序退出 等待并关闭其他线程。
    socketThread.quit();
    resHandleThread.quit();
    socketThread.wait();
    resHandleThread.wait();

    CommonFunc::setIniFile();
    CloseHandle(hMutex);
    hMutex = NULL;
    return 0;
}

bool init()
{
    bool ret = true;
    ret = ret & CommonFunc::getCurrentWndCountry(); // 获取系统设置的区域
    if(!CommonFunc::getScreenSize()) //如果读取屏幕失败，使用默认的屏幕大小设置
    {
        QRect defaultRect = QRect(0,0,1920,1080);
        CommonFunc::setData(SCREENRECT,&defaultRect);
    }
    ret &= initServerconfig();
    ret &= CommonFunc::initRespondHandle(); //
    ret &= CommonFunc::initClientSocketThread(); //初始化网络线程
    QString strDefaultStorePath = "";
    CommonFunc::readDataFromFile("setting","defaultStoreAddr",strDefaultStorePath);
    if(strDefaultStorePath.isEmpty())
    {
        ret &= CommonFunc::getSystemDownloadPath(); //获取系统默认下载位置
    }else
    {
        CommonFunc::setData(DEFAULTSTOREADDR,&strDefaultStorePath); //获取系统默认下载位置
        ret &= 1;
    }

    ret &= CommonFunc::getVersionFromIniFile();
    ret &= CommonFunc::getQuitModeFromIniFile();

    return ret;
}
//从配置文件中获取服务器ip和port
bool initServerconfig()
{
    bool ret = false;
    QString ip;
    QString port;
    ip.clear();
    port.clear();

    ret = CommonFunc::readDataFromFile("serverinfo","ip",ip);//读取ip
    ret &= CommonFunc::readDataFromFile("serverinfo","port",port); //读取port
    if(ret)
    {

        ret &= CommonFunc::setData(SERVERIP,&ip);
        ret &= CommonFunc::setData(SERVERPORT,&port);

    }
    return ret;

}


