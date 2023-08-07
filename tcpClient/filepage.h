#ifndef FILEPAGE_H
#define FILEPAGE_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "protocol.h"
#include <QMenu>
#include <QFile>
#include <QFileDialog>
#include <QTimer>
#include "up_downpage.h"
#include "fileinfopage.h"

class filePage : public QWidget
{
    Q_OBJECT
public:
    filePage(QWidget *parent = nullptr);
    void updateFileList(protocol::PDU* pdu);
    static filePage& getInstance();
    void emitSignal();
    void emitDownLoadSignal(protocol::PDU* pdu);


public slots:
    void createDir(); //创建文件夹
    void flushFile(); //刷新文件
    void goBack(); //返回
    void switchDir(QListWidgetItem * item); //切换路径
    void deleteDir();
    void deleteFile();
    void renameFile();
    void uploadFileEnd();
    void showFileInfo(protocol::PDU* pdu);
    void downLoadFile();

private:
    QListWidget* m_pFileListW;
    QPushButton* m_pReturnPB;
    QPushButton* m_pCreateDirPB;
    QPushButton* m_pDelDirPB;
    QPushButton* m_pRenameFilePB;
    QPushButton* m_pFlushFilePB;
    QPushButton* m_pUpLoadFilePB;
    QPushButton* m_pDownLoadPB;
    QPushButton* m_pDelFilePB;
    QPushButton* m_pShareFilePB;
    QString absolutedFilePath;
    QString uploadFileName;
    QFile* uploadfile;  //记录上传文件
    QString fileName;
    QString absolutedDonwloadFileName;


signals:
    void createFileItem(QString curPath,QString absolutedFilePath);
    void createDownLoadFileItem(QString ServerfileName,QString absolutedFileName);

private slots:
    void widgetListRequested(const QPoint &pos);
    void uploadFile();
    void openUp_downPage();
    void getFileInfo();
};

#endif // FILEPAGE_H
