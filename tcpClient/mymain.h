#ifndef MYMAIN_H
#define MYMAIN_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
class m_WindowFlag;
class OpeWidget;
QT_END_NAMESPACE
class myMain : public QWidget
{
    Q_OBJECT
public:
    static myMain* getInstance(QMainWindow* parent = nullptr);
private:
    myMain(QWidget *parent = nullptr);
    myMain(const myMain&) = delete;
    myMain& operator=(const myMain&) = delete;
    static myMain *m_pMyMain;
    m_WindowFlag* topWidget;
    OpeWidget* bottomWidget;
    QScreen* screen; //屏幕对象
public:
    const int realW; //物理屏幕宽度
    const int realH; //物理屏幕高
    const int initW; //初始化屏幕宽
    const int initH; //初始化屏幕高
    QPoint c_Position; //鼠标位置
    bool Dragging; //是否正在拖动
    QPoint f_Position; //窗口位置

signals:

};

#endif // MYMAIN_H
