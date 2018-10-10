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

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_userNameLine_editingFinished();
    void on_passLine_editingFinished();
    void getServerAns();

private:
    Ui::LogInDialog *ui;
    QString user_and_pass;
    QTcpSocket *m_socket;
};

#endif // LOGINDIALOG_H
