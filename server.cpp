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

void server::setLogger(Logger *_log)
{
    log = _log;
}
//_____________________________________________________________________________

void server::startListening(const QHostAddress &address, quint16 port)
{
    if (this->listen(address, port))
    {
        *log << "| Start listening";
    }
    else
    {
        *log << "| Not listening";
    }
}
//_____________________________________________________________________________

void server::stopListening()
{
    this->close();
    *log << "| Stop listening";
}
//_____________________________________________________________________________

void server::slotNewConnection()
{
    QTcpSocket *socket = this->nextPendingConnection();

    connect(socket, &QTcpSocket::readyRead, this, &server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &server::slotDisconnected);

    *log << "| Client " + QString::number(socket->socketDescriptor()) + " connected";

    socket->write("You are connect\n");
    *log << "|| Send client connect status";

    clients.insert(socket);
    for (auto client: clients)
        client->write("New client connected");
}
//_____________________________________________________________________________

void server::slotReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *> (sender());
    QByteArray data = socket->readAll();
    *log << "| Message from " + QString::number(socket->socketDescriptor()) + " client: " + data;
    for (auto client: clients)
        client->write(data);
}
//_____________________________________________________________________________

void server::slotDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *> (sender());
    *log << "| Client disconnected";
    clients.remove(socket);
    socket->close();
}
//=============================================================================
