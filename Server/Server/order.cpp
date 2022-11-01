#include "order.h"



Order::Order(Client *client, QString newOrderStr){
    this->time = QTime::currentTime();
    this->client = client;

    newOrderStr.remove("$New order = ");
    if(newOrderStr.left(5)=="sell "){
        this->act = Act::Sell;
        newOrderStr.remove("sell ");
    }else if(newOrderStr.left(4)=="buy "){
        this->act = Act::Buy;
        newOrderStr.remove("buy ");
    }else{
        qDebug()<<"Order error";
    }
    QString rubAndUsd = newOrderStr;
    newOrderStr.resize(newOrderStr.indexOf(" "));
    QString usd = newOrderStr;
    rubAndUsd.remove(usd);          //if rub==usd --> rub=0
    this->usd = usd.toInt();
    this->rub = rubAndUsd.toInt();
    if(this->rub == 0)
        this->rub = this->usd;
    this->active = true;
    this->usdDeal = this->usd;
}

QString Order::getOrder(){
    QString timeStr = time.toString() + " ";
    QString clientName = client->getName() + ":";

    QString actStr = " sell ";
    if(act == Act::Buy)
        actStr = " buy ";

    QString activeStr = " Active";
    if(usd<=0){
        activeStr = " Closed";
        usd = usdDeal;
    }

    QString usdStr;
    usdStr.setNum(usd);
    usdStr.append(" USD by ");

    QString rubStr;
    rubStr.setNum(rub);
    rubStr.append(" RUB");

    return timeStr+ clientName + actStr + usdStr + rubStr + activeStr;
}
