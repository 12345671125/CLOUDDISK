#include "filepage.h"
#include "QInputDialog"
#include "MainWIndow/opewidget.h"
#include "up_downpage.h"
#include "fileinfopage.h"
#include "PDU/protocol.h"
#include "style.h"
#include <QMessageBox>
#include <QTcpSocket>

#include "../commonfunc.h"
#include "../commondata.h"
#include "ClientSocket/clientSocketThread.h"
#include <QEvent>

filePage* filePage::m_pFilePage = nullptr;

filePage::filePage(QWidget *parent)
    : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground,true);
    this->uploadfile = nullptr;
    this->m_pFileListW = new QListWidget;
    this->m_pCreateDirPB = new QPushButton("创建文件夹",this);

    this->m_pFlushFilePB = new QPushButton("刷新文件",this);
    QVBoxLayout* thirdVBL = new QVBoxLayout;
    thirdVBL->addWidget(m_pCreateDirPB);

    thirdVBL->addWidget(m_pFlushFilePB);

    this->m_pUpLoadFilePB =  new QPushButton("上传文件",this);
    this->m_pDownLoadPB = new QPushButton("下载文件",this);
    this->m_pShareFilePB = new QPushButton("任务界面",this);
    thirdVBL->addWidget(this->m_pUpLoadFilePB);
    thirdVBL->addWidget(this->m_pDownLoadPB);
    thirdVBL->addWidget(this->m_pShareFilePB);
    thirdVBL->setContentsMargins(0,0,0,0);
    QHBoxLayout* pMain = new QHBoxLayout;
    pMain->addWidget(m_pFileListW);
    pMain->addLayout(thirdVBL);
    pMain->setContentsMargins(0,5,5,20);
    this->m_pDownLoadPB->setEnabled(false); //设置下载文件按钮默认为不可用

    setLayout(pMain);

    connect(m_pCreateDirPB,SIGNAL(clicked(bool)),this,SLOT(createDir()));
    connect(m_pFlushFilePB,SIGNAL(clicked(bool)),this,SLOT(flushFile()));
    connect(m_pFileListW,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(switchDir(QListWidgetItem*)));
    connect(m_pFileListW,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(widgetListRequested(QPoint)));
    connect(m_pUpLoadFilePB,SIGNAL(clicked(bool)),this,SLOT(uploadFile()));
    connect(this,SIGNAL(createFileItem(QString,QString)),(up_downPage::getInstance()),SLOT(createFileItem(QString,QString)));
    connect(m_pShareFilePB,SIGNAL(clicked()),this,SLOT(openUp_downPage()));
    connect(m_pDownLoadPB,SIGNAL(clicked()),this,SLOT(downLoadFile()));
    connect(this,SIGNAL(createDownLoadFileItem(QString,QString)),(up_downPage::getInstance()),SLOT(createDownloadFileItem(QString,QString)));
    connect(this->m_pFileListW,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(enableDownloadBtn(QListWidgetItem*)));
    this->m_pFileListW->setContextMenuPolicy(Qt::CustomContextMenu); //添加菜单策略

    this->setStyle(m_filePage_Light_style);

}
filePage *filePage::getInstance(QWidget *parent)
{
    if(filePage::m_pFilePage == nullptr)
    {
        filePage::m_pFilePage = new filePage(parent);
    }
    return filePage::m_pFilePage;
}

void filePage::updateFileList(protocol::PDU* pdu)
{
    OpeWidget::getinstance()->getfilePage()->m_pFileListW->clear();
    uint FileLsitLenth = pdu->uiMsgLen/sizeof(protocol::FileInfo);
    protocol::FileInfo* FileList = (protocol::FileInfo*)malloc(sizeof(protocol::FileInfo) * FileLsitLenth);
    memcpy((char*)FileList,(char*)&pdu->caMsg,pdu->uiMsgLen);
    for(uint i = 2; i<FileLsitLenth ; i++){   //去除. ..
        QListWidgetItem *pItem = new QListWidgetItem();
        if(FileList[i].iFileType == protocol::FILE_TYPE_DIR)
        {
            pItem->setIcon(QIcon(QPixmap(":/img/FileDir.jpg")));
            pItem->setWhatsThis("DIR");
        }
        else if(FileList[i].iFileType == protocol::FILE_TYPE_FILE)
        {
            // qDebug()<<FileList[i].caFileName;
            pItem->setIcon(QIcon(QPixmap(":/img/FILE.jpg")));
            pItem->setWhatsThis("FILE");
        }
        pItem->setText(FileList[i].caFileName);
        OpeWidget::getinstance()->getfilePage()->m_pFileListW->addItem(pItem);

    }

    free(pdu);
    pdu = NULL;

}

void filePage::emitTaskSignal(QString strUploadFileAbsolutedName)
{
    qDebug()<<"emitSignal";
    QString strCurPath = *reinterpret_cast<QString*>(CommonFunc::QueryData(STRCURPATH));
    emit createFileItem(strCurPath,strUploadFileAbsolutedName);
}

void filePage::setStyle(QString style)
{
    this->style()->unpolish(this);
    this->setStyleSheet(style);
    this->style()->polish(this);
    this->update();
}

void filePage::getFileInfoRes(protocol::PDU *pdu)
{
    char buffer[128] = {};
    memcpy_s(buffer,128,pdu->caData,64);
    QString ret = QString::fromUtf8(buffer);
    CommonFunc::removeNullCharacters(ret);
    if(ret.compare("-1") == 0)
    {
        QMessageBox::information(NULL,"上传文件","文件已存在!");
    }else
    {
        QString strUploadFileAbsolutedName =*reinterpret_cast<QString*>(CommonFunc::QueryData(UPLOADFILEABSOLUTEDNAME));
        emitTaskSignal(strUploadFileAbsolutedName);
        up_downPage::getInstance()->show();
    }
    free(pdu);
    pdu = NULL;
}

void filePage::handleDownloadFile(protocol::PDU *pdu)
{
    char* buffer = new char[128];
    memset(buffer,0,128);
    memcpy_s(buffer,128,(char*)pdu->caData,128);
    QString strRet = QString(buffer);
    CommonFunc::removeNullCharacters(strRet);
    if(strRet.compare("file is not exists!") == 0)
    {
        QMessageBox::information(NULL,"上传文件","文件不存在！");
    }else
    {
        QString strCurPath = *reinterpret_cast<QString*>(CommonFunc::QueryData(STRCURPATH)); //读取现在系统所在路径
        CommonFunc::removeNullCharacters(strCurPath); //去除路径后面可能存在的null
        QString strServerFilePath =strCurPath  + '/' + this->strTargetDownloadFileName; //路径合并
        this->emit createDownLoadFileItem(strServerFilePath,this->strAbsolutedDonwloadFileName);
        up_downPage::getInstance()->show();//显示下载页面
    }
    delete[] buffer;
    free(pdu);
    pdu = NULL;
}

bool filePage::event(QEvent *event)
{
    if(event->type() == QEvent::Show)
    {
        this->flushFile();
        this->m_pDownLoadPB->setEnabled(false);
    }
   return QWidget::event(event);
}

void filePage::uploadFile()
{

    QString strUploadFileName = QFileDialog::getOpenFileName(this,"选择文件");
    if(strUploadFileName.isEmpty()){
        return;
    }
    clientSocketThread* m_pclientSocketThread  = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
    connect(this,SIGNAL(sendUploadFile(QString)),m_pclientSocketThread,SLOT(sendUploadFileReq(QString)));
    emit sendUploadFile(strUploadFileName);
    disconnect(this,SIGNAL(sendUploadFile(QString)),m_pclientSocketThread,SLOT(sendUploadFileReq(QString)));
}

void filePage::openUp_downPage()
{
    up_downPage::getInstance()->show();
}
void filePage::createDir()
{
    QString strCreateNewFolder = {};
    QString strNewFolderName = {};
    CommonFunc::readLangFromFile("strCreateNewFolder",strCreateNewFolder);
    CommonFunc::readLangFromFile("strNewFolderName",strNewFolderName);
    bool ret = false;
    QString strNewDir = QInputDialog::getText(this,strCreateNewFolder,strNewFolderName,QLineEdit::Normal,"",&ret);
    if(ret == true)
    {
        if(!strNewDir.isEmpty()){
            if(strNewDir.length()>64){
                QString foldernameErr = {};
                CommonFunc::readLangFromFile("foldernameErr",foldernameErr);
                QMessageBox::warning(this,strCreateNewFolder,foldernameErr);
            }else{

                QString strName = *reinterpret_cast<QString*>(CommonFunc::QueryData(USERNAME));
                QString strCurPath = *reinterpret_cast<QString*>(CommonFunc::QueryData(STRCURPATH));
                clientSocketThread* m_pclientSocketThread  = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
                QObject::connect(this,SIGNAL(sendCreateDir(QString,QString,QString)),m_pclientSocketThread,SLOT(sendCreateDirReq(QString,QString,QString)));
                emit sendCreateDir(strNewDir,strName,strCurPath);
                QObject::disconnect(this,SIGNAL(sendCreateDir(QString,QString,QString)),m_pclientSocketThread,SLOT(sendCreateDirReq(QString,QString,QString)));

            }

        }else{
            QMessageBox::warning(this,"新建文件夹","新文件夹名字不能为空！");
        }
    }


}

void filePage::flushFile()
{
    QString CurPath = *reinterpret_cast<QString*>(CommonFunc::QueryData(STRCURPATH));
    clientSocketThread* m_pclientSocketThread  = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
    connect(this,SIGNAL(sendflushReq(QString)),m_pclientSocketThread,SLOT(sendFlushFileReq(QString)));
    emit sendflushReq(CurPath);
    disconnect(this,SIGNAL(sendflushReq(QString)),m_pclientSocketThread,SLOT(sendFlushFileReq(QString)));
}

void filePage::goBack()
{
    QString CurPath = *reinterpret_cast<QString*>(CommonFunc::QueryData(STRCURPATH));
    if(CurPath.indexOf(QChar::Null) != -1)
    {
        CurPath.truncate(CurPath.indexOf(QChar::Null));
    }
    QString rootPath = *reinterpret_cast<QString*>(CommonFunc::QueryData(STRROOTPATH));
    if(rootPath.indexOf(QChar::Null) != -1)
    {
        rootPath.truncate(rootPath.indexOf(QChar::Null));
    }
    if(CurPath.compare(rootPath) == 0)
    {
        QMessageBox::information(this,"切换目录","到底啦!");
    }
    else
    {
        CurPath = CurPath.mid(0,CurPath.lastIndexOf("/"));
        clientSocketThread* m_pclientSocketThread  = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
        QObject::connect(this,SIGNAL(sendGoBack(QString)),m_pclientSocketThread,SLOT(sendGoBackReq(QString)));
        emit sendGoBack(CurPath);
        QObject::disconnect(this,SIGNAL(sendGoBack(QString)),m_pclientSocketThread,SLOT(sendGoBackReq(QString)));

        CommonFunc::setData(STRCURPATH,&CurPath); //修改全局数据类中的路径
    }

}

void filePage::switchDir(QListWidgetItem * item)
{
    if(item->whatsThis() == "DIR"){
        QString CurPath = *reinterpret_cast<QString*>(CommonFunc::QueryData(STRCURPATH));
        if(CurPath.indexOf(QChar::Null) != -1)//如果末尾有 \0 去除末尾 \0
        {
            CurPath.truncate(CurPath.indexOf(QChar::Null));
        }
        CurPath.append("/").append(item->text());

        clientSocketThread* m_pclientSocketThread  = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
        QObject::connect(this,SIGNAL(sendSwitchDir(QString)),m_pclientSocketThread,SLOT(sendSwitchReq(QString)));
        emit sendSwitchDir(CurPath);
        QObject::disconnect(this,SIGNAL(sendSwitchDir(QString)),m_pclientSocketThread,SLOT(sendSwitchReq(QString)));

        CommonFunc::setData(STRCURPATH,&CurPath); //修改全局数据类中的路径

        qDebug()<<CurPath;
    }
    if(item->whatsThis() == "FILE");
}

void filePage::deleteDir()
{
    if(this->m_pFileListW->currentItem() == nullptr || this->m_pFileListW->currentItem()->whatsThis() != QString("DIR"))
    {
        return;
    }
    else
    {
        if(QMessageBox::information(this,"删除文件夹","此操作会删除当前选中文件夹和文件夹中所有文件，是否继续?",
                QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
        {
            QString strDirName = this->m_pFileListW->currentItem()->text().append('\0');
            QString strCurPath = *reinterpret_cast<QString*>(CommonFunc::QueryData(STRCURPATH));

            clientSocketThread* m_pclientSocketThread  = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
            connect(this,SIGNAL(sendDeleteDir(QString,QString)),m_pclientSocketThread,SLOT(sendDeleteDirReq(QString,QString)));
            emit this->sendDeleteDir(strDirName,strCurPath);
            disconnect(this,SIGNAL(sendDeleteDir(QString,QString)),m_pclientSocketThread,SLOT(sendDeleteDirReq(QString,QString)));
        }

    }

}

void filePage::deleteFile()
{
    if(this->m_pFileListW->currentItem() == nullptr || this->m_pFileListW->currentItem()->whatsThis() != QString("FILE"))
    {
        return;
    }
    else
    {
        if(QMessageBox::information(this,"删除文件","此操作会删除当前选中文件，是否继续?",
                QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
        {
            QString strFName = this->m_pFileListW->currentItem()->text().append('\0');
            QString strCurPath = *reinterpret_cast<QString*>(CommonFunc::QueryData(STRCURPATH));

            clientSocketThread* m_pclientSocketThread  = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
            connect(this,SIGNAL(sendDeleteFile(QString,QString)),m_pclientSocketThread,SLOT(sendDeleteFileReq(QString,QString)));
            emit this->sendDeleteFile(strFName,strCurPath);
            disconnect(this,SIGNAL(sendDeleteFile(QString,QString)),m_pclientSocketThread,SLOT(sendDeleteFileReq(QString,QString)));
        }

    }
}

void filePage::renameFile()
{
    if(this->m_pFileListW->currentItem() == nullptr || this->m_pFileListW->currentItem()->whatsThis() != QString("FILE"))
    {
        return;
    }
    else
    {
        bool ret = false;
        QString strNewFName  = QInputDialog::getText(this,"文件重命名","新文件名:",QLineEdit::Normal,QString(),&ret);
        QString strCurFName = this->m_pFileListW->currentItem()->text().append('\0');
        if(ret == true)
        {
            if(!strNewFName.isEmpty() && !(strNewFName.length()>64))
            {
                //发送renameFile请求
                clientSocketThread* m_pclientSocketThread  = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
                QObject::connect(this,SIGNAL(sendRenameFile(QString,QString)),m_pclientSocketThread,SLOT(sendRenameFileReq(QString,QString)));
                emit sendRenameFile(strNewFName,strCurFName);
                QObject::disconnect(this,SIGNAL(sendRenameFile(QString,QString)),m_pclientSocketThread,SLOT(sendRenameFileReq(QString,QString)));

            }else{
                QMessageBox::critical(this,"文件重命名","文件名不能为空且文件名不能超过64个字符");
            }
        }
        else
        {
            return;
        }


    }
}



void filePage::widgetListRequested(const QPoint &pos)
{
    QMenu fileMenuList =  QMenu(this);
    QMenu dirMenuList =  QMenu(this);
    QMenu defaultMenuList =  QMenu(this);


    fileMenuList.setStyleSheet(QMenu_Item_selected); //设置样式表
    dirMenuList.setStyleSheet(QMenu_Item_selected);
    defaultMenuList.setStyleSheet(QMenu_Item_selected);


    QAction renameFile = QAction(QIcon(QPixmap(":/img/rename.png")),"重命名文件",this);
    QAction renameDir = QAction(QIcon(QPixmap(":/img/rename.png")),"重命名文件夹",this);
    QAction uploadFile = QAction(QIcon(QPixmap(":/img/upload.png")),"上传文件",this);
    QAction downloadFile = QAction(QIcon(QPixmap(":/img/download.png")),"下载文件",this);
    QAction deleteFile = QAction(QIcon(QPixmap(":/img/del.png")),"删除文件",this);
    QAction deleteDir = QAction(QIcon(QPixmap(":/img/del.png")),"删除文件夹",this);
    QAction goback = QAction(QIcon(QPixmap(":/img/back.png")),"返回上一层",this);
    QAction createDir = QAction(QIcon(QPixmap(":/img/new.png")),"新建文件夹",this);
    QAction shareFile = QAction(QIcon(QPixmap(":/img/share.png")),"分享文件",this);
    QAction flushFile = QAction(QIcon(QPixmap(":/img/flush.png")),"刷新文件",this);
    QAction info = QAction(QIcon(QPixmap(":/img/info.png")),"属性",this);
    QObject::connect(&renameFile,SIGNAL(triggered()),this,SLOT(renameFile()));
    QObject::connect(&goback,SIGNAL(triggered()),this,SLOT(goBack()));
    QObject::connect(&flushFile,SIGNAL(triggered()),this,SLOT(flushFile()));
    QObject::connect(&createDir,SIGNAL(triggered()),this,SLOT(createDir()));
    QObject::connect(&deleteDir,SIGNAL(triggered()),this,SLOT(deleteDir()));
    QObject::connect(&deleteFile,SIGNAL(triggered()),this,SLOT(deleteFile()));
    QObject::connect(&uploadFile,SIGNAL(triggered()),this,SLOT(uploadFile()));
    QObject::connect(&info,SIGNAL(triggered()),this,SLOT(getFileInfo()));
    QObject::connect(&downloadFile,SIGNAL(triggered()),this,SLOT(downLoadFile()));
    fileMenuList.addAction(&renameFile);
    fileMenuList.addAction(&deleteFile);
    fileMenuList.addAction(&downloadFile);
    fileMenuList.addAction(&shareFile);
    fileMenuList.addAction(&flushFile);
    fileMenuList.addAction(&info);
    dirMenuList.addAction(&renameDir);
    dirMenuList.addAction(&deleteDir);
    dirMenuList.addAction(&flushFile);
    defaultMenuList.addAction(&goback);
    defaultMenuList.addAction(&createDir);
    defaultMenuList.addAction(&uploadFile);
    defaultMenuList.addAction(&flushFile);

    if(this->m_pFileListW->itemAt(mapFromGlobal(QCursor::pos())) == NULL){
        this->m_pFileListW->setCurrentItem(NULL);
        defaultMenuList.exec(QCursor::pos()); //让 QMeunList在鼠标的位置执行
    }else{
        if(this->m_pFileListW->currentItem()->whatsThis() == "FILE")
        {
            fileMenuList.exec(QCursor::pos());
        }
        else if(this->m_pFileListW->currentItem()->whatsThis() == "DIR")
        {
            dirMenuList.exec(QCursor::pos());
        }
    }


}

void filePage::getFileInfo()
{
    QString strFileName = this->m_pFileListW->currentItem()->text();
    QString strUserName = *reinterpret_cast<QString*>(CommonFunc::QueryData(USERNAME));

    clientSocketThread* m_pclientSocketThread  = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
    QObject::connect(this,SIGNAL(sendGetFileInfo(QString,QString)),m_pclientSocketThread,SLOT(sendGetFileInfo(QString,QString)));
    emit sendGetFileInfo(strFileName,strUserName);
    QObject::disconnect(this,SIGNAL(sendGetFileInfo(QString,QString)),m_pclientSocketThread,SLOT(sendGetFileInfo(QString,QString)));
}

void filePage::enableDownloadBtn(QListWidgetItem *Item)
{
    if(this->m_pDownLoadPB->isEnabled() == FALSE)
    {
        this->m_pDownLoadPB->setEnabled(true);
    }
}

void filePage::showFileInfo(protocol::PDU* pdu)
{
    if(pdu->uiMsgLen == 0){
        QMessageBox::critical(this,"error!","获取文件信息时出现未知错误！");
        return;
    }
    protocol::FileDetail* fileDetail = (protocol::FileDetail*)malloc(pdu->uiMsgLen);
    memcpy((char*)fileDetail,(char*)&pdu->caMsg,pdu->uiMsgLen);
    QString filename = QString::fromUtf8(fileDetail->data,fileDetail->filenameLen);
    QString filesize = QString::fromUtf8(fileDetail->data+fileDetail->filenameLen,fileDetail->fileSizeLen);
    QString uploadTime = QString::fromUtf8(fileDetail->data+fileDetail->filenameLen+fileDetail->fileSizeLen,fileDetail->uploadTimeLen);
    QString uploaduser = QString::fromUtf8(fileDetail->data+fileDetail->filenameLen+fileDetail->fileSizeLen+fileDetail->uploadTimeLen,fileDetail->uploaduserLen);
    FileInfoPage *fileInfoPage = new FileInfoPage(filename,filesize,uploadTime,uploaduser);
    fileInfoPage->show();

    free(pdu);
    pdu = NULL;
}

void filePage::downLoadFile()
{
    if(this->m_pFileListW->currentItem() == nullptr)
    {
        return;
    }
    QString strTargetFileName = this->m_pFileListW->currentItem()->text(); //读取用户点击的文件名
    CommonFunc::removeNullCharacters(strTargetFileName);
    QString defaultPath = *reinterpret_cast<QString*>(CommonFunc::QueryData(DEFAULTSTOREADDR));
    QString strFileSavedPath =QFileDialog::getSaveFileName(this,"下载文件",defaultPath+"/"+strTargetFileName);
    if(strFileSavedPath.isEmpty()){
        return;
    }
    CommonFunc::removeNullCharacters(strFileSavedPath);
    CommonFunc::removeNullCharacters(strTargetFileName);
    this->strAbsolutedDonwloadFileName = strFileSavedPath;
    this->strTargetDownloadFileName = strTargetFileName;

    clientSocketThread* m_pclientSocketThread  = reinterpret_cast<clientSocketThread*>(CommonFunc::QueryData(CLIENTSOCKETTHREAD));
    QObject::connect(this,SIGNAL(sendDownLoadFile(QString,QString)),m_pclientSocketThread,SLOT(sendDownLoadFileReq(QString,QString)));
    emit sendDownLoadFile(strTargetFileName,*reinterpret_cast<QString*>(CommonFunc::QueryData(STRCURPATH)));
    QObject::disconnect(this,SIGNAL(sendDownLoadFile(QString,QString)),m_pclientSocketThread,SLOT(sendDownLoadFileReq(QString,QString)));
    //emit createDownLoadFileItem(strServerFilePath,strFileSavedPath);
}

void filePage::delDir(protocol::PDU* pdu)
{
        char result[64] = {'\0'};
        memcpy(result,pdu->caData,64);
        qDebug()<<"delDirRet："<<result;
        if(strcmp(result,"success") == 0)
        {
            QMessageBox::information(NULL,"删除文件夹","删除成功");
        }
        else if(strcmp(result,"dir not exist") == 0)
        {
            QMessageBox::information(NULL,"删除文件夹","文件夹不存在，删除失败");
        }else if(strcmp(result,"remove fault") == 0)
        {
            QMessageBox::information(NULL,"删除文件夹","删除失败");
        }
        this->flushFile(); //刷新文件


        free(pdu);
        pdu = NULL;
}

void filePage::delFile(protocol::PDU* pdu)
{
    char result[64] = {'\0'};
    memcpy(result,pdu->caData,64);
    if(strcmp(result,"success") == 0)
    {
        QMessageBox::information(NULL,"删除文件","删除成功");
    }
    else if(strcmp(result,"file not exist") == 0)
    {
        QMessageBox::information(NULL,"删除文件","文件不存在，删除失败");
    }else if(strcmp(result,"remove fault") == 0)
    {
        QMessageBox::information(NULL,"删除文件","删除失败");
    }
    this->flushFile(); //刷新文件


    free(pdu);
    pdu = NULL;
}

void filePage::renameFile(protocol::PDU* pdu)
{
    char result[64] = {'\0'};
    memcpy(result,pdu->caData,64);
    if(strcmp(result,"success") == 0)
    {
        QMessageBox::information(NULL,"重命名文件","重命名成功");
    }
    else if(strcmp(result,"file not exist") == 0)
    {
        QMessageBox::information(NULL,"重命名文件","文件不存在，重命名失败");
    }else if(strcmp(result,"rename fault") == 0)
    {
        QMessageBox::information(NULL,"重命名文件","重命名失败");
    }
   this->flushFile();//刷新文件


    free(pdu);
    pdu = NULL;
}
