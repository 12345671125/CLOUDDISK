#ifndef M_WINDOWFLAG_H
#define M_WINDOWFLAG_H

#include <QWidget>
#include <QIcon>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include <QGuiApplication>
#include <QScreen>

QT_BEGIN_NAMESPACE
class logoWidget;
class windowBtnG;
QT_END_NAMESPACE

class m_WindowFlag : public QWidget
{
    Q_OBJECT
public:
    explicit m_WindowFlag(QWidget *parent = nullptr);
   static m_WindowFlag& getinstance(QWidget* parent);

private:
    QIcon* appIcon;
    QHBoxLayout* mainLayout;
    QHBoxLayout* buttonG;
    QHBoxLayout* logoG;
    logoWidget* logoWidget;
    windowBtnG* myBtnG;

signals:

};

#endif // M_WINDOWFLAG_H
