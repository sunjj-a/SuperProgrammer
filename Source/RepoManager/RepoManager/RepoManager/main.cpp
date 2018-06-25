#include "RepoManager.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RepoManager w;
    w.show();
    return a.exec();
}
