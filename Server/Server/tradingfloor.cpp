#include "tradingfloor.h"


TradingFloor::TradingFloor()
{

}

void TradingFloor::addOrder(Order *newOrder)
{
    activeOrders.push_back(newOrder);
}

QString TradingFloor::getActiveClintOrders(Client *client)
{
    QString activeClientOrders = "\r"+client->getName() + " active orders = \r";
    bool empty = true;
    for(int i =0;i<activeOrders.size();i++){
        if(activeOrders[i]->getClient() == client){
            activeClientOrders.append(activeOrders[i]->getOrder());
            activeClientOrders.append("\r");
            empty = false;
        }
    }
    if(empty == true){
        activeClientOrders = "No active orders";
    }
    return activeClientOrders;
}

QString TradingFloor::getHistoryOrders()
{
    QString str = "\rHistory orders = \r";

    QVector<OrderDB> allOrdersDB = historyOrdersDB.allOrdersDB();
    if(allOrdersDB.size() == 0){
        str = "No history orders";
        return str;
    }

    for(int i =0;i<allOrdersDB.size();i++){
        str.append(allOrdersDB[i].toString());
        str.append("\r");
    }
    return str;
}

QString TradingFloor::getHistoryClintOrders(Client *client)
{
    QString historyClientOrders = "\r"+client->getName() + " history orders = \r";

    QVector<OrderDB> clientOrdersDB = historyOrdersDB.clientOrdersDB(client->getName());
    if(clientOrdersDB.size() == 0){
        historyClientOrders = "No history orders";
        return historyClientOrders;
    }

    for(int i =0;i<clientOrdersDB.size();i++){
        historyClientOrders.append(clientOrdersDB[i].toString());
        historyClientOrders.append("\r");
    }
    return historyClientOrders;
}

QString TradingFloor::getActiveOrders()
{
    QString str = "\rActive orders = \r";

    for(int i =0;i<activeOrders.size();i++){
        str.append(activeOrders[i]->getOrder());
        str.append("\r");
    }
    if(activeOrders.size() == 0){
        str = "No active orders";
    }
    return str;
}
void TradingFloor::start()
{
    while(true){
        QVector<Order *> buyOrders;
        QVector<Order *> sellOrders;


        for(int i =0;i<activeOrders.size();i++){
            if(activeOrders[i]->getAct() == Act::Buy)
                buyOrders.push_back(activeOrders[i]);
            else if(activeOrders[i]->getAct() == Act::Sell)
                sellOrders.push_back(activeOrders[i]);
            else
                qDebug("Trading floor act error");
        }

        if(buyOrders.size() == 0 || sellOrders.size() == 0){
            qDebug()<<"TradingFloor: No orders in list";
            return;
        }


        Order *minSellOrder = sellOrders[0];
        for(int i = 0;i<sellOrders.size();i++)
            if(minSellOrder->getRub()>sellOrders[i]->getRub())
                minSellOrder = sellOrders[i];
        Order *maxBuyOrder = buyOrders[0];
        for(int i = 0;i<buyOrders.size();i++)
            if(maxBuyOrder->getRub()<buyOrders[i]->getRub())
                maxBuyOrder = buyOrders[i];

        if(maxBuyOrder->getRub()<minSellOrder->getRub()){
            return;
        }
        else{
            quint32 minUsd = maxBuyOrder->getUsd()<minSellOrder->getUsd()?
                        maxBuyOrder->getUsd():minSellOrder->getUsd();


            qDebug()<<minSellOrder->getOrder() + " Destroyed";
            qDebug()<<maxBuyOrder->getOrder() + " Destroyed";

            int dealValue = 0;
            for(int i = 0;i<activeOrders.size();i++){
                if(activeOrders[i] == maxBuyOrder){
                    updatedOrder = activeOrders[i];
                    activeOrders[i]->subtractUsd(minUsd);
                    dealValue = minUsd*activeOrders[i]->getRub();
                    activeOrders[i]->getClient()->addBalance(-dealValue);
                    if(activeOrders[i]->getUsd() == 0){
                        historyOrdersDB.insertOrder(activeOrders[i]);
                        activeOrders.remove(i);
                    }
                    emit orderUpdate();
                    break;
                }
            }
            for(int i = 0;i<activeOrders.size();i++){
                if(activeOrders[i] == minSellOrder){
                    updatedOrder = activeOrders[i];
                    activeOrders[i]->subtractUsd(minUsd);
                    activeOrders[i]->getClient()->addBalance(dealValue);
                    if(activeOrders[i]->getUsd() == 0){
                        historyOrdersDB.insertOrder(activeOrders[i]);
                        activeOrders.remove(i);
                    }
                    emit orderUpdate();
                    break;
                }
            }

            //return;
        }
    }
}
