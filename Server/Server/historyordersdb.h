#ifndef HISTORYORDERSDB_H
#define HISTORYORDERSDB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>

#include <order.h>

struct OrderDB{
    QString time;
    QString username;
    QString act;
    int usd;
    int rub;

    QString toString(){
        return time+ " " + username + ": " + act + " " +
                QString().setNum(usd) + " USD by " +
                QString().setNum(rub) + " RUB";
    }
};

class HistoryOrdersDB
{

private:
    QSqlDatabase db;
    QSqlQuery *query;
    void insertOrderDB(OrderDB &orderDB);
public:
    HistoryOrdersDB();
    QVector<OrderDB> clientOrdersDB(QString name);
    QVector<OrderDB> allOrdersDB();
    void insertOrder(Order *order);
};

#endif // HISTORYORDERSDB_H
