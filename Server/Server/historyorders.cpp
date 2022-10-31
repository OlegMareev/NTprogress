#include "historyorders.h"

HistoryOrders::HistoryOrders()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./ordersHistory.db");
    if(!db.open())
    {
        qDebug("ordersHistory open error");
    }

    query = new QSqlQuery(db);
    query->exec("CREATE TABLE OrdersHistory(Create time TEXT, Username TEXT, Act TEXT, Usd INT, Rub INT);");
}

QVector<Order*> HistoryOrders::getOrders(){
    QVector<Order*> orders;
    query->exec("SELECT Time,Username,Act,Usd,Rub FROM OrdersHistory");
    while(
          query->next()){

        Order *order = new Order();
        orde
        order.time = query->value(0).toString();
        order.username = query->value(1).toString();
        order.act = query->value(2).toString();
        order.usd = query->value(3).toInt();
        order->rub = query->value(4).toInt();

        orders.push_back(order);
    }
    return orders;
}
