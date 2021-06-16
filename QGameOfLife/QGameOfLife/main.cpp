#include "QGameOfLife.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGameOfLife w;
    w.show();
    return a.exec();
}
