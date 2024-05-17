#ifndef PUBLICFILEPAGE_H
#define PUBLICFILEPAGE_H
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


class publicFilePage : public QWidget
{
    Q_OBJECT
public:
    void updateFileList(protocol::PDU* pdu);
    static publicFilePage* getInstance(QWidget* parent);
    void emitTaskSignal(QString strUploadFileAbsolutedName);
    void emitDownLoadSignal(protocol::PDU* pdu);
    void setStyle(QString style);
    void getFileInfoRes(protocol::PDU* pdu);
    void handleDownloadFile(protocol::PDU* pdu);

protected:
    bool event(QEvent *event) override;


public slots:
    void flushFile(); //刷新文件
    void getPublicFilePath();//获取公共文件路径
    // void deleteDir();
    // void deleteFile();
    // void renameFile();
    void showFileInfo(protocol::PDU* pdu);
    void downLoadFile();
    // void delFile(protocol::PDU* pdu);
    // void renameFile(protocol::PDU* pdu);

private:
    publicFilePage(QWidget *parent = nullptr);
    publicFilePage(const publicFilePage&) = delete;
    publicFilePage& operator=(const publicFilePage&) = delete;
    static publicFilePage *m_pPublicFilePage;

    QListWidget* m_pFileListW;

    QPushButton* m_pFlushFilePB;
    QPushButton* m_pUpLoadFilePB;
    QPushButton* m_pDownLoadPB;
    // QPushButton* m_pDelFilePB;
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
    void sendflushPublicReq(QString CurPath);

    void sendUploadPublicFile(QString strAbsoluteUploadFileName);
    void sendDownLoadFile(QString strTargetFileName,QString curPath);
    void sendGetFileInfo(QString strFileName,QString strUserName);
    void sendGetPublicFilePath();
private slots:
    void widgetListRequested(const QPoint &pos);
    void uploadFile();
    void openUp_downPage();
    void getFileInfo();
    void enableDownloadBtn(QListWidgetItem* Item);
};

#endif // PUBLICFILEPAGE_H
