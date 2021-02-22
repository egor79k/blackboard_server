#include <QHostAddress>
#include "server.h"

//=============================================================================
// server
//=============================================================================
server::server()
{
    connect(this, &QTcpServer::newConnection, this, &server::slotNewConnection);
}
//_____________________________________________________________________________

server::~server() {}
//_____________________________________________________________________________

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
//_____________________________________________________________________________

void server::slotNewConnection()
{
    socket = this->nextPendingConnection();

    connect(socket, &QTcpSocket::readyRead, this, &server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &server::slotDisconnected);

    qDebug() << socket->socketDescriptor() << " client connected";

    socket->write("You are connect");
    qDebug() << "Send client connect status - YES";
}
//_____________________________________________________________________________

void server::slotReadyRead()
{
    QByteArray Data = socket->readAll();
    qDebug() << "Message from client: " << Data;
    socket->write(Data);
}
//_____________________________________________________________________________

void server::slotDisconnected()
{
    qDebug() << "Disconnect";
    socket->deleteLater();
}
//=============================================================================
