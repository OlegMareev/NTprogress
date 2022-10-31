#ifndef HISTORYORDERS_H
#define HISTORYORDERS_H

#include <QSqlDatabase>
#include <QSqlQuery>




class HistoryOrders
{
private:
    QSqlDatabase db;
    QSqlQuery *query;
public:
    HistoryOrders();

    int size();
    QVector<Order*> getOrders();
    void insertValue(Order &order);
};

#endif // HISTORYORDERS_H
