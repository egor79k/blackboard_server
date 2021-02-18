#include <QHostAddress>
#include "server.h"

server::server() {}

server::~server() {}

void server::startServer()
{
    if (this->listen(QHostAddress::Any, 5555))
    {
        qDebug() << "Listening";
    }
    else
    {
        qDebug() << "Not listening";
    }
}

void server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));

    qDebug() << socketDescriptor << " Client connected";

    socket->write("You are connect");
    qDebug() << "Send client connect status - YES";
}

void server::sockReady()
{
    Data = socket->readAll();
    qDebug() << "Message from client: " << Data;
    socket->write(Data);
}

void server::sockDisc()
{
    qDebug() << "Disconnect";
    socket->deleteLater();
}
