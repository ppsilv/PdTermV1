#include "PdTermmainterminal.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    /*
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
    */

    try {
        QApplication a(argc, argv);
        PdTermMainTerminal w;
        w.show();
        return a.exec();
    } catch (const std::exception& e) {
        qCritical() << "Exceção capturada:" << e.what();
        QMessageBox::critical(nullptr, "Erro Fatal",
                              QString("Erro não tratado: %1").arg(e.what()));
        return 1;
    } catch (...) {
        qCritical() << "Erro desconhecido";
        QMessageBox::critical(nullptr, "Erro Fatal", "Erro desconhecido");
        return 1;
    }
}
