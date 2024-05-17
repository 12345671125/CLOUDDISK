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


class up_downPage : public QWidget
{
    Q_OBJECT
public:

    static up_downPage* getInstance();
public slots:
    void createFileItem(QString curPath,QString absolutedPath);
    void createDownloadFileItem(QString ServerfilePath,QString absolutedFileName);
private:
    up_downPage(QWidget *parent = nullptr);
    up_downPage(const up_downPage&) = delete;
    up_downPage& operator=(const up_downPage&) = delete;
    static up_downPage *m_pUp_downPage;
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
};

#endif // UP_DOWNPAGE_H
