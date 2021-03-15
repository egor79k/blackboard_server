#ifndef PARSER_H
#define PARSER_H

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QObject>

#include "server.h"

//=============================================================================
class Parser : public QObject
{
    struct MethodParams
    {};

    Q_OBJECT

public:
    virtual void handleRequest(const QByteArray &data, Server *server) = 0;
};
//=============================================================================


//=============================================================================
class JsonParser : public Parser
{
    Q_OBJECT

    QMap<QString, void(Server::*)()> methods = {
        {"create_layer", &Server::createLayer}};

public:
    void handleRequest(const QByteArray &data, Server *server);
};
//=============================================================================

#endif // PARSER_H
