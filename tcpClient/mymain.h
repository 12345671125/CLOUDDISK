#ifndef MYMAIN_H
#define MYMAIN_H

#include <QWidget>


QT_BEGIN_NAMESPACE
class m_WindowFlag;
class OpeWidget;
QT_END_NAMESPACE
class myMain : public QWidget
{
    Q_OBJECT
public:
    explicit myMain(QWidget *parent = nullptr);
    static myMain& getInstance(QWidget* parent = nullptr);
private:
    m_WindowFlag* topWidget;
    OpeWidget* bottomWidget;
signals:

};

#endif // MYMAIN_H
