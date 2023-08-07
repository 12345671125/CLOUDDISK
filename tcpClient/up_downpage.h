#ifndef UP_DOWNPAGE_H
#define UP_DOWNPAGE_H

#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include <QDebug>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QThread>
#include "itemui.h"


class up_downPage : public QWidget
{
    Q_OBJECT
public:
    explicit up_downPage(QWidget *parent = nullptr);
    static up_downPage& getInstance();
    void setPage(int index);
public slots:
    void createFileItem(QString curPath,QString absolutedPath);
    void createDownloadFileItem(QString ServerfilePath,QString absolutedFileName);
private:
    QStackedWidget* stackedWidget;
    QListWidget* upPage;
    QListWidget* downPage;
    QListWidget* switchList;

private slots:
    void cancelTask(QListWidgetItem* item);
signals:
    void createTask(QString curPath,QString absolutedPath);
    void createDownloadTask(QString ServerfilePath,QString absolutedFileName);


private slots:
    void switchPage();
};

#endif // UP_DOWNPAGE_H
