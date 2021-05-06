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
//-----------------------------------------------------------------------------
// External API
//_____________________________________________________________________________

void Server::addLayer(const Serializer &args)
{
    AddLayerArgs layer_data;
    args.deserialize(layer_data);
    
    qDebug() << "| Add layer called of" << layer_data.layer_type << "type"; // with args:" << args.getData();

    scene.push_back(QSharedPointer<Layer> (new Layer(layer_data, curr_sender_id)));

    for (auto client: clients)
        if (client->getID() != curr_sender_id)
            client->addLayer(args);

}
//_____________________________________________________________________________

void Server::wrongRequest(const Serializer &args)
{
    qDebug().noquote() << "| Wrong request recieved with args:" << args.getData();
}
//-----------------------------------------------------------------------------
// Slots
//_____________________________________________________________________________

void Server::slotNewConnection()
{
    QTcpSocket *socket = this->nextPendingConnection();

    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::slotDisconnected);

    clients.push_back(QSharedPointer<Client>(new Client(socket)));
    auto client = clients.back();

    qDebug() << "| Client " << socket->socketDescriptor() << " connected";

#ifdef JSON_SERIALIZER
    client->initClient(JsonSerializer(InitClientArgs(clients.back()->getID())));

    for (auto layer : scene)
        client->addLayer(JsonSerializer(layer->getAddLayerArgs()));
#else
static_assert(false, "No serializer defined.");
#endif
}
//_____________________________________________________________________________

void Server::slotReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *> (sender());

    curr_sender_id = socket->socketDescriptor();

    uint64_t header_size = 0;

    while (socket->read(reinterpret_cast<char*>(&header_size), sizeof(header_size)) ==
           sizeof(header_size))
    {
        Q_ASSERT(header_size > 0);

        QByteArray data = socket->read(header_size);
        Q_ASSERT(static_cast<uint64_t>(data.size()) == header_size);

        RequestHeader header;

#ifdef JSON_SERIALIZER
        JsonSerializer(data).deserialize(header);
#else
static_assert(false, "No serializer defined.");
#endif

        data = socket->read(header.argument_size);
        Q_ASSERT(data.size() == header.argument_size);

#ifdef JSON_SERIALIZER
        JsonSerializer serial_arg(data);
#else
static_assert(false, "No serializer defined.");
#endif

        Q_ASSERT(api_func.contains(header.method));
        qDebug() << "| Client" << socket->socketDescriptor() << "called" << header.method;
        (this->*api_func[header.method])(serial_arg);
    }
}
//_____________________________________________________________________________

void Server::slotDisconnected()

{
    QTcpSocket *socket = qobject_cast<QTcpSocket *> (sender());

    qDebug() << "| Client disconnected" << socket;

    for (auto client = clients.begin(); client != clients.end(); ++client)
    {
        if (**client == Client(socket))
        {
            clients.erase(client);
            break;
        }
    }

    socket->close();
}
//=============================================================================
