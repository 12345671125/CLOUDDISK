#include "filepage.h"
#include "clientwin.h"
#include "QInputDialog"



filePage::filePage(QWidget *parent)
    : QWidget(parent)
{
    this->uploadfile = nullptr;
    this->m_pFileListW = new QListWidget;
    this->m_pReturnPB = new QPushButton("返回",this);
    this->m_pCreateDirPB = new QPushButton("创建文件夹",this);
    this->m_pDelDirPB = new QPushButton("删除文件夹",this);
    this->m_pRenameFilePB = new QPushButton("文件重命名",this);
    this->m_pFlushFilePB = new QPushButton("刷新文件",this);

    QVBoxLayout* pDirVBL = new QVBoxLayout;
    pDirVBL->addWidget(m_pReturnPB);
    pDirVBL->addWidget(m_pCreateDirPB);
    pDirVBL->addWidget(m_pDelDirPB);
    pDirVBL->addWidget(m_pRenameFilePB);
    pDirVBL->addWidget(m_pFlushFilePB);

    this->m_pUpLoadFilePB =  new QPushButton("上传文件",this);
    this->m_pDownLoadPB = new QPushButton("下载文件",this);
    this->m_pDelFilePB = new QPushButton("删除文件",this);
    this->m_pShareFilePB = new QPushButton("打开任务进度界面",this);

    QVBoxLayout* pFileVBL = new QVBoxLayout;
    pFileVBL->addWidget(this->m_pUpLoadFilePB);
    pFileVBL->addWidget(this->m_pDownLoadPB);
    pFileVBL->addWidget(this->m_pDelFilePB);
    pFileVBL->addWidget(this->m_pShareFilePB);

    QHBoxLayout* pMain = new QHBoxLayout;
    pMain->addWidget(m_pFileListW);
    pMain->addLayout(pDirVBL);
    pMain->addLayout(pFileVBL);

    setLayout(pMain);

    connect(m_pCreateDirPB,SIGNAL(clicked(bool)),this,SLOT(createDir()));
    connect(m_pFlushFilePB,SIGNAL(clicked(bool)),this,SLOT(flushFile()));
    connect(m_pReturnPB,SIGNAL(clicked(bool)),this,SLOT(goBack()));
    connect(m_pFileListW,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(switchDir(QListWidgetItem*)));
    connect(m_pDelDirPB,SIGNAL(clicked(bool)),this,SLOT(deleteDir()));
    connect(m_pDelFilePB,SIGNAL(clicked(bool)),this,SLOT(deleteFile()));
    connect(m_pRenameFilePB,SIGNAL(clicked(bool)),this,SLOT(renameFile()));
    connect(m_pFileListW,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(widgetListRequested(QPoint)));
    connect(m_pUpLoadFilePB,SIGNAL(clicked(bool)),this,SLOT(uploadFile()));
    connect(this,SIGNAL(createFileItem(QString,QString)),&(up_downPage::getInstance()),SLOT(createFileItem(QString,QString)));
    connect(m_pShareFilePB,SIGNAL(clicked()),this,SLOT(openUp_downPage()));
    connect(m_pDownLoadPB,SIGNAL(clicked()),this,SLOT(downLoadFile()));
    connect(this,SIGNAL(createDownLoadFileItem(QString,QString)),&(up_downPage::getInstance()),SLOT(createDownloadFileItem(QString,QString)));

    this->m_pFileListW->setContextMenuPolicy(Qt::CustomContextMenu); //添加菜单策略

}

void filePage::updateFileList(protocol::PDU *pdu)
{
    OpeWidget::getinstance().getfilePage()->m_pFileListW->clear();
    if(pdu  == NULL) return;
    uint FileLsitLenth = pdu->uiMsgLen/sizeof(protocol::FileInfo);
    protocol::FileInfo* FileList = (protocol::FileInfo*)malloc(sizeof(protocol::FileInfo) * FileLsitLenth);
    memcpy((char*)FileList,(char*)pdu->caMsg,pdu->uiMsgLen);
    for(uint i = 2; i<FileLsitLenth ; i++){   //去除. ..
        QListWidgetItem *pItem = new QListWidgetItem();
        if(FileList[i].iFileType == protocol::FILE_TYPE_DIR)
        {
            qDebug()<<FileList[i].caFileName;
            pItem->setIcon(QIcon(QPixmap(":/img/FileDir.jpg")));
            pItem->setWhatsThis("DIR");
        }
        else if(FileList[i].iFileType == protocol::FILE_TYPE_FILE)
        {
            qDebug()<<FileList[i].caFileName;
            pItem->setIcon(QIcon(QPixmap(":/img/FILE.jpg")));
            pItem->setWhatsThis("FILE");
        }
        pItem->setText(FileList[i].caFileName);
        OpeWidget::getinstance().getfilePage()->m_pFileListW->addItem(pItem);

    }

}

filePage &filePage::getInstance()
{
   static filePage filepage;
    return filepage;
}

void filePage::emitSignal()
{
    qDebug()<<"emitSignal";
    emit createFileItem(clientWin::getInstance().curPath(),this->absolutedFilePath);
}

//void filePage::emitDownLoadSignal(protocol::PDU* pdu)
//{
//    if(this->absolutedDonwloadFileName.isEmpty()){
//        return;
//    }
//    char message[64] = {'\0'};
//    memcpy(message,pdu->caData,64);

//   /* if(strcpy(message,"OK") == 0){

//    }*//*else{
//        qDebug()<<"所选文件不存在!";
//    }*/

//}

void filePage::uploadFile()
{

    qDebug()<<"uploadFile";
    this->fileName = QFileDialog::getOpenFileName(this,"选择文件");
    if(fileName.isEmpty()){
        return;
    }
    QFile *file = new QFile(fileName);
//    file->open(QIODevice::ReadOnly);
    this->absolutedFilePath = fileName;
    protocol::FileInfo_s* savedFileInfo = protocol::createFileInfo_s(fileName.mid(fileName.lastIndexOf('/')+1,fileName.length()).toStdString().c_str(),file->size(),clientWin::getInstance().getLoginName().toStdString().c_str());
    protocol::PDU* pdu = protocol::createPDU(sizeof(protocol::FileInfo_s));
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_UPLOADFILEINFO_REQUEST;
    memcpy((char*)pdu->caMsg,(char*)savedFileInfo,sizeof(protocol::FileInfo_s));
    clientWin::getInstance().getTcpSocket().write((char*)pdu,pdu->PDULen);
    free(pdu);
    up_downPage::getInstance().setPage(0);
    up_downPage::getInstance().show();
    qDebug()<<this->absolutedFilePath;
//    file->close();
    delete file;
    file = nullptr;
    this->emitSignal();
}

void filePage::openUp_downPage()
{
    up_downPage::getInstance().show();
}
void filePage::createDir()
{
    QString strNewDir = QInputDialog::getText(this,"新建文件夹","新建文件夹名字");
    if(!strNewDir.isEmpty()){
        if(strNewDir.length()>64){
            QMessageBox::warning(this,"新建文件夹","新文件夹名字不能超过64个字符!");
        }else{
            QString strName = clientWin::getInstance().getLoginName();
            QString strCurPath = clientWin::getInstance().curPath();
            qDebug()<<"curPath:"<<strCurPath;
            protocol::PDU* pdu = protocol::createPDU(strCurPath.length()+1);
            pdu->uiMsgType = protocol::ENUM_MSG_TYPE_CREATE_DIR_REQUEST;
            strncpy(pdu->caData,strName.toStdString().c_str(),strName.size());
            strncpy((char*)pdu->caMsg,strCurPath.toStdString().c_str(),strCurPath.size());
            memcpy(pdu->caData+64,strNewDir.toStdString().c_str(),strNewDir.size());
            clientWin::getInstance().getTcpSocket().write((char*)pdu,pdu->PDULen);
            free(pdu);
            pdu = nullptr;
        }

    }else{
        QMessageBox::warning(this,"新建文件夹","新文件夹名字不能为空！");
    }

}

void filePage::flushFile()
{
    QString CurPath = clientWin::getInstance().curPath();
    protocol::PDU* pdu = protocol::createPDU(CurPath.size() + 1);
    pdu->uiMsgType = protocol::ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
    memcpy((char*)pdu->caMsg,CurPath.toStdString().c_str(),CurPath.size());
    clientWin::getInstance().getTcpSocket().write((char*)pdu,pdu->PDULen);
    free(pdu);
    pdu = nullptr;
}

void filePage::goBack()
{
    QString CurPath = clientWin::getInstance().curPath();
    qDebug()<<clientWin::getInstance().curPath();
    if(CurPath == ("./"+clientWin::getInstance().getLoginName()))
    {
        QMessageBox::information(this,"切换目录","到底啦!");
    }
    else
    {
        CurPath = CurPath.mid(0,CurPath.lastIndexOf("/"));
        protocol::PDU* pdu = protocol::createPDU(CurPath.size() + 1);
        pdu->uiMsgType = protocol::ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
        memcpy((char*)pdu->caMsg,CurPath.toStdString().c_str(),CurPath.size());
        clientWin::getInstance().getTcpSocket().write((char*)pdu,pdu->PDULen);
        free(pdu);
        pdu = nullptr;
        clientWin::getInstance().modifyCurPath(CurPath);
    }

}

void filePage::switchDir(QListWidgetItem * item)
{
    if(item->whatsThis() == "DIR"){
        QString CurPath = clientWin::getInstance().curPath();
        CurPath.append("/").append(item->text());
        protocol::PDU* pdu = protocol::createPDU(CurPath.size() + 1);
        pdu->uiMsgType = protocol::ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
        memcpy((char*)pdu->caMsg,CurPath.toStdString().c_str(),CurPath.size());
        clientWin::getInstance().getTcpSocket().write((char*)pdu,pdu->PDULen);
        free(pdu);
        pdu = nullptr;
        clientWin::getInstance().modifyCurPath(CurPath);
        qDebug()<<clientWin::getInstance().curPath();
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
            QString DirName = this->m_pFileListW->currentItem()->text().append('\0');
            QString curDir = clientWin::getInstance().curPath();
            protocol::PDU* pdu = protocol::createPDU(curDir.length()+1);
            pdu->uiMsgType = protocol::ENUM_MSG_TYPE_DELETE_DIR_REQUEST;
            memcpy(pdu->caData,DirName.toStdString().c_str(),DirName.length());
            memcpy((char*)pdu->caMsg,curDir.toStdString().c_str(),curDir.length());
            clientWin::getInstance().getTcpSocket().write((char*)pdu,pdu->PDULen);
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
            QString fileName = this->m_pFileListW->currentItem()->text().append('\0');
            QString curPath = clientWin::getInstance().curPath();
            protocol::PDU* pdu = protocol::createPDU(curPath.length()+1);
            pdu->uiMsgType = protocol::ENUM_MSG_TYPE_DELETE_FILE_REQUEST;
            memcpy(pdu->caData,fileName.toStdString().c_str(),fileName.length());
            memcpy((char*)pdu->caMsg,curPath.toStdString().c_str(),curPath.length());
            clientWin::getInstance().getTcpSocket().write((char*)pdu,pdu->PDULen);
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
        QString newName  = QInputDialog::getText(this,"文件重命名","新文件名:");
//        newName.append(this->m_pFileListW->currentItem()->text().mid((
//            this->m_pFileListW->currentItem()->text().lastIndexOf("."),
//            this->m_pFileListW->currentItem()->text().length()-1)));
        if(!newName.isEmpty() && !(newName.length()>64))
        {
            qDebug()<<newName;
            QString fileName = this->m_pFileListW->currentItem()->text().append('\0');
            QString curPath = clientWin::getInstance().curPath();
            protocol::PDU* pdu = protocol::createPDU(curPath.length()+1);
            pdu->uiMsgType = protocol::ENUM_MSG_TYPE_RENAME_FILE_REQUEST;
            memcpy(pdu->caData,fileName.toStdString().c_str(),fileName.length());
            memcpy(pdu->caData+64,newName.toStdString().c_str(),newName.length());
            memcpy((char*)pdu->caMsg,curPath.toStdString().c_str(),curPath.length());
            clientWin::getInstance().getTcpSocket().write((char*)pdu,pdu->PDULen);
        }else{
            QMessageBox::critical(this,"文件重命名","文件名不能为空且文件名不能超过64个字符");
        }

    }
}



void filePage::widgetListRequested(const QPoint &pos)
{
    QMenu fileMenuList =  QMenu(this);
    QMenu dirMenuList =  QMenu(this);
    QMenu defaultMenuList =  QMenu(this);
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
//    qDebug()<<m_pFileListW->itemAt(mapFromGlobal(QCursor::pos()));
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
void filePage::uploadFileEnd()
{
    qDebug()<<"uploadFileEnd";
    protocol::PDU pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_UPLOADFIN_FILE_REQUEST,this->fileName);
    clientWin::getInstance().getTcpSocket().write((char*)&pdu,pdu.PDULen);
}

void filePage::getFileInfo()
{
    QString filename = QString::fromLocal8Bit(this->m_pFileListW->currentItem()->text().toStdString().c_str(),this->m_pFileListW->currentItem()->text().length());
    filename = filename.append('\0');
    protocol::PDU pdu = protocol::PDU::default_request(protocol::ENUM_MSG_TYPE_GETFILEINFO_REQUEST,"");
    memcpy(pdu.caData,clientWin::getInstance().getLoginName().append('\0').toStdString().c_str(),clientWin::getInstance().getLoginName().length());
    memcpy(pdu.caData+64,filename.toStdString().c_str(),filename.length());
    clientWin::getInstance().getTcpSocket().write((char*)&pdu,pdu.PDULen);
}

void filePage::showFileInfo(protocol::PDU* pdu)
{
    if(pdu == NULL) return;
    if(pdu->uiMsgLen == 0){
        QMessageBox::critical(this,"error!","获取文件信息时出现未知错误！");
        return;
    }
    protocol::FileDetail* fileDetail = (protocol::FileDetail*)malloc(pdu->uiMsgLen);
    memcpy((char*)fileDetail,(char*)pdu->caMsg,pdu->uiMsgLen);
    QString filename = QString::fromLocal8Bit(fileDetail->data,fileDetail->filenameLen);
    QString filesize = QString::fromLocal8Bit(fileDetail->data+fileDetail->filenameLen,fileDetail->fileSizeLen);
    QString uploadTime = QString::fromLocal8Bit(fileDetail->data+fileDetail->filenameLen+fileDetail->fileSizeLen,fileDetail->uploadTimeLen);
    QString uploaduser = QString::fromLocal8Bit(fileDetail->data+fileDetail->filenameLen+fileDetail->fileSizeLen+fileDetail->uploadTimeLen,fileDetail->uploaduserLen);
    FileInfoPage *fileInfoPage = new FileInfoPage(filename,filesize,uploadTime,uploaduser);
    fileInfoPage->show();
}

void filePage::downLoadFile()
{
    QString defaultFileName = this->m_pFileListW->currentItem()->text();
    defaultFileName.append('\0');
    QString savedPath = QFileDialog::getSaveFileName(this,"下载文件",defaultFileName);
    if(savedPath.isEmpty()){
        return;
    }
    up_downPage::getInstance().setPage(1);
    up_downPage::getInstance().show();
    QString ServerfilePath = clientWin::getInstance().curPath() + '/' + defaultFileName;
    this->absolutedDonwloadFileName = savedPath;
    emit createDownLoadFileItem(ServerfilePath,this->absolutedDonwloadFileName);
}
