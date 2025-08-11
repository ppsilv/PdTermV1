#ifndef PDTERMXMODEM_H
#define PDTERMXMODEM_H

#include <QObject>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

class PdTermXmodem : public QObject
{
    Q_OBJECT
public:
    explicit PdTermXmodem(QObject *parent = nullptr);
    void enviarArquivoXmodem();

    // Callbacks que devem ser fornecidos pela aplicação
    void (*envia_dados_serial)(const QByteArray &data);
    QByteArray (*recebe_dados_serial)(int timeout_ms);

signals:
    void transmissaoCancelada();
    void transmissaoConcluida();
    void erroOcorreu(const QString &mensagem);
    void progressoAtualizado(int porcentagem);

private:
    bool esperarPorByte(char byteEsperado, int timeout_ms = 3000);
    char calcularChecksum(const QByteArray &dados);
};

#endif // PDTERMXMODEM_H
