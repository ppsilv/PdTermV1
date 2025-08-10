#include "pgtermmainterminal.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PgTermMainTerminal w;
    w.show();
    return a.exec();
}
