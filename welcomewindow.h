#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
class WelcomeWindow;
}

class WelcomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);
    ~WelcomeWindow();

private slots:
    void on_pushButton_clicked();
    void on_serverAddress_textChanged(const QString &arg1);
    void onConnectionSuccessfull();
    void onReadyRead();

private:
    Ui::WelcomeWindow *ui;
    QTcpSocket *socket;
    // Data Stream su cui il cliente potrà scrivere sul socket
    // usande il seguente comando: in.setDevice(tcpSocket);
    //                             in.setVersion(QDataStream::Qt_4_0);
    QDataStream in;
    QString addr;
};

#endif // WELCOMEWINDOW_H
