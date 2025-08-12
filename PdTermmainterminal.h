#ifndef PGTERMMAINTERMINAL_H
#define PGTERMMAINTERMINAL_H

#include <QMainWindow>
#include <QProgressBar>

/*
 * pdtermmainterminal.h
 */

#include "pdtermserial.h"
#include "pdtermxmodem.h"
#include "worker.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class PdTermMainTerminal;
}
QT_END_NAMESPACE

class PdTermMainTerminal : public QMainWindow
{
    Q_OBJECT

public:
    PdTermMainTerminal(QWidget *parent = nullptr);
    ~PdTermMainTerminal();
    void setTextAtPosition(int row, int col, const QString &text, const QColor &color = Qt::green);
    void ensureLineExists(int row);  // Novo método auxiliar
    bool eventFilter(QObject *obj, QEvent *event) override;
    bool flag_from_serial_write_to_terminal;
    bool flag_from_terminal_write_to_serial;
    QString openFileXmodem();
    void setSerialDiretion();
    void resetSerialDiretion();


public slots:
    void appendTerminalText(const QString &text, const QColor &color = Qt::green, bool newLine = true);
    void limparTexto();  // Slot para limpar o QPlainTextEdit
    void keyPressEvent_old(QKeyEvent *event);// override;  // Adicione esta linha
    /* Serial */
    void onSerialDataReceived(const QByteArray &data);
    void onSerialError(const QString &error);
    void onSerialStatusChanged(const QString &status);
    void on_actionSerialSettings();
    void testeTelaTerminal();
    void onTransmissaoCancelada();
    void onTransmissaoConcluida();
    void onErroOcorreu(const QString &mensagem);
    void onProgressoAtualizado(int porcentagem);

private:
    QProgressBar *progressBar;
    Ui::PdTermMainTerminal *ui;
    Worker *m_worker;
    QThread *m_thread;
    PdTermSerial *m_serial;
    PdTermXmodem *m_xmodem;
    void setupXmodemSignals();
    void setupSerialSignals();

    QByteArray receiveSerialData(int timeout_ms);
    void sendSerialData(const QByteArray& data);
    void setFlagSerial(bool flag);

    //Metodo
    void setup_ui();
    void setup_connect();
};
#endif // PGTERMMAINTERMINAL_H



