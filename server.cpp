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

    QSharedPointer<Layer> layer (new Layer(layer_data, curr_sender_id));
    scene.push_back(layer);

#ifdef JSON_SERIALIZER
    for (auto client: clients)
        if (client->getID() != curr_sender_id)
            client->addLayer(JsonSerializer(layer->getAddLayerArgs()));
        else
            client->confirmAddLayer(JsonSerializer(ConfirmAddLayerArgs(layer->layer_id)));
#else
static_assert(false, "No serializer defined.");
#endif
}
//_____________________________________________________________________________

void Server::clearBoard(const Serializer &)
{
    scene.clear();

    for (auto client: clients)
        if (client->getID() != curr_sender_id)
            client->clearBoard();
}
//_____________________________________________________________________________

void Server::deleteLayer(const Serializer &args)
{
    DeleteLayerArgs layer_arg;
    args.deserialize(layer_arg);

    for (auto layer = scene.begin(); layer != scene.end(); ++layer)
        if ((*layer)->layer_id == layer_arg.layer_id)
        {
            saveHistory(*layer);
            scene.erase(layer);
            break;
        }

    for (auto client: clients)
        if (client->getID() != curr_sender_id)
            client->deleteLayer(JsonSerializer(DeleteLayerArgs(layer_arg.layer_id)));
}
//_____________________________________________________________________________

void undo(const Serializer &)
{

}
//_____________________________________________________________________________

void Server::saveHistory(QSharedPointer<Layer> layer)
{
    QSharedPointer<QJsonObject> saved_layer(new QJsonObject);
    layer->getAddLayerArgs().serialize(*saved_layer);
    history.push(saved_layer);
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

    client->finishBoardInit();
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
