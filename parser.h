#ifndef PARSER_H
#define PARSER_H

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QObject>

class Server;

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

#endif // PARSER_H
