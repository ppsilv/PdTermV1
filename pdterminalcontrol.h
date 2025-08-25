#ifndef PDTERMINALCONTROL_H
#define PDTERMINALCONTROL_H

#include <QObject>
#include <QHash>
#include <functional>

class PdTerminalControl : public QObject
{
    Q_OBJECT
public:
    explicit PdTerminalControl(QObject *parent = nullptr);
    enum State {
        Normal,
        Escape,
        CSI, // Control Sequence Introducer
        OSC  // Operating System Command
    };

    void processData(const QByteArray &data);
    void resetTerminal();

signals:
    void clearScreen();
    void setCursorPosition(int row, int col);
    void setTextColor(int color);

    void setBold(bool enabled);

    void resetAllAttributes();
    void unknownSequence(const QByteArray &seq);

private:
    State m_state;
    QByteArray m_sequenceBuffer;
    QHash<QByteArray, std::function<void(const QByteArray&)>> m_csiCommands;

    void processNormalChar(char c);
    void processEscapeSequence(char c);
    void processCSISequence(char c);
    void processOSCSequence(char c);
    void setupCSICommands();
};
#endif // PDTERMINALCONTROL_H
