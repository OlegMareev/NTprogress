#include "client.h"

Client::Client(){}

Client::Client(QString name){this->name = name;}

void Client::addBalance(int value)
{
    balance += value;
}
QString Client::getBalance() { return QString().setNum(balance);}
QString Client::getName(){return name;}
void Client::setName(QString name){this->name = name;}
