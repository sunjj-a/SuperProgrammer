#include "OddsMonitorWarning.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OddsMonitorWarning w;
    w.show();
    return a.exec();
}
