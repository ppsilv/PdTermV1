#include "pdtermxmodem.h"

#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QDebug> // For outputting the file path
#include <QElapsedTimer>
#include <QThread>
#include <QCoreApplication>


PdTermXmodem::PdTermXmodem(QObject *parent): QObject{parent}
{
    total_envios=0;
    total_blocos=0;
    total_blocos_reenviados=0;
}

void PdTermXmodem::printFile( QByteArray fileData )
{
    // Converter para QString e imprimir no console
    QString fileContent = QString::fromUtf8(fileData);

    // Imprime o conteúdo no formato:
    // 0000 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
    qDebug() << "Conteúdo do arquivo em hexadecimal:";
    for (int i = 0; i < fileData.size(); i += 16) {
        // Imprime o endereço (4 dígitos hex)
        QString line = QString("%1").arg(i, 4, 16, QChar('0')).toUpper() + " ";

        // Imprime os bytes em hex (16 por linha)
        for (int j = 0; j < 16; j++) {
            if (i + j < fileData.size()) {
                line += QString("%1 ").arg((uchar)fileData.at(i + j), 2, 16, QChar('0')).toUpper();

                // Adiciona um espaço extra após o 8º byte
                if (j == 7)
                    line += " ";

            } else {
                line += "   "; // Preenche com espaços se não houver dados
            }
        }

        qDebug().noquote() << line;

        // Limita a impressão aos primeiros 1024 bytes para não sobrecarregar
        if (i >= 1024) {
            qDebug() << "... (arquivo muito grande, mostrando apenas primeiros 1024 bytes)";
            break;
        }
    }
}

void PdTermXmodem::enviarArquivoXmodem()
{
    QString filePath = QFileDialog::getOpenFileName(
        nullptr,
        tr("Open File"),
        QDir::homePath(), // Start in the user's home directory
        tr("Text files (*.bin);;All files (*.*)"));

    //***********************************************
    // Sem arquivo retorna
    if (filePath.isEmpty()) {
        emit transmissaoCancelada();
        emit progressoAtualizado(100);
        return;
    }

    //***********************************************
    // Abrir o arquivo
    QFile arquivo(filePath);
    if (!arquivo.open(QIODevice::ReadOnly)) {
        emit erroOcorreu("Falha ao abrir o arquivo");
        return;
    }
    qDebug() << "[INFO]- Arquivo aberto";

    //***********************************************
    // Carregar o arquivo
    // 1.1 Ler arquivo completo para buffer
    QByteArray fileData = arquivo.readAll();
    arquivo.close();

    if (fileData.isEmpty()) {
        emit erroOcorreu("Arquivo vazio");
        return;
    }

    //printFile(fileData);

    //Set the input directions to xmodem
    setFlag(false);
    QByteArray resposta = recebe_dados_serial(io_context, 1000);

    // 2. Iniciar transferência enviando 0x33 (ASCII #3)
    enviarDados(QByteArray(1, 0x33));

    // 3. Aguardar NACK (0x15)
    if (!esperarPorByte(0x15)) {
        emit erroOcorreu("Timeout aguardando NACK");
        setFlag(true);
        return;
    }


    // 4. Protocolo Xmodem
    const int BLOCK_SIZE = 128;
    int blockNumber = 1;
    int bytesSent = 0;
    bool cancelado = false;
    total_envios=0;
    total_blocos_reenviados=0;
    reenvio=0;

    while (bytesSent < fileData.size() && !cancelado) {
        // Preparar bloco
        QByteArray block;
        block.append(0x01); // SOH
        block.append(blockNumber); // Número do bloco
        block.append(255 - blockNumber); // Complemento do número do bloco

        qDebug() << "Enviando block [" << blockNumber << "] ";

        // Dados (128 bytes)
        int bytesToCopy = qMin(BLOCK_SIZE, fileData.size() - bytesSent);
        block.append(fileData.mid(bytesSent, bytesToCopy));

        // Preencher com EOF se necessário
        if (bytesToCopy < BLOCK_SIZE) {
            block.append(QByteArray(BLOCK_SIZE - bytesToCopy, 0x1A));
        }

        // Calcular checksum
        char checksum = calcularChecksum(block.mid(3, BLOCK_SIZE));
        block.append(checksum);

        printFile(block);
        // Enviar bloco
        //enviarDados(block);

        for (int i = 0; i < block.size(); ++i) {
            enviarDados(QByteArray(1, block.at(i)));  // ✅ Forma ideal
            //qDebug() << "Env: "<< block.at(i);
            QThread::msleep(1);
        }

        // Aguardar ACK (0x06) ou NACK (0x15)
        total_envios++;
        char resposta = 0;
        qDebug()<< "Aguardando 0x06 (ACK)";
        int answer= esperarAckNack(1000);
        if( answer == 2 ){
            total_blocos_reenviados++;
            reenvio++;
            if ( reenvio == 5 ){
                emit erroOcorreu("Falha na transferência");
                setFlag(true);
                return;
            }
            continue;
        }
        reenvio=0;
        if( answer == 0 ){
            emit erroOcorreu("Falha na transferência");
            setFlag(true);
            return;
        }
        qDebug() << "Block " << blockNumber << " recebido com sucesso";

       // if (!esperarPorByte(0x06, 10000)) { // Timeout de 10s para ACK
       //     qDebug()<< "Vou aguardar 0x15 (NACK)";
       //     if (!esperarPorByte(0x15)) {
       //         emit erroOcorreu("Falha na transferência");
       //         setFlag(true);
       //         return;
       //     }
       //     // NACK recebido, reenviar bloco
       //     continue;
       // }

        // Atualizar progresso
        bytesSent += bytesToCopy;
        int progresso = (bytesSent * 100) / fileData.size();
        //emit progressoAtualizado(progresso);

        blockNumber++;

        if (blockNumber > 255) blockNumber = 1;
    }
    qDebug() << "Terminando enviando EOT";
    // 5. Enviar EOT (0x04) para finalizar
    enviarDados(QByteArray(1, 0x04));

    // 6. Concluir
    emit transmissaoConcluida();
    setFlag(true);
    total_blocos=blockNumber-1;
    qDebug() << "Total blocos do arquivo: "<<total_blocos;
    qDebug() << "Total blocos reenviados: "<<total_blocos_reenviados;
    qDebug() << "Total blocos   enviados: "<<total_envios;
}

bool PdTermXmodem::esperarPorByte(char byteEsperado, int timeout_ms) {
    // Verificações iniciais
    if (!recebe_dados_serial || !io_context) {
        qCritical() << "Recepção não inicializada!";
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    while (!timer.hasExpired(timeout_ms)) {
        QByteArray resposta = recebe_dados_serial(io_context, 100);
        //qDebug()<< "Dados recebido [" << resposta << "] ";
        if (!resposta.isEmpty()) {
            qDebug() << "Byte recebido:" << resposta.toHex();
            if (resposta.contains(byteEsperado)) {
                return true;
            }
        }
        QCoreApplication::processEvents(); // Mantém a UI responsiva
        QThread::msleep(50);
    }
    qWarning() << "Timeout esperando pelo byte" << QString::number(byteEsperado, 16);
    return false;
}

int PdTermXmodem::esperarAckNack(int timeout_ms) {
    // Verificações iniciais
    if (!recebe_dados_serial || !io_context) {
        qCritical() << "Recepção não inicializada!";
        return false;
    }

    QElapsedTimer timer;
    timer.start();

    while (!timer.hasExpired(timeout_ms)) {
        QByteArray resposta = recebe_dados_serial(io_context, 100);
        //qDebug()<< "Dados recebido [" << resposta << "] ";
        if (!resposta.isEmpty()) {
            //qDebug() << "Byte recebido:" << resposta.toHex();
            if (resposta.contains(0x06)) {
                //qDebug()<< "Recebido 06";
                return 1;
            }
            if (resposta.contains(0x15)) {
                //qDebug()<< "Recebido 15";
                return 2;
            }
            return 0;
        }

        QCoreApplication::processEvents(); // Mantém a UI responsiva
        QThread::msleep(50);
    }

    qWarning() << "Timeout esperando pelo byte pela resposta..." ;
    return 0;
}

char PdTermXmodem::calcularChecksum(const QByteArray &dados)
{
    char checksum = 0;
    for (char byte : dados) {
        checksum += byte;
    }
    return checksum;
}
