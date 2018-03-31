#include "ImageToTextOcr.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageToTextOcr w;
    w.show();
    return a.exec();
}
