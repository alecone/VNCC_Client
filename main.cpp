#include "welcomewindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap;
    pixmap.load(":/images/sfondo_unipg.png");

    QSplashScreen * splash = new QSplashScreen;
    splash->setPixmap(pixmap);
    splash->show();

    QTcpSocket *socket = new QTcpSocket();
    WelcomeWindow w;
    w.setSocket(socket);
    QTimer::singleShot(2000, splash, SLOT(close()));
    QTimer::singleShot(2100, &w, SLOT(show()));

    return a.exec();
}
