#include "client.h"

Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(parent);
    connect(socket,&QTcpSocket::readyRead,this,&Client::readMessage);
    connect(socket,&QTcpSocket::destroyed,socket,&QTcpSocket::deleteLater);
}

QString Client::text()
{
    QString returnedLog = log;
    log = "";
    return returnedLog;
}

void Client::connectWithServer()
{
    socket->connectToHost(ip,port);
}

void Client::sendMessage(QString str)
{
    QByteArray sendText;
    sendText.clear();
    QDataStream outputData(&sendText,QIODevice::WriteOnly);
    //inputData.setVersion(QDataStream::Version::Qt_6_4);
    outputData<<quint16(0)<<str;
    outputData.device()->seek(0);
    outputData<<quint16(sendText.size()- sizeof(quint16));
    socket->write(sendText);
}

void Client::readMessage()
{
    QDataStream inputData(socket);
    //inputData.setVersion(QDataStream::Version::Qt_6_4);
    if(inputData.status() == QDataStream::Ok){
        for(;;){
            if(blockSize == 0){
                if(socket->bytesAvailable()<2)
                    break;
                inputData>>blockSize;
            }
            if(socket->bytesAvailable()<blockSize){
                break;
            }
            QString str;
            inputData>>str;
            blockSize = 0;
            if(str.left(11) == "$Balance = "){
                balance = str;
                balance.remove('$');
                emit updateBalance();
            }
            else if(str.left(8) == "$Name = "){
                name = str;
                name.remove("$Name = ");
                emit updateName();
            }
            else{
                log.append(str);
                emit updateLog();
            }
        }
    }
    else{
        log.append("DataStream error");
    }

}
