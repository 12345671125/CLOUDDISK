#ifndef FILEPAGE_H
#define FILEPAGE_H
#pragma once
#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>
#include <QFile>
#include <QFileDialog>
#include <QTimer>
#include "PDU/protocol.h"
QT_BEGIN_NAMESPACE
namespace Ui { class clientWin; };
QT_END_NAMESPACE


class filePage : public QWidget
{
    Q_OBJECT
public:
    void updateFileList(protocol::PDU* pdu);
    static filePage* getInstance(QWidget* parent);
    void emitTaskSignal(QString strUploadFileAbsolutedName);
    void emitDownLoadSignal(protocol::PDU* pdu);
    void setStyle(QString style);
    void getFileInfoRes(protocol::PDU* pdu);
    void handleDownloadFile(protocol::PDU* pdu);

protected:
    bool event(QEvent *event) override;


public slots:
    void createDir(); //创建文件夹
    void flushFile(); //刷新文件
    void goBack(); //返回
    void switchDir(QListWidgetItem * item); //切换路径
    void deleteDir();
    void deleteFile();
    void renameFile();
    void showFileInfo(protocol::PDU* pdu);
    void downLoadFile();
    void delDir(protocol::PDU* pdu);
    void delFile(protocol::PDU* pdu);
    void renameFile(protocol::PDU* pdu);

private:
    filePage(QWidget *parent = nullptr);
    filePage(const filePage&) = delete;
    filePage& operator=(const filePage&) = delete;
    static filePage *m_pFilePage;

    QListWidget* m_pFileListW;

    QPushButton* m_pCreateDirPB;

    QPushButton* m_pFlushFilePB;
    QPushButton* m_pUpLoadFilePB;
    QPushButton* m_pDownLoadPB;
    QPushButton* m_pDelFilePB;
    QPushButton* m_pShareFilePB;
    QString absolutedFilePath;
    QString uploadFileName;
    QFile* uploadfile;  //记录上传文件
    QString fileName;
    QString strAbsolutedDonwloadFileName;
    QString strTargetDownloadFileName;



signals:
    void createFileItem(QString curPath,QString absolutedFilePath);
    void createDownLoadFileItem(QString ServerfileName,QString absolutedFileName);
    void sendflushReq(QString CurPath);
    void sendCreateDir(QString strNewDir,QString strName,QString curPath);
    void sendGoBack(QString strCurPath);
    void sendSwitchDir(QString strCurPath);
    void sendRenameFile(QString strNewFName,QString strCurFName);
    void sendDeleteFile(QString strFName,QString strCurPath);
    void sendDeleteDir(QString strDirName,QString strCurPath);
    void sendUploadFile(QString strAbsoluteUploadFileName);
    void sendDownLoadFile(QString strTargetFileName,QString curPath);
    void sendGetFileInfo(QString strFileName,QString strUserName);
private slots:
    void widgetListRequested(const QPoint &pos);
    void uploadFile();
    void openUp_downPage();
    void getFileInfo();
    void enableDownloadBtn(QListWidgetItem* Item);
};

#endif // FILEPAGE_H
