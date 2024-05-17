#ifndef WINDOWBTNG_H
#define WINDOWBTNG_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QHBoxLayout;
class QIcon;
QT_END_NAMESPACE

class windowBtnG : public QWidget
{
    Q_OBJECT
public:
    static windowBtnG* getInstance(QWidget* parent = nullptr);
private:
    windowBtnG(QWidget *parent = nullptr);
    windowBtnG(const windowBtnG&) = delete;
    windowBtnG& operator=(const windowBtnG&) = delete;
    static windowBtnG* windowbtnG;
    bool hasFull = false;
    bool hasLight = true;
    QPushButton* quitBtn;
    QPushButton* fullBtn;
    QPushButton* minmunBtn;
    QPushButton* switchStyleBtn;
    QIcon* quitIcon;
    QIcon* fullIcon;
    QIcon* minmunIcon;
    QIcon* normalSizeIcon;
    QIcon* lightIcon;
    QIcon* nightIcon;
    QHBoxLayout* mainHBL;

private slots:
    void quit();
    void fullScreen();
    void minmun();
    void switchStyle();
signals:

};

#endif // WINDOWBTNG_H
