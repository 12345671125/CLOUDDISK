#ifndef LOGOWIDGET_H
#define LOGOWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QHBoxLayout;
class QIcon;
class QPushButton;
class QLabel;
QT_END_NAMESPACE

class logoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit logoWidget(QWidget *parent = nullptr);
    ~logoWidget();
    static logoWidget& getInstance(QWidget *parent = nullptr);
private:
    QHBoxLayout* mainHBL;
    QIcon* appIcon;
    QPushButton* Icon;
    QLabel* apptitle;
    int width = 32;
    int height = 32;
signals:

};

#endif // LOGOWIDGET_H
