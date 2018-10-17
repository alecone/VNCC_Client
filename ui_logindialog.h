/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_LogInDialog
{
public:
    QPushButton *registerButton;
    QPushButton *loginButton;
    QPushButton *cancelButton;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *userNameLine;
    QLineEdit *passLine;

    void setupUi(QDialog *LogInDialog)
    {
        if (LogInDialog->objectName().isEmpty())
            LogInDialog->setObjectName(QStringLiteral("LogInDialog"));
        LogInDialog->resize(400, 300);
        cancelButton = new QPushButton(LogInDialog);
        cancelButton->setObjectName("cancelButton");
        cancelButton->setText("Cancel");
        cancelButton->setGeometry(290, 230, 90, 25);
        loginButton = new QPushButton(LogInDialog);
        loginButton->setObjectName("loginButton");
        loginButton->setText("Log In");
        loginButton->setGeometry(156, 230, 90, 25);
        registerButton = new QPushButton(LogInDialog);
        registerButton->setObjectName("registerButton");
        registerButton->setText("Register");
        registerButton->setGeometry(20, 230, 90, 25);
        label = new QLabel(LogInDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 100, 81, 16));
        label_2 = new QLabel(LogInDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 140, 81, 16));
        userNameLine = new QLineEdit(LogInDialog);
        userNameLine->setObjectName(QStringLiteral("userNameLine"));
        userNameLine->setGeometry(QRect(130, 100, 241, 22));
        passLine = new QLineEdit(LogInDialog);
        passLine->setObjectName(QStringLiteral("passLine"));
        passLine->setGeometry(QRect(130, 140, 241, 22));
        passLine->setEchoMode(QLineEdit::Password);

        retranslateUi(LogInDialog);

        QMetaObject::connectSlotsByName(LogInDialog);
    } // setupUi

    void retranslateUi(QDialog *LogInDialog)
    {
        LogInDialog->setWindowTitle(QApplication::translate("LogInDialog", "Dialog", nullptr));
        label->setText(QApplication::translate("LogInDialog", "User Name   ", nullptr));
        label_2->setText(QApplication::translate("LogInDialog", "Password    ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LogInDialog: public Ui_LogInDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
