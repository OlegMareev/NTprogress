#include "historyordersdb.h"

HistoryOrdersDB::HistoryOrdersDB()
{
    QString dirictory = "./HistoryOrdersDB.db";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dirictory);
    if(!db.open())
        qDebug("ordersHistory open error");
    query = new QSqlQuery(db);
    query->exec("CREATE TABLE OrdersHistory(Time TEXT, Username TEXT, Act TEXT, Usd INT, Rub INT);");
}

QVector<OrderDB> HistoryOrdersDB::clientOrdersDB(QString name)
{
    QVector<OrderDB> clientOrders;
    query->exec("SELECT * FROM OrdersHistory WHERE Username = '"+name+"'");
    while (query->next()){
        OrderDB order;
        order.time = query->value(0).toString();
        order.username = query->value(1).toString();
        order.act = query->value(2).toString();
        order.usd = query->value(3).toInt();
        order.rub = query->value(4).toInt();
        clientOrders.push_back(order);
    }
    return clientOrders;
}

QVector<OrderDB> HistoryOrdersDB::allOrdersDB(){
    QVector<OrderDB> allOrders;
    query->exec("SELECT * FROM OrdersHistory");
    while (query->next()){
        OrderDB order;
        order.time = query->value(0).toString();
        order.username = query->value(1).toString();
        order.act = query->value(2).toString();
        order.usd = query->value(3).toInt();
        order.rub = query->value(4).toInt();
        allOrders.push_back(order);
    }
    return allOrders;
}

void HistoryOrdersDB::insertOrderDB(OrderDB &orderDB)
{
    query->prepare("INSERT INTO OrdersHistory(Time,Username,Act,Usd,Rub) "
                   "VALUES (:Time, :Username,:Act,:Usd,:Rub)");
    query->bindValue(":Time",orderDB.time);
    query->bindValue(":Username",orderDB.username);
    query->bindValue(":Act",orderDB.act);
    query->bindValue(":Usd",orderDB.usd);
    query->bindValue(":Rub",orderDB.rub);
    query->exec();
}

void HistoryOrdersDB::insertOrder(Order *order)
{
    OrderDB orderDB;
    orderDB.time = order->getTime();
    orderDB.username = order->getClientName();
    if(order->getAct() ==Act::Buy)
        orderDB.act = "Buy";
    else
        orderDB.act = "Sell";
    orderDB.usd = order->getUsdDeal();
    orderDB.rub = order->getRub();
    insertOrderDB(orderDB);
}
