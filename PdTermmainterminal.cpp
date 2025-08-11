/*
 * pdtermmainterminal.cpp
 */

#include <QMainWindow>
#include "PdTermmainterminal.h"
#include "./ui_PdTermmainterminal.h"
#include <QStatusBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextBlock>
#include <QThread>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>

PdTermMainTerminal::PdTermMainTerminal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PdTermMainTerminal)
    , m_worker(new Worker)
    , m_thread(new QThread(this))
{
    ui->setupUi(this);
    // Adicione isto para habilitar leitura de teclas:
    this->setFocusPolicy(Qt::StrongFocus);

    QLayout *layout = this->layout(); // Pega o layout existente


    // Agora inicialize o serial
    m_serial = new PdTermSerial(this);

    // Configurações iniciais Serial
    setupSerial();
    setup_ui();
    setup_connect();

    if (!layout) {
        layout = new QVBoxLayout(this); // Cria um novo se não houver
        setLayout(layout);
    }
   //layout->addWidget(statusBar);

    //Exemplo: Adicionar um QLabel à direita da statusBar padrão
    QLabel *customLabel = new QLabel("Versão 1.0", this);
    statusBar()->addPermanentWidget(customLabel);
    statusBar()->showMessage("Mensagem no rodapé");
    /*************************************************************************************/
    /*************************************************************************************/
    /*************************************************************************************/
    // Cria worker e thread
 //   m_worker = new Worker;
 //   m_thread = new QThread(this);

 //   // Move o worker para a thread
 //   m_worker->moveToThread(m_thread);
/*
 * ME PARE4CE QUE O CODIGO ACIMA INICIA A THREAD
 *
 * E O CODIGO ABAIXO TAMBÉM.. ANALIZAR ISSO QUANDO DER!!!
 */
    // Conexões importantes
    //connect(m_thread, &QThread::started, m_worker, &Worker::statusUpdated);
//    connect(m_worker, &Worker::statusUpdated, this, [this](const QString &msg) {
//        statusBar()->showMessage(msg);  // Conexão com o nome correto
//    });

    // Garante limpeza automática
//    connect(m_thread, &QThread::finished, m_worker, &QObject::deleteLater);
//    connect(m_thread, &QThread::finished, m_thread, &QObject::deleteLater);

    // Inicia a thread
//    m_thread->start();
    /*************************************************************************************/
    /*************************************************************************************/
    /*************************************************************************************/



    //Cursor
    // No construtor da sua classe:
    ui->plainTextEdit->setCursorWidth(2);          // Largura do cursor

    // Cria um menu "File" com ação "Exit"
//   QMenu *fileMenu = menuBar()->addMenu("&File");  // Adiciona à barra de menus
//    QAction *exitAction = new QAction("&Exit", this);  // Cria ação Exit
//    fileMenu->addAction(exitAction);  // Adiciona ao menu
    // Conecta a ação ao slot que fecha a aplicação
//    connect(exitAction, &QAction::triggered, this, &PdTermMainTerminal::close);

    connect(ui->action_Exit_2, &QAction::triggered, this, &PdTermMainTerminal::close);

    connect(ui->action_Limpar, &QAction::triggered, this, &PdTermMainTerminal::limparTexto);
    // No construtor da sua janela (após ui->setupUi(this))
    connect(ui->menubar, &QMenuBar::triggered, this, [this]() {
        ui->plainTextEdit->setFocus();  // Restaura o foco após clicar em qualquer menu
    });

    ui->plainTextEdit->setFocus();
    // No construtor da MainWindow:
    ui->plainTextEdit->installEventFilter(this);
}

PdTermMainTerminal::~PdTermMainTerminal()
{
    m_thread->quit();
    m_thread->wait();  // Opcional: espera a thread finalizar
    delete m_serial;  // Garante que a porta serial seja fechada
    delete ui;
}
bool PdTermMainTerminal::eventFilter(QObject *obj, QEvent *event) {
    if (obj == ui->plainTextEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        QByteArray dataToSend;

        // Tecla com caractere visível (ex: 'A', '1')
        if (!keyEvent->text().isEmpty()) {
            char asciiChar = keyEvent->text().at(0).toLatin1();
            dataToSend.append(asciiChar);
        }
        // Teclas especiais (Enter, Backspace, etc.)
        else {
            switch (keyEvent->key()) {
            case Qt::Key_Return: dataToSend.append('\n'); break;  // 0x0A
            case Qt::Key_Backspace: dataToSend.append(0x08); break; // 0x08
            case Qt::Key_Escape: dataToSend.append(0x1B); break;   // 0x1B
            default: return false; // Ignora outras teclas não mapeadas
            }
        }

        // Envia os bytes via serial (se houver dados)
        if (!dataToSend.isEmpty() && m_serial->isConnected()) {
            m_serial->sendData(dataToSend); // Chama seu método existente
        }

        return true; // Consome o evento
    }
    return QObject::eventFilter(obj, event);
}
/*
bool PdTermMainTerminal::eventFilter2(QObject *obj, QEvent *event) {
    if (obj == ui->plainTextEdit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        qDebug() << "Tecla pressionada no plainTextEdit:" << keyEvent->key();

        // Obtém o caractere Unicode (pode ser vazio para teclas como Shift, Ctrl, etc.)
        QString text = keyEvent->text();



        if (!text.isEmpty()) {
            // Pega o primeiro caractere da string (evita combinações como Shift+A)
            byte byteToSend = text.at(0).toLatin1(); // Converte para byte ASCII (0-255)

            qDebug() << "Tecla pressionada (byte):" << (int)byteToSend;

            // Exemplo: Enviar pela serial
            if (m_serial->isConnected() && !text.isEmpty()) {
                qDebug() << "Serial conectada e Tecla pressionada " << byteToSend;
                m_serial->sendData(byteToSend );
            }
        }


        return true; // Consome o evento (opcional)


        // Exemplo: Capturar Enter/Return
        if (keyEvent->key() == Qt::Key_Return) {
            qDebug() << "Enter pressionado!";
            return true;  // Evento consumido (não passa para o plainTextEdit)
        }
    }
    return QMainWindow::eventFilter(obj, event);
}
*/
void PdTermMainTerminal::testeTelaTerminal()
{
    // Exemplo 1: Texto verde padrão (com quebra de linha)
    appendTerminalText("$ Usuário logado.");
    // Exemplo 2: Texto vermelho (sem quebra de linha)
    appendTerminalText("ERRO: ", Qt::red, false);
    appendTerminalText("Falha na conexão.", Qt::red);
    // Exemplo 3: Texto personalizado (laranja)
    appendTerminalText("Alerta: ", QColor(255, 165, 0), false);
    appendTerminalText("Temperatura crítica.", QColor(255, 165, 0));

    // Exemplo 1: Escrever "Hello" na linha 5, coluna 10 (verde padrão)
    setTextAtPosition(12, 10, "Hello");

    // Exemplo 2: Escrever "Error" na linha 3, coluna 0 (vermelho)
    setTextAtPosition(13, 25, "Error", Qt::red);

    // Exemplo 3: Sobrescrever parte de uma linha
    setTextAtPosition(14, 50, "Status: OK");  // Linha 0, coluna 0
    setTextAtPosition(15, 60, "FAIL", Qt::red);  // Altera "OK" para "FAIL"


    // Exemplo 1: Escrever "Hello" na linha 5, coluna 10 (verde padrão)
    setTextAtPosition(19, 10, "Hello");

    // Exemplo 2: Escrever "Error" na linha 3, coluna 0 (vermelho)
    setTextAtPosition(18, 25, "Error", Qt::red);

    // Exemplo 3: Sobrescrever parte de uma linha
    setTextAtPosition(17, 50, "Status: OK");  // Linha 0, coluna 0
    setTextAtPosition(16, 60, "FAIL", Qt::red);  // Altera "OK" para "FAIL"


    // Cabeçalho estilizado
    setTextAtPosition(0, 0, "┌──────────────────────────────┐ ", Qt::cyan);
    setTextAtPosition(1, 0, "│   MEU TERMINAL CUSTOMIZADO   │ ", Qt::yellow);
    setTextAtPosition(2, 0, "└──────────────────────────────┘ ", Qt::cyan);

    // Menu interativo
    setTextAtPosition(4, 0, "1. Iniciar servidor", Qt::green);
    setTextAtPosition(5, 0, "2. Conectar ao banco", Qt::white);

    // Exemplo 1: Texto verde padrão (com quebra de linha)
    appendTerminalText("$ Usuário logado.");
    // Exemplo 2: Texto vermelho (sem quebra de linha)
    appendTerminalText("ERRO: ", Qt::red, false);
    appendTerminalText("Falha na conexão.", Qt::red);
    // Exemplo 3: Texto personalizado (laranja)
    appendTerminalText("Alerta: ", QColor(255, 165, 0), false);
    appendTerminalText("Temperatura crítica.", QColor(255, 165, 0));


}
void PdTermMainTerminal::keyPressEvent(QKeyEvent *event) {
    qDebug() << "Tecla pressionada:" << event->text();

    if (m_serial->isConnected() && event->text().length() > 0) {
        qDebug() << "Serial conectada e Tecla pressionada " << event->text().toUtf8();
        m_serial->sendData(event->text().toUtf8());
    }

    // Exemplo: Ctrl+L para limpar
    if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_L) {
        ui->plainTextEdit->clear();
    }
    if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_S) {
        ui->plainTextEdit->clear();
        event->accept();  // ⭐ Importante!
    }
    if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_U) {
        ui->plainTextEdit->clear();
        event->accept();  // ⭐ Importante!
    }
    if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_Y) {
        ui->plainTextEdit->clear();
    }

    // Exemplo: Ctrl+Q para terminar
    if (event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_Q) {
        this->close();
        event->accept();  // ⭐ Importante!
    } else {
        event->ignore();
    }
    // Primeiro verifique se a serial está conectada
    if (m_serial && m_serial->isConnected()) {
        // Trate teclas especiais e caracteres imprimíveis
        if (!event->text().isEmpty() && event->text().at(0).isPrint()) {
            QString text = event->text();
            m_serial->sendData(text.toUtf8());

            // Opcional: mostre o caractere localmente também
            ui->plainTextEdit->insertPlainText(text);

            // Impede o processamento padrão para evitar duplicação
            event->accept();
            return;
        }

        // Trate teclas especiais (como Enter)
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            m_serial->sendData("\r\n");  // Envia quebra de linha
            ui->plainTextEdit->insertPlainText("\n");  // Mostra localmente
            event->accept();
            return;
        }
    }
    // Ou envie o texto para o terminal
    if (event->key() >= Qt::Key_Space && event->key() <= Qt::Key_ydiaeresis) {
        ui->plainTextEdit->insertPlainText(event->text());
    }

    QMainWindow::keyPressEvent(event);  // Mantenha o comportamento padrão
}

void PdTermMainTerminal::limparTexto() {
    ui->plainTextEdit->clear();  // Limpa todo o conteúdo do QPlainTextEdit
}
//*******************************************************************************************
//*******************************ON    SERIAL************************************************
//*******************************************************************************************
void PdTermMainTerminal::onSerialDataReceived(const QByteArray &data)
{
    appendTerminalText(QString::fromUtf8(data), Qt::green);
}

void PdTermMainTerminal::onSerialError(const QString &error)
{
    appendTerminalText("[ERRO SERIAL] " + error, Qt::red);
    statusBar()->showMessage(error, 5000);
}

void PdTermMainTerminal::onSerialStatusChanged(const QString &status)
{
    statusBar()->showMessage(status);
    //appendTerminalText("[STATUS] " + status, Qt::blue);
    //statusBar()->setText(status);
}

//*******************************************************************************************
//*************************************SERIAL************************************************
//*******************************************************************************************
void PdTermMainTerminal::setupSerial()
{
    connect(m_serial, &PdTermSerial::dataReceived,this, &PdTermMainTerminal::onSerialDataReceived);
    connect(m_serial, &PdTermSerial::errorOccurred,this, &PdTermMainTerminal::onSerialError);
    connect(m_serial, &PdTermSerial::statusChanged,this, &PdTermMainTerminal::onSerialStatusChanged);

    // Configuração inicial padrão
    m_serial->setBaudRate(115200);  // Baud rate padrão
}

void PdTermMainTerminal::setup_ui(){

    //Cursor
    ui->plainTextEdit->setCursorWidth(2);          // Largura do cursor

}
void PdTermMainTerminal::setup_connect(){
    // Conexões existentes...
    connect(ui->actionConectar, &QAction::triggered, this, [this]() {
        m_serial->connectSerial("/dev/ttyUSB0");  // Ou obtenha a porta de um QComboBox
    });

    connect(ui->actionDesconectar, &QAction::triggered,
            m_serial, &PdTermSerial::disconnectSerial);

    connect(ui->on_actionSerialSettings, &QAction::triggered,
            this, &PdTermMainTerminal::on_actionSerialSettings);
}

void PdTermMainTerminal::on_actionSerialSettings()
{
    QDialog dialog(this);
    QFormLayout layout(&dialog);

    // Seletor de porta
    QComboBox portCombo;
    m_serial->refreshPorts(&portCombo);
    layout.addRow("Porta:", &portCombo);

    // Baud rate
    QComboBox baudCombo;
    baudCombo.addItems({"9600", "19200", "38400", "57600", "115200", "230400", "460800", "921600"});
    baudCombo.setCurrentText(QString::number(m_serial->getCurrentBaudRate()));
    layout.addRow("Baud Rate:", &baudCombo);

    // Bits de dados
    QComboBox dataBitsCombo;
    dataBitsCombo.addItem("5", QSerialPort::Data5);
    dataBitsCombo.addItem("6", QSerialPort::Data6);
    dataBitsCombo.addItem("7", QSerialPort::Data7);
    dataBitsCombo.addItem("8", QSerialPort::Data8);
    dataBitsCombo.setCurrentIndex(3); // 8 bits por padrão
    layout.addRow("Bits de Dados:", &dataBitsCombo);

    // Botões
    QDialogButtonBox buttons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout.addRow(&buttons);
    connect(&buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        m_serial->connectSerial(
            portCombo.currentText(),
            baudCombo.currentText().toInt(),
            static_cast<QSerialPort::DataBits>(dataBitsCombo.currentData().toInt()),
            QSerialPort::NoParity,  // Pode adicionar combo para paridade também
            QSerialPort::OneStop    // E para stop bits
            );
    }
}

//*******************************************************************************************
//*******************************************************************************************


void PdTermMainTerminal::appendTerminalText(const QString &text, const QColor &color, bool newLine) {
    // Salva a cor atual do texto
    QTextCharFormat originalFormat = ui->plainTextEdit->currentCharFormat();

    // Define a nova cor
    QTextCharFormat colorFormat;
    colorFormat.setForeground(color);
    ui->plainTextEdit->setCurrentCharFormat(colorFormat);

    // Adiciona o texto (com ou sem quebra de linha)
    //if (newLine) {
    //    ui->plainTextEdit->appendPlainText(text);
    ////} else {
    ui->plainTextEdit->insertPlainText(text);
    //}

    // Restaura a formatação original
    ui->plainTextEdit->setCurrentCharFormat(originalFormat);

    // Rolagem automática para o final
    QTextCursor cursor = ui->plainTextEdit->textCursor();

    cursor.movePosition(QTextCursor::End);
    ui->plainTextEdit->setTextCursor(cursor);
    ui->plainTextEdit->setEnabled(true);
    ui->plainTextEdit->setCursorWidth(2);
    ui->plainTextEdit->setReadOnly(false); // Permite edição
}


void PdTermMainTerminal::setTextAtPosition(int row, int col, const QString &text, const QColor &color) {
    // Garante que o documento tem linhas suficientes
    ensureLineExists(row);

    QTextCursor cursor(ui->plainTextEdit->document());

    // Posiciona o cursor no início da linha desejada
    cursor.movePosition(QTextCursor::Start);
    for (int i = 0; i < row; ++i) {
        if (!cursor.movePosition(QTextCursor::Down)) {
            // Se a linha não existe, insere uma nova
            cursor.movePosition(QTextCursor::End);
            cursor.insertText("\n");
        }
    }

    // Move para a coluna exata
    cursor.movePosition(QTextCursor::StartOfLine);
    int currentLineLength = cursor.block().text().length();

    // Se a coluna desejada é maior que o fim da linha, preenche com espaços
    if (col > currentLineLength) {
        cursor.movePosition(QTextCursor::EndOfLine);
        cursor.insertText(QString(col - currentLineLength, ' '));
    } else {
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, col);
    }

    // Insere o texto com a cor especificada
    QTextCharFormat format;
    format.setForeground(color);
    cursor.setCharFormat(format);
    cursor.insertText(text);

    // Mantém a visibilidade
    ui->plainTextEdit->setTextCursor(cursor);
}

// Garante que a linha exista (cria linhas vazias se necessário)
void PdTermMainTerminal::ensureLineExists(int row) {
    QTextDocument *doc = ui->plainTextEdit->document();
    int currentLines = doc->lineCount();

    if (row >= currentLines) {
        QTextCursor cursor(doc);
        cursor.movePosition(QTextCursor::End);

        for (int i = currentLines; i <= row; ++i) {
            cursor.insertText("\n");
        }
    }
}
