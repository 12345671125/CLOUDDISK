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
   static m_WindowFlag* getinstance(QWidget* parent = nullptr);
    void setStyle(QString style);
private:
    m_WindowFlag(QWidget *parent = nullptr);
     m_WindowFlag(const m_WindowFlag&) = delete;
     m_WindowFlag& operator=(const m_WindowFlag&) = delete;
    static m_WindowFlag* windowFlag;
    QIcon* appIcon;
    QHBoxLayout* mainLayout;
    QHBoxLayout* buttonG;
    QHBoxLayout* logoG;
    logoWidget* logoWidget;
    windowBtnG* myBtnG;

protected:
    virtual void mousePressEvent(QMouseEvent* event); //鼠标按下事件
    virtual void mouseMoveEvent(QMouseEvent* event); //鼠标移动事件
    virtual void mouseReleaseEvent(QMouseEvent* event); //鼠标释放事件

signals:

};

#endif // M_WINDOWFLAG_H
