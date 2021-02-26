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

server::~server()
{
   this->close();
}
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
    QTcpSocket *socket = this->nextPendingConnection();

    connect(socket, &QTcpSocket::readyRead, this, &server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &server::slotDisconnected);

    qDebug() << "| Client " << socket->socketDescriptor() << " connected";

    clients.push_back(socket);
}
//_____________________________________________________________________________

void server::slotReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *> (sender());
    QByteArray data = socket->readAll();
    qDebug() << "| Message from" << socket->socketDescriptor() << "client:" << data;
    for (auto client: clients)
        client->write(data);
}
//_____________________________________________________________________________

void server::slotDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *> (sender());
    qDebug() << "| Client disconnected";
    clients.removeOne(socket);
    socket->close();
}
//=============================================================================
