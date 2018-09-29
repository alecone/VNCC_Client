/********************************************************************************
** Form generated from reading UI file 'welcomewindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WELCOMEWINDOW_H
#define UI_WELCOMEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WelcomeWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QLineEdit *serverAddress;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *WelcomeWindow)
    {
        if (WelcomeWindow->objectName().isEmpty())
            WelcomeWindow->setObjectName(QStringLiteral("WelcomeWindow"));
        WelcomeWindow->resize(642, 411);
        WelcomeWindow->setWindowOpacity(0.95);
        centralWidget = new QWidget(WelcomeWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(270, 230, 93, 28));
        serverAddress = new QLineEdit(centralWidget);
        serverAddress->setObjectName(QStringLiteral("serverAddress"));
        serverAddress->setGeometry(QRect(170, 190, 291, 22));
        serverAddress->setCursor(QCursor(Qt::ArrowCursor));
        serverAddress->setMaxLength(15);
        serverAddress->setAlignment(Qt::AlignCenter);
        serverAddress->setReadOnly(false);
        serverAddress->setClearButtonEnabled(true);
        WelcomeWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(WelcomeWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 642, 26));
        WelcomeWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(WelcomeWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        WelcomeWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(WelcomeWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        WelcomeWindow->setStatusBar(statusBar);

        retranslateUi(WelcomeWindow);

        QMetaObject::connectSlotsByName(WelcomeWindow);
    } // setupUi

    void retranslateUi(QMainWindow *WelcomeWindow)
    {
        WelcomeWindow->setWindowTitle(QApplication::translate("WelcomeWindow", "UniPg Cloud", nullptr));
        pushButton->setText(QApplication::translate("WelcomeWindow", "Connect", nullptr));
        serverAddress->setText(QApplication::translate("WelcomeWindow", "127.0.0.1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WelcomeWindow: public Ui_WelcomeWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WELCOMEWINDOW_H
