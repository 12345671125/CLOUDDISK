/*opedb类负责与数据库进行通信*/

#ifndef OPEDB_H
#define OPEDB_H
#pragma once


#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include "protocol.h"

class OpeDB : public QObject
{
    Q_OBJECT
public:
    explicit OpeDB(QObject *parent = nullptr);
    static OpeDB& getInsance();  //静态成员函数，单列模式创建数据库对象
    bool handleRegit(const char* name, const char* pwd); //用于处理注册
    bool handleLogin(const char *name, const char *pwd); //用于处理登录
    void handleOffline(const char *name);//用于处理注销
    QStringList handleSearchUser(const char *userName); //用于处理搜索用户
    QStringList handleAllOnline(); //用于显示所有用户
    int handleAddFriend(const char* pername,const char* name,int type = 0);//用于添加好友
    QStringList handleFlushFriends(const char* userName);//用于刷新好友
    void handleDelFriend(const char* username,const char* pername); //用于删除好友
    QStringList handleGetOnlineFriend(const char* userName); //获取在线好友
    //    QStringList handleGetFOnlineStatus(const char* username); //用于刷新好友状态
    bool handleSaveFileInfo(const protocol::FileInfo_s fileInfo);
    QStringList handleGetFileInfo(const char* username,const char* filename);
    ~OpeDB();
signals:

public slots:

private:
    QSqlDatabase m_db; //数据库对象，用于连接数据库
    void init(); //用于数据库对象的初始化
    int getId(const char* username);
    QString getUserName(const int id);

};

#endif // OPEDB_H
