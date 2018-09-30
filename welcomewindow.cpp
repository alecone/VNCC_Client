#include "welcomewindow.h"
#include "ui_welcomewindow.h"
#include <QDebug>
#include <QFile>

WelcomeWindow::WelcomeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WelcomeWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket();
    // TODO: Implement here all signal and slot connection
    connect(socket, SIGNAL(connected()), this, SLOT(onConnectionSuccessfull()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()), Qt::DirectConnection);
}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}

void WelcomeWindow::on_pushButton_clicked()
{
    //TODO: Implement here connection
    qDebug() <<"Button clicked";
    ui->pushButton->setText("Connecting");
    qDebug() <<"Connecting to " << addr;
    socket->connectToHost(addr, 9001, QIODevice::ReadWrite, QAbstractSocket::IPv4Protocol);

}

void WelcomeWindow::on_serverAddress_textChanged(const QString &arg1)
{
    addr = arg1;
    qDebug() << "Text changed: " << arg1;
}

void WelcomeWindow::onConnectionSuccessfull()
{
    qDebug() << "Connected to server";
    qDebug() << "Socket: " << socket->socketDescriptor();
}

void WelcomeWindow::onReadyRead()
{
    qDebug() << "Something is here, ready to read";
    QFile file("fromServer.txt");
    if(!(file.open(QIODevice::Append)))
    {
        qDebug("File cannot be opened.");
        exit(0);
    }
    QByteArray read = socket->read(socket->bytesAvailable());
    qDebug() << "Read    : " << read.size();
    file.write(read);
    file.close();

}
