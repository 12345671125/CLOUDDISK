#ifndef MSGWIDGET_H
#define MSGWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QHBoxLayout;
QT_END_NAMESPACE

class msgWidget : public QWidget
{
    Q_OBJECT
public:
    explicit msgWidget(QWidget *parent = nullptr);
   static msgWidget& getInstance(QWidget *parent = nullptr);
    QLineEdit* inputEdit;
    QPushButton* sendMagBtn;

private:
    QHBoxLayout* bottomHBL;

signals:

};

#endif // MSGWIDGET_H
