#if 1
#include <QApplication>
#include "clientwin.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
        clientWin::getInstance().show();
       return a.exec();
}
#endif
#if 0
#include <QApplication>
#include "opewidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
        OpeWidget w;
        w.show();
       return a.exec();
}
#endif
#if 0
#include <QApplication>
#include "online.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
        Online w;
        w.show();
       return a.exec();
}
#endif

#if 0
#include <QApplication>
#include "friend.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
        Friend w;
        w.show();
       return a.exec();
}
#endif

