#include "serverwin.h"
#include <QApplication>
#include "opedb.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OpeDB::getInsance();
    serverWin w;
    w.show();
    return a.exec();
}
