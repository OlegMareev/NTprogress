#include "client.h"

void Client::addBalance(int value)
{
    balance += value;
}
QString Client::getBalance() { return QString().setNum(balance);}
QString Client::getName(){return name;}
void Client::setName(QString name){this->name = name;}
