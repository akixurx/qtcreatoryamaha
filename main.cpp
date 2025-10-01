#include "yamitracker.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Yamitracker w;
    w.show();
    return a.exec();
}
