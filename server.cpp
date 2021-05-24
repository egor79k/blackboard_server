#include "server.h"


//=============================================================================
// Server
//=============================================================================
Server::Server()
{
    connect(this, &QTcpServer::newConnection, this, &Server::slotNewConnection);
    connect(&read_manager, &PackageReadManager::packageReceived,
            this, &Server::slotPackageReceived);
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
        qDebug() << "| Listening";
    }
    else
    {
        qDebug() << "| Not listening";
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

    saveHistory(layer, Client::HSCT::ADD_LAYER);

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

    for (auto client: clients) {
        client->clearHistory();
        if (client->getID() != curr_sender_id)
            client->clearBoard();
    }
}
//_____________________________________________________________________________

void Server::deleteLayer(const Serializer &args)
{
    DeleteLayerArgs layer_arg;
    args.deserialize(layer_arg);

    for (auto layer = scene.begin(); layer != scene.end(); ++layer)
        if ((*layer)->layer_id == layer_arg.layer_id)
        {
            saveHistory(*layer, Client::HSCT::DELETE_LAYER);
            scene.erase(layer);
            break;
        }

    for (auto client: clients)
        if (client->getID() != curr_sender_id)
            client->deleteLayer(JsonSerializer(DeleteLayerArgs(layer_arg.layer_id)));
}
//_____________________________________________________________________________

void Server::undo(const Serializer &)
{
    QSharedPointer<Client::HistorySlot> slot{};

    for (auto client: clients)
        if (client->getID() == curr_sender_id)
        {
            slot = client->rollBackHistory();
            break;
        }

    if (slot.isNull())
        return;

    switch (slot->change_type)
    {
        case Client::HSCT::ADD_LAYER:
        {
            AddLayerArgs layer_data;
            JsonSerializer(slot->layer).deserialize(layer_data);

            for (auto layer = scene.begin(); layer != scene.end(); ++layer)
                if ((*layer)->layer_id == layer_data.layer_id)
                {
                    scene.erase(layer);
                    break;
                }

        #ifdef JSON_SERIALIZER
            for (auto client: clients)
                client->deleteLayer(JsonSerializer(DeleteLayerArgs(layer_data.layer_id)));
        #else
        static_assert(false, "No serializer defined.");
        #endif

            break;
        }

        case Client::HSCT::CHANGE_LAYER:
            break;

        case Client::HSCT::DELETE_LAYER:
        {
            AddLayerArgs layer_data;
            JsonSerializer(slot->layer).deserialize(layer_data);

            QSharedPointer<Layer> layer (new Layer(layer_data, curr_sender_id));
            scene.push_back(layer);

        #ifdef JSON_SERIALIZER
            for (auto client: clients)
                    client->addLayer(JsonSerializer(layer->getAddLayerArgs()));
        #else
        static_assert(false, "No serializer defined.");
        #endif

            break;
        }
    }
}
//_____________________________________________________________________________

void Server::saveHistory(QSharedPointer<Layer> layer, Client::HSCT change_type)
{
    QSharedPointer<Client::HistorySlot> slot(new Client::HistorySlot);
    layer->getAddLayerArgs().serialize(slot->layer);
    slot->change_type = change_type;

    for (auto client: clients)
        if (client->getID() == curr_sender_id)
        {
            client->saveHistory(slot);
            break;
        }

/*
    QSharedPointer<QJsonObject> saved_layer(new QJsonObject);
    layer->getAddLayerArgs().serialize(*saved_layer);
    history.push(saved_layer);*/
}
//-----------------------------------------------------------------------------
// Slots
//_____________________________________________________________________________

void Server::slotNewConnection()
{
    QTcpSocket *socket = this->nextPendingConnection();
    connect(socket, &QTcpSocket::disconnected, this, &Server::slotDisconnected);
    read_manager.addSocket(socket);

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

void Server::slotPackageReceived(QTcpSocket* socket,
                                 const QJsonObject& header,
                                 const QByteArray& argument)
{
    Q_ASSERT(socket != nullptr);
    Q_ASSERT(!header.isEmpty());
    Q_ASSERT(argument.size() > 0);

    curr_sender_id = socket->socketDescriptor();

    RequestHeader header_obj;
    header_obj.deserialize(header);

#ifdef JSON_SERIALIZER
    JsonSerializer serial_arg(argument);
#else
static_assert(false, "No serializer defined.");
#endif

    Q_ASSERT(api_func.contains(header_obj.method));
    qDebug() << "| Client" << socket->socketDescriptor() << "called" << header_obj.method;
    (this->*api_func[header_obj.method])(serial_arg);
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

    read_manager.removeSocket(socket);
    socket->close();
}
//=============================================================================
