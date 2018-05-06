#include "GenProductTitle.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GenProductTitle w;
    w.show();
    return a.exec();
}
