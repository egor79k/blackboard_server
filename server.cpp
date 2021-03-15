#include <QHostAddress>
#include "server.h"

//=============================================================================
// Server
//=============================================================================
Server::Server()
{
    connect(this, &QTcpServer::newConnection, this, &Server::slotNewConnection);
}
//_____________________________________________________________________________

Server::~Server()
{
   this->close();
}
//_____________________________________________________________________________

void Server::startServer()
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

void Server::createLayer()
{

}
//_____________________________________________________________________________

void Server::slotNewConnection()
{
    QTcpSocket *socket = this->nextPendingConnection();

    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::slotDisconnected);

    qDebug() << "| Client " << socket->socketDescriptor() << " connected";

    clients.push_back(socket);
}
//_____________________________________________________________________________

void Server::slotReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *> (sender());
    QByteArray data = socket->readAll();
    qDebug() << "| Message from" << socket->socketDescriptor() << "client:" << data << "size:" << QString(data).split(' ')[0].toInt();

    for (auto client: clients)
        client->write(data);
}
//_____________________________________________________________________________
void Server::slotDisconnected()

{
    QTcpSocket *socket = qobject_cast<QTcpSocket *> (sender());
    qDebug() << "| Client disconnected";
    clients.removeOne(socket);
    socket->close();
}
//=============================================================================
