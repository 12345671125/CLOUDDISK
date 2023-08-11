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
    explicit windowBtnG(QWidget *parent = nullptr);
    ~windowBtnG();
    static windowBtnG& getInstance(QWidget* parent = nullptr);
private:
    QPushButton* quitBtn;
    QPushButton* fullBtn;
    QPushButton* collapseBtn;
    QIcon* quitIcon;
    QIcon* fullIcon;
    QIcon* collapseIcon;
    QHBoxLayout* mainHBL;
signals:

};

#endif // WINDOWBTNG_H
