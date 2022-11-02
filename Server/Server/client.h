#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpServer>
#include <QTcpSocket>

class Client
{
private:
    int balance = 0;
    QString name = "";
public:
    Client();;
    Client(QString name);
    QTcpSocket *socket;

    void addBalance(int value);
    QString getBalance();
    QString getName();
    void setName(QString name);
};

#endif // CLIENT_H
