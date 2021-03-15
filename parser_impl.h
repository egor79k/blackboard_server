#ifndef PARSER_IMPL_H
#define PARSER_IMPL_H

#include <QSharedPointer>
#include <QJsonValue>

#include "parser.h"
#include "server.h"

class JsonParser : public Parser
{
    Q_OBJECT

    void parseAddLayerArgs(const QJsonValue &args, Server *server);
    void parseWrongRequestArgs(const QJsonValue &args, Server *server);

    QMap<QString, void(JsonParser::*)(const QJsonValue &, Server *)> methods = {
        {"s_add_layer", &JsonParser::parseAddLayerArgs},
        {"c_add_layer", &JsonParser::parseWrongRequestArgs}};

public:
    void handleRequest(const QByteArray &data, Server *server) override;
};

#endif // PARSER_IMPL_H
