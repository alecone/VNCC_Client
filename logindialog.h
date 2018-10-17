#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class LogInDialog;
}

class LogInDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogInDialog(QWidget *parent = nullptr);
    ~LogInDialog();

public slots:
    void setSocket(QTcpSocket *mainSocket);

signals:
    void authenticated(void);
    void doItAgain(void);

private slots:
    bool checkDomain(QString user_and_pass);
    void on_userNameLine_editingFinished();
    void on_passLine_editingFinished();
    void getServerAns();
    void on_registerButton_clicked();
    void on_loginButton_clicked();
    void on_cancelButton_clicked();
    void reject();

private:
    Ui::LogInDialog *ui;
    QString user_and_pass;
    QTcpSocket *m_socket;
    const QString REGISTER = "REGISTER";
    const QString LIN = "LOGIN";
    const QString LINOK = "LOGIN_OK";
    const QString LINNOK = "LOGIN_NOK";
    const QString REG = "REGISTER";
    const QString REGOK = "REGISTER_OK";
    const QString REGNOK = "REGISTER_NOK";
    const QString LOUT = "LOGOUT";
    const QString OK = "OK";
    const QString NOK = "NOK";

};

#endif // LOGINDIALOG_H
