#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H

#include <QMainWindow>

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

    void on_serverAddress_returnPressed();

private:
    Ui::WelcomeWindow *ui;
};

#endif // WELCOMEWINDOW_H
