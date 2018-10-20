#include "logindialog.h"
#include "ui_logindialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>

LogInDialog::LogInDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogInDialog)
{
    ui->setupUi(this);
    ui->loginButton->setDisabled(true);
    ui->registerButton->setDisabled(true);
}

LogInDialog::~LogInDialog()
{
    delete ui;
}

void LogInDialog::setSocket(QTcpSocket *mainSocket)
{
    m_socket = mainSocket;
//    connect(m_socket, SIGNAL(readyRead()), this, SLOT(getServerAns()), Qt::DirectConnection);
}
bool LogInDialog::checkDomain(QString user_and_pass)
{
    if(user_and_pass.split("|").at(0).split("@").at(1) == "studenti.unipg.it" ||
            user_and_pass.split("|").at(0).split("@").at(1) == "unipg.it")
        return true;
    else
        return false;
}
void LogInDialog::getServerAns()
{
    m_socket->waitForReadyRead();
    QByteArray read = m_socket->read(m_socket->bytesAvailable());
    qDebug() << "Risp form Server is: " << read;

    if(QString::fromStdString(read.toStdString()) == LINOK)
    {
        qDebug() << "Log In succesfully";
        emit authenticated();
        this->hide();
    }
    else if (QString::fromStdString(read.toStdString()) == REGOK) {
        ui->registerButton->setDisabled(true);
        ui->loginButton->setDisabled(true);
        QMessageBox::information(this, tr("Registration Done"),
                                 tr("Proceed with Log in"),
                                 QMessageBox::Ok);
        ui->passLine->clear();
        ui->userNameLine->clear();
        user_and_pass ="";
        qDebug() << "Registraton OOOK";
    }
    else if (QString::fromStdString(read.toStdString()) == LINNOK) {
        QMessageBox::warning(this, tr("Log In Error"),
                                       tr("User and/or password incorrect"),
                                       QMessageBox::Ok);
        qDebug() << "Log IN Error, unrecognized user and/or password?";
        ui->userNameLine->clear();
        ui->passLine->clear();
        ui->loginButton->setDisabled(true);
        ui->registerButton->setDisabled(true);
        user_and_pass ="";
    }
    else if (QString::fromStdString(read.toStdString()) == REGNOK) {
        QMessageBox::warning(this, tr("Registration Error"),
                                       tr("Incorrect domain"),
                                       QMessageBox::Ok);
        qDebug() << "Registration Error";
        ui->userNameLine->clear();
        ui->passLine->clear();
        ui->loginButton->setDisabled(true);
        ui->registerButton->setDisabled(true);
        user_and_pass ="";
    }
}

void LogInDialog::on_userNameLine_editingFinished()
{
    user_and_pass = ui->userNameLine->text();
    user_and_pass.append("|");
    ui->loginButton->setDisabled(false);
    ui->registerButton->setDisabled(false);
}

void LogInDialog::on_passLine_editingFinished()
{
    user_and_pass.append(ui->passLine->text());
    qDebug() << user_and_pass;
}

void LogInDialog::on_registerButton_clicked()
{
    ui->registerButton->setDisabled(true);
    if(!user_and_pass.split('@')[0].contains('.') || user_and_pass.split('@')[0].split('.').length() > 2)
    {
        QMessageBox::warning(this, tr("Registration Error"),
                                       tr("User Name must be: name.surname@"),
                                       QMessageBox::Ok);
        ui->userNameLine->clear();
        ui->passLine->clear();
        ui->loginButton->setDisabled(true);
        ui->registerButton->setDisabled(true);
        user_and_pass ="";
        return;
    }
    m_socket->write(REGISTER.toStdString().c_str(), REGISTER.length());
    m_socket->waitForReadyRead();
    QByteArray recv = m_socket->read(m_socket->bytesAvailable());
    QString risp = QString::fromStdString(recv.toStdString());
    qDebug() << "Resp from server is " << risp;
    if(risp != "OK")
        QMessageBox::warning(this, tr("Registration Error"),
                                       tr("Somithing is wrong"),
                                       QMessageBox::Ok);
    if(checkDomain(user_and_pass))
    {
        m_socket->write(user_and_pass.toStdString().c_str(), user_and_pass.length());
        this->getServerAns();
    }
    else
    {
        m_socket->write(user_and_pass.toStdString().c_str(), user_and_pass.length());
        qDebug() << "NOT valid unipg domain";
        this->getServerAns();
    }

}

void LogInDialog::on_loginButton_clicked()
{
    ui->loginButton->setDisabled(true);
    m_socket->write(LIN.toStdString().c_str(), LIN.length());
    m_socket->waitForReadyRead();
    QByteArray recv = m_socket->read(m_socket->bytesAvailable());
    QString risp = QString::fromStdString(recv.toStdString());
    qDebug() << "Resp from server is " << risp;
    if(risp != "OK")
        exit(0);
    m_socket->write(user_and_pass.toStdString().c_str(), user_and_pass.length());
    this->getServerAns();
}

void LogInDialog::on_cancelButton_clicked()
{
    ui->userNameLine->clear();
    ui->passLine->clear();
    ui->loginButton->setDisabled(true);
    ui->registerButton->setDisabled(true);
    user_and_pass ="";
}

void LogInDialog::reject()
{
    QMessageBox::StandardButton resBtn = QMessageBox::Yes;
//    if (changes) {
        resBtn = QMessageBox::question( this, "VNNC",
                                        tr("Are you sure?\n"),
                                        QMessageBox::No | QMessageBox::Yes);
//    }
    if (resBtn == QMessageBox::Yes) {
        emit doItAgain();
        QDialog::reject();
    }
}
