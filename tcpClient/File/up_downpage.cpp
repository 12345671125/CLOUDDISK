#include "up_downpage.h"
#include "itemui.h"
#include "commonfunc.h"
#include "commondata.h"

up_downPage* up_downPage::m_pUp_downPage = nullptr;

up_downPage::up_downPage(QWidget *parent)
    : QWidget{parent}
{
    this->switchList = new QListWidget(this);
    this->switchList->addItem("上传文件");
    this->switchList->addItem("下载文件");
    this->upPage = new QListWidget(this);
    this->downPage = new QListWidget(this);
    this->stackedWidget = new QStackedWidget(this);
    this->stackedWidget->addWidget(this->upPage);
    this->stackedWidget->addWidget(this->downPage);
    this->switchList->setMaximumWidth(100);
    this->stackedWidget->setMinimumWidth(450);
    this->upPage->setMinimumWidth(this->stackedWidget->minimumWidth());
    this->upPage->setSortingEnabled(true);
    upPage->setResizeMode(QListView::Adjust);
    upPage->setViewMode(QListView::IconMode);
    this->setWindowTitle("上传下载任务");
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(this->switchList);
    mainLayout->addWidget(this->stackedWidget);
    setLayout(mainLayout);
    QObject::connect(switchList,SIGNAL(currentRowChanged(int)),stackedWidget,SLOT(setCurrentIndex(int)));
    this->setWindowIcon(QIcon(QPixmap(":/img/logo.png")));
}

up_downPage *up_downPage::getInstance()
{
    if(up_downPage::m_pUp_downPage == nullptr)
    {
        up_downPage::m_pUp_downPage = new up_downPage();
    }
    return up_downPage::m_pUp_downPage;
}

void up_downPage::createFileItem(QString curPath,QString absolutedPath)
{
    //qDebug()<<"createFileItem";
    QString FileName = CommonFunc::getFileNameFromAbsolutePath(absolutedPath);
    ItemUI* itemui = new ItemUI(FileName,this->upPage->width()-40,30,this);
    connect(itemui,SIGNAL(cancelTask(QListWidgetItem*)),this,SLOT(cancelTask(QListWidgetItem*)));
    connect(this,SIGNAL(createTask(QString,QString)),itemui,SLOT(createTask(QString,QString)));
    this->upPage->addItem(itemui->getListWidgetItem());
    this->upPage->setItemWidget(itemui->getListWidgetItem(),itemui->getWidget());
    emit createTask(curPath,absolutedPath);
    disconnect(this,SIGNAL(createTask(QString,QString)),itemui,SLOT(createTask(QString,QString)));
}

void up_downPage::createDownloadFileItem(QString ServerfilePath,QString absolutedFileName)
{
    //qDebug()<<"createDownloadFileItem";
    this->stackedWidget->setCurrentIndex(1);
    QString strFileName = CommonFunc::getFileNameFromAbsolutePath(absolutedFileName); //从绝对路径中截取文件名
    ItemUI* itemui = new ItemUI(strFileName,this->upPage->width()-40,30,this);
    connect(itemui,SIGNAL(cancelTask(QListWidgetItem*)),this,SLOT(cancelTask(QListWidgetItem*)));
    connect(this,SIGNAL(createDownloadTask(QString,QString)),itemui,SLOT(createDownloadTask(QString,QString)));
    this->downPage->addItem(itemui->getListWidgetItem());
    this->downPage->setItemWidget(itemui->getListWidgetItem(),itemui->getWidget());
    emit createDownloadTask(ServerfilePath,absolutedFileName);
    disconnect(this,SIGNAL(createDownloadTask(QString,QString)),itemui,SLOT(createDownloadTask(QString,QString)));
}
void up_downPage::cancelTask(QListWidgetItem* item)
{

    if(this->upPage->indexFromItem(item).isValid()){
        QModelIndex index  = this->upPage->indexFromItem(item);
        /*使剩下行上移*/
        this->upPage->takeItem(this->upPage->indexFromItem(item).row());
        if(this->upPage->count()-1 > index.row()){
            for(int i = index.row()+1;i<this->upPage->count();i++){
                //            qDebug()<<"flag1";
                this->upPage->insertItem(i-1,this->upPage->item(i));
                //            qDebug()<<"flag2";
                this->upPage->removeItemWidget(this->upPage->item(i));
                //            this->upPage->takeItem(i);
            }

        }
        this->upPage->viewport()->update();
        this->upPage->update();
    }else{
        QModelIndex index  = this->downPage->indexFromItem(item);
        /*使剩下行上移*/
        this->downPage->takeItem(this->downPage->indexFromItem(item).row());
        if(this->downPage->count()-1 > index.row()){
            for(int i = index.row()+1;i<this->downPage->count();i++){
                //            qDebug()<<"flag1";
                this->downPage->insertItem(i-1,this->downPage->item(i));
                //            qDebug()<<"flag2";
                this->downPage->removeItemWidget(this->downPage->item(i));
                //            this->upPage->takeItem(i);
            }

        }
        this->downPage->viewport()->update();
        this->downPage->update();
    }


}
