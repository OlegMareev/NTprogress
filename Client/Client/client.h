#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    Client() = delete;
    Client(QObject*);
    QString log ="";
    QString balance;
    QString name;
private:
    const QString ip = "127.0.0.1";
    quint16 port = 2323;
    quint16 blockSize = 0;
    QTcpSocket *socket;
public:
    QString text();
    void connectWithServer();
    void sendMessage(QString str);
private slots:
    void readMessage();
signals:
    void updateLog();
    void updateName();
    void updateBalance();
};

#endif // CLIENT_H
