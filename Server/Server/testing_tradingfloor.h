#ifndef TESTING_TRADINGFLOOR_H
#define TESTING_TRADINGFLOOR_H

#include <QTest>
#include <QObject>

#include <tradingfloor.h>
class Testing_TradingFloor : public QObject
{
    Q_OBJECT
private:
    Client *clientA = nullptr;
    Client *clientB = nullptr;
    Order *orderA = nullptr;
    Order *orderB = nullptr;

    void initParams();
public:
    explicit Testing_TradingFloor(QObject *parent = nullptr);

private slots:
    void test_getActiveOrders();
    void test_getActiveClintOrders();
    void test_start();
};

#endif // TESTING_TRADINGFLOOR_H
