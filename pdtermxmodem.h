#ifndef PDTERMXMODEM_H
#define PDTERMXMODEM_H

#include <QObject>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread> // Não é estritamente necessário aqui, mas útil para Qt::QueuedConnection

class PdTermXmodem : public QObject
{
    Q_OBJECT
public:
    explicit PdTermXmodem(QObject *parent = nullptr);

    void enviarArquivoXmodem();
    void setFilePath(QString filePath);

    // Callbacks com mesma estrutura
    typedef QByteArray (*SerialIOFunc)(void* context, int timeout_ms); // Para recepção
    typedef void (*SerialIOFuncSend)(void* context, const QByteArray& data); // Para envio (agora com const)
    typedef void (*SerialIOFuncFlag)(void* context, bool flag); // Para envio (agora com const)

    SerialIOFunc recebe_dados_serial = nullptr;
    SerialIOFuncSend envia_dados_serial = nullptr;
    SerialIOFuncFlag set_envia_flag_serial = nullptr;
    void* io_context = nullptr; // Contexto único para ambos

    // Método de envio simplificado
    void enviarDados(const QByteArray& data) {
        if (envia_dados_serial && io_context) {
            envia_dados_serial(io_context, data); // Agora aceita const
        }
    }

    void setFlag(bool flag) {
        if (set_envia_flag_serial && io_context) {
            set_envia_flag_serial(io_context, flag); // Agora aceita const
        }
    }

signals:
    // Esses sinais serão conectados aos slots da janela principal (que vive na GUI thread)
    void transmissaoCancelada();
    void transmissaoConcluida();
    void erroOcorreu(const QString &mensagem);
    void progressoAtualizado(int porcentagem);
    // Um sinal para informar que o processo terminou e a thread pode ser finalizada
    void finished(); // NOVO SINAL

public slots:
    // Um slot para solicitar o cancelamento do envio de forma segura
    void cancelarTransmissao(); // NOVO SLOT

private:
    QString m_filePath;
    bool m_cancelado; // NOVA VARIÁVEL: Flag para cancelamento solicitado
    int reenvio;
    int total_envios;
    int total_blocos;
    int total_blocos_reenviados;

    int esperarAckNack(int timeout_ms);
    bool esperarPorByte(char byteEsperado, int timeout_ms = 3000);
    char calcularChecksum(const QByteArray &dados);
    void printFile( QByteArray fileContent );

};

#endif // PDTERMXMODEM_H
