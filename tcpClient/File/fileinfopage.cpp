#include "fileinfopage.h"
#include "ui_fileinfopage.h"

FileInfoPage::FileInfoPage(QString FileName,QString FileSize,QString createdTime,QString location,QWidget *parent):
    QWidget(parent),
    ui(new Ui::FileInfoPage)
{
    ui->setupUi(this);
    this->FileName = FileName;
    double size  = ((double)FileSize.toInt()/1024/1024);
    this->FileSize = QString("%1").arg(size);
    this->createdTime = createdTime;
    this->location = location;
    this->ui->label->setText("INFO");
    this->ui->listWidget->addItem("文件名 : " + this->FileName);
    this->ui->listWidget->addItem("文件大小 : " + this->FileSize + "MB");
    this->ui->listWidget->addItem("创建时间 : " + this->createdTime);
    this->ui->listWidget->addItem("上传人 : " + this->location);
    this->setWindowIcon(QIcon(QPixmap(":/img/logo.png")));
}

FileInfoPage::~FileInfoPage()
{
    delete ui;
}
