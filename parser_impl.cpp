#include "parser_impl.h"

//=============================================================================
// JsonParser
//=============================================================================
void JsonParser::handleRequest(const QByteArray &data, Server *server)
{
    QJsonObject request = QJsonDocument::fromJson(data).object();

    //ServApiFunc method = methods[request["method"].toString()];

    //*ServApiFunc::method.call();
    (this->*methods[request["method"].toString()])(request["args"], server);
}
//_____________________________________________________________________________
// Server API functions parsers
//_____________________________________________________________________________

void JsonParser::parseAddLayerArgs(const QJsonValue &args, Server *server)
{
}
//_____________________________________________________________________________

void JsonParser::parseWrongRequestArgs(const QJsonValue &args, Server *server)
{
}
//=============================================================================
