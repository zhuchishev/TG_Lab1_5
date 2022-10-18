#include "appwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    srand(time(0));
    QApplication a(argc, argv);
    appWindow w;
    w.show();
    return a.exec();
}
