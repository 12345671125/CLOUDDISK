#ifndef COMMONFUNC_H
#define COMMONFUNC_H


class QString;
class QWidget;
#include <QObject>
class CommonFunc
{
public:
    static bool getCurrentWndCountry();
    static long long QueryData(int code); //从commondata查询数据
    static bool setData(int code, void* value); //设置数据到commondata
    static bool readLangFromFile(QString key,QString& strRet);//从ini文件中获取控件文言
    static bool readDataFromFile(QString section,QString key,QString& strRet); //从ini文件中获取数据
    static bool readErrorFromFile(QString key,QString& strRet); //从ini文件中获取错误文言
    static bool getScreenSize();
    static bool setTitleToControl(QWidget* control,QString key);
    static QString mergePaths(QString path);
    static bool initClientSocketThread();
    static bool initRespondHandle();
    static void removeNullCharacters(QString& str);
    static QString getFileNameFromAbsolutePath(QString fileAbsolutePath);
    static int getTcpDataSize(const QString sourceData);
    static int getSystemDownloadPath();
    static int getVersionFromIniFile();
    static int getQuitModeFromIniFile();
    static int setIniFile();
    static bool writeIniFile(QString section,QString value);
private:
    CommonFunc();
};

#endif // COMMONFUNC_H
