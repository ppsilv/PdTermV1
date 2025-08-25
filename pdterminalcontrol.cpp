#include "pdterminalcontrol.h"
#include <QDebug>

PdTerminalControl::PdTerminalControl(QObject *parent)
    : QObject(parent), m_state(Normal)
{
    setupCSICommands();
}

void PdTerminalControl::processData(const QByteArray &data)
{
    for (char c : data) {
        switch (m_state) {
        case Normal:
            processNormalChar(c);
            break;
        case Escape:
            processEscapeSequence(c);
            break;
        case CSI:
            processCSISequence(c);
            break;
        case OSC:
            processOSCSequence(c);
            break;
        }
    }
}

void PdTerminalControl::resetTerminal()
{
    m_state = Normal;
    m_sequenceBuffer.clear();
    emit resetAllAttributes();
    emit clearScreen();
    emit setCursorPosition(1, 1);
}

void PdTerminalControl::processNormalChar(char c)
{
    if (c == '\x1B') { // ESC character
        m_state = Escape;
        m_sequenceBuffer.clear();
    }
    // Outros caracteres normais são tratados pelo terminal
}

void PdTerminalControl::processEscapeSequence(char c)
{
    if (c == '[') {
        m_state = CSI;
    } else if (c == ']') {
        m_state = OSC;
    } else {
        // Comandos de escape simples
        switch (c) {
        case 'c': // Reset terminal
            resetTerminal();
            break;
        case 'D': // Scroll down
            // Implementar se necessário
            break;
        case 'E': // New line
            // Implementar se necessário
            break;
        default:
            qDebug() << "Unknown escape sequence:" << c;
        }
        m_state = Normal;
    }
}

void PdTerminalControl::processCSISequence(char c)
{
    if (c >= 0x40 && c <= 0x7E) { // Final character
        QByteArray params = m_sequenceBuffer;
        m_sequenceBuffer.clear();
        m_state = Normal;

        QByteArray command;
        command += c;

        if (m_csiCommands.contains(command)) {
            m_csiCommands[command](params);
        } else {
            emit unknownSequence("\x1B[" + params + command);
        }
    } else {
        m_sequenceBuffer += c;
    }
}

void PdTerminalControl::processOSCSequence(char c)
{
    // Implementar comandos OSC se necessário
    if (c == '\a' || c == '\x1B') { // BEL or ESC termina OSC
        m_state = Normal;
        m_sequenceBuffer.clear();
    } else {
        m_sequenceBuffer += c;
    }
}

void PdTerminalControl::setupCSICommands()
{
    // Cursor Position
    m_csiCommands["H"] = [this](const QByteArray &params) {
        QStringList parts = QString(params).split(';');
        int row = parts.value(0, "1").toInt();
        int col = parts.value(1, "1").toInt();
        emit setCursorPosition(row, col);
    };

    // Clear Screen
    m_csiCommands["J"] = [this](const QByteArray &params) {
        if (params.isEmpty() || params == "0") {
            emit clearScreen();
        }
        // Outras variações podem ser implementadas aqui
    };

    // Text Attributes
    m_csiCommands["m"] = [this](const QByteArray &params) {
        QStringList attrs = QString(params).split(';');
        for (const QString &attr : attrs) {
            int code = attr.toInt();
            switch (code) {
            case 0: // Reset
                emit resetAllAttributes();
                break;
            case 1: // Bold
                emit setBold(true);
                break;
            case 22: // Normal intensity (not bold)
                emit setBold(false);
                break;
            case 30 ... 37: // Text color
                emit setTextColor(code - 30);
                break;
            }
        }
    };

    // Adicione mais comandos CSI conforme necessário
}
