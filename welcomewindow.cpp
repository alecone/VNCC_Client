#include "welcomewindow.h"
#include "ui_welcomewindow.h"
#include <QDebug>
#include <QFile>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QFileDialog>
#include <QMessageBox>

WelcomeWindow::WelcomeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WelcomeWindow)
{
    ui->setupUi(this);
}

WelcomeWindow::~WelcomeWindow()
{
    delete ui;
}

void WelcomeWindow::setSocket(QTcpSocket *mainSocket)
{
    m_socket = mainSocket;
    connect(m_socket, SIGNAL(connected()), this, SLOT(onConnectionSuccessfull()));
}
void WelcomeWindow::on_pushButton_clicked()
{
    ui->pushButton->setText("Connecting");
    ui->pushButton->setDisabled(true);
    qDebug() <<"Connecting to " << addr;
    m_socket->connectToHost(addr, 9001, QIODevice::ReadWrite, QAbstractSocket::IPv4Protocol);
}

void WelcomeWindow::on_serverAddress_textChanged(const QString &arg1)
{
    addr = arg1;
}

void WelcomeWindow::onConnectionSuccessfull()
{
    qDebug() << "Connected to server";
    ui->pushButton->setText("Connected");
    logIn = new LogInDialog(this);

    connect(logIn, SIGNAL(authenticated()), this, SLOT(startUserSession()));

    logIn->setSocket(m_socket);
    logIn->show();
}

void WelcomeWindow::startUserSession()
{
    QString get = "GET";
    // Ready to read from socket
    ui->pushButton->hide();
    ui->serverAddress->hide();
    ui->title->hide();

    treeDir = new Ui::TreeDir();
    treeDir->setupUi(this);
    treeDir->treeWidget->show();
    treeDir->removeButton->show();
    treeDir->downloadButton->show();
    treeDir->uploadButton->show();
    treeDir->treeWidget->setSizePolicy(ui->centralWidget->sizePolicy());
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(setTreeDirectory()), Qt::DirectConnection);
    m_socket->write(get.toStdString().c_str(), get.length());
}

void setIcon(QTreeWidgetItem *child, QString name)
{
    QString ext = name.split(".").at(1).toLower();
    if(ext == "mp3" || ext == "m4a" || ext == "flac" || ext == "wav")
        child->setIcon(0, QIcon(":/images/music.png"));
    else if(ext == "mp4" || ext == "mov")
        child->setIcon(0, QIcon(":/images/clip.png"));
    else if(ext == "png" || ext == "jpg" || ext == "jpeg")
        child->setIcon(0, QIcon(":/images/pic.png"));
    else
        child->setIcon(0, QIcon(":/images/file.png"));
}

void setTree(QJsonObject obJson, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *child = new QTreeWidgetItem();
    if(obJson["type"].toString() == "file")
    {
        child->setData(0, 0, obJson.toVariantMap()["name"]);
        parent->addChild(child);
        setIcon(child, obJson["name"].toString());
    }
    else
    {
        child->setData(0, 0, obJson.toVariantMap()["name"]);
        child->setIcon(0, QIcon(":/images/folder.png"));
        parent->addChild(child);
        QJsonArray array = obJson["children"].toArray();
        foreach(auto x , array)
            setTree(x.toObject(), child);
    }
}
void WelcomeWindow::setTreeDirectory()
{
    disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(setTreeDirectory()));
    QByteArray read = m_socket->read(m_socket->bytesAvailable());

    QJsonParseError jsonError;
    QJsonDocument readJson = QJsonDocument::fromJson(read,&jsonError);
    if (jsonError.error != QJsonParseError::NoError){
    qDebug() << jsonError.errorString();
    }
    QJsonObject obJson = readJson.object();

    QTreeWidgetItem* parent = new QTreeWidgetItem();
    parent->setText(0, "Your Cloud");
    parent->setIcon(0, QIcon(":/images/folder.png"));
    setTree(obJson, parent);
    treeDir->treeWidget->addTopLevelItem(parent);

}

void WelcomeWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    treeDir->uploadButton->setDisabled(false);
    treeDir->removeButton->setDisabled(true);
    treeDir->downloadButton->setDisabled(true);
    selectedItem = item->text(column);
    if(selectedItem.contains("."))
    {
        treeDir->uploadButton->setDisabled(true);
        treeDir->removeButton->setDisabled(false);
        treeDir->downloadButton->setDisabled(false);
    }
}

void WelcomeWindow::on_downloadButton_clicked()
{
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(fileSize()), Qt::DirectConnection);
    QString dialog = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    QDir::homePath() +"/"+ selectedItem, NULL, NULL, QFileDialog::ShowDirsOnly);
    if(dialog.split("/").last() == "")
        return;
    if(dialog.split("/").last() == selectedItem)
        selectedItemPath = dialog;
    else
    {
        dialog = dialog.split("/").last();
        if(dialog.contains("."))
            dialog = dialog.split(".").first();
        selectedItemPath = dialog + "." + selectedItem.split(".").last();

    }
    qDebug() << selectedItem;
    m_socket->write(selectedItem.toStdString().c_str(), selectedItem.length());
    treeDir->downloadButton->setDisabled(true);
    treeDir->treeWidget->setDisabled(true);
}

QString correctFile(QString fileName)
{
    if(!(QFile::exists(fileName)))
       return fileName;
    else
    {
        QString ret;
        ret = fileName.split("/").last().split(".")[0];
        ret = ret + "_copy." + fileName.split("/").last().split(".")[1];
        return correctFile(ret);
    }
}

void WelcomeWindow::fileSize()
{
    disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(fileSize()));

    selectedItemPath = correctFile(selectedItemPath);
    qDebug() << "Creating " << selectedItemPath;
    QFile file(selectedItemPath);
    if(!(file.open(QIODevice::NewOnly)))
    {
        qDebug("File cannot be opened");
        exit(0);
    }

    QByteArray read = m_socket->read(bufferSize);
    QString temp(read);
    lengthToDownload = read.toInt();
    bar = new QProgressDialog();
    bar->setMaximum(lengthToDownload);
    bar->setWindowModality(Qt::WindowModal);
    bar->show();
//    foreach (auto x, read) {
//        qDebug() << "array: " << x;
//    }
    // Send OK_TO_GO
    m_socket->write(OK.toStdString().c_str(), OK.length());
    this->downloadFileFromServer();
}

void WelcomeWindow::downloadFileFromServer()
{
    qDebug() << "Opening " << selectedItemPath;
    QFile file(selectedItemPath);
    if(!(file.open(QIODevice::Append)))
    {
        qDebug("File cannot be opened");
        exit(0);
    }
    int tot = (int)lengthToDownload;
    QByteArray read;
    while(lengthToDownload > 0)
    {
        m_socket->waitForReadyRead();
        do {
            bar->setValue(tot-(int)lengthToDownload);
            read = m_socket->read(bufferSize);
            file.write(read);
            lengthToDownload -= read.size();
        } while (read.size() != 0);
    }
    bar->close();
    file.close();
    treeDir->downloadButton->setDisabled(false);
    treeDir->treeWidget->setDisabled(false);
}

void WelcomeWindow::on_uploadButton_clicked()
{
    QString dialog = QFileDialog::getOpenFileName(this, tr("Save File"),
                                                    QDir::homePath());
    dialog = dialog.split("/").last();
    if(dialog == "")
    {
        QMessageBox::warning(this, tr("Upload on your Cloud"),
                                       tr("A folder has been selected.\n"
                                          "Sorry, this is not supported yet :("),
                                       QMessageBox::Ok);
        return;
    }

    qDebug() << selectedItem;
    m_socket->write(selectedItem.toStdString().c_str(), selectedItem.length());
    treeDir->downloadButton->setDisabled(true);
    treeDir->treeWidget->setDisabled(true);
}
