#include "pdtermserial.h"
#include <QMessageBox>
#include <QDebug>

PdTermSerial::PdTermSerial(QObject *parent) : QObject(parent),
    serial(new QSerialPort(this)),
    currentBaudRate(9600),
    currentDataBits(QSerialPort::Data8),
    currentParity(QSerialPort::NoParity),
    currentStopBits(QSerialPort::OneStop),
    currentFlowControl(QSerialPort::NoFlowControl)
{
    connect(serial, &QSerialPort::readyRead, this, &PdTermSerial::handleReadyRead);
    connect(serial, &QSerialPort::errorOccurred, this, &PdTermSerial::handleError);
}

PdTermSerial::~PdTermSerial()
{
    if(serial->isOpen()) {
        serial->close();
    }
}

void PdTermSerial::refreshPorts(QComboBox *portCombo)
{
    if (!portCombo) return;

    portCombo->clear();
    const auto ports = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &port : ports) {
        QString portName = port.portName();
        // Mostra informações completas da porta no tooltip
        QString tooltip = QString("Porta: %1\nDescrição: %2\nFabricante: %3\nID: %4")
                              .arg(port.portName())
                              .arg(port.description())
                              .arg(port.manufacturer())
                              .arg(port.serialNumber());

        portCombo->addItem(portName);
        portCombo->setItemData(portCombo->count()-1, tooltip, Qt::ToolTipRole);

        qDebug() << "Porta encontrada:" << portName << "-" << port.description();
    }

    emit statusChanged(portCombo->count() > 0
                           ? QString("%1 portas encontradas").arg(portCombo->count())
                           : "Nenhuma porta encontrada");
}

bool PdTermSerial::connectSerial(const QString &portName, int baudRate,
                                 QSerialPort::DataBits dataBits,
                                 QSerialPort::Parity parity,
                                 QSerialPort::StopBits stopBits,
                                 QSerialPort::FlowControl flowControl)
{
    if (serial->isOpen()) {
        emit statusChanged("Já conectado a " + serial->portName());
        return true;
    }

    serial->setPortName(portName);

    if (!serial->open(QIODevice::ReadWrite)) {
        emit errorOccurred("Não foi possível abrir a porta " + portName);
        return false;
    }

    // Aplica todas as configurações
    if (!serial->setBaudRate(baudRate) ||
        !serial->setDataBits(dataBits) ||
        !serial->setParity(parity) ||
        !serial->setStopBits(stopBits) ||
        !serial->setFlowControl(flowControl)) {

        QString errorMsg = "Falha na configuração da porta:\n";
        if (!serial->setBaudRate(baudRate)) errorMsg += "- Baud rate inválido\n";
        if (!serial->setDataBits(dataBits)) errorMsg += "- Bits de dados inválidos\n";
        if (!serial->setParity(parity)) errorMsg += "- Paridade inválida\n";
        if (!serial->setStopBits(stopBits)) errorMsg += "- Stop bits inválidos\n";
        if (!serial->setFlowControl(flowControl)) errorMsg += "- Flow control inválido\n";

        emit errorOccurred(errorMsg);
        serial->close();
        return false;
    }

    // Armazena as configurações atuais
    currentBaudRate = baudRate;
    currentDataBits = dataBits;
    currentParity = parity;
    currentStopBits = stopBits;
    currentFlowControl = flowControl;

    emit statusChanged(QString("Conectado a %1 @ %2 bauds, %3 bits, %4 paridade, %5 stop bits")
                           .arg(portName)
                           .arg(baudRate)
                           .arg(dataBits)
                           .arg(parity == QSerialPort::NoParity ? "Sem" :
                                    parity == QSerialPort::EvenParity ? "Par" : "Ímpar")
                           .arg(stopBits == QSerialPort::OneStop ? "1" : "1.5 ou 2"));

    return true;
}

void PdTermSerial::disconnectSerial()
{
    if (serial->isOpen()) {
        QString portName = serial->portName();
        serial->close();
        emit statusChanged("Desconectado de " + portName);
    }
}

bool PdTermSerial::isConnected() const
{
    return serial && serial->isOpen();
}

void PdTermSerial::sendData(const QByteArray &data)
{
    if (!serial->isOpen()) {
        emit errorOccurred("Porta serial não está aberta");
        return;
    }

    qint64 bytesWritten = serial->write(data);
    if (bytesWritten == -1) {
        emit errorOccurred("Falha no envio: " + serial->errorString());
    } else if (bytesWritten < data.size()) {
        emit errorOccurred(QString("Dados parcialmente enviados (%1/%2 bytes)")
                               .arg(bytesWritten).arg(data.size()));
    } else {
        serial->flush();
        emit statusChanged(QString("%1 bytes enviados com sucesso").arg(bytesWritten));
    }
}

// Métodos de configuração individual
void PdTermSerial::setBaudRate(int baudRate)
{
    if (serial->isOpen() && serial->setBaudRate(baudRate)) {
        currentBaudRate = baudRate;
        emit statusChanged(QString("Baud rate alterado para %1").arg(baudRate));
    }
}

void PdTermSerial::setDataBits(QSerialPort::DataBits dataBits)
{
    if (serial->isOpen() && serial->setDataBits(dataBits)) {
        currentDataBits = dataBits;
        emit statusChanged(QString("Bits de dados alterados para %1").arg(dataBits));
    }
}

void PdTermSerial::setParity(QSerialPort::Parity parity)
{
    if (serial->isOpen() && serial->setParity(parity)) {
        currentParity = parity;
        emit statusChanged(QString("Paridade alterada para %1")
                               .arg(parity == QSerialPort::NoParity ? "Nenhuma" :
                                        parity == QSerialPort::EvenParity ? "Par" : "Ímpar"));
    }
}

void PdTermSerial::setStopBits(QSerialPort::StopBits stopBits)
{
    if (serial->isOpen() && serial->setStopBits(stopBits)) {
        currentStopBits = stopBits;
        emit statusChanged(QString("Stop bits alterados para %1")
                               .arg(stopBits == QSerialPort::OneStop ? "1" : "1.5 ou 2"));
    }
}

void PdTermSerial::setFlowControl(QSerialPort::FlowControl flowControl)
{
    if (serial->isOpen() && serial->setFlowControl(flowControl)) {
        currentFlowControl = flowControl;
        emit statusChanged(QString("Flow control alterado para %1")
                               .arg(flowControl == QSerialPort::NoFlowControl ? "Nenhum" :
                                        flowControl == QSerialPort::HardwareControl ? "Hardware" : "Software"));
    }
}

// Métodos para obter configurações atuais
int PdTermSerial::getCurrentBaudRate() const { return currentBaudRate; }
QSerialPort::DataBits PdTermSerial::getCurrentDataBits() const { return currentDataBits; }
QSerialPort::Parity PdTermSerial::getCurrentParity() const { return currentParity; }
QSerialPort::StopBits PdTermSerial::getCurrentStopBits() const { return currentStopBits; }
QSerialPort::FlowControl PdTermSerial::getCurrentFlowControl() const { return currentFlowControl; }

// Slots privados
void PdTermSerial::handleReadyRead()
{
    QByteArray data = serial->readAll();
    if (!data.isEmpty()) {
        emit dataReceived(data);
        emit statusChanged(QString("%1 bytes recebidos").arg(data.size()));
    }
}

void PdTermSerial::handleError(QSerialPort::SerialPortError error)
{
    if (error != QSerialPort::NoError) {
        QString errorStr = QString("Erro serial (%1): %2")
        .arg(error)
            .arg(serial->errorString());
        emit errorOccurred(errorStr);

        if (serial->isOpen() && error == QSerialPort::ResourceError) {
            serial->close();
            emit statusChanged("Conexão perdida - porta fechada");
        }
    }
}
