#include "testing_tradingfloor.h"

void Testing_TradingFloor::initParams()
{
    delete clientA;
    delete clientB;
    delete orderA;
    delete orderB;

    clientA = new Client;
    clientB = new Client;

    clientA->setName("ClientA");
    clientB->setName("ClientB");

    orderA = new Order(clientA,Act::Buy, 1,60);
    orderB = new Order(clientB,Act::Sell,1,55);
}

Testing_TradingFloor::Testing_TradingFloor(QObject *parent)
    : QObject{parent}
{
    initParams();
}

void Testing_TradingFloor::test_getActiveOrders(){
    initParams();
    TradingFloor tf;
    QCOMPARE(tf.getActiveOrders(),"No active orders");

    tf.addOrder(orderA);
    if(!tf.getActiveOrders().contains("ClientA: buy 1 USD by 60 RUB Active"))
        QFAIL("GetActiveOrders FAIL");

    tf.addOrder(orderB);
    if(!(tf.getActiveOrders().contains("ClientB: sell 1 USD by 55 RUB Active") &&
         tf.getActiveOrders().contains("ClientA: buy 1 USD by 60 RUB Active")))
        QFAIL("GetActiveOrders FAIL");
}

void Testing_TradingFloor::test_getActiveClintOrders(){
    initParams();
    TradingFloor tf;
    QCOMPARE(tf.getActiveClintOrders(clientA),"No active orders");
    QCOMPARE(tf.getActiveClintOrders(clientB),"No active orders");

    tf.addOrder(orderA);
    if(!tf.getActiveClintOrders(clientA).contains("ClientA: buy 1 USD by 60 RUB Active"))
        QFAIL("GetActiveClintOrders FAIL");
    QCOMPARE(tf.getActiveClintOrders(clientB),"No active orders");

    tf.addOrder(orderB);
    if(!(tf.getActiveClintOrders(clientB).contains("ClientB: sell 1 USD by 55 RUB Active") &&
         (!tf.getActiveClintOrders(clientB).contains("ClientA: buy 1 USD by 60 RUB Active"))))
        QFAIL("GetActiveClintOrders FAIL");
}

void Testing_TradingFloor::test_start(){

    auto chekingDealValue = [](Order *firstOrder,Order *secondOrder){
        TradingFloor tf;
        tf.addOrder(firstOrder);
        tf.addOrder(secondOrder);
        int dealValue = firstOrder->getRub();

        QString activeOrders = tf.getActiveOrders();
        if(activeOrders.contains("No active orders"))
            QFAIL("addOrder FAIL");

        int first = activeOrders.indexOf(firstOrder->getClient()->getName());
        int second = activeOrders.indexOf(secondOrder->getClient()->getName());
        if(first>second || second == -1 || first == -1)
            QFAIL("addOrder FAIL");

        tf.start();
        QCOMPARE(tf.getActiveOrders(),"No active orders");

        short znak = 1;
        if(firstOrder->getAct() == Act::Buy)
            znak = -1;

        QCOMPARE(firstOrder->getClient()->getBalance().toInt(),znak*dealValue);
        QCOMPARE(secondOrder->getClient()->getBalance().toInt(),(-znak)*dealValue);
    };
    {// test whose dealValue will be the first
        initParams();
        chekingDealValue(orderA,orderB);
        orderA = nullptr;
        orderB = nullptr;

        initParams();
        chekingDealValue(orderB,orderA);
        orderA = nullptr;
        orderB = nullptr;
    }
    {//examle from task
        Client *user1 = new Client("User1");
        Client *user2 = new Client("User2");
        Client *user3 = new Client("User3");

        Order *orderUser1 = new Order(user1,Act::Buy,10,62);
        Order *orderUser2 = new Order(user2,Act::Buy,20,63);
        Order *orderUser3 = new Order(user3,Act::Sell,50,61);

        TradingFloor tf;
        tf.deleteHistory();
        QCOMPARE(tf.getHistoryOrders(),"No history orders");
        tf.addOrder(orderUser1);
        tf.addOrder(orderUser2);
        tf.addOrder(orderUser3);
        tf.start();

        QString history = tf.getHistoryOrders();
        if(history.indexOf(user2->getName()) > history.indexOf(user1->getName()))
            QFAIL("Start FAIL");

        QCOMPARE(user1->getBalance().toInt(),-620);
        QCOMPARE(user2->getBalance().toInt(),-1260);
        QCOMPARE(user3->getBalance().toInt(),1880);

        if(!tf.getActiveClintOrders(user3).contains("User3: sell 20 USD by 61 RUB Active"))
            QFAIL("Start FAIL");
    }
}

