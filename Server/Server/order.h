#ifndef ORDER_H
#define ORDER_H

#include <client.h>
#include <QTime>

enum class Act
{
    Sell,
    Buy
};


class Order
{
private:
    QTime time;
    quint32 usdDeal;
    Client *client;
    Act act;
    quint32 usd;
    quint32 rub;
    bool active;
public:
    Order() = delete;
    Order(Client *client,QString newOrderStr);
    Client *getClient(){return client;}
    QString getOrder();
    Act getAct(){return act;}
    quint32 getUsd(){return usd;}
    quint32 getUsdDeal(){return usdDeal;}
    quint32 getRub(){return rub;}
    QString getTime(){return time.toString();}
    QString getClientName(){return client->getName();}
    void subtractUsd(quint32 usd){this->usd -= usd;}

};

#endif // ORDER_H
