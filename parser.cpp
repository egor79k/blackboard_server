#include "parser.h"


//=============================================================================
// JsonParser
//=============================================================================
void JsonParser::handleRequest(const QByteArray &data, Server *server)
{
    QJsonObject request = QJsonDocument::fromJson(data).object();

    (server->*methods[request["method"].toString()])();
}
