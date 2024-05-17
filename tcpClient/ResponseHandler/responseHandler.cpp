#include "ResponseHandler/responseHandler.h"
#include <windows.h>
#include "commonfunc.h"
#include "commondata.h"
#include "PDU/protocol.h"
#include <QMessageBox>
#include "LoginAndRegist/loginAndRegist.h"
#include "MainWIndow/opewidget.h"

responseHandler &responseHandler::getInstance(QObject *parent)
{
    static responseHandler instance(parent);
    return instance;
}

responseHandler::~responseHandler()
{

}

void responseHandler::initHandle()
{
    connect(this,SIGNAL(m_sRegistRet(int)),loginAndRegist::getInstance(),SLOT(showRegistRet(int)));
    connect(this,SIGNAL(m_sPublicChatTrans(protocol::PDU*)),OpeWidget::getinstance(),SLOT(ShowPublicChat(protocol::PDU*)));
    connect(this,SIGNAL(m_sShowAddFriendRet(protocol::PDU*)),OpeWidget::getinstance(),SLOT(ShowAddFriendRet(protocol::PDU*)));
    connect(this,SIGNAL(m_sLoginSuccess()),loginAndRegist::getInstance(),SLOT(loginSuccess()));
    connect(this,SIGNAL(m_sLoginError()),loginAndRegist::getInstance(),SLOT(loginError()));
    connect(this,SIGNAL(m_sAllOnline(protocol::PDU*)),OpeWidget::getinstance(),SLOT(AllOnlineRes(protocol::PDU*)));
    connect(this,SIGNAL(m_sSearchUser(protocol::PDU*)),OpeWidget::getinstance(),SLOT(SearchUserRes(protocol::PDU*)));
    connect(this,SIGNAL(m_sAddFriend(protocol::PDU*)),OpeWidget::getinstance(),SLOT(addFriend(protocol::PDU*)));
    connect(this,SIGNAL(m_sCreateDir(protocol::PDU*)),OpeWidget::getinstance(),SLOT(createDir(protocol::PDU*)));
    connect(this,SIGNAL(m_sFlushFile(protocol::PDU*)),OpeWidget::getinstance(),SLOT(FlushFile(protocol::PDU*)));
    connect(this,SIGNAL(m_sPrivateChatTrans(protocol::PDU*)),OpeWidget::getinstance(),SLOT(ShowPrivateChat(protocol::PDU*)));
    connect(this,SIGNAL(m_sDelDir(protocol::PDU*)),OpeWidget::getinstance(),SLOT(ShowDelDirRet(protocol::PDU*)));
    connect(this,SIGNAL(m_sDelFile(protocol::PDU*)),OpeWidget::getinstance(),SLOT(ShowDelFileRet(protocol::PDU*)));
    connect(this,SIGNAL(m_sRenameFile(protocol::PDU*)),OpeWidget::getinstance(),SLOT(ShowRenameFileRet(protocol::PDU*)));
    connect(this,SIGNAL(m_sShowUploadFileInfoRes(protocol::PDU*)),OpeWidget::getinstance(),SLOT(ShowUploadFileInfoRet(protocol::PDU*)));
    connect(this,SIGNAL(m_sHandleDownloadFile(protocol::PDU*)),OpeWidget::getinstance(),SLOT(HandleDownloadFile(protocol::PDU*)));
    connect(this,SIGNAL(m_sHandleDownloadPublicFile(protocol::PDU*)),OpeWidget::getinstance(),SLOT(HandleDownloadPublicFile(protocol::PDU*)));
    connect(this,SIGNAL(m_sShowFileInfo(protocol::PDU*)),OpeWidget::getinstance(),SLOT(ShowFileInfo(protocol::PDU*)));
    connect(this,SIGNAL(m_sShowUploadPublicFileInfoRes(protocol::PDU*)),OpeWidget::getinstance(),SLOT(ShowUploadPublicFileInfoRet(protocol::PDU*)));
    connect(this,SIGNAL(m_sFlushPublicFile(protocol::PDU*)),OpeWidget::getinstance(),SLOT(FlushPublicFile(protocol::PDU*)));
    connect(this,SIGNAL(m_sFlushFriend(protocol::PDU*)),OpeWidget::getinstance(),SLOT(FlushFriend(protocol::PDU*)));
}

void responseHandler::handleShowAddFriendRet(protocol::PDU* resPdu)
{
   emit this->m_sShowAddFriendRet(resPdu);
}
responseHandler::responseHandler(QObject *parent) :
    QObject(parent)
{

}

void responseHandler::handleLoginPathRes(protocol::PDU* resPdu)
{
    char serverPath[MAX_PATH] = {};
    memcpy_s(serverPath,MAX_PATH,(char*)resPdu->caMsg,resPdu->uiMsgLen);
    QString strPath = serverPath;
    /*qDebug()<<strPath*/;
    if(!strPath.isEmpty())
    {
        CommonFunc::setData(SERVERPATH,&strPath);
    }
    free(resPdu);
    resPdu = NULL;
}

void responseHandler::handlePublieFilePathRes(protocol::PDU *resPdu)
{
    char publicFilePath[MAX_PATH] = {};
    memcpy_s(publicFilePath,MAX_PATH,(char*)resPdu->caMsg,resPdu->uiMsgLen);
    QString strPath = publicFilePath;
    CommonFunc::removeNullCharacters(strPath);
    if(!strPath.isEmpty())
    {
        CommonFunc::setData(PUBLICFILEPATH,&strPath);
    }
    free(resPdu);
    resPdu = NULL;
}

void responseHandler::handleRegistRes(protocol::PDU* resPdu)
{
    //qDebug() << resPdu->caData;
    if(strcmp(resPdu->caData,REGIST_OK) == 0){
        emit this->m_sRegistRet(0);
    }else if(strcmp(resPdu->caData,REGIST_FAILED) == 0){
        emit this->m_sRegistRet(-1);
    }
    free(resPdu);
    resPdu = NULL;
}

void responseHandler::handleLoginRes(protocol::PDU* resPdu)
{
    //qDebug() << resPdu->caData;
    if(strcmp(resPdu->caData,LOGIN_OK) == 0){
        QString strUsername = *reinterpret_cast<QString*>(CommonFunc::QueryData(USERNAME));
        //QString strServerPath = *reinterpret_cast<QString*>(CommonFunc::QueryData(SERVERPATH));
        QString strCurPath = CommonFunc::mergePaths(strUsername);

        if(strCurPath.indexOf(QChar::Null) != -1)
        {
            strCurPath.truncate(strCurPath.indexOf(QChar::Null));
        }

        CommonFunc::setData(STRROOTPATH,&strCurPath);
        CommonFunc::setData(STRCURPATH,&strCurPath);

        emit this->m_sLoginSuccess(); //发送登录成功信号
    }else
    {
        emit this->m_sLoginError(); //发送登录失败信号
    }
    free(resPdu);
    resPdu = NULL;
}

void responseHandler::handleAllOnlineRes(protocol::PDU* resPdu)
{
    emit this->m_sAllOnline(resPdu);
}

void responseHandler::handSearchUserRes(protocol::PDU* resPdu)
{
    emit this->m_sSearchUser(resPdu);
}

void responseHandler::handleAddFriendReq(protocol::PDU* resPdu)
{
    emit this->m_sAddFriend(resPdu);
}

void responseHandler::handAddFriendRes(protocol::PDU* resPdu)
{
    emit this->m_sShowAddFriendRet(resPdu);
}

void responseHandler::handleFlushFriendRes(protocol::PDU* resPdu)
{
    emit this->m_sFlushFriend(resPdu);
}

void responseHandler::handlePrivateChatTrans(protocol::PDU* resPdu)
{
    emit this->m_sPrivateChatTrans(resPdu);
}

void responseHandler::handlePublicChatTrans(protocol::PDU* resPdu)
{
    emit this->m_sPublicChatTrans(resPdu);
}

void responseHandler::handleCreateDirRes(protocol::PDU* resPdu)
{
    emit this->m_sCreateDir(resPdu);
}

void responseHandler::handleFlushFileRes(protocol::PDU* resPdu)
{
    emit this->m_sFlushFile(resPdu);
}

void responseHandler::handleDeleteFileRes(protocol::PDU* resPdu)
{
    emit this->m_sDelFile(resPdu);
}

void responseHandler::handleDeleteDirRes(protocol::PDU* resPdu)
{
    emit this->m_sDelDir(resPdu);
}

void responseHandler::handleRenameFileRes(protocol::PDU* resPdu)
{
    emit this->m_sRenameFile(resPdu);
}

void responseHandler::handleUploadFileFinRes(protocol::PDU* resPdu)
{

}

void responseHandler::handleGetFileInfoRes(protocol::PDU* resPdu)
{
    emit this->m_sShowFileInfo(resPdu);
}

void responseHandler::handleErrorRes(protocol::PDU* resPdu)
{

}

void responseHandler::handleGetUploadFileInfoRes(protocol::PDU *resPdu)
{
    emit this->m_sShowUploadFileInfoRes(resPdu);
}

void responseHandler::handleOpenUploadAndDownLoadPage()
{
    emit this->m_sOpenUploadAndDownLoadPage();
}

void responseHandler::handleDownloadFile(protocol::PDU* resPdu)
{
    emit this->m_sHandleDownloadFile(resPdu);
}

void responseHandler::handleGetUploadPublicFileInfoRes(protocol::PDU *resPdu)
{
    emit this->m_sShowUploadPublicFileInfoRes(resPdu);
}

void responseHandler::handleFlushPublicFileRes(protocol::PDU *resPdu)
{
    emit this->m_sFlushPublicFile(resPdu);
}

void responseHandler::handleDownloadPublicFile(protocol::PDU *resPdu)
{
    emit this->m_sHandleDownloadPublicFile(resPdu);
}
