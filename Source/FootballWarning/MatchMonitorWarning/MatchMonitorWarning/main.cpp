#include "MmwMonitorWarning.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MmwMonitorWarning w;
    w.show();
    return a.exec();
}
