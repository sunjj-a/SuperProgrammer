#include "ParseWord.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ParseWord w;
    w.show();
    return a.exec();
}
