#include <QCoreApplication>

#include "server.h"

#include<testing_tradingfloor.h>

int main(int argc, char *argv[])
{
    QTest::qExec(new Testing_TradingFloor);

    QCoreApplication a(argc, argv);
    Server server;
    return a.exec();
}
