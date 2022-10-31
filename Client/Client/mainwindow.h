#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <client.h>
#include <ordersmenu.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateLog();
    void updateBalance();
    void updateName();


    void on_connectWithServerBtn_clicked();
    void on_sendBtn_clicked();

private:
    Client *client;
    OrdersMenu *ordersMenu;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
