/*protocol类为协议类，规定cs端之间的通信细节，用于cs端传输数据*/

#ifndef PROTOCOL_H
#define PROTOCOL_H
#pragma once

#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <QString>
#include <QList>
#include <QDateTime>

typedef unsigned int uint;

#define REGIST_OK "1"
#define REGIST_FAILED "0"

#define LOGIN_OK "1"
#define LOGIN_FAILED "0"

#define DIR_NOT_EXIST  "0"
#define FILE_NAME_EXIT "0"

#define CREATE_DIR_SUCESS "1"
#define MAXFILENAME 256

namespace protocol {

enum FILE_TYPE{
    FILE_TYPE_DIR = 0,
    FILE_TYPE_FILE,
    FILE_TYPE_MAX = 0x00ffffff
};

enum ENUM_MSG_TYPE{
    ENUM_MSG_TYPE_MIN = 0,
    ENUM_MSG_TYPE_SERVER_LOCALPATH_REQUEST, //请求服务器路径
    ENUM_MSG_TYPE_SERVER_LOCALPATH_RESPOND,//服务器路径相应

    ENUM_MSG_TYPE_PUBLICFILEPATH_REQUEST,//请求公共路径
    ENUM_MSG_TYPE_PUBLICFILEPATH_RESPOND,//响应公共路径请求

    ENUM_MSG_TYPE_REGIST_REQUEST, //注册请求
    ENUM_MSG_TYPE_REGIST_RESPOND, //注册回复

    ENUM_MSG_TYPE_LOGIN_REQUEST, //登录请求
    ENUM_MSG_TYPE_LOGIN_RESPOND, //登录回复

    ENUM_MSG_TYPE_ALL_ONLINE_REQUEST, //在线用户请求
    ENUM_MSG_TYPE_ALL_ONLINE_RESPOND,  //在线用户回复

    ENUM_MSG_TYPE_SEARCHUSER_REQUEST, //搜索用户请求
    ENUM_MSG_TYPE_SEARCHUSER_RESPOND, //搜索用户回复

    ENUM_MSG_TYPE_ADDFRIEND_REQUEST, //添加好友请求
    ENUM_MSG_TYPE_ADDFRIEND_RESPOND,//添加好友回复

    ENUM_MSG_TYPE_ADDFRIEND_AGREE, //同意添加好友
    ENUM_MSG_TYPE_ADDFRIEND_REFUSE,//拒绝添加好友

    ENUM_MSG_TYPE_FLUSH_FRIEND_REQUEST,//刷新好友请求
    ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND,//刷新好友回复

    ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST, //删除好友请求
    ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND, //删除好友回复

    ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST, //私聊请求
    ENUM_MSG_TYPE_PRIVATE_CHAT_TRANSMIT, //服务器转发消息
    ENUM_MSG_TYPE_PRIVATE_CHAT_RESPOND, //私聊回复

    ENUM_MSG_TYPE_PUBLIC_CHAT_REQUEST, //群聊请求
    ENUM_MSG_TYPE_PUBLIC_CHAT_TRANSMIT, //服务器转发消息
    ENUM_MSG_TYPE_PUBLIC_CHAT_RESPOND, //群聊请求

    ENUM_MSG_TYPE_FONLINE_STATUS_REQUEST, //查询好友是否在线请求
    ENUM_MSG_TYPE_FONLINE_STATUS_RESPOND, //查询好友是否在线回复

    ENUM_MSG_TYPE_CREATE_DIR_REQUEST, //创建文件夹请求
    ENUM_MSG_TYPE_CREATE_DIR_RESPOND, //创建文件夹响应

    ENUM_MSG_TYPE_FLUSH_FILE_REQUEST, //刷新文件请求
    ENUM_MSG_TYPE_FLUSH_FILE_RESPOND, //刷新文件回复

    ENUM_MSG_TYPE_FLUSH_PUBLIC_FILE_REQUEST, //刷新公共文件请求
    ENUM_MSG_TYPE_FLUSH_PUBLIC_FILE_RESPOND, //刷新公共文件回复

    ENUM_MSG_TYPE_DELETE_DIR_REQUEST, //删除文件夹请求
    ENUM_MSG_TYPE_DELETE_DIR_RESPOND, //删除文件夹回复

    ENUM_MSG_TYPE_DELETE_FILE_REQUEST, //删除文件请求
    ENUM_MSG_TYPE_DELETE_FILE_RESPOND, //删除文件回复

    ENUM_MSG_TYPE_RENAME_FILE_REQUEST, //文件重命名请求
    ENUM_MSG_TYPE_RENAME_FILE_RESPOND, //文件重命名回复

    ENUM_MSG_TYPE_UPLOADFILEINFO_REQUEST, //上传文件信息请求
    ENUM_MSG_TYPE_UPLOADFILEINFO_RESPOND, //上传文件信息回复

    ENUM_MSG_TYPE_UPLOADPUBLICFILEINFO_REQUEST, //上传公共文件信息请求
    ENUM_MSG_TYPE_UPLOADPUBLICFILEINFO_RESPOND,//上传公共文件信息响应

    ENUM_MSG_TYPE_GETFILEINFO_REQUEST, //文件信息请求
    ENUM_MSG_TYPE_GETFILEINFO_RESPOND, //文件信息回复

    ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST, //上传文件请求
    ENUM_MSG_TYPE_UPLOADGET_FILE_RESPOND, //服务器得到上传文件请求
    ENUM_MSG_TYPE_UPLOADBEG_FILE_REQUEST, //客户端上传文件开始请求
    ENUM_MSG_TYPE_UPLOADBEGING_FILE_REQUEST, //客户端已获取文件大小请求
    ENUM_MSG_TYPE_UPLOADFIN_FILE_REQUEST, //客户端上传文件结束请求
    ENUM_MSG_TYPE_UPLOADFIN_FILE_RESPOND, //上传文件结束回复

    ENUM_MSG_TYPE_CANCEL_UPLOAD_FILE_REQUEST,//取消文件请求
    ENUM_MSG_TYPE_CANCEL_UPLOAD_FILE_RESPOND,//取消文件回复

    ENUM_MSG_TYPE_DOWNLOAD_FILE_REQUEST, //下载文件请求
    ENUM_MSG_TYPE_DOWNLOADGET_FILE_RESPOND, //服务器得到下载文件请求
    ENUM_MSG_TYPE_DOWNLOADBEG_FILE_RESPOND, //服务器开始发送文件回复
    ENUM_MSG_TYPE_DOWNLOADREADY_FILE_REQUEST, //客户端已准备接收文件请求
    ENUM_MSG_TYPE_DOWNLOADFIN_FILE_REQUEST, //接收文件结束请求
    ENUM_MSG_TYPE_DOWNLOADFIN_FILE_RESPOND, //服务器发送文件结束回复

    ENUM_MSG_TYPE_DOWNLOAD_PUBLUC_FILE_REQUEST, //下载公共文件请求
    ENUM_MSG_TYPE_DOWNLOADGET_PUBLIC_FILE_RESPOND, //服务器得到下载公共文件请求

    ENUM_MSG_TYPE_ERROR_RESPOND,  //严重请求错误
    ENUM_MSG_TYPE_MAX = 0x00ffffff,
};
struct PDU
{
    static PDU default_respond(uint Type,QString respondStr,uint MsgSize = 0); //默认请求模板
    static PDU default_request(uint Type,QString requestStr,uint MsgSize = 0); //默认回复模板
    uint PDULen; //总的协议数据单元大小
    uint uiMsgType; //消息类型
    char caData[128]; //文件名
    uint uiMsgLen; //实际消息长度
    int caMsg[]; //实际消息  直接申请一个指针应该是一个效果
};

//struct dataPackage
//{
//    char filename[64];
//    qint64 dataSize; //本次数据大小
//    qint64 sentFileSize;//已发送数据大小
//    qint64 totalFileSize; //总数据大小
//};
struct FileInfo
{
    int totalSize; //总文件结构体大小
    int iFileType;   //文件类型
    char caFileName[MAXFILENAME]; //文件名
    qint64 FileSize; //文件大小
    int pathLen; //保存路径长
    char savaPath[]; //所要保存的路径
};
struct FileInfo_s
{
    char filename[MAXFILENAME] ;   //文件名
    qint64 filesize;    //文件大小
    QDateTime uploadTime;   //上传时间
    char uploadname[64];     //上传者
};
struct FileInfoList
{
    int FileListLength;   //存放文件数量
    int FileListSize; //总文件数组大小
    int structSize;   //总文件结构体大小
    protocol::FileInfo FileList[];  //具体文件数组
};
struct FileDetail
{
    int filenameLen;
    int fileSizeLen;
    int uploadTimeLen;
    int uploaduserLen;
    char data[];
};

PDU* createPDU(uint uiMsgLen);
//dataPackage* createDataPackage(QString fileName,qint64 dataSize,qint64 sentFileSize,qint64 totalFileSize);
FileInfoList* createFileInfoList(int length);
FileInfo* createFileInfo(const int iFileType,const char* caFileName,const qint64 FileSize,int pathLen);
FileInfo_s* createFileInfo_s(const char* filename,const qint64 filesize,const char* uploadname);
};

#endif // PROTOCOL_H
