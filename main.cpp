#include "PdTermmainterminal.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PdTermMainTerminal w;
    w.show();
    return a.exec();
}
