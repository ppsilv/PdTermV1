#include "PdTermmainterminal.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Força o reconhecimento do WMClass (importante para Linux)
    QApplication::setDesktopFileName("PdTermV1");


    // Configurações críticas para Linux

    QApplication::setWindowIcon(QIcon(":/icons/pdtermv2.svg"));

    // Força o carregamento do tema correto
    QIcon::setThemeName("hicolor");
    QIcon::setFallbackThemeName("hicolor");

   // a.setWindowIcon(QIcon(":/icons/pdtermv2.svg")); // Caminho no resource
    PdTermMainTerminal w;
    w.show();
    return a.exec();
}
