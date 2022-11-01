#ifndef ORDERSMENU_H
#define ORDERSMENU_H

#include <QWidget>
#include <client.h>
namespace Ui {
class OrdersMenu;
}

class OrdersMenu : public QWidget
{
    Q_OBJECT
private:
    QString act;
    Client*client;
public:
    explicit OrdersMenu(Client*client, QWidget *parent = nullptr);
    ~OrdersMenu();

private slots:
    void on_sellRBtn_clicked();
    void on_buyRBtn_clicked();
    void on_createOrderBtn_clicked();
    void on_allActiveBtn_clicked();
    void on_myActiveBtn_clicked();
    void on_allHistoryBtn_clicked();
    void on_myHistoryBtn_clicked();

private:
    Ui::OrdersMenu *ui;
};

#endif // ORDERSMENU_H
