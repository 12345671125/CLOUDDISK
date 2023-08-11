#if 1
#include <QApplication>
#include <QIcon>
#include "ClientSocket/clientwin.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    clientWin::getInstance().setWindowIcon(QIcon(QPixmap(":/img/logo.png")));
    clientWin::getInstance().setWindowTitle("CloudDisk");
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

