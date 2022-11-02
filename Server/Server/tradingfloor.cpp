#include "tradingfloor.h"


TradingFloor::TradingFloor()
{
    historyOrdersDB = new HistoryOrdersDB;
}

void TradingFloor::addOrder(Order *newOrder)
{
    activeOrders.push_back(newOrder);
}

QString TradingFloor::getActiveClintOrders(Client *client)
{
    QString activeClientOrders = "\r"+client->getName() + " active orders = \r";
    bool empty = true;

    for(auto it: activeOrders){
        if(it->getClient() == client){
            activeClientOrders.append(it->getOrder());
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

    QVector<OrderDB> allOrdersDB = historyOrdersDB->allOrdersDB();
    if(allOrdersDB.size() == 0){
        str = "No history orders";
        return str;
    }
    for(auto it :allOrdersDB){
        str.append(it.toString());
        str.append("\r");
    }
    return str;
}

QString TradingFloor::getHistoryClintOrders(Client *client)
{
    QString historyClientOrders = "\r"+client->getName() + " history orders = \r";

    QVector<OrderDB> clientOrdersDB = historyOrdersDB->clientOrdersDB(client->getName());
    if(clientOrdersDB.size() == 0){
        historyClientOrders = "No history orders";
        return historyClientOrders;
    }
    for(auto it : clientOrdersDB){
        historyClientOrders.append(it.toString());
        historyClientOrders.append("\r");
    }
    return historyClientOrders;
}

void TradingFloor::deleteHistory() {
    historyOrdersDB->deleteAll();
    delete historyOrdersDB;
    historyOrdersDB = new HistoryOrdersDB;
}

QString TradingFloor::getActiveOrders()
{
    QString str = "\rActive orders = \r";
    if(activeOrders.size() == 0){
        str = "No active orders";
        return str;
    }
    for(auto it : activeOrders){
        str.append(it->getOrder());
        str.append("\r");
    }
    return str;
}
void TradingFloor::start()
{
    while(true){
        QVector<Order *> buyOrders;
        QVector<Order *> sellOrders;

        for(auto it :activeOrders){
            if(it->getAct() == Act::Buy)
                buyOrders.push_back(it);
            else if(it->getAct() == Act::Sell)
                sellOrders.push_back(it);
            else
                qDebug("Trading floor act error");
        }

        if(buyOrders.size() == 0 || sellOrders.size() == 0){
            qDebug()<<"TradingFloor: No orders to trade";
            return;
        }

        Order *minSellOrder = sellOrders[0];
        for(auto it :sellOrders)
            if(minSellOrder->getRub()>it->getRub())
                minSellOrder = it;
        Order *maxBuyOrder = buyOrders[0];
        for(auto it: buyOrders)
            if(maxBuyOrder->getRub()<it->getRub())
                maxBuyOrder = it;

        if(maxBuyOrder->getRub()<minSellOrder->getRub()){
            return;
        }
        else{
            quint32 minUsd = maxBuyOrder->getUsd()<minSellOrder->getUsd()?
                        maxBuyOrder->getUsd():minSellOrder->getUsd();

            int dealValue = 0;
            for(int i =0;i<activeOrders.size();i++){
                if(activeOrders[i] == maxBuyOrder || activeOrders[i] == minSellOrder){
                    dealValue = minUsd*activeOrders[i]->getRub();
                break;
                }
            }

            {   //update max buy order in activeOrders
                auto it = std::find(activeOrders.begin(),activeOrders.end(),maxBuyOrder);
                updatedOrder = *it;
                (*it)->subtractUsd(minUsd);
                (*it)->getClient()->addBalance(-dealValue);
                if((*it)->getUsd() == 0){
                    historyOrdersDB->insertOrder((*it));
                    delete (*it);
                    activeOrders.erase(it);
                }
                emit orderUpdate();
            }
            {   //update min sell order in activeOrders
                auto it = std::find(activeOrders.begin(),activeOrders.end(),minSellOrder);
                updatedOrder = (*it);
                (*it)->subtractUsd(minUsd);
                (*it)->getClient()->addBalance(dealValue);
                if((*it)->getUsd() == 0){
                    historyOrdersDB->insertOrder((*it));
                    delete (*it);
                    activeOrders.erase(it);
                }
                emit orderUpdate();
            }
        }
    }
}
