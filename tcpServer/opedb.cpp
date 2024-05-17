/*opedb类负责与数据库进行通信*/


#include "opedb.h"

OpeDB::OpeDB(QObject *parent) : QObject(parent)
{
    this->m_db = QSqlDatabase::addDatabase("QSQLITE");
    this->init();
}

OpeDB& OpeDB::getInsance()
{
    static OpeDB instance;
    return instance;
}

bool OpeDB::handleRegit(const char *name, const char *pwd)
{
    if(name == nullptr || pwd == nullptr ){   //当name 或 pwd 为NULL时
        return false;
    }
    QString sqlString = QString("insert into userInfo(name,pwd) values('%1','%2')").arg(name).arg(pwd);
    qDebug()<<sqlString;
    QSqlQuery query;
    return query.exec(sqlString);

}

bool OpeDB::handleLogin(const char *name, const char *pwd)
{
    if(name == nullptr || pwd == nullptr ){   //当name 或 pwd 为NULL时
        return false;
    }
    QString sqlString = QString("select * from userInfo where name = \'%1\' and pwd = \'%2\' and  online = 0").arg(name).arg(pwd);
    //    qDebug()<<sqlString;
    QSqlQuery query;
    query.exec(sqlString);
    if(query.next()){
        QString sqlString = QString("update userInfo set online = 1 where name = \'%1\' and pwd = \'%2\'").arg(name).arg(pwd);
        QSqlQuery query;
        query.exec(sqlString);
        return true;
    }else{
        return false;
    }
}

void OpeDB::handleOffline(const char *name)
{
    if(name == nullptr){   //当name 或 pwd 为NULL时
        return;
    }
    QString queryStr = QString("update userInfo set online = 0 where name = \'%1\'").arg(name);
    qDebug()<<queryStr;
    QSqlQuery query;
    query.exec(queryStr);

}

QStringList OpeDB::handleSearchUser(const char *userName)
{

    QString queryStr = QString("select name from userInfo where name like \'%%1%\'").arg(userName);
    qDebug()<<queryStr;
    QSqlQuery query;
    query.exec(queryStr);
    QStringList result;
    result.clear();
    while(query.next()){
        result.append(query.value(0).toString());
    }
    return result;
}

QStringList OpeDB::handleAllOnline()
{
    QString queryStr = QString("select name from userInfo where online=1");
    qDebug()<<queryStr;
    QSqlQuery query;
    query.exec(queryStr);
    QStringList result;
    result.clear();
    while(query.next()){
        result.append(query.value(0).toString());
    }
    return result;
}

int OpeDB::handleAddFriend(const char *pername, const char *name,int type)
{
    if(pername == NULL || name == NULL){
        return -1;
    }
    if(type == 0){
        QString data = QString("select * from friend where (id = (select id from userInfo where name = \'%1\') and friendId = (select id from userInfo where name = \'%2\')) or (id = (select id from userInfo where name = \'%3\') and friendId = (select id from userInfo where name = \'%4\'))").arg(pername).arg(name).arg(name).arg(pername);
        qDebug() << data;
        QSqlQuery query;
        query.exec(data);
        if(query.next())
        {
            return 0; //双方已经是好友
        }
        else
        {
            data = QString("select online from userInfo where name = \'%1\'").arg(pername);
            QSqlQuery query;
            query.exec(data);
            if(query.next()){
                int ret = query.value(0).toInt();
                if(ret == 1){
                    return 1;//在线
                }
                else if(ret == 0){
                    return 2;//不在线
                }
            }else{
                return 3;//用户不存在
            }

        }

    }else if(type == 1){
        int userId = this->getId(name);
        int perId = this->getId(pername);
        if(userId != -1 && perId != -1){
            QString data = QString("insert into friend(id,friendId) values(%1,%2);").arg(userId).arg(perId);
            qDebug()<<data;
            QSqlQuery query;
            query.exec(data);
        }

    }

}

QStringList OpeDB::handleFlushFriends(const char* userName)
{
    QStringList resultList;
    resultList.clear();
    if(userName == nullptr) return resultList;
    int userid = this->getId(userName);
    //     qDebug()<<userName;
    //qDebug()<<userid;
    QString data = QString("select id,online from userInfo where id in (select friendId from friend,userInfo where friend.id = %1 and friend.id = userInfo.id)").arg(userid);
    QSqlQuery query;
    query.exec(data);
    //qDebug()<<query.next();
    while(query.next()){
        int ret = query.value(0).toInt();
        resultList.push_back(this->getUserName(ret).append(":" + query.value(1).toString()));
    }
    data = QString("select id,online from userInfo where id in (select friend.id from friend,userInfo where friend.id = userInfo.id and friend.friendId = %1)").arg(userid);
    query.exec(data);
    while(query.next()){
        int ret = query.value(0).toInt();
        resultList.push_back(this->getUserName(ret).append(":" + query.value(1).toString()));
    }

    return resultList;

}

void OpeDB::handleDelFriend(const char *username, const char *pername)
{
    if(username == nullptr || pername == nullptr) return;
    int perId = getId(pername);
    int userId = getId(username);
    qDebug()<<pername;
    qDebug()<<username;
    QString data = QString("delete from friend where (id = %1 and friendId = %2) or (id = %3 and friendId = %4)").arg(perId).arg(userId).arg(userId).arg(perId);
    qDebug()<<data;
    QSqlQuery query;
    query.exec(data);

}

QStringList OpeDB::handleGetOnlineFriend(const char *userName)
{
    QStringList resultList;
    resultList.clear();
    if(userName == nullptr) return resultList;
    int userid = this->getId(userName);
    //qDebug()<<userName;
    //qDebug()<<userid;
    QString data = QString("select friend.friendId from friend,userInfo where friend.id = %1 and friend.id = userInfo.id and userInfo.online = 1").arg(userid); //从数据库中获取用户好友id

    QSqlQuery query;
    query.exec(data);
    while(query.next()){        //通过用户id获取用户名
        int ret = query.value(0).toInt();
        resultList.push_back(this->getUserName(ret));
    }
    data = QString("select friend.id from friend,userInfo where friend.friendId = %1 and friend.friendId = userInfo.id and userInfo.online = 1").arg(userid); //从数据库中获取用户好友id

    query.exec(data);
    while(query.next()){       //通过用户id获取用户名
        int ret = query.value(0).toInt();
        resultList.push_back(this->getUserName(ret)); //将获取到的用户名添加到string
    }

    return resultList;
}

bool OpeDB::handleSaveFileInfo(const protocol::FileInfo_s fileInfo)
{
    QString data = QString("insert into fileInfo(filename,filesize,uploadtime,uploadname) values('%1',%2,'%3','%4')").arg(fileInfo.filename).arg(fileInfo.filesize).arg(fileInfo.uploadTime.toString()).arg(fileInfo.uploadname);
    qDebug()<<data;
    QSqlQuery query;
    return query.exec(data);
}

QStringList OpeDB::handleGetFileInfo(const char *username, const char *filename)
{
    QStringList result = QStringList();
    result.clear();
    if(username == NULL || filename == NULL) return result;
    QString data = QString("select filename,filesize,uploadtime,uploadname from fileInfo where filename = '%1' and uploadname = '%2'").arg(filename).arg(username);
    QSqlQuery query;
    qDebug()<<data;
    query.exec(data);
    if(query.next()){
        result.push_back(query.value(0).toString());
        result.push_back(query.value(1).toString());
        result.push_back(query.value(2).toString());
        result.push_back(query.value(3).toString());
    }
    return result;

}



//QStringList OpeDB::handleGetFOnlineStatus(const char *username)
//{
//    QStringList resultList;
//    resultList.clear();
//    if(username == nullptr) return resultList;
//    int userid = this->getId(username);
//    //     qDebug()<<userName;
//    //qDebug()<<userid;
//    QString dataName = QString("select friend.friendId,userInfo.online from friend,userInfo where friend.id = %1 and friend.id = userInfo.id").arg(userid);
////    QString dataOnline = QString("select online from friend where id = %1").arg(userid);
//    QSqlQuery query;
////    QSqlQuery query1;
//    query.exec(dataName);
//    //qDebug()<<query.next();
//    while(query.next()){
//        int ret = query.value(0).toInt();
//        resultList.push_back(this->getUserName(ret).append(":" + query.value(0).toString()));
//    }
//    dataName = QString("select friend.id,userInfo.online from friend,userInfo where friend.id = userInfo.id and friend.friendId = %1").arg(userid);
//    query.exec(dataName);
//    while(query.next()){
//        int ret = query.value(0).toInt();
//        resultList.push_back(this->getUserName(ret).append(":" + query.value(0).toString()));
//    }

//    return resultList;


//}

OpeDB::~OpeDB()
{
    this->m_db.close();
}

void OpeDB::init()
{
    this->m_db.setHostName("localhost");//添加数据库用户名
    this->m_db.setDatabaseName("D:\\code\\CLOUDDISK-master\\tcpServer\\debug\\cloud.db"); //添加对应数据库名
    if(this->m_db.open()){  //调用open方法打开数据库
        QSqlQuery query;//创建数据库查询对象
        query.exec("select * from userInfo");//数据库查询对象执行对应sql语句
        while(query.next()){
            QString data = QString("%1,%2,%3").arg(query.value(0).toString(),query.value(1).toString(),query.value(2).toString());
            qDebug() << data;
        }
    }else{
        QMessageBox::critical(NULL,"打开数据库","打开数据库失败");
    }
}

int OpeDB::getId(const char *username)  //用于从数据库中通过用户名获取对应用户id
{
    int id = 0;
    if(username  == NULL){
        return -1;
    }else{
        QString data = QString("select id from userInfo where name = \'%1\'").arg(username);
        //qDebug()<<data;
        QSqlQuery query;
        query.exec(data);
        if(query.next()){
            id = query.value(0).toInt();
        }
        //qDebug()<<"userId"<<id;
        return id;
    }
}

QString OpeDB::getUserName(const int id) //用于从数据库中通过用户id获取用户名
{
    QString FriendName;
    QString data = QString("select name from userInfo where id = %1").arg(id);
    qDebug()<<data;
    QSqlQuery query;
    query.exec(data);
    if(query.next()){
        FriendName = QString(query.value(0).toString());
    }
    //    qDebug()<<FriendName;
    return FriendName;
}
