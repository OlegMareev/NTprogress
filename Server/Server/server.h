#ifndef SERVER_H
#define SERVER_H

#include <tradingfloor.h>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    QTcpSocket *groupCommand = nullptr;
private:
    const quint16 port = 2323;
    quint16 blockSize = 0;
    QTcpSocket *socketPtr;
    QVector<Client*> clients;
    TradingFloor tradingFloor;

    Client* getClient(QTcpSocket*clientSocket);
    void sendAnswer(QString str,QTcpSocket*clientSocket);
    void sendToClient(QString str,QTcpSocket*clientSocket);
private slots:
    void incomingConnection(qintptr socketDescriptor) override;
    void readyRead();
    void disconnect();
    void orderUpdate();
};

#endif // SERVER_H
