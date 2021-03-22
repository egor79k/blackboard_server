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

void Client::initClient(const Serializer& args)
{
    QByteArray json_args = args.getData();
    QByteArray json_header = (JsonSerializer(RequestHeader(getID(), "init_client", json_args.size()))).getData();
    uint64_t header_size = json_header.size();
    QByteArray request(reinterpret_cast<char *>(&header_size), sizeof(header_size));
    request.append(json_header);
    request.append(json_args);
    socket->write(request);
}
//_____________________________________________________________________________

void Client::addLayer(const Serializer& args)
{

}
//=============================================================================
