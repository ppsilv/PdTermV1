#ifndef PDTERMSERIAL_H
#define PDTERMSERIAL_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QComboBox>

class PdTermSerial : public QObject
{
    Q_OBJECT
public:
    explicit PdTermSerial(QObject *parent = nullptr);
    ~PdTermSerial();

    // Gerenciamento de portas
    void refreshPorts(QComboBox *portCombo);

    // Conexão com configuração completa
    bool connectSerial(const QString &portName,
                       int baudRate = 9600,
                       QSerialPort::DataBits dataBits = QSerialPort::Data8,
                       QSerialPort::Parity parity = QSerialPort::NoParity,
                       QSerialPort::StopBits stopBits = QSerialPort::OneStop,
                       QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl);

    void disconnectSerial();
    bool isConnected() const;

    // Envio de dados
    void sendData(const QByteArray &data);

    // Configurações individuais
    void setBaudRate(int baudRate);
    void setDataBits(QSerialPort::DataBits dataBits);
    void setParity(QSerialPort::Parity parity);
    void setStopBits(QSerialPort::StopBits stopBits);
    void setFlowControl(QSerialPort::FlowControl flowControl);

    // Obter configurações atuais
    int getCurrentBaudRate() const;
    QSerialPort::DataBits getCurrentDataBits() const;
    QSerialPort::Parity getCurrentParity() const;
    QSerialPort::StopBits getCurrentStopBits() const;
    QSerialPort::FlowControl getCurrentFlowControl() const;

signals:
    void dataReceived(const QByteArray &data);
    void errorOccurred(const QString &error);
    void statusChanged(const QString &status);

private slots:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort *serial;
    int currentBaudRate;
    QSerialPort::DataBits currentDataBits;
    QSerialPort::Parity currentParity;
    QSerialPort::StopBits currentStopBits;
    QSerialPort::FlowControl currentFlowControl;
};

#endif // PDTERMSERIAL_H
