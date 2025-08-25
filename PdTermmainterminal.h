#ifndef PGTERMMAINTERMINAL_H
#define PGTERMMAINTERMINAL_H

#include <QMainWindow>
#include <QProgressBar>

/*
 * pdtermmainterminal.h
 */
#include "pdterminalcontrol.h"
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
    bool flag_from_serial_write_to_VT100;
    QString openFileXmodem();

public slots:
    void appendTerminalText(const QString &text, const QColor &color = Qt::green, bool newLine = true);
    void limparTexto();  // Slot para limpar o QPlainTextEdit
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
    void clearScreen();
    void setCursorPosition(int row, int col);
    void setTextColor(int color);
    void setBold(bool enabled);
    void unknownSequence(const QByteArray &seq);

private:
    QProgressBar *progressBar;
    Ui::PdTermMainTerminal *ui;
    Worker *m_worker;
    QThread *m_thread;
    PdTermSerial *m_serial;
    PdTermXmodem *m_xmodem;
    PdTerminalControl *m_control;
    QColor cor;

    void setupXmodemSignals();
    void setupSerialSignals();
    void setupControlSignals();

    QByteArray receiveSerialData(int timeout_ms);
    void sendSerialData(const QByteArray& data);
    void setFlagSerial(bool flag);

    //Metodo
    void setup_ui();
    void setup_connect();
    void writeTerminal(const QString &mensagem, bool newline=false);

    //flags
    bool flagsetBold;

};
#endif // PGTERMMAINTERMINAL_H



