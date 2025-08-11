#include "pdtermxmodem.h"

#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QDebug> // For outputting the file path

PdTermXmodem::PdTermXmodem(QObject *parent): QObject{parent}
{}

void PdTermXmodem::enviarArquivoXmodem()
{
    QString filePath = QFileDialog::getOpenFileName(
        nullptr,
        tr("Open File"),
        QDir::homePath(), // Start in the user's home directory
        tr("Text files (*.txt);;All files (*.*)"));


    if (filePath.isEmpty()) {
        emit transmissaoCancelada();
        emit progressoAtualizado(100);
        return;
    }
}

bool PdTermXmodem::esperarPorByte(char byteEsperado, int timeout_ms)
{
    QByteArray resposta;
    while (timeout_ms > 0) {
        resposta = recebe_dados_serial(100); // Verifica a cada 100ms
        if (resposta.contains(byteEsperado)) {
            return true;
        }
        timeout_ms -= 100;
    }
    return false;
}

char PdTermXmodem::calcularChecksum(const QByteArray &dados)
{
    char checksum = 0;
    for (char byte : dados) {
        checksum += byte;
    }
    return checksum;
}
