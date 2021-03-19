#ifndef SERVER_H
#define SERVER_H

#define JSON_SERIALIZER

#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QMap>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>

#include "client_api.h"
#include "serializers.h"


//=============================================================================
class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server();
    ~Server();

    void startServer();       // Start listening incoming connections

// External server API
//--------------------
    void addLayer(const Serializer &args);      // Add new layer to layers_list
    void wrongRequest(const Serializer &args);  // Unknown request

public slots:
    void slotNewConnection(); // New pending connection
    void slotReadyRead();     // Client socket ready to read
    void slotDisconnected();  // Client disconnected

private:
    QList<QTcpSocket *> clients;
    QMap<QString, void(Server::*)(const Serializer &)> api_func = {
        {"s_add_layer", &Server::addLayer}
};
    //QList<Layer> layers_list;
};
//=============================================================================

#endif // SERVER_H
