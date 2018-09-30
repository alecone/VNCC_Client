#include "welcomewindow.h"
#include "ui_welcomewindow.h"
#include <QDebug>

WelcomeWindow::WelcomeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WelcomeWindow)
{
    ui->setupUi(this);
    // TODO: Implement here all signal and slot connection
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
}

void WelcomeWindow::on_serverAddress_textChanged(const QString &arg1)
{
    QString addr;
    addr = arg1;
    qDebug() << "Text changed: " << arg1;
}

void WelcomeWindow::on_serverAddress_returnPressed()
{
    qDebug() << "Final address chosen";
}
