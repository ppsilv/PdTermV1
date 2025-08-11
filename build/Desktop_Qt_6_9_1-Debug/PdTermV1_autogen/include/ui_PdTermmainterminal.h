/********************************************************************************
** Form generated from reading UI file 'PdTermmainterminal.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PDTERMMAINTERMINAL_H
#define UI_PDTERMMAINTERMINAL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PdTermMainTerminal
{
public:
    QAction *actionExit;
    QAction *action_Exit;
    QAction *actionPdTermPro_V1;
    QAction *actionDesenvolvido_por_PdSilva;
    QAction *actionLicen_a_GPLv3;
    QAction *actiongithub_com_ppsilv_pdtermpro;
    QAction *actionEnviar;
    QAction *action_Exit_2;
    QAction *action_Limpar;
    QAction *action_Enviar_arquivo;
    QAction *actionConectar;
    QAction *actionDesconectar;
    QAction *actionteste;
    QAction *actiontsdfasd;
    QAction *actionConfigura_o;
    QWidget *centralwidget;
    QPlainTextEdit *plainTextEdit;
    QMenuBar *menubar;
    QMenu *menuConectar;
    QMenu *menuSobre;
    QMenu *menuLimpar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *PdTermMainTerminal)
    {
        if (PdTermMainTerminal->objectName().isEmpty())
            PdTermMainTerminal->setObjectName("PdTermMainTerminal");
        PdTermMainTerminal->resize(800, 600);
        actionExit = new QAction(PdTermMainTerminal);
        actionExit->setObjectName("actionExit");
        actionExit->setMenuRole(QAction::MenuRole::NoRole);
        action_Exit = new QAction(PdTermMainTerminal);
        action_Exit->setObjectName("action_Exit");
        actionPdTermPro_V1 = new QAction(PdTermMainTerminal);
        actionPdTermPro_V1->setObjectName("actionPdTermPro_V1");
        actionDesenvolvido_por_PdSilva = new QAction(PdTermMainTerminal);
        actionDesenvolvido_por_PdSilva->setObjectName("actionDesenvolvido_por_PdSilva");
        actionLicen_a_GPLv3 = new QAction(PdTermMainTerminal);
        actionLicen_a_GPLv3->setObjectName("actionLicen_a_GPLv3");
        actiongithub_com_ppsilv_pdtermpro = new QAction(PdTermMainTerminal);
        actiongithub_com_ppsilv_pdtermpro->setObjectName("actiongithub_com_ppsilv_pdtermpro");
        actionEnviar = new QAction(PdTermMainTerminal);
        actionEnviar->setObjectName("actionEnviar");
        action_Exit_2 = new QAction(PdTermMainTerminal);
        action_Exit_2->setObjectName("action_Exit_2");
        action_Limpar = new QAction(PdTermMainTerminal);
        action_Limpar->setObjectName("action_Limpar");
        action_Enviar_arquivo = new QAction(PdTermMainTerminal);
        action_Enviar_arquivo->setObjectName("action_Enviar_arquivo");
        actionConectar = new QAction(PdTermMainTerminal);
        actionConectar->setObjectName("actionConectar");
        actionDesconectar = new QAction(PdTermMainTerminal);
        actionDesconectar->setObjectName("actionDesconectar");
        actionteste = new QAction(PdTermMainTerminal);
        actionteste->setObjectName("actionteste");
        actiontsdfasd = new QAction(PdTermMainTerminal);
        actiontsdfasd->setObjectName("actiontsdfasd");
        actionConfigura_o = new QAction(PdTermMainTerminal);
        actionConfigura_o->setObjectName("actionConfigura_o");
        centralwidget = new QWidget(PdTermMainTerminal);
        centralwidget->setObjectName("centralwidget");
        plainTextEdit = new QPlainTextEdit(centralwidget);
        plainTextEdit->setObjectName("plainTextEdit");
        plainTextEdit->setGeometry(QRect(0, 0, 801, 551));
        plainTextEdit->setAutoFillBackground(true);
        plainTextEdit->setStyleSheet(QString::fromUtf8("QPlainTextEdit {\n"
"    background-color: black;   /* Fundo preto */\n"
"    color: lime;              /* Letra verde vibrante */\n"
"    font-family: Consolas;    /* Fonte monoespa\303\247ada (opcional) */\n"
"    font-size: 12pt;          /* Tamanho da fonte (opcional) */\n"
"}"));
        plainTextEdit->setBackgroundVisible(true);
        PdTermMainTerminal->setCentralWidget(centralwidget);
        menubar = new QMenuBar(PdTermMainTerminal);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 23));
        menuConectar = new QMenu(menubar);
        menuConectar->setObjectName("menuConectar");
        menuSobre = new QMenu(menubar);
        menuSobre->setObjectName("menuSobre");
        menuLimpar = new QMenu(menubar);
        menuLimpar->setObjectName("menuLimpar");
        PdTermMainTerminal->setMenuBar(menubar);
        statusbar = new QStatusBar(PdTermMainTerminal);
        statusbar->setObjectName("statusbar");
        PdTermMainTerminal->setStatusBar(statusbar);

        menubar->addAction(menuConectar->menuAction());
        menubar->addAction(menuLimpar->menuAction());
        menubar->addAction(menuSobre->menuAction());
        menuConectar->addAction(actionConectar);
        menuConectar->addAction(actionDesconectar);
        menuConectar->addAction(actionConfigura_o);
        menuSobre->addSeparator();
        menuSobre->addSeparator();
        menuSobre->addAction(actionteste);
        menuSobre->addSeparator();
        menuSobre->addAction(actionPdTermPro_V1);
        menuSobre->addAction(actionDesenvolvido_por_PdSilva);
        menuSobre->addAction(actionLicen_a_GPLv3);
        menuSobre->addAction(actiongithub_com_ppsilv_pdtermpro);
        menuSobre->addSeparator();
        menuSobre->addAction(action_Exit_2);
        menuLimpar->addAction(action_Limpar);
        menuLimpar->addAction(action_Enviar_arquivo);

        retranslateUi(PdTermMainTerminal);

        QMetaObject::connectSlotsByName(PdTermMainTerminal);
    } // setupUi

    void retranslateUi(QMainWindow *PdTermMainTerminal)
    {
        PdTermMainTerminal->setWindowTitle(QCoreApplication::translate("PdTermMainTerminal", "PdTermMainTerminal", nullptr));
        actionExit->setText(QCoreApplication::translate("PdTermMainTerminal", "Exit", nullptr));
        action_Exit->setText(QCoreApplication::translate("PdTermMainTerminal", "&Exit", nullptr));
        actionPdTermPro_V1->setText(QCoreApplication::translate("PdTermMainTerminal", "PdTermPro V1", nullptr));
        actionDesenvolvido_por_PdSilva->setText(QCoreApplication::translate("PdTermMainTerminal", "Desenvolvido por PdSilva", nullptr));
        actionLicen_a_GPLv3->setText(QCoreApplication::translate("PdTermMainTerminal", "Licen\303\247a: GPLv3", nullptr));
        actiongithub_com_ppsilv_pdtermpro->setText(QCoreApplication::translate("PdTermMainTerminal", "github.com/ppsilv/pdtermpro", nullptr));
        actionEnviar->setText(QCoreApplication::translate("PdTermMainTerminal", "Enviar", nullptr));
        action_Exit_2->setText(QCoreApplication::translate("PdTermMainTerminal", "&Exit", nullptr));
#if QT_CONFIG(shortcut)
        action_Exit_2->setShortcut(QCoreApplication::translate("PdTermMainTerminal", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        action_Limpar->setText(QCoreApplication::translate("PdTermMainTerminal", "&Limpar           Ctrl+L", nullptr));
        action_Enviar_arquivo->setText(QCoreApplication::translate("PdTermMainTerminal", "&Enviar arquivo   Ctrl+S", nullptr));
        actionConectar->setText(QCoreApplication::translate("PdTermMainTerminal", "Conectar           Ctrl+Y", nullptr));
        actionDesconectar->setText(QCoreApplication::translate("PdTermMainTerminal", "Desconectar    Ctrl+U", nullptr));
        actionteste->setText(QCoreApplication::translate("PdTermMainTerminal", "-", nullptr));
        actiontsdfasd->setText(QCoreApplication::translate("PdTermMainTerminal", "tsdfasd", nullptr));
        actionConfigura_o->setText(QCoreApplication::translate("PdTermMainTerminal", "Configura\303\247\303\243o", nullptr));
        menuConectar->setTitle(QCoreApplication::translate("PdTermMainTerminal", "Conex\303\243o", nullptr));
        menuSobre->setTitle(QCoreApplication::translate("PdTermMainTerminal", "Sobre", nullptr));
        menuLimpar->setTitle(QCoreApplication::translate("PdTermMainTerminal", "Ferramentas", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PdTermMainTerminal: public Ui_PdTermMainTerminal {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PDTERMMAINTERMINAL_H
