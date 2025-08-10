#ifndef PGTERMMAINTERMINAL_H
#define PGTERMMAINTERMINAL_H

#include <QMainWindow>
/*
 * pgtermmainterminal.h
 */

#include "pdtermserial.h"
#include "worker.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class PgTermMainTerminal;
}
QT_END_NAMESPACE

class PgTermMainTerminal : public QMainWindow
{
    Q_OBJECT

public:
    PgTermMainTerminal(QWidget *parent = nullptr);
    ~PgTermMainTerminal();
    void setTextAtPosition(int row, int col, const QString &text, const QColor &color = Qt::green);
    void ensureLineExists(int row);  // Novo método auxiliar

public slots:
    void appendTerminalText(const QString &text, const QColor &color = Qt::green, bool newLine = true);
    void limparTexto();  // Slot para limpar o QPlainTextEdit
    void keyPressEvent(QKeyEvent *event) override;  // Adicione esta linha
    /* Serial */
    void onSerialDataReceived(const QByteArray &data);
    void onSerialError(const QString &error);
    void onSerialStatusChanged(const QString &status);
    void on_actionSerialSettings_triggered();

private:
    Ui::PgTermMainTerminal *ui;
    Worker *m_worker;
    QThread *m_thread;
    PdTermSerial *m_serial;
    void setupSerial();

    //Metodo
    void setup_ui();
    void setup_connect();
};
#endif // PGTERMMAINTERMINAL_H
