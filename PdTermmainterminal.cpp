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
#include <QToolBar>
#include <QPainter>


PdTermMainTerminal::PdTermMainTerminal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PdTermMainTerminal)
    , m_worker(new Worker)
    , m_thread(new QThread(this))
    , m_serial(new PdTermSerial(this))
    , m_xmodemWorker(nullptr)
    , m_workerThread(nullptr)
    , m_control(new PdTerminalControl(this))
{
    qDebug() << "Construtor MainWindow iniciado";
    ui->setupUi(this);
    qDebug() << "UI setup completo";

    ansiterm = new PdTerminalControl ();

    ui->toolbar->setFixedWidth(800);
    ui->toolbar->setMinimumHeight(64);
    //********************************************************************************************************
    //toolBar Criando manualmente porque se fizer tudo no arquivo UI aparece duplicado, porque o qtcreator cria
    //uma toolbar por padrao. pelo projeto escolhido
    // Crie ações manualmente
    QAction *actionEnviarArquivo = new QAction(QIcon(":/icons/icons/sendfile.png"), "Enviar arquivo", this);
    QAction *actionCancelarEnvio = new QAction(QIcon(":/icons/icons/cancel.png"), "Cancelar envio", this);

    connect(actionEnviarArquivo, &QAction::triggered, this, &PdTermMainTerminal::on_botaoEnviar_clicked);
    connect(actionCancelarEnvio, &QAction::triggered, this, [this]() {
        if (m_xmodemWorker) {
            QMetaObject::invokeMethod(m_xmodemWorker, "cancelarTransmissao", Qt::QueuedConnection);
        }
    });

    ui->toolbar->addAction(actionEnviarArquivo);
    ui->toolbar->addAction(actionCancelarEnvio);

    ui->toolbar->addSeparator();


    QAction *actionConectar = new QAction(QIcon(":/icons/icons/connect.png"), "Conectar RS232", this);
    QAction *actionDesconectar = new QAction(QIcon(":/icons/icons/disconnect.png"), "Desconectar RS232", this);

    connect(actionConectar, &QAction::triggered, this, [this]() {
        m_serial->connectSerial("/dev/ttyUSB0");  // Ou obtenha a porta de um QComboBox
    });
    connect(actionDesconectar, &QAction::triggered, m_serial, &PdTermSerial::disconnectSerial);


    QAction *actionConfigurar = new QAction(QIcon(":/icons/icons/settings.png"), "Configurar RS232", this);
    connect(actionConfigurar, &QAction::triggered,this, &PdTermMainTerminal::on_actionSerialSettings);

    ui->toolbar->addAction(actionConectar);
    ui->toolbar->addAction(actionDesconectar);
    ui->toolbar->addSeparator();
    ui->toolbar->addAction(actionConfigurar);

    // No construtor
    // Cria LED de status na statusbar
    m_serialStatusLed = new QPushButton(this);
    m_serialStatusLed->setFlat(true);
    m_serialStatusLed->setEnabled(false); // Não clicável

    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->toolbar->addWidget(spacer);
    ui->toolbar->addSeparator();
    ui->toolbar->addWidget(spacer);




    label = new QLabel();
    label->setText("  ");
    ui->toolbar->addWidget(label);
    label->setFixedSize(525,64);


    // 1. Crie um NOVO LED para a toolbar (não use o ui->led existente)
    toolbarLed = new QLabel(this);
    toolbarLed->setFixedSize(64, 64);
    toolbarLed->setPixmap(QPixmap(":/icons/icons/led_green.png").scaled(64, 64));



    ui->toolbar->addSeparator();
    // 2. Adicione à toolbar
    ui->toolbar->addWidget(toolbarLed);


    // Estado inicial
    updateSerialStatus(false);

    // Estado inicial
    ui->actionCancelarEnvio->setEnabled(false);
    ui->actionDesconectar->setEnabled(false);
    //Fim da criação da toolbar
    //**********************************************************************************************************

    this->setWindowTitle("PdTermV1");
    this->setWindowIcon(QIcon(":icons/pdtermv2.svg"));
    ui->setupUi(this);
    // Adicione isto para habilitar leitura de teclas:
    this->setFocusPolicy(Qt::StrongFocus);

    QLayout *layout = this->layout(); // Pega o layout existente

    flag_from_serial_write_to_terminal = true;
    flag_from_terminal_write_to_serial = true;
    flag_from_serial_write_to_VT100 = false;

    cor = Qt::green;
    this->flagsetBold = false;

    //***********************************************************************************
    // XMODEM
    progressBar = new QProgressBar(); // Sem parent! A statusBar assumirá o controle
    progressBar->setTextVisible(true);
    progressBar->setFixedWidth(200);
    progressBar->setFixedHeight(18);

    // StyleSheet (como você já tinha)
    progressBar->setStyleSheet(
        "QProgressBar {"
        "    border: 1px solid grey; border-radius: 5px;"
        "    background: #f0f0f0;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: #69F0AE; border-radius: 5px;"
        "}"
        );
    // Adiciona à statusBar (à direita)
    ui->statusbar->addPermanentWidget(progressBar);
    // Opcional: Espaçamento entre widgets na statusBar
    ui->statusbar->setStyleSheet("QStatusBar::item { border: none; margin: 2px; }");;
    //------------------------statusbar---------------------------------------------------


    //***********************************************************************************
    // SERIAL
    // Agora inicialize o serial
    //m_serial = new PdTermSerial(this);
    // Configurações iniciais Serial
    setupSerialSignals();
    setup_ui();
    setup_connect();

    if (!layout) {
        layout = new QVBoxLayout(this); // Cria um novo se não houver
        setLayout(layout);
    }
   //layout->addWidget(statusBar);

    //Exemplo: Adicionar um QLabel à direita da statusBar padrão
    QLabel *customLabel = new QLabel("Versão 1.1", this);
    statusBar()->addPermanentWidget(customLabel);


    //Cursor
    // No construtor da sua classe:
    ui->plainTextEdit->setCursorWidth(2);          // Largura do cursor

    connect(ui->action_Exit_2, &QAction::triggered, this, &PdTermMainTerminal::close);

    connect(ui->action_Limpar, &QAction::triggered, this, &PdTermMainTerminal::limparTexto);
    // No construtor da sua janela (após ui->setupUi(this))
    connect(ui->menubar, &QMenuBar::triggered, this, [this]() {
        ui->plainTextEdit->setFocus();  // Restaura o foco após clicar em qualquer menu
    });

    ui->plainTextEdit->setFocus();
    // No construtor da MainWindow:
    ui->plainTextEdit->installEventFilter(this);

    qDebug() << "Construtor terminado completo";

}

PdTermMainTerminal::~PdTermMainTerminal()
{
    qDebug() << "Destrutor MainWindow iniciado";
    cleanupThread();
    if (m_workerThread && m_workerThread->isRunning()) {
        m_workerThread->quit();
        m_workerThread->wait(1000); // Timeout de 1 segundo
    }
    delete m_workerThread; // A thread já deve ter se deletado, mas é seguro
    delete m_serial;
    delete ui;
    qDebug() << "Destrutor MainWindow finalizado";
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
        if (!dataToSend.isEmpty() && m_serial->isConnected() && flag_from_terminal_write_to_serial ) {
            m_serial->sendData(dataToSend); // Chama seu método existente
        }

        return true; // Consome o evento
    }
    return QObject::eventFilter(obj, event);
}
QString PdTermMainTerminal::openFileXmodem()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    QDir::homePath(), // Start in the user's home directory
                                                    tr("Text files (*.txt);;All files (*.*)"));

    return filePath;
}
void PdTermMainTerminal::testeTelaTerminal()
{
    appendTerminalText("$ Usuário logado.");
    appendTerminalText("ERRO: ", Qt::red, false);
    appendTerminalText("Falha na conexão.", Qt::red);
    appendTerminalText("Alerta: ", QColor(255, 165, 0), false);
    appendTerminalText("Temperatura crítica.", QColor(255, 165, 0));
    setTextAtPosition(12, 10, "Hello");
    setTextAtPosition(13, 25, "Error", Qt::red);
    setTextAtPosition(15, 60, "FAIL", Qt::red);  // Altera "OK" para "FAIL"
    setTextAtPosition(19, 10, "Hello");
    setTextAtPosition(18, 25, "Error", Qt::red);
    setTextAtPosition(17, 50, "Status: OK");  // Linha 0, coluna 0
    setTextAtPosition(16, 60, "FAIL", Qt::red);  // Altera "OK" para "FAIL"
    // Cabeçalho estilizado
    setTextAtPosition(0, 0, "┌──────────────────────────────┐ ", Qt::cyan);
    setTextAtPosition(1, 0, "│   MEU TERMINAL CUSTOMIZADO   │ ", Qt::yellow);
    setTextAtPosition(2, 0, "└──────────────────────────────┘ ", Qt::cyan);
    // Menu interativo
    setTextAtPosition(4, 0, "1. Iniciar servidor", Qt::green);
    setTextAtPosition(5, 0, "2. Conectar ao banco", Qt::white);
}


void PdTermMainTerminal::limparTexto() {
    ui->plainTextEdit->clear();  // Limpa todo o conteúdo do QPlainTextEdit
}
//*******************************************************************************************
//*************************************ON CONTROL********************************************
//*******************************************************************************************
void PdTermMainTerminal::setupControlSignals(){
    // Conectar sinais aos slots apropriados
    connect(m_control, &PdTerminalControl::clearScreen, this, &PdTermMainTerminal::clearScreen);
    connect(m_control, &PdTerminalControl::setCursorPosition, this, &PdTermMainTerminal::setCursorPosition);
    connect(m_control, &PdTerminalControl::setTextColor, this, &PdTermMainTerminal::setTextColor);
    connect(m_control, &PdTerminalControl::setBold, this, &PdTermMainTerminal::setBold);
    connect(m_control, &PdTerminalControl::unknownSequence, this, &PdTermMainTerminal::unknownSequence);
}
void PdTermMainTerminal::setTextColor(int color){
    cor = color;
}

void PdTermMainTerminal::setBold(bool enabled){
    this->flagsetBold = enabled;
}

void PdTermMainTerminal::unknownSequence(const QByteArray &seq){
    qDebug() << "Control sequence unknown "<<seq;
}
void PdTermMainTerminal::clearScreen(){
    ui->plainTextEdit->clear();
}

void PdTermMainTerminal::setCursorPosition(int row, int col){
    setTextAtPosition(row, col, "");
}

void PdTermMainTerminal::writeTerminal(const QString &mensagem, bool newline){
    if(flag_from_serial_write_to_VT100 == true){
        m_control->processData(QByteArray(mensagem.toUtf8()));
    }
    if(this->flagsetBold == true){
        QTextCharFormat boldFormat;
        boldFormat.setForeground(cor);
        boldFormat.setFontWeight(QFont::ExtraBold);
        ui->plainTextEdit->textCursor().insertText(mensagem, boldFormat);
        this->flagsetBold = false;
    }else{
        if (mensagem.contains("\x1B[2J")) {
            qDebug() << "Sequência de limpeza encontrada!";
            this->ui->plainTextEdit->clear();
            QString substring = mensagem.mid(4);
            this->appendTerminalText(substring, cor , newline);
        }else if(mensagem.contains("\x1B[")){
            flag_from_serial_write_to_VT100 = true;
        }else{
            this->appendTerminalText(mensagem, cor , newline);
        }
    }
}
//*******************************************************************************************
//*************************************CONTROL **********************************************
//*******************************************************************************************

//*******************************************************************************************
//*************************************ON XMODEM************************************************
//*******************************************************************************************
/*
void PdTermMainTerminal::setupXmodemSignals(){
    // Conectando os signals aos slots
    connect(ui->action_Enviar_arquivo, &QAction::triggered, this, [this]() {
        m_xmodemWorker->enviarArquivoXmodem();
    });
    connect(m_xmodemWorker, &PdTermXmodem::transmissaoCancelada, this, &PdTermMainTerminal::onTransmissaoCancelada);
    connect(m_xmodemWorker, &PdTermXmodem::transmissaoConcluida, this, &PdTermMainTerminal::onTransmissaoConcluida);
    connect(m_xmodemWorker, &PdTermXmodem::erroOcorreu,          this, &PdTermMainTerminal::onErroOcorreu);
    connect(m_xmodemWorker, &PdTermXmodem::progressoAtualizado,  progressBar, &QProgressBar::setValue);

}

void PdTermMainTerminal::onTransmissaoCancelada(){
    qDebug() << "[INOF]-Transmissão cancelada pelo usuário";
    QMessageBox::information(this, "Xmodem", "Transmissão cancelada");
}

void PdTermMainTerminal::onTransmissaoConcluida() {
    qDebug() << "Transmissão concluída com sucesso!";
    QMessageBox::information(this, "Xmodem", "Arquivo enviado com sucesso!");
}

void PdTermMainTerminal::onErroOcorreu(const QString &mensagem) {
    qDebug() << "Erro na transmissão:" << mensagem;
    QMessageBox::critical(this, "Erro Xmodem", mensagem);
}

void PdTermMainTerminal::onProgressoAtualizado(int porcentagem) {
    qDebug() << "Progresso:" << porcentagem << "%";
    // Atualiza uma barra de progresso na UI, se existir
    QString color;
    if (porcentagem < 50)       color = "#FF5252";  // Vermelho
    else if (porcentagem < 80)  color = "#FFD740";  // Amarelo
    else                        color = "#69F0AE";  // Verde

    progressBar->setStyleSheet(
        "QProgressBar::chunk { background-color: " + color + "; }"
    );
    progressBar->setValue(porcentagem);
}
*/
QByteArray PdTermMainTerminal::receiveSerialData(int timeout_ms) {
    return m_serial->waitForData(timeout_ms);
}

void PdTermMainTerminal::sendSerialData(const QByteArray& data) {
    m_serial->sendData(data); // Seu método existente
}

void PdTermMainTerminal::setFlagSerial(bool flag) {
    if( flag ){
        flag_from_serial_write_to_terminal = true;
        flag_from_terminal_write_to_serial = true;
    }else{
        flag_from_serial_write_to_terminal = false;
        flag_from_terminal_write_to_serial = false;
    }
}
//*******************************************************************************************
//*************************************XMODEM************************************************
//*******************************************************************************************

//*******************************************************************************************
//*******************************ON    SERIAL************************************************
//*******************************************************************************************
void PdTermMainTerminal::setupSerialSignals()
{
    connect(m_serial, &PdTermSerial::dataReceived,this, &PdTermMainTerminal::onSerialDataReceived);
    connect(m_serial, &PdTermSerial::errorOccurred,this, &PdTermMainTerminal::onSerialError);
    connect(m_serial, &PdTermSerial::statusChanged,this, &PdTermMainTerminal::onSerialStatusChanged);

    // Configuração inicial padrão
    m_serial->setBaudRate(115200);  // Baud rate padrão
}

void PdTermMainTerminal::onSerialDataReceived(const QByteArray &data)
{
    if( flag_from_serial_write_to_terminal ){
        //appendTerminalText(QString::fromUtf8(data), Qt::green);
        if (ansiterm->processData(data) )
            writeTerminal(QString::fromUtf8(data));
    }
}

void PdTermMainTerminal::onSerialError(const QString &error)
{
    appendTerminalText("[ERRO SERIAL] " + error, Qt::red);
    statusBar()->showMessage(error, 5000);
}

void PdTermMainTerminal::onSerialStatusChanged(const QString &status)
{
    qDebug()<<"onSerialStatusChanged "<<status;
    if(status.contains("Conectado")){
        statusBar()->showMessage(status);
        updateSerialStatus(true);
    }else if(status.contains("Desconectado")){
        statusBar()->showMessage(status);
        updateSerialStatus(false);
    }
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

// Método para atualizar o status
void PdTermMainTerminal::updateSerialStatus(bool connected) {
    QPixmap led1(64, 64);
    led1.fill(Qt::transparent);

    QPainter painter(&led1);
    painter.setRenderHint(QPainter::Antialiasing);

    // Usa cores do tema do sistema
    QColor color;
    if (connected) {
        //color = palette().color(QPalette::Highlight); // Cor de destaque do tema
        color.setGreen(255);
        toolbarLed->setPixmap(QPixmap(":/icons/icons/led_green.png")); // .scaled(48, 48));
    } else {
        //color = palette().color(QPalette::Text); // Cor de texto do tema
        color.setRed(255);
        toolbarLed->setPixmap(QPixmap(":/icons/icons/led_red.png"));
    }

    //painter.setBrush(color);
    //painter.setPen(Qt::NoPen);
    //painter.drawEllipse(3, 3, 20, 20);

    //ui->led->setPixmap(led1);
   // ui->led->setToolTip(connected ? "Serial conectada" : "Serial desconectada");

}
//*******************************************************************************************
//*********************************END SERIAL************************************************
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

//Nova IMPLEMENTACAO
void PdTermMainTerminal::on_botaoEnviar_clicked()
{
    cleanupThread();
    // Abra o diálogo na thread principal
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Selecionar Arquivo"),
        QDir::homePath(),
        tr("Arquivos binários (*.bin);;Todos os arquivos (*)")
        );

    if (filePath.isEmpty()) {
        return;
    }

    // Mude para usar ações da toolbar
    ui->actionEnviarArquivo->setEnabled(false);
    ui->actionCancelarEnvio->setEnabled(true);

    // 2. Limpa qualquer thread/worker anterior
    if (m_workerThread && m_workerThread->isRunning()) {
        m_workerThread->quit();
        m_workerThread->wait();
        delete m_workerThread;
        m_workerThread = nullptr;
    }
    if (m_xmodemWorker) {
        delete m_xmodemWorker;
        m_xmodemWorker = nullptr;
    }

    // 3. Cria NOVOS objetos Thread e Worker
    m_workerThread = new QThread(this);
    m_xmodemWorker = new PdTermXmodem();

    // 4. Configure filePath ANTES de mover para thread
    m_xmodemWorker->setFilePath(filePath);

    // 5. Configure os callbacks
    m_xmodemWorker->recebe_dados_serial = [](void* context, int timeout_ms) -> QByteArray {
        PdTermSerial* serial = static_cast<PdTermSerial*>(context);
        return serial->waitForData(timeout_ms);
    };

    m_xmodemWorker->envia_dados_serial = [](void* context, const QByteArray& data) {
        PdTermSerial* serial = static_cast<PdTermSerial*>(context);
        serial->sendData(data);
    };

    m_xmodemWorker->set_envia_flag_serial = [](void* context, bool flag) {
        PdTermSerial* serial = static_cast<PdTermSerial*>(context);
        qDebug() << "Flag setada para:" << flag;
    };

    m_xmodemWorker->io_context = m_serial;

    // 6. Move o worker para a nova thread
    m_xmodemWorker->moveToThread(m_workerThread);

    // 7. Conecte os sinais de finalização
    connect(m_xmodemWorker, &PdTermXmodem::finished, m_workerThread, &QThread::quit);
    connect(m_xmodemWorker, &PdTermXmodem::finished, m_xmodemWorker, &PdTermXmodem::deleteLater);
    connect(m_workerThread, &QThread::finished, m_workerThread, &QThread::deleteLater);
    connect(m_workerThread, &QThread::finished, this, [this]() {
        m_workerThread = nullptr;
        ui->actionEnviarArquivo->setEnabled(true);
        ui->actionCancelarEnvio->setEnabled(false);
    });


    // 8. Conecte os sinais de status
    connect(m_xmodemWorker, &PdTermXmodem::transmissaoConcluida, this, &PdTermMainTerminal::onTransmissaoConcluida, Qt::QueuedConnection);
    connect(m_xmodemWorker, &PdTermXmodem::transmissaoCancelada, this, &PdTermMainTerminal::onTransmissaoCancelada, Qt::QueuedConnection);
    connect(m_xmodemWorker, &PdTermXmodem::erroOcorreu, this, &PdTermMainTerminal::onErroOcorreu, Qt::QueuedConnection);
    connect(m_xmodemWorker, &PdTermXmodem::progressoAtualizado, this, &PdTermMainTerminal::onProgressoAtualizado, Qt::QueuedConnection);

    // 9. Conecte o cancelamento
    // Conexão do botão Enviar (agora é ação)
    connect(ui->actionEnviarArquivo, &QAction::triggered,
            this, &PdTermMainTerminal::on_botaoEnviar_clicked);

    // Conexão do botão Cancelar (agora é ação)
    connect(ui->actionCancelarEnvio, &QAction::triggered, this, [this]() {
        if (m_xmodemWorker) {
            QMetaObject::invokeMethod(m_xmodemWorker, "cancelarTransmissao", Qt::QueuedConnection);
        }
    });

    // 9. Conecte o início da execução (APENAS UMA DESTAS DUAS OPÇÕES)
    // OPÇÃO 1: Usando invokeMethod (mais seguro)
    connect(m_workerThread, &QThread::started, this, [this, filePath]() {
        QMetaObject::invokeMethod(m_xmodemWorker, "enviarArquivoXmodem",
                                  Qt::QueuedConnection,
                                  Q_ARG(QString, filePath));
    });

    // 10. Conexão de INÍCIO - Esta é a que estava faltando!
    connect(m_workerThread, &QThread::started, m_xmodemWorker, [this]() {
        // Isso executa NA THREAD WORKER!
        m_xmodemWorker->enviarArquivoXmodem();
    });

    // 11. Inicia a Thread!
    m_workerThread->start();
    qDebug() << "Thread iniciada, aguardando started signal...";
}
// Implementações dos Slots da MainWindow
void PdTermMainTerminal::onTransmissaoConcluida() {
    qDebug() << "Transmissão concluída com sucesso!";
    statusBar()->showMessage("Transmissão concluída com sucesso!");
    // A reativação do botão é feita no sinal finished da thread
}

void PdTermMainTerminal::onTransmissaoCancelada() {
    qDebug() << "Transmissão cancelada.";
    statusBar()->showMessage("Transmissão cancelada.");
}

void PdTermMainTerminal::onErroOcorreu(const QString &mensagem) {
    qDebug() << "Erro na transmissão:" << mensagem;
    statusBar()->showMessage("Erro: " + mensagem);
    QMessageBox::critical(this, "Erro XMODEM", mensagem);
    // O sinal 'finished' também será emitido, limpando a UI.
}

void PdTermMainTerminal::onProgressoAtualizado(int porcentagem) {
    // Atualiza a barra de progresso na UI
    progressBar->setValue(porcentagem);
}

void PdTermMainTerminal::onWorkerFinished()
{
    // Este slot é chamado quando a thread do worker termina
    qDebug() << "Thread do worker finalizada";

    // Limpa os ponteiros (já devem ter sido deletados automaticamente pelas conexões)
    m_xmodemWorker = nullptr;
    m_workerThread = nullptr;

    // Reativa o botão de enviar (se necessário)
    ui->actionEnviarArquivo->setEnabled(true);
    ui->actionCancelarEnvio->setEnabled(false);

    // Opcional: atualiza status na UI
    statusBar()->showMessage("Transmissão finalizada");
    connect(m_workerThread, &QThread::finished, this, [this]() {
        m_workerThread = nullptr;
        ui->actionEnviarArquivo->setEnabled(true);
        ui->actionCancelarEnvio->setEnabled(false);
    });
}

void PdTermMainTerminal::onConectarClicked()
{

}
void PdTermMainTerminal::onDesconectarClicked()
{

}


void PdTermMainTerminal::cleanupThread()
{
    if (m_workerThread && m_workerThread->isRunning()) {
        // Primeiro, cancela a transmissão se estiver ativa
        if (m_xmodemWorker) {
            QMetaObject::invokeMethod(m_xmodemWorker, "cancelarTransmissao", Qt::BlockingQueuedConnection);
        }

        // Para a thread de forma segura
        m_workerThread->quit();
        if (!m_workerThread->wait(2000)) { // Timeout de 2 segundos
            qWarning() << "Thread não terminou a tempo, terminando forçadamente";
            m_workerThread->terminate();
            m_workerThread->wait();
        }
    }

    delete m_workerThread;
    m_workerThread = nullptr;

    // Não delete m_xmodemWorker - ele é gerenciado pela thread
    m_xmodemWorker = nullptr;
}
