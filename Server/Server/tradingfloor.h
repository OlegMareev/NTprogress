#ifndef TRADINGFLOOR_H
#define TRADINGFLOOR_H

#include <order.h>
#include <vector>
#include <historyordersdb.h>

class TradingFloor: public QObject
{
    Q_OBJECT
    HistoryOrdersDB *historyOrdersDB;
    QVector<Order*> activeOrders;
public:
    TradingFloor();
    void addOrder(Order *newOrder);
    void start();
    QString getActiveOrders();
    QString getActiveClintOrders(Client *client);
    QString getHistoryOrders();
    QString getHistoryClintOrders(Client *client);
    Order *updatedOrder;
    void deleteHistory();;
signals:
    void orderUpdate();
};

#endif // TRADINGFLOOR_H
