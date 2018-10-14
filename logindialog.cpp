#include "logindialog.h"
#include "ui_logindialog.h"
#include <QDebug>

LogInDialog::LogInDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogInDialog)
{
    ui->setupUi(this);
}

LogInDialog::~LogInDialog()
{
    delete ui;
}

void LogInDialog::setSocket(QTcpSocket *mainSocket)
{
    m_socket = mainSocket;
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(getServerAns()), Qt::DirectConnection);
}
//void LogInDialog::on_buttonBox_accepted()
//{
//    // Talk with the server
//    m_socket->write(user_and_pass.toStdString().c_str(), user_and_pass.length());
//}

void LogInDialog::getServerAns()
{
    QByteArray read = m_socket->read(m_socket->bytesAvailable());
    qDebug() << "Risp form Server is: " << read;
    if(read.size() == 1)
    {
        int ans = (int)read.at(0);
        if(ans == 1)
        {
            m_socket->disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(getServerAns()));
            emit authenticated();
        }
        else
        {
            ui->label->clear();
            ui->label_2->clear();
            qDebug() << "Unrecognized user and/or password";
        }

    }
}

void LogInDialog::on_userNameLine_editingFinished()
{
    user_and_pass = ui->userNameLine->text();
    user_and_pass.append("|");
}

void LogInDialog::on_passLine_editingFinished()
{
    user_and_pass.append(ui->passLine->text());
}

void LogInDialog::on_registerButton_clicked()
{

}

void LogInDialog::on_loginButton_clicked()
{

}

void LogInDialog::on_cancelButton_clicked()
{
    this->hide();
}
