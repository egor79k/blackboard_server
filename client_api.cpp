#include "client_api.h"

//=============================================================================
// Client
//=============================================================================
Client::Client(QTcpSocket *socket_) :
    socket(socket_)
{}
//_____________________________________________________________________________

bool Client::operator==(const Client &other) const
{
    return socket == other.socket;
}
//_____________________________________________________________________________

Client::id_type Client::getID() const
{
    return socket->socketDescriptor();
}
//_____________________________________________________________________________

QTcpSocket *Client::getSocket() const
{
    return socket;
}
//_____________________________________________________________________________

void Client::callMethod(const char *method, const Serializer &args)
{
    QByteArray json_args = args.getData();
    QByteArray json_header = (JsonSerializer(RequestHeader(getID(), method, json_args.size()))).getData();
    uint64_t header_size = json_header.size();
    QByteArray request(reinterpret_cast<char *>(&header_size), sizeof(header_size));
    request.append(json_header);
    request.append(json_args);
    //qDebug() << "| callMethod" << method << "HEADER_SIZE =" << json_header.size() << "HEADER:" << json_header.chopped(json_header.size() - 10) << "ARGS" << json_args;
    //qDebug() << "| Full request SIZE =" << request.size() << "REQUEST:" << request;
    qDebug() << "| Call client's" << getID() << "method" << method;
    socket->write(request);
}
//_____________________________________________________________________________

void Client::saveHistory(QSharedPointer<HistorySlot> slot)
{
    history.push(slot);
    //qDebug().noquote() << "|Client" << getID() << "saved history:\n" << JsonSerializer(slot->layer).getData();
}
//_____________________________________________________________________________

QSharedPointer<Client::HistorySlot> Client::rollBackHistory()
{
    if (!history.isEmpty())
        return history.pop();
    return QSharedPointer<Client::HistorySlot> (nullptr);
}
//_____________________________________________________________________________

void Client::initClient(const Serializer &args)
{
    callMethod("c_init_client", args);
}
//_____________________________________________________________________________

void Client::finishBoardInit()
{
    callMethod("c_finish_board_init", NO_ARGS_SERIALIZER);
}
//_____________________________________________________________________________

void Client::addLayer(const Serializer &args)
{
    callMethod("c_add_layer", args);
}
//_____________________________________________________________________________

void Client::confirmAddLayer(const Serializer &args)
{
    callMethod("c_confirm_add_layer", args);
}
//_____________________________________________________________________________

void Client::clearBoard()
{
    callMethod("c_clear_board", NO_ARGS_SERIALIZER);
}
//_____________________________________________________________________________

void Client::deleteLayer(const Serializer &args)
{
    callMethod("c_delete_layer", args);
}
//=============================================================================
