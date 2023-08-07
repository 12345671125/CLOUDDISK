#ifndef FILEINFOPAGE_H
#define FILEINFOPAGE_H

#include <QWidget>

namespace Ui {
class FileInfoPage;
}

class FileInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit FileInfoPage(QString FileName,QString FileSize,QString createdTime,QString location,QWidget *parent = nullptr);
    ~FileInfoPage();

private:
    Ui::FileInfoPage *ui;
    QString FileName;
    QString FileSize;
    QString createdTime;
    QString location;
};

#endif // FILEINFOPAGE_H
