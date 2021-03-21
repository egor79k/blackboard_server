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
// External API
//_____________________________________________________________________________

void Server::addLayer(const Serializer &args)
{
    qDebug() << "Add layer called!";
}
//_____________________________________________________________________________

void Server::wrongRequest(const Serializer &args)
{
    qDebug() << "Wrong request recieved!";
}
//_____________________________________________________________________________
// Slots
//_____________________________________________________________________________

void Server::slotNewConnection()
{
    QTcpSocket *socket = this->nextPendingConnection();

    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::slotDisconnected);

    qDebug() << "| Client " << socket->socketDescriptor() << " connected";

    clients.push_back(socket);
    clientsn.push_back(QSharedPointer<Client>(new Client(socket)));
}
//_____________________________________________________________________________

void Server::slotReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *> (sender());

    uint64_t header_size = 0;

    while (socket->read(reinterpret_cast<char*>(&header_size), sizeof(header_size)) ==
           sizeof(header_size))
    {
        Q_ASSERT(header_size > 0);

        QByteArray data = socket->read(header_size);
        Q_ASSERT(data.size() == header_size);

        QJsonObject header = QJsonDocument::fromJson(data).object();
        int arg_size = header["argument_size"].toInt();
        Q_ASSERT(arg_size > 0);

        data = socket->read(arg_size);
        Q_ASSERT(data.size() == arg_size);

#ifdef JSON_SERIALIZER
        JsonSerializer serial_arg(data);
#else
        Q_ASSERT(false && "JSON_SERIALIZER undefined");
#endif

        Q_ASSERT(api_func.contains(header["method"].toString()));
        qDebug() << "| Client" << socket->socketDescriptor() << "called" << header["method"].toString();
        (this->*api_func[header["method"].toString()])(serial_arg);
    }

    //for (auto client: clientsn)
        //client->write("Common message");
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
