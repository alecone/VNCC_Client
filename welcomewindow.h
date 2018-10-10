#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "logindialog.h"
#include "ui_directorytree.h"
#include <QProgressDialog>

namespace Ui {
class WelcomeWindow;
}

class WelcomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);
    ~WelcomeWindow();


public slots:
    void setSocket(QTcpSocket *mainSocket);
    void startUserSession(void);

private slots:
    void on_pushButton_clicked();
    void on_serverAddress_textChanged(const QString &arg1);
    void onConnectionSuccessfull();
    void fileSize();
    void downloadFileFromServer();
    void setTreeDirectory();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_downloadButton_clicked();

private:
    Ui::WelcomeWindow *ui;
    LogInDialog *logIn;
    Ui::TreeDir *treeDir;
    // Data Stream su cui il cliente potrà scrivere sul socket
    // usande il seguente comando: in.setDevice(tcpSocket);
    //                             in.setVersion(QDataStream::Qt_4_0);
    QDataStream in;
    QString addr;
    QTcpSocket *m_socket;
    QString selectedItem;
    QString selectedItemPath;
    qint64 lengthToDownload;
    qint64 lengthToUpload;
    const QString OK = "OK";
    const QString NOK = "NOK";
    const qint64 bufferSize = 1024;
    QProgressDialog *bar;
};

#endif // WELCOMEWINDOW_H
