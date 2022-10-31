#include "ordersmenu.h"
#include "./ui_ordersmenu.h"

OrdersMenu::OrdersMenu(Client*client,QWidget *parent) :
    client(client),
    QWidget(parent),
    ui(new Ui::OrdersMenu)
{
    ui->setupUi(this);
    ui->sellRBtn->click();
}

OrdersMenu::~OrdersMenu()
{
    delete ui;
}

void OrdersMenu::on_sellRBtn_clicked(){act = "sell ";}
void OrdersMenu::on_buyRBtn_clicked(){act = "buy ";}

void OrdersMenu::on_createOrderBtn_clicked()
{
    QString usd;
    usd.setNum(ui->usdSpinBox->value());
    usd.push_back(" ");
    QString rub;
    rub.setNum(ui->rubSpinBox->value());

    QString newOrder = "$New order = ";
    newOrder+= act+usd+rub;
    client->sendMessage(newOrder);
}

void OrdersMenu::on_allActiveBtn_clicked()
{
    client->sendMessage("$All active orders");
}
void OrdersMenu::on_myActiveBtn_clicked()
{
    client->sendMessage("$My active orders");
}
void OrdersMenu::on_allHistoryBtn_clicked()
{
    client->sendMessage("$All history orders");
}
void OrdersMenu::on_myHistoryBtn_clicked()
{
    client->sendMessage("$My history orders");
}

