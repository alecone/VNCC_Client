#include "welcomewindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString proPath = QDir::currentPath();

    QSplashScreen * splash = new QSplashScreen;
    proPath.append("/images/sfondo_unipg.png");
    splash->setPixmap(QPixmap(proPath));
    splash->show();

    QTcpSocket *socket = new QTcpSocket();
    WelcomeWindow w;
    w.setSocket(socket);
    QTimer::singleShot(3000, splash, SLOT(close()));
    QTimer::singleShot(3000, &w, SLOT(show()));

    return a.exec();
}
