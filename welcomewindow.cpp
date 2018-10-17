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
#include <QCloseEvent>


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
    connect(logIn, SIGNAL(doItAgain()), this, SLOT(retryConnection()));

    logIn->setSocket(m_socket);
    logIn->show();
}

void WelcomeWindow::startUserSession()
{
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
    treeDir->newDir->show();
    treeDir->treeWidget->setSizePolicy(ui->centralWidget->sizePolicy());
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(setTreeDirectory()), Qt::DirectConnection);
    m_socket->write(TREE.toStdString().c_str(), TREE.length());
}
void WelcomeWindow::retryConnection()
{
    m_socket->write(DISCONNECT.toStdString().c_str(), DISCONNECT.length());
    m_socket->disconnectFromHost();
    ui->pushButton->setText("Connect");
    ui->pushButton->setDisabled(false);
    qDebug() <<"Connection aborted ";
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
    QByteArray read = m_socket->read(m_socket->bytesAvailable());
    disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(setTreeDirectory()));
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
    selectedItem = item->text(column);

    if(selectedItem.contains("."))
    {
        treeDir->uploadButton->setDisabled(true);
        treeDir->removeButton->setDisabled(false);
        treeDir->newDir->setDisabled(true);
        treeDir->downloadButton->setDisabled(false);
    }else if (selectedItem == "Your Cloud")
    {
        treeDir->uploadButton->setDisabled(true);
        treeDir->removeButton->setDisabled(true);
        treeDir->newDir->setDisabled(true);
        treeDir->downloadButton->setDisabled(true);
    }else{
        treeDir->uploadButton->setDisabled(false);
        treeDir->removeButton->setDisabled(false);
        treeDir->downloadButton->setDisabled(true);
        treeDir->newDir->setDisabled(false);
    }
}

void WelcomeWindow::on_downloadButton_clicked()
{
    m_socket->write(DW.toStdString().c_str(), DW.length());
    m_socket->waitForReadyRead();
    QByteArray risp = m_socket->read(m_socket->bytesAvailable());
    QString resp(risp);
    if(resp == OK)
        qDebug() << "OK to go to download";
    else
    {
        qDebug() << "Somethig is wrong";
        exit(0);
    }
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
        for(int i=0; i<fileName.split('/').length(); i++)
        {
            if(i == fileName.split('/').length())
            {
                ret += fileName.split("/").last().split(".")[0];
                ret += "_copy." + fileName.split("/").last().split(".")[1];
            }
            ret = fileName.split('/').at(i) + '/';
        }
        return correctFile(ret);
    }
}

void WelcomeWindow::fileSize()
{
    disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(fileSize()));

    selectedItemPath = correctFile(selectedItemPath);
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
    // Send OK_TO_GO
    m_socket->write(OK.toStdString().c_str(), OK.length());
    this->downloadFileFromServer();
}

void WelcomeWindow::downloadFileFromServer()
{
    QFile file(selectedItemPath);
    if(!(file.open(QIODevice::Append)))
    {
        qDebug("File cannot be opened");
        exit(0);
    }
    int tot = lengthToDownload;
    QByteArray read;
    while(lengthToDownload > 0)
    {
        m_socket->waitForReadyRead();
        do {
            bar->setValue(tot-lengthToDownload);
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
    m_socket->write(UP.toStdString().c_str(), UP.length());
    m_socket->waitForReadyRead();
    QByteArray risp = m_socket->read(m_socket->bytesAvailable());
    QString resp(risp);
    if(resp == OK)
        qDebug() << "OK to go to upload";
    else
    {
        qDebug() << "Somethig is wrong";
        exit(0);
    }
    QString path = QFileDialog::getOpenFileName(this, tr("Choose File"),
                                                    QDir::homePath());
    QString nameToUp = path.split("/").last();
    if(nameToUp == "")
    {
        QMessageBox::warning(this, tr("Upload on your Cloud"),
                                       tr("A folder has been selected.\n"
                                          "Sorry, this is not supported yet :("),
                                       QMessageBox::Ok);
        return;
    }

    qDebug() << selectedItem; // Path on server
    qDebug() << nameToUp; // Name of file
    treeDir->downloadButton->setDisabled(true);
    treeDir->removeButton->setDisabled(true);
    treeDir->uploadButton->setDisabled(true);
    treeDir->treeWidget->setDisabled(true);

    m_socket->write(selectedItem.toStdString().c_str(), selectedItem.length());
    m_socket->waitForReadyRead();
    risp = m_socket->read(m_socket->bytesAvailable());
    QString resp2(risp);
    if(resp2 == OK)
        qDebug() << "OK sent path on server";
    else
    {
        qDebug() << "Somethig is wrong";
        exit(0);
    }
    m_socket->write(nameToUp.toStdString().c_str(), nameToUp.length());
    m_socket->waitForReadyRead();
    risp = m_socket->read(m_socket->bytesAvailable());
    QString resp4(risp);
    if(resp4 == OK)
        qDebug() << "OK sent file name";
    else
    {
        qDebug() << "Somethig is wrong";
        exit(0);
    }

    QFile upFile(path);
    if (upFile.open(QIODevice::ReadOnly)){
        lengthToUpload = upFile.size();  //when file does open.
        upFile.close();
    }
    QString length = QString::number(lengthToUpload);
    m_socket->write(length.toStdString().c_str(), length.length());
    m_socket->waitForReadyRead();
    risp = m_socket->read(m_socket->bytesAvailable());
    QString resp3(risp);
    if(resp3 == OK)
        qDebug() << "OK sent length";
    else
    {
        qDebug() << "Somethig is wrong";
        exit(0);
    }

    if(!(upFile.open(QIODevice::ReadOnly)))
    {
        qDebug() << "Cannot open file";
        exit(0);
    }
    bar = new QProgressDialog();
    bar->setMaximum(lengthToUpload);
    bar->setWindowModality(Qt::WindowModal);
    bar->show();
    int tot = lengthToUpload;
    QByteArray read;
    while(lengthToUpload > 0)
    {
        bar->setValue(tot-lengthToUpload);
        read = upFile.read(bufferSize);
        lengthToUpload -= read.length();
        m_socket->write(read);
    }
    bar->close();
    upFile.close();
    treeDir->treeWidget->setDisabled(false);
    // Check OK
    m_socket->waitForReadyRead();
    risp = m_socket->read(m_socket->bytesAvailable());
    QString resp5(risp);
    if(resp5 == OK)
    {
        // Send Tree Command in order to update
        connect(m_socket, SIGNAL(readyRead()), this, SLOT(setTreeDirectory()), Qt::DirectConnection);
        treeDir->treeWidget->clear();
        m_socket->write(TREE.toStdString().c_str(), TREE.length());
    }
    else
    {
        QMessageBox::warning(this, tr("ERROR"),
                                       tr("Upload failed"),
                                       QMessageBox::Ok);
    }
}


void WelcomeWindow::on_removeButton_clicked()
{
    m_socket->write(RM.toStdString().c_str(), RM.length());
    m_socket->waitForReadyRead();
    QByteArray risp = m_socket->read(m_socket->bytesAvailable());
    QString resp(risp);
    if(resp == OK)
        qDebug() << "OK to go to upload";
    else
    {
        qDebug() << "Somethig is wrong";
        exit(0);
    }

    qDebug() << selectedItem; // File to remove

    treeDir->downloadButton->setDisabled(true);
    treeDir->uploadButton->setDisabled(true);
    treeDir->treeWidget->setDisabled(true);
    treeDir->removeButton->setDisabled(true);

    m_socket->write(selectedItem.toStdString().c_str(), selectedItem.length());
    m_socket->waitForReadyRead();
    risp = m_socket->read(m_socket->bytesAvailable());
    QString final(risp);
    if(final == OK)
    {
        qDebug() << "OK ask tree update";
        connect(m_socket, SIGNAL(readyRead()), this, SLOT(setTreeDirectory()), Qt::DirectConnection);
        treeDir->treeWidget->clear();
        m_socket->write(TREE.toStdString().c_str(), TREE.length());

    }
    else
    {
        qDebug() << "Somethig is wrong";
        QMessageBox::warning(this, tr("ERROR"),
                                       tr("Remove file failed"),
                                       QMessageBox::Ok);
    }
    treeDir->treeWidget->setDisabled(false);
}

void WelcomeWindow::on_newDir_clicked()
{
    m_socket->write(ND.toStdString().c_str(), ND.length());
    m_socket->waitForReadyRead();
    QByteArray risp = m_socket->read(m_socket->bytesAvailable());
    QString resp(risp);
    if(resp == OK)
        qDebug() << "OK to go to new file";
    else
    {
        qDebug() << "Somethig is wrong";
        exit(0);
    }

    qDebug() << selectedItem; // Dir where to create new dir

    treeDir->downloadButton->setDisabled(true);
    treeDir->uploadButton->setDisabled(true);
    treeDir->treeWidget->setDisabled(true);
    treeDir->newDir->setDisabled(true);

    m_socket->write(selectedItem.toStdString().c_str(), selectedItem.length());
    m_socket->waitForReadyRead();
    risp = m_socket->read(m_socket->bytesAvailable());
    QString final(risp);
    if(final == OK)
    {
        qDebug() << "OK ask tree update";
        connect(m_socket, SIGNAL(readyRead()), this, SLOT(setTreeDirectory()), Qt::DirectConnection);
        treeDir->treeWidget->clear();
        m_socket->write(TREE.toStdString().c_str(), TREE.length());

    }
    else
    {
        qDebug() << "Somethig is wrong";
        QMessageBox::warning(this, tr("ERROR"),
                                       tr("New Folder creation failed"),
                                       QMessageBox::Ok);
    }
    treeDir->treeWidget->setDisabled(false);
}

void WelcomeWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "VNNC",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::No | QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        m_socket->write(DISCONNECT.toStdString().c_str(), DISCONNECT.length());
        m_socket->disconnectFromHost();
        event->accept();
    }
}
