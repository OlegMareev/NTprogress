#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new Client(this);
    ordersMenu = new OrdersMenu(client);
    ui->gridLayout->addWidget(ordersMenu,4,0,1,3);
    connect(client,&Client::updateLog,this,&MainWindow::updateLog);
}

MainWindow::~MainWindow()
{
    delete ordersMenu;
    delete client;
    delete ui;
}


void MainWindow::on_connectWithServerBtn_clicked()
{
    client->connectWithServer();

    connect(client,&Client::updateName,this,&MainWindow::updateName);
    client->sendMessage("$My name " + ui->nameLE->text());

    connect(client,&Client::updateBalance,this,&MainWindow::updateBalance);
    client->sendMessage("$My balance");
}


void MainWindow::on_sendBtn_clicked()
{
    client->sendMessage(ui->messageLE->text());
}

void MainWindow::updateLog()
{
    ui->log->append(client->text());
}

void MainWindow::updateBalance()
{
    ui->balanceLabel->setText(client->balance);
}

void MainWindow::updateName()
{
    ui->nameLE->setText(client->name);
    ui->nameLE->setReadOnly(true);
}


