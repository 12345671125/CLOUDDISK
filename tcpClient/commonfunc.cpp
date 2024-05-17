#include "commonfunc.h"
#include <QString>
#include <QLocale>
#include "commondata.h"
#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QGuiApplication>
#include<QScreen>
#include <QRect>
#include<QWidget>
#include<QTcpSocket>
#include "ClientSocket/clientSocketThread.h"
#include "ResponseHandler/responseHandler.h"
#include <QStandardPaths>
#define pData long long

bool CommonFunc::getCurrentWndCountry()
{
    QLocale objLocale;
    QString strLanguage =objLocale.languageToString(objLocale.language());
    if(CommonFunc::setData(LOCALLANGUAGE,&strLanguage))
    {
        return true;
    }
    return false;
}

long long CommonFunc::QueryData(int code)
{
    pData data = 0;
    data = CommonData::getInstance().QueryOrSetData(code,0,NULL);

    return data;
}

bool CommonFunc::setData(int code, void* value)
{
    if(CommonData::getInstance().QueryOrSetData(code,1,value))
    {
        return  true;
    };

    return false;
}

bool CommonFunc::readLangFromFile(QString key,QString& strRet)
{
    QString strLocalLang =*reinterpret_cast<QString*>(QueryData(LOCALLANGUAGE)); //获取当前国家信息
    QString strIniFileName = strLocalLang.append(INIFILEEXTENSION); //获取ini文件名
    QDir objDir = QDir(QApplication::applicationFilePath());
    objDir.cdUp();
    QString strDirPath = objDir.path();
    if(!strDirPath.isEmpty())
    {
        QFile objIniFile = QFile(strDirPath + "\\" + strIniFileName);
        if(!objIniFile.exists())
        {
            return false;
        }
        QSettings objIniSetFile = QSettings(objIniFile.fileName(),QSettings::IniFormat);
        objIniSetFile.beginGroup("translate");
        strRet = objIniSetFile.value(key).toString();
        return true;
    }
    return false;
}

bool CommonFunc::readDataFromFile(QString section, QString key, QString &strRet)
{
    QString strLocalLang =*reinterpret_cast<QString*>(QueryData(LOCALLANGUAGE)); //获取当前国家信息
     QString strIniFileName = strLocalLang.append(INIFILEEXTENSION); //获取ini文件名
     QDir objDir = QDir(QApplication::applicationFilePath()); //获取当前app所在全路径
     objDir.cdUp(); //获取父路径
     QString strDirPath = objDir.path();
     if(!strDirPath.isEmpty()) //路径非空判断
     {
         QFile objIniFile = QFile(strDirPath + "\\" + strIniFileName);
         if(!objIniFile.exists())
         {
             return false;
         }
         QSettings objIniSetFile = QSettings(objIniFile.fileName(),QSettings::IniFormat);
         objIniSetFile.beginGroup(section);
         strRet = objIniSetFile.value(key).toString();
         return true;
     }
     return false;
}

bool CommonFunc::readErrorFromFile(QString key, QString &strRet)
{
    QString strLocalLang =*reinterpret_cast<QString*>(QueryData(LOCALLANGUAGE)); //获取当前国家信息
    QString strIniFileName = strLocalLang.append(INIFILEEXTENSION); //获取ini文件名
    QDir objDir = QDir(QApplication::applicationFilePath());
    objDir.cdUp();
    QString strDirPath = objDir.path();
    if(!strDirPath.isEmpty())
    {
        QFile objIniFile = QFile(strDirPath + "\\" + strIniFileName);
        if(!objIniFile.exists())
        {
            return false;
        }
        QSettings objIniSetFile = QSettings(objIniFile.fileName(),QSettings::IniFormat);
        objIniSetFile.beginGroup("error");
        strRet = objIniSetFile.value(key).toString();
        return true;
    }
    return false;
}

bool CommonFunc::getScreenSize()
{
    bool ret = false;
    QRect rect = QGuiApplication::primaryScreen()->geometry(); //获取屏幕大小;
    if (rect.width() != 0 && rect.height() != 0)
    {
        CommonFunc::setData(SCREENRECT,&rect);
        ret = true;
    }
    return ret;
}

bool CommonFunc::setTitleToControl(QWidget *control, QString key)
{
    bool ret = false;
    if(control)
    {
        QString strTmp = {};
        CommonFunc::readLangFromFile(key,strTmp);
        if(!strTmp.isEmpty())
        {
            control->setWindowTitle(strTmp);
            ret = true;
        }else
        {
            ret = false;
        }

    }else
    {
        ret = false;
    }
    return ret;
}

QString CommonFunc::mergePaths(QString path)
{
    QString ret = {};
    ret.clear();
    QString serverPath = *reinterpret_cast<QString*>(CommonFunc::QueryData(SERVERPATH));
    if(!serverPath.isEmpty() && !path.isEmpty())
    {
        ret = serverPath.append("/").append(path);//服务器地址拼接上/再拼接上新的路径
        ret.append('\0');//在末尾加上'\0'
    }
    return ret;
}

bool CommonFunc::initClientSocketThread()
{
    bool ret = true;
    clientSocketThread* pClientSocketThread  = NULL;
    pClientSocketThread = new clientSocketThread();
    if(!pClientSocketThread)
    {
        ret &= false;
    }
    CommonFunc::setData(CLIENTSOCKETTHREAD,pClientSocketThread);
    return ret;

}

bool CommonFunc::initRespondHandle()
{
    bool ret = true;
    responseHandler* resHandler = &responseHandler::getInstance();
    if(resHandler == NULL)
    {
        ret &= false;
        return ret;
    }
    CommonFunc::setData(RESPONSEHANDLE,resHandler);
    return ret;
}

void CommonFunc::removeNullCharacters(QString &str)
{
        if(str.indexOf(QChar::Null) != -1)//如果末尾有 \0 去除末尾 \0
        {
            str.truncate(str.indexOf(QChar::Null));
        }
}

QString CommonFunc::getFileNameFromAbsolutePath(QString fileAbsolutePath)
{
    QString strFileName = fileAbsolutePath.mid(fileAbsolutePath.lastIndexOf("/")+1,fileAbsolutePath.length()-1); //从文件完整路径中截取文件名

    CommonFunc::removeNullCharacters(strFileName); //去除末尾可能存在的null

    return strFileName;
}

int CommonFunc::getTcpDataSize(const QString sourceData)
{
    int ret = 0;
    ret = sourceData.toUtf8().size(); //获取sourceData 转换为Utf8后的大小
    return ret;
}

int CommonFunc::getSystemDownloadPath()
{
    int ret = 0;
    QStringList strSystemDownloadPathList =  QStandardPaths::standardLocations(QStandardPaths::DownloadLocation);
    if(!strSystemDownloadPathList.isEmpty())
    {
        CommonFunc::setData(DEFAULTSTOREADDR,&strSystemDownloadPathList[0]);
        ret = 1;
    }
    return ret;
}

int CommonFunc::getVersionFromIniFile()
{
    int ret = 0;
    QString strVersion = "";
    CommonFunc::readDataFromFile("setting","version",strVersion);
    if(!strVersion.isEmpty())
    {
        CommonFunc::setData(VERSION,&strVersion);
        QCoreApplication::setApplicationVersion(strVersion);
        ret = 1;
    }
    return ret;
}

int CommonFunc::getQuitModeFromIniFile()
{
    int ret = 0;
    QString strQuitMode = "";
    CommonFunc::readDataFromFile("setting","quitMode",strQuitMode);
    if(!strQuitMode.isEmpty())
    {
        CommonFunc::setData(QUITMODE,&strQuitMode);
        ret = 1;
    }
    return ret;
}

int CommonFunc::setIniFile()
{
    QString strQuitMode = *reinterpret_cast<QString*>(CommonFunc::QueryData(QUITMODE));
    QString strDefaultStorePath = *reinterpret_cast<QString*>(CommonFunc::QueryData(DEFAULTSTOREADDR));
    CommonFunc::writeIniFile("setting/quitMode",strQuitMode);
    CommonFunc::writeIniFile("setting/defaultStoreAddr",strDefaultStorePath);
}

bool CommonFunc::writeIniFile(QString section, QString value)
{
    QString strLocalLang =*reinterpret_cast<QString*>(QueryData(LOCALLANGUAGE)); //获取当前国家信息
    QString strIniFileName = strLocalLang.append(INIFILEEXTENSION); //获取ini文件名
    QDir objDir = QDir(QApplication::applicationFilePath()); //获取当前app所在全路径
    objDir.cdUp(); //获取父路径
    QString strDirPath = objDir.path();
    if(!strDirPath.isEmpty()) //路径非空判断
    {
        QFile objIniFile = QFile(strDirPath + "\\" + strIniFileName);
        if(!objIniFile.exists())
        {
            return false;
        }
        QSettings objIniSetFile = QSettings(objIniFile.fileName(),QSettings::IniFormat);

        objIniSetFile.setValue(section,value);
        return true;
    }
    return false;
}
CommonFunc::CommonFunc() {}
