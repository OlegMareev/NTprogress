#include "server.h"

Server::Server()
{
    if(listen(QHostAddress::Any, port)){
        qDebug()<<"Server working";
    }
    else{
        qDebug()<<"Listen error";
    }
    connect(&tradingFloor,&TradingFloor::orderUpdate,this,&Server::orderUpdate);
}

Client *Server::getClient(QTcpSocket *clientSocket)
{
    auto it = std::find_if(clients.begin(),clients.end(),
                           [clientSocket](Client *c){return c->socket == clientSocket;});
    if(it != clients.end())
        return *it;

    qDebug()<<"getClient error";
    return nullptr;
}

void Server::sendAnswer(QString str, QTcpSocket *clientSocket)
{
    if(str.left(1) == '$'){
        if(str == "$My balance"){
            str = getClient(clientSocket)->getBalance();
            str.push_front("$Balance = ");
        }
        else if(str.left(sizeof("$New order = ")-1) == "$New order = "){
            Order *newOrder = new Order(getClient(clientSocket),str);
            tradingFloor.addOrder(newOrder);
            str = "Created new order(";
            str.push_back(newOrder->getOrder());
            str.push_back(")");
            clientSocket = groupCommand;
            sendToClient(str,clientSocket);

            tradingFloor.start();
            return;
        }
        else if(str == "$My active orders"){
            str = tradingFloor.getActiveClintOrders(getClient(clientSocket));
        }
        else if(str == "$All active orders"){
            str = tradingFloor.getActiveOrders();
        }
        else if(str == "$All history orders"){
            str = tradingFloor.getHistoryOrders();
        }
        else if(str == "$My history orders"){
            str = tradingFloor.getHistoryClintOrders(getClient(clientSocket));
        }
        else if(str.left(9) == "$My name "){
            str.remove("$My name ");
            getClient(clientSocket)->setName(str);
            str.push_front("$Name = ");
        }
        else if(str == "$Successful connection"){
            str.remove('$');
        }
        else{
            str = "Unknow command";
        }
    }
    else {
        str.push_front(": ");
        str.push_front(getClient(clientSocket)->getName());
        clientSocket = groupCommand;
    }
    sendToClient(str,clientSocket);
}

void Server::sendToClient(QString str,QTcpSocket*clientSocket)
{
    QByteArray sendText;
    sendText.clear();
    QDataStream outputData(&sendText,QIODevice::WriteOnly);
    outputData<<quint16(0)<<str;
    outputData.device()->seek(0);
    outputData<<quint16(sendText.size()- sizeof(quint16));

    if(clientSocket == groupCommand){
        for(auto &it : clients)
            it->socket->write(sendText);
    }
    else{
        clientSocket->write(sendText);
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socketPtr = new QTcpSocket;
    socketPtr->setSocketDescriptor(socketDescriptor);
    connect(socketPtr,&QTcpSocket::readyRead,this,&Server::readyRead);
    connect(socketPtr,&QTcpSocket::disconnected,socketPtr,&QTcpSocket::deleteLater);
    connect(socketPtr,&QTcpSocket::disconnected,this,&Server::clientDisconnected);

    Client *newClient = new Client;
    newClient->socket = socketPtr;
    newClient->setName("");
    clients.push_back(newClient);
    qDebug()<<"Connecting with new client: "<<socketDescriptor;
    sendAnswer("$Successful connection",socketPtr);
}

void Server::readyRead()
{
    socketPtr = (QTcpSocket*)sender();
    QDataStream inputData(socketPtr);
    if(inputData.status() == QDataStream::Ok){
        for(;;){
            if(blockSize == 0){
                if(socketPtr->bytesAvailable()<2)
                    break;
                inputData>>blockSize;
            }
            if(socketPtr->bytesAvailable()<blockSize){
                break;
            }
            QString str;
            inputData>>str;
            blockSize = 0;
            qDebug()<<str;
            sendAnswer(str,socketPtr);
        }
    }else{
        qDebug()<<"DataStream error";
    }
}

void Server::clientDisconnected()
{
    socketPtr = (QTcpSocket*)sender();
    auto it = std::find_if(clients.begin(),clients.end(),
                           [this](Client *c){return c->socket == socketPtr;});
    if(it != clients.end()){
        qDebug()<<"Client"<<(*it)->getName() <<" disconnected";
        delete *it;
        clients.erase(it);
    }
}

void Server::orderUpdate()
{
    Order *last = tradingFloor.updatedOrder;
    sendToClient(last->getOrder(),last->getClient()->socket);
    sendAnswer("$My balance",last->getClient()->socket);
}
